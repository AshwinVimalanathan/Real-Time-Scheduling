#define osObjectsPublic                     // define objects in main module
//#include "osObjects.h"                      // RTOS object definitions
#include "cmsis_os.h"                       // CMSIS RTOS header file
#include <stdio.h>
#include <math.h>
#include "Board_LED.h"                  // ::Board Support:LED
#include "Board_Joystick.h"             // ::Board Support:Joystick

//------- ITM Stimulus Port definitions for printf ------------------- //
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

struct __FILE { int handle;  };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) {
  if (DEMCR & TRCENA) {
    while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch;
  }
  return(ch);
}

volatile unsigned short JoystickState; //variable controlling and representing the state of the joystick 


void semaphore_ThreadA (void const *argument); //initialization of sempahore threads
void semaphore_ThreadB (void const *argument); 
void semaphore_ThreadC (void const *argument);
osThreadDef (semaphore_ThreadA, osPriorityBelowNormal, 1, 0); //setting the priority of the threads                   
osThreadDef (semaphore_ThreadB, osPriorityNormal, 1, 0);      //thread 1 has lowest priority, thread 2 has medium priority
osThreadDef (semaphore_ThreadC, osPriorityAboveNormal, 1, 0); //and thread 3 has highest priority              
osSemaphoreDef(semaphore); //define semaphore object
osSemaphoreId (semaphore_id); // identification for semaphore
osThreadId T_semaphore_IDA; //identification for the threads
osThreadId T_semaphore_IDB;	
osThreadId T_semaphore_IDC;	


void mutex_ThreadA (void const *argument); //intialization of mutex threads
void mutex_ThreadB (void const *argument); 
void mutex_ThreadC (void const *argument);
osThreadDef (mutex_ThreadA, osPriorityBelowNormal, 1, 0);  //set priority just like how it did previously                  
osThreadDef (mutex_ThreadB, osPriorityNormal, 1, 0);                   
osThreadDef (mutex_ThreadC, osPriorityAboveNormal, 1, 0);  
osMutexDef(mutex); 
osMutexId (mutex_id);
osThreadId T_mutex_IDA;
osThreadId T_mutex_IDB;	
osThreadId T_mutex_IDC;


void resemble_semaphore_ThreadA (void const *argument); //intialization of resemble semaphore threads
void resemble_semaphore_ThreadB (void const *argument); 
void resemble_semaphore_ThreadC (void const *argument);
osThreadDef (resemble_semaphore_ThreadA, osPriorityBelowNormal, 1, 0);                   
osThreadDef (resemble_semaphore_ThreadB, osPriorityNormal, 1, 0);                   
osThreadDef (resemble_semaphore_ThreadC, osPriorityAboveNormal, 1, 0);  
osThreadId T_resemble_semaphore_IDA;
osThreadId T_resemble_semaphore_IDB;
osThreadId T_resemble_semaphore_IDC;


unsigned int factorial(unsigned int n){ 
  if (n == 0) 
      return 1; 
  return n * factorial(n - 1); 
}

double exponent(double x, double y) {
	  if(y==0){
			return 1;
		}
		else{
    int i = 0;
    double result = x;
    for(i = 0; i < y - 1; i++) {
        result = result * x;
    }
    return result;
	  }
}

void semaphore_ThreadA (void const *argument) {
	osSemaphoreWait(semaphore_id, osWaitForever);
	int i = 0;
	double x = 0;
	for(i=0; i<240000; i++){
		x = x * 3;
	}
	osSemaphoreRelease(semaphore_id);
}

void semaphore_ThreadB (void const *argument) { 
	osSemaphoreWait(semaphore_id, osWaitForever);
	int i = 0;
	double x = 0;
	double temp = 0;
	for(i=0; i<120000; i++){
		temp = i%20;
		x = x / 2;
	}
	osSemaphoreRelease(semaphore_id);
}

void semaphore_ThreadC (void const *argument){
	osSemaphoreWait(semaphore_id, osWaitForever);
	int i = 0;
	double x = 0;
	for(i=0; i<60000; i++){
		x++;
	}
	osSemaphoreRelease(semaphore_id);
}



void mutex_ThreadA (void const *argument) { 
	osMutexWait(mutex_id, osWaitForever);
	int i = 0;
	double x = 0;
	for(i=0; i<60000; i++){
		x = x + 4;
	}
	osMutexRelease(mutex_id);
}

void mutex_ThreadB (void const *argument) {
	osMutexWait(mutex_id, osWaitForever);
	int i = 0;
	double x = 0;
	for(i=0; i<240000; i++){
		x = x / 3;
	}
	osMutexRelease(mutex_id);
}

void mutex_ThreadC (void const *argument){ 
	osMutexWait(mutex_id, osWaitForever);
	int i = 0;
	double x = 0;
	for(i=0; i<120000; i++){
		x++;
	}
	osMutexRelease(mutex_id);
}



int resemble_semaphore_token = 0; //global variable represents a token

void resemble_semaphore_ThreadA (void const *argument) {
	if (resemble_semaphore_token == 0){
		resemble_semaphore_token++; //token has been taken therefore other threads cannot execute
		int i = 0; //perform random computation
		double x = 0;
		for(i=0; i<120000; i++){
			x++;
		}
		resemble_semaphore_token = 0; //setting the token back to zero in order for other threads available to take
	}
}

void resemble_semaphore_ThreadB (void const *argument) { 
	if (resemble_semaphore_token == 0){
		resemble_semaphore_token++;
		int i = 0;
		double x = 0;
		for(i=0; i<60000; i++){
			x = (x * 2)/5;
		}
		resemble_semaphore_token = 0;
	}
}

void resemble_semaphore_ThreadC (void const *argument){ 
	if (resemble_semaphore_token == 0){
		resemble_semaphore_token++;	
		int i = 0;
		double x = 0;
		double temp;
		for(i=0; i<240000; i++){
			temp = i/(x+1);
			x = (x + 1)/x;
		}
		resemble_semaphore_token = 0;
	}
}

int main (void) {
	
	osKernelInitialize ();
	LED_Initialize ();
	Joystick_Initialize();
	
	printf("Semaphore Solution - Joystick Direction Right(Pin 24)\n");
	printf("Mutex Solution - Joystick Direction Down(Pin 25)\n");
	printf("Resemble Semaphore Solution - Joystick Direction Left(Pin 26)\n");

	while(1){
		JoystickState = Joystick_GetState();
		if (JoystickState == (JOYSTICK_RIGHT)){ //user input will select pin 24
			printf("Running Semaphore Solution\n");
			T_semaphore_IDA = osThreadCreate(osThread(semaphore_ThreadA), NULL);
			T_semaphore_IDB = osThreadCreate(osThread(semaphore_ThreadB), NULL);
			T_semaphore_IDC = osThreadCreate(osThread(semaphore_ThreadC), NULL);
			osKernelStart ();
			osDelay(500);
		}
		else if (JoystickState == (JOYSTICK_DOWN)){ // pin 25 represented down button
			printf("Running Mutex Solution\n");
			T_mutex_IDA = osThreadCreate(osThread(mutex_ThreadA), NULL);
			T_mutex_IDB = osThreadCreate(osThread(mutex_ThreadB), NULL);
			T_mutex_IDC = osThreadCreate(osThread(mutex_ThreadC), NULL);
			osKernelStart ();
			osDelay(500);
		}
		else if (JoystickState == (JOYSTICK_LEFT)){ // pin 26 
			printf("Running Resemble Semaphore Solution\n");
			T_resemble_semaphore_IDA = osThreadCreate(osThread(resemble_semaphore_ThreadA), NULL);
			T_resemble_semaphore_IDB = osThreadCreate(osThread(resemble_semaphore_ThreadB), NULL);
			T_resemble_semaphore_IDC = osThreadCreate(osThread(resemble_semaphore_ThreadC), NULL);
			osKernelStart ();
			osDelay(500);
		}
	}              						
}