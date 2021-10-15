#ifndef lab1_h
#define lalb1_h

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/systick.h"

#define FREQUENCY 16000000
#define ON 1
#define OFF 0

bool red_flag = false,
blue_flag = false;
unsigned int counter = 0;

void systick_handler ();
void systick_init();
void portf_init ();
void toggle_red ();
void toggle_blue ();

#endif