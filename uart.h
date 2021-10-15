#ifndef UART_H_
#define UART_H_

#define PART_TM4C123GH6PM

#include <stdint.h>
#include <stdbool.h>

#include "inc\hw_ssi.h"
#include "inc\hw_types.h"
#include "inc\tm4c1294ncpdt.h"
#include "driverlib\pin_map.h"
#include "inc\hw_memmap.h"
#include "driverlib\timer.h"
#include "driverlib\ssi.h"
#include "driverlib\gpio.h"
#include "driverlib\sysctl.h"
#include "driverlib\interrupt.h"
#include "driverlib\systick.h"
#include "driverlib\debug.h"
#include "driverlib/uart.h"
#include "inc/hw_uart.h"
void uart_init(void);
char uart_receive(void);
void uart_transmit(char x);
void uart_receive_string (uint8_t* str);
void uart_transmit_string (uint8_t* data);
#endif