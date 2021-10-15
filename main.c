#define PART_TM4C123GH6PM

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "lcd.h"
#include "uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define MAX_SIZE_OF_MY_BUFFER 12

//global variables used by system
char cities[][10] = {"London", "Paris", "Madrid", "Rome", "Athens", "Ankara", "Istanbul", "Cairo", "Moscow", "Jerusalem"};
char timediff[] = {0, 1, 1, 1, 2, 2, 2, 2, 3, 3};
unsigned int selection; 
QueueHandle_t xQueuel; 
QueueHandle_t xQueue2;

//prototypes
unsigned int Read_From_Keyboard(void);
void Task1(void *pvParameters);
void Task2(void *pvParameters);
void Task3(void *pvParameters);
void Read_Time(char buf[]);
void toString (char c, char b []);

int main()
{
	xQueuel=xQueueCreate(1,8); //create queue of size 1 type:char
		xQueue2=xQueueCreate(1,8); //create queue of size 1 type:char
	 
    xTaskCreate(Task1, "Time Controller", configMINIMAL_STACK_SIZE, NULL, 10, NULL);//create task that responsible of updating our timer.

		xTaskCreate(Task2,"LCD Controller",configMINIMAL_STACK_SIZE,NULL,10,NULL);//create task that responsible of reciecve current time and country and display it.
    
    xTaskCreate(Task3,"UART Controller",configMINIMAL_STACK_SIZE,NULL,10,NULL);//create task that responsible to show to user current country list and send his selection to microcontroller.



	  vTaskStartScheduler();//start FreeRtos Schedular
		
	  for(;;);
}

//work of time
 void Task1(void *pvParameters) {
   typedef struct Message 
   {
      unsigned char hours;
      unsigned char minutes;
      unsigned char seconds; 
   } AMessage; //create message structure
   AMessage Tim; 
   xQueueReceive(xQueue2, &Tim, portMAX_DELAY); // Receive initial time 
   while (1) { 
	vTaskDelay(pdMS_TO_TICKS(1000)); //delay 1 sec
	Tim.seconds++; // increment one second to current time
	if(Tim.seconds == 60) {//check 1 minute passed
		Tim.seconds = 0;
		Tim.minutes++; // increment one minute to current time
		if (Tim.minutes == 60){//check 1 hour passed
			Tim.minutes = 0; 
			Tim.hours++; // increment one hour to current time
			if(Tim.hours == 24){//check 1 day passed
				Tim.hours = 0; // timer reset to 00:00:00
			}
		}
	}
 xQueueSendToBack(xQueuel, &Tim, portMAX_DELAY);//send current time to queue 1
  }
 }
/*
 this function convert number to string
 */ 
void toString (char ch, char b []){
	int i = 2;
	for (int j =0; j<2; j++){
		b[j]='0';
	}
	//make string of 2 char;
	while (ch != 0){
		i--;		
		b[i]=((ch%10)+'0');
		ch/=10;
	}
	//put number in created string
	b[2]='\0';//add null terminator

}
/*
 this function read initial time fromm user through uart communication
 */ 
void Read_Time(char buf[]) { 
 unsigned char c,k=0; 
 while(1){
	c=uart_receive();//recieve char from user
  uart_transmit(c); //send to uart recieved value
	if(c =='\r')break;//check end of message
	buf[k] = c; //put every character of messuge in string called buffer
	k++; 
 }
 buf[k]='\0'; //add null terminator
} 
/*
 this function read user country selection
 */ 			
unsigned int Read_From_Keyboard() {
      unsigned int Total; 
	    unsigned char N;
     	Total = 0; 
      while(1){
				if((UART0_FR_R&(1<<4)) == 0)
				{					
      N = uart_receive(); 
			uart_transmit(N);
      if(N == '\r') break;//if user press enter to recieve value
        N = N-'0';
        Total = 10*Total + N; //update value with current input
      } 
		}
		return Total;//return use value
}

 void Task2(void *pvParameters) {
	 char txt[7];
   typedef struct Message 
   {
      unsigned char hours;
      unsigned char minutes;
      unsigned char seconds; 
   } AMessage; //create message structure
   AMessage Tim; 
	 LCD_init();//initialize pur lcd
	 LCD_Clear();//clear lcd
	 selection=0;//out initial country selection is country 0
	 while(1) { 
		 
	  xQueueReceive(xQueuel, &Tim, portMAX_DELAY); //recieve current time from queue 1
		  LCD_Clear();
		 LCD_PrintLn(0,0,cities[selection]);
     Tim.hours = Tim.hours + timediff[selection]; //change output time depend on timezone of selected country
		 if(Tim.hours > 23)Tim.hours = Tim.hours - 24; //adjust time to make hours overflow when it is greater than 24
		 
		 toString(Tim.hours, txt);//put current hours in string
//		 if(Tim.hours < 10) {
//			 txt[1] = txt[0]; 
//			 txt[0] = '0'; 
//			 txt[2] = '\0';
//		 }
		
		 LCD_PrintLn(1,0,txt); //print hours in lcd
		 LCD_Cursor(1,2);
		 LCD_Show(':'); //print ':' in lcd
		 toString(Tim.minutes, txt);//put current minutes in string

//	   if(Tim.minutes < 10){
//		   txt[1] = txt[0];
//		   txt[0] = '0';
//		   txt[2] = '\0'; 
//		 } 
		 LCD_PrintLn(1,3,txt); //print hours in lcd
		 LCD_Cursor(1,5);
		 LCD_Show(':'); //print ':' in lcd
		 toString(Tim.seconds, txt);//put current seconds in string

//	   if(Tim.seconds < 10) {
//		   txt[1] = txt[0]; 
//		   txt[0] = '0';
//		   txt[2] = '\0'; 
//		 } 
		 LCD_PrintLn(1,6,txt); //print hours in lcd
	 }
}
	 
 


 void Task3(void *pvParameters) {
	char k, Buffer[10]; 
  typedef struct Message 
  { 
    unsigned char hh;
    unsigned char mm; 
    unsigned char ss; 
  } AMessage; //create message structure
  AMessage Tim; 
  uart_init();//initialize uart
	//make uart sent request  to user through pc
	uart_transmit_string("\n\rTime in Different Countries"); 
	uart_transmit_string("\n\r "); 
	uart_transmit_string("\n\rEnter the time in London (hh:mm:ss): ");
	Read_Time(Buffer);// read current time from user
	//update message with recieved timediff from user.
	Tim.hh = 10*(Buffer[0] - '0') + Buffer[1] - '0'; 
	Tim.mm = 10*(Buffer[3] - '0') + Buffer[4] - '0'; 
	Tim.ss = 10*(Buffer[6] - '0') + Buffer[7] - '0'; 	
  xQueueSend(xQueue2, &Tim, 0); //send message to queue 2
   while (1) {
	//make uart sent country list to user through pc
   uart_transmit_string("\n\r\n\rSelect a City:");
   for(k = 0; k < 10; k++) 
   {
   	 uart_transmit_string("\n\r");
	   uart_transmit(k+'0'); 
	   uart_transmit_string(". "); 
	   uart_transmit_string(cities[k]); 
	 } 
	 uart_transmit_string("\n\rSelection: "); 
	 selection = Read_From_Keyboard();
	 //selection is updated whenever user enter value and press enter


} 
}  
