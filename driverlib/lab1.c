#include "lab1.h"



void systick_handler (){
  red_flag = true;
  if (counter == 1){
    blue_flag = true;
    counter = 0;
  }
  counter++;
}

void systick_init(){
  SysTickDisable();
  SysTickIntRegister(systick_handler);
  SysTickPeriodSet((0.5*FREQUENCY) - 1);
  SysTickIntEnable();
  SysTickEnable();
}

void portf_init (){
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1);
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_2);
  GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,OFF);
  GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,OFF);

}

void toggle_red (){
  red_flag = false;
  while(1){
    if (GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_1) == GPIO_PIN_1)
      GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,OFF);
    else
      GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,GPIO_PIN_1);

  }
}

void toggle_blue (){
  blue_flag = false;
  while (1){
    if (GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_2) == GPIO_PIN_2)
      GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,OFF);
    else
      GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,GPIO_PIN_2);
  }
}
