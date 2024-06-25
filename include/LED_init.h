/*
 * LED_init.h
 *
 *  Created on: 13-Jun-2024
 *      Author: lenovo
 */

#ifndef LED_INIT_H_
#define LED_INIT_H_


void leds_init(void)
{
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0X00000020;       //F Clock
    delay = SYSCTL_RCGC2_R;             // Delay
    GPIO_PORTF_LOCK_R = 0x4C4F434B;     // Unlock PortF
    GPIO_PORTF_CR_R     =   0X1F;       // Allow changes to PF4-0
    GPIO_PORTF_AMSEL_R  =   0X00;       // Disable analog Function
    GPIO_PORTF_PCTL_R   =   0X00000000; // GPIO Clear Bit PCTL
    GPIO_PORTF_DIR_R    =   0X0E;       //GPIO Direction: PF4,PF0 is OUTPUT & PF1,2,3 are INPUT
    GPIO_PORTF_AFSEL_R  =   0X00;       //Disable Alternate Functions

}


#endif /* LED_INIT_H_ */
