#include "sapi.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


#define NUM_BUTTONS 4
#define BUTTON_PRESSED  0   // Estado del botón presionado
#define BUTTON_RELEASED 1   // Estado del botón liberado

bool_t teclas[4] = { CIAA_BOARD_BUTTON, TEC2, TEC3, TEC4 };
bool_t leds[4] = { CIAA_BOARD_LED, LED1, LED2, LED3 };

volatile bool_t ledStates[NUM_BUTTONS] = { 0, 0, 0, 0 };

// Tarea de titilado para cada LED
void vLedBlinkTask(void *pvParameters) {
    int ledIndex = (int)pvParameters; // Índice del LED

    for (;;) {
        if (ledStates[ledIndex]) {
            gpioToggle(leds[ledIndex]);          // Alternar estado del LED
            vTaskDelay(pdMS_TO_TICKS(500));      // Intervalo de titilado
        } else {
            gpioWrite(leds[ledIndex], 0);        // Apagar el LED si no está titilando
            vTaskDelay(pdMS_TO_TICKS(50));      // Pequeño delay para evitar consumir CPU
        }
    }
}

// Tarea para detectar botones y activar/desactivar LEDs
void vButtonPollingTask(void *pvParameters) {
    (void)pvParameters;

    int buttonStates[NUM_BUTTONS] = { 1, 1, 1, 1 }; // Inicialmente todos los botones "liberados"

    for (;;) {
        for (int i = 0; i < NUM_BUTTONS; i++) {
            int currentButtonState = gpioRead(teclas[i]); // Leer estado del botón

            // Detectar flanco de bajada (botón presionado)
            if (currentButtonState == 0 && buttonStates[i] == 1) {
                ledStates[i] = !ledStates[i]; // Alternar estado del LED (titilando o apagado)
            }

            buttonStates[i] = currentButtonState; // Actualizar estado del botón
        }
        vTaskDelay(pdMS_TO_TICKS(50)); // Delay para evitar rebotes
    }
}

int main(void) {
    // Inicializar hardware
    boardConfig();

    // Crear tareas de titilado para cada LED
    for (int i = 0; i < NUM_BUTTONS; i++) {
        xTaskCreate(vLedBlinkTask, "BlinkLED", configMINIMAL_STACK_SIZE, (void *)i, 1, NULL);
    }

    // Crear tarea de polling de botones
    xTaskCreate(vButtonPollingTask, "ButtonPolling", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    // Iniciar el scheduler de FreeRTOS
    vTaskStartScheduler();

    // Si llega aquí, hubo un error
    for (;;) {
    }
}

