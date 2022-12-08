/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include <stdio.h>
#include <math.h>
#include "Board_LED.h"                  // ::Board Support:LED
#include "LPC17xx.h"

#define PRESCALE (25000-1)


//referencing the LPC1768 documentation
void delay_time(int time)
{
	int delay, i;
	for (delay = time; delay>0; delay--)
	{
		for (i = 1000; i > 0; i --)
		{
			
		}
	}
	
}
  
void led_ThreadA (void const *argument); // thread function
void led_ThreadB (void const *argument); // thread function
void led_ThreadC (void const *argument);


osThreadDef (led_ThreadA, osPriorityNormal, 1, 0);                   // thread object
osThreadDef (led_ThreadB, osPriorityAboveNormal, 1, 0);                   // thread object
osThreadDef (led_ThreadC, osPriorityHigh, 1, 0);  


/*###########################################################
	Virtual Timer declaration and call back method
############################################################*/


osThreadId T_led_IDA;
osThreadId T_led_IDB;	
osThreadId T_led_IDC;	

// Toggle the LED associated with the timer
void callback(void const *param){
	switch( (uint32_t) param){
		case 0:
			LED_On(6); //for visual LED demosntration purposes
			osSignalSet(T_led_IDA,0x01); //setting signal to let the thread know it can go
		  LED_Off(6);
			break;
		case 1:
			LED_On(7);
			osSignalSet(T_led_IDB,0x02);
  		LED_Off(7);
		  break;
		case 2:
		  LED_On(8);
			osSignalSet(T_led_IDC,0x03);
  		LED_Off(8);
		  break;
	}
}
osTimerDef(timer0_handle, callback);
osTimerDef(timer1_handle, callback);
osTimerDef(timer2_handle, callback);

//#############################################################


/*#############################################################
  Flash LED 0, signal to thread 2, wait for 3 to finish
*#############################################################*/
void led_ThreadA (void const *argument) {
	for (;;) {
		osSignalWait(0x01,osWaitForever);// first parameter is the signal flag to be asserted, next param is max time thread should wait to be signalled/
		LED_On(0);
		delay_time(200);
		//osDelay(50);
		//osSignalSet	(T_led_ID2,0x01);
		LED_Off(0); 
	}
}

/*################################################################
  Flash LED 2, signal to thread 3, wait for thread 1 to finish
*################################################################*/
void led_ThreadB (void const *argument) {
	for (;;) 	{
		osSignalWait(0x02,osWaitForever);
		LED_On(1);	
		delay_time(100);
    //osDelay(50);		
		LED_Off(1);
		//osSignalSet	(T_led_ID2,0x01);
	}
}


/*################################################################
  Flash LED 4, signal to thread 1, wait for thread 2 to finish
*################################################################*/
void led_ThreadC (void const *argument){
	for (;;) 	{
		osSignalWait (0x03,osWaitForever);
		LED_On(2);
		delay_time(50);
		//osDelay(50);
		LED_Off(2);
		//osSignalSet	(T_led_ID1,0x03);
	}
}


/*###################################################################
  Create and start threads
 *###################################################################*/
/*
int main (void) {
	
	//initTimer0();
	osKernelInitialize (); 
	//Virtual timer create and start
	osTimerId timer_A = osTimerCreate(osTimer(timer0_handle), osTimerPeriodic, (void *)0);	
	osTimerId timer_B = osTimerCreate(osTimer(timer1_handle), osTimerPeriodic, (void *)1);	
	osTimerId timer_C = osTimerCreate(osTimer(timer2_handle), osTimerPeriodic, (void *)2);	
	
  LED_Initialize ();	
	osTimerStart(timer_A, 400);	
	osTimerStart(timer_B, 400);	
	osTimerStart(timer_C, 200);
	
	//Signal and wait threads
	T_led_IDA = osThreadCreate(osThread(led_ThreadA), NULL);
	T_led_IDB = osThreadCreate(osThread(led_ThreadB), NULL);
  T_led_IDC = osThreadCreate(osThread(led_ThreadC), NULL);
	
	//osKernelStart ();      
	osDelay(osWaitForever);               						
	
	for (;;);
}
*/