#include "uart.h"

void uart_init()
{// Enabling clock to UART 0 and Port A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    
    // Configure GPIO Pins for UART mode.
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    
    //Use the internal 16MHz oscillator as the UART clock source
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_SYSTEM);
    
    //UARTDisable(UART0_BASE);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600,(UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE |UART_CONFIG_WLEN_8));//SysCtlClockGet() -> 16000000 //parity none, one stop bit, 8 bits
}
char uart_receive()
{ 
    //Polling until data is received
    while ((UART0_FR_R&(1<<4)) != 0);
    //return the data register content
    return UART0_DR_R;
  }

void uart_transmit(char x)
{
	//Polling until data can be transmitted
    while ((UART0_FR_R&(1<<5)) != 0);
    //Put the data in the data register
    UART0_DR_R = x;
}
void uart_receive_string (uint8_t* str)
{
  uint8_t i=0;
  str[i] = uart_receive();
  while (str[i] != '.'){
    i++;
    str[i] = uart_receive();
  }
  str[i] = '\0';
}

void uart_transmit_string (uint8_t* data)
	{
  while (*data != '\0'){
    uart_transmit(*data);
    data++;
  }
}