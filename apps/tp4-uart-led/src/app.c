/*============================================================================
 * Autor:
 * Licencia:
 * Fecha:
 *===========================================================================*/

// DEPENDENCIAS

#include "../../example/inc/app.h"         // <= Su propia cabecera (opcional)

#include "sapi.h"        // <= Biblioteca sAPI

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
   // ---------- CONFIGURACIONES ------------------------------
	 uartConfig( UART_USB, 9600 );
	 char byteIngresado;
   // Inicializar y configurar la plataforma
   boardConfig();

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE ) {

	   // Leer byte desde UART_USB por polling
	              if (uartReadByte(UART_USB, &byteIngresado)) {
	            	  switch( byteIngresado )
	            	  {
	            	      case '1':
	            	    	  gpioToggle(CIAA_BOARD_LED);
	            	    	  break;
	            	      case '2':
	            	    	  gpioToggle(LED1);
	            	    	  	            	    	  break;
	            	      case '3':
	            	    	  gpioToggle(LED2);
	            	    	  	            	    	  break;
	            	      case '4':
	            	    	  gpioToggle(LED3);
	            	    	  	            	    	  break;
	            	      default :
	            	          break;
	            	  }
	              }
   }




   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontroladore y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}
