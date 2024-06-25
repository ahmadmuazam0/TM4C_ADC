/*
 * UART_init.h
 *
 *  Created on: 24-Jun-2024
 *      Author: lenovo
 */

#ifndef INCLUDE_UART_INIT_H_
#define INCLUDE_UART_INIT_H_


#include "inc/tm4c123gh6pm.h"
#include <stdio.h>
#include <stdint.h>


void UART5_init( void ){
    SYSCTL_RCGCUART_R |= (1<<5) ;  /* enable clock to UART5 */
    SYSCTL_RCGCGPIO_R |= (1<<4) ;   /*enable clock to GPIO Port E*/


    /* UART5 initialization */
    UART5_CTL_R = 0;         /* UART5 module disable */
    UART5_IBRD_R = 8;      /* for 9600 baud rate, integer = 104 */
    UART5_FBRD_R = 44;       /* for 9600 baud rate, fractional = 11*/
    UART5_CC_R = 0;          /*select system clock*/
    UART5_LCRH_R = 0x60;     /* data length 8-bit, not parity bit, no FIFO */
    UART5_CTL_R = 0x301;     /* Enable UART5 module, Rx and Tx */

    /* UART5 TX5 and RX5 use PE4 and PE5. Configure them digital and enable alternate function */
    GPIO_PORTE_PCTL_R &= 0;
    GPIO_PORTE_DEN_R = 0x30;      /* set PE4 and PE5 as digital */
    GPIO_PORTE_AFSEL_R = 0x30;    /* Use PE4,PE5 alternate function */
    GPIO_PORTE_AMSEL_R = 0;    /* Turn off analog function*/
    GPIO_PORTE_PCTL_R = 0x00110000;     /* configure PE4 and PE5 for UART */
}


void UART5_Transmitter(unsigned char data)
{
    while((UART5_FR_R & (1<<5)) != 0); /* wait until Tx buffer not full */
    UART5_DR_R = data;                  /* before giving it another byte */
}


void printstring(char *str)
{
  while(*str)
    {
        UART5_Transmitter(*(str++));
    }
}



#endif /* INCLUDE_UART_INIT_H_ */
