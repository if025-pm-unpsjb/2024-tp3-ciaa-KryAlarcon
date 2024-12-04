#include "sapi.h"

#define NUM_BUTTONS 4

bool_t teclas[4] = { CIAA_BOARD_BUTTON, TEC2, TEC3, TEC4 };
bool_t leds[4] = { CIAA_BOARD_LED, LED1, LED2, LED3 };

int estadosBotones[NUM_BUTTONS] = { 0, 0, 0, 0 };  // Estado actual de los botones
int estadosLeds[NUM_BUTTONS] = { 0, 0, 0, 0 }; // Estado de los LEDs (0: apagado, 1: encendido)

int main(void) {

	// Inicializar y configurar la plataforma
	boardConfig();

	int buttonStates[NUM_BUTTONS] = { 0, 0, 0, 0 }; // Estado actual de los botones
	int ledStates[NUM_BUTTONS] = { 0, 0, 0, 0 }; // Estado de los LEDs (0: apagado, 1: encendido)

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
	}

	return 0;
}
