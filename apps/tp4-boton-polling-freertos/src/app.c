#include "sapi.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

void task(void *p);

#define NUM_BUTTONS 4
#define BUTTON_PRESSED  0   // Estado del botón presionado
#define BUTTON_RELEASED 1   // Estado del botón liberado

bool_t teclas[4] = { CIAA_BOARD_BUTTON, TEC2, TEC3, TEC4 };
bool_t leds[4] = { CIAA_BOARD_LED, LED1, LED2, LED3 };

int estadosBotones[NUM_BUTTONS] = { 0, 0, 0, 0 }; // Estado actual de los botones
int estadosLeds[NUM_BUTTONS] = { 0, 0, 0, 0 }; // Estado de los LEDs (0: apagado, 1: encendido)

int main(void) {

	boardConfig();

	// Crear la tarea de polling del botón
	xTaskCreate(task, "T1", configMINIMAL_STACK_SIZE + 50, NULL,
			configMAX_PRIORITIES - 1, NULL);

	// Iniciar el scheduler de FreeRTOS
	vTaskStartScheduler();

	// Si el sistema llega aquí, hubo un error
	for (;;) {
	}

}

void task(void *pvParameters) {

	while (1) {
		for (int i = 0; i < NUM_BUTTONS; i++) {
			// Leer el estado del botón actual (0: presionado, 1: liberado)
			int estadoActualBoton = gpioRead(teclas[i]);

			// Detectar el cambio de estado del botón de presionado a liberado
			if (estadoActualBoton == 0 && estadosBotones[i] == 1) {
				// Si el botón fue presionado (cambio de liberado a presionado)
				if (estadosLeds[i] == 0) {
					// Si el LED está apagado, encenderlo y actualizar el estado
					estadosLeds[i] = 1;
					// Código para encender el LED i
					gpioWrite(leds[i], estadosLeds[i]);
				} else {
					// Si el LED está encendido, apagarlo y actualizar el estado
					estadosLeds[i] = 0;
					// Código para apagar el LED i
					gpioWrite(leds[i], estadosLeds[i]);
				}
			}

			// Actualizar el estado del botón
			estadosBotones[i] = estadoActualBoton;
		}
		// Retardo para evitar rebotes (debounce)
		vTaskDelay(pdMS_TO_TICKS(50));
	}
}
