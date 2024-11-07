/*
 * main.c
 *
 *  Created on: 13 oct. 2022
 *      Author: fep
 */

#include "FreeRTOS.h"
#include "task.h"
#include "app.h"         // <= Su propia cabecera (opcional)
#include "sapi.h"        // <= Biblioteca sAPI
#include "semphr.h"

void task(void *p);


/* Tasks WCETs. */
#define TASK1_WCET		1000
#define TASK2_WCET		2000
#define TASK3_WCET		1000
#define TASK4_WCET		2000
/* Tasks periods. */
#define TASK1_PERIOD 	4000
#define TASK2_PERIOD 	6000
#define TASK3_PERIOD 	9000
#define TASK4_PERIOD 	11000


/*-----------------------------------------------------------*/

struct xTaskStruct {
	TickType_t wcet;
	TickType_t period;
	gpioMap_t led;
};

typedef struct xTaskStruct xTask;


xTask task1 = { TASK1_WCET, TASK1_PERIOD, CIAA_BOARD_LED };
xTask task2 = { TASK2_WCET, TASK2_PERIOD, LED1};
xTask task3 = { TASK3_WCET, TASK3_PERIOD, LED2};
xTask task4 = { TASK4_WCET, TASK4_PERIOD, LED3};

int main( void )
{
        // Inicializar y configurar la placa de desarrollo
	boardConfig();

        xTaskCreate( task, "T1", configMINIMAL_STACK_SIZE + 50, (void*) &task1, configMAX_PRIORITIES - 1, NULL );
        xTaskCreate( task, "T2", configMINIMAL_STACK_SIZE + 50, (void*) &task2, configMAX_PRIORITIES - 2, NULL );
        xTaskCreate( task, "T3", configMINIMAL_STACK_SIZE + 50, (void*) &task3, configMAX_PRIORITIES - 3, NULL );
        xTaskCreate( task, "T4", configMINIMAL_STACK_SIZE + 50, (void*) &task4, configMAX_PRIORITIES - 4, NULL );
        vTaskStartScheduler();
             for(;;);
}

/*-----------------------------------------------------------*/

static void vBusyWait( TickType_t ticks )
{
    TickType_t elapsedTicks = 0;
    TickType_t currentTick = 0;
    while ( elapsedTicks < ticks ) {
        currentTick = xTaskGetTickCount();
        while ( currentTick == xTaskGetTickCount() ) {
            __asm__("nop");
        }
        elapsedTicks++;
    }
}
/*-----------------------------------------------------------*/

void task( void *pvParameters )
{

	//char cMessage[ mainMAX_MSG_LEN ];
		unsigned int uxReleaseCount = 0;
		TickType_t pxPreviousWakeTime = 0;
		xTask *task = (xTask*) pvParameters;


	for( ;; )
	{
		gpioWrite( task->led, 1 );
		vBusyWait( task->wcet );

		gpioWrite( task->led, 0 );

		vTaskDelayUntil( &pxPreviousWakeTime, task->period );

				uxReleaseCount += 1;
			}


			vTaskDelete( NULL );
}
