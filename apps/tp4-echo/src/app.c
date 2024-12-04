#include <stdint.h>
#include <stdbool.h>
#include "uart.h"  // Incluye las definiciones específicas para la UART en tu hardware.

#define UART_BUFFER_SIZE 128

static char uart_buffer[UART_BUFFER_SIZE];

void uart_echo_server(void) {
    uint16_t index = 0;

    while (1) {
        // Polling para leer datos de la UART
        if (uart_is_data_ready()) { // Función específica que indica si hay datos disponibles en UART
            char c = uart_read_char(); // Función para leer un carácter desde la UART

            // Almacenar el carácter en el buffer
            uart_buffer[index++] = c;

            // Si el carácter es '\n', enviamos el mensaje completo de vuelta
            if (c == '\n' || index >= UART_BUFFER_SIZE - 1) {
                uart_buffer[index] = '\0'; // Terminar la cadena para seguridad

                // Enviar el mensaje de vuelta por la UART
                for (uint16_t i = 0; i < index; i++) {
                    uart_send_char(uart_buffer[i]); // Función para enviar un carácter por UART
                }

                // Reiniciar el índice del buffer
                index = 0;
            }
        }
    }
}

int main(void) {
    // Inicializar la UART
    uart_init(); // Inicializa UART con configuración predefinida (velocidad, bits de parada, etc.)

    // Inicia el servidor de eco
    uart_echo_server();

    // Nunca deberíamos llegar aquí
    return 0;
}
