#include "../../example/inc/app.h"         // <= Su propia cabecera (opcional)

#include "sapi.h"        // <= Biblioteca sAPI

#define BUFFER_SIZE 128

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{

   boardConfig();
   char buffer[BUFFER_SIZE]; // Buffer para almacenar el mensaje recibido
   size_t index = 0;         // Índice del buffer

   uartConfig( UART_USB, 9600 );

   char byteIngresado;
   while (true) {


           // Leer byte desde UART_USB por polling
           if (uartReadByte(UART_USB, &byteIngresado)) {
               // Almacenar el byte leído en el buffer
               buffer[index++] = byteIngresado;

               // Si se encuentra el terminador '\n' o se llena el buffer
               if (byteIngresado == '\r' || index >= BUFFER_SIZE - 1) {
                   buffer[index] = '\0'; // Terminar el string
                   index = 0;            // Reiniciar el índice del buffer

                       uartWriteString(UART_USB, buffer);

               }
           }
   }



   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontroladore y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}
