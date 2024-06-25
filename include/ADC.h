/*
 * ADC.h
 *
 *  Created on: 08-Jun-2024
 *      Author: Muazam
 *
 *  This header file includes the ADC and sample Sequencer initialization with interrupt enabled
 *
 *  Here is the IRQ of ADC for testing, You can define yours
 *
 *  Go to Startup file --> extren the custom Interrupt Handler i.e

      extern void ADC1_SS3_Handler( void );

 *  Replace the DefaultHandler with custom one in vector table i.e

 (line:105) Timer_interrupt_Handler

 *  Define the ISR in main code of any other header/source file and include it into the main i.e.
        unsigned int adc_value;
        void ADC1_SS3_Handler(void){
            adc_value = (ADC1_SSFIFO3_R) & (0X00000FFF) ; // read adc conversion result from SS3 FIFO
            ADC1_ISC_R = ADC_ISC_IN3;      Clear conversion clear flag bit
            ADC1_PSSI_R |= (1<<3);         Enable SS3 conversion or start sampling data from AN0
            }
 *
 *
 */

#ifndef INCLUDE_ADC_H_
#define INCLUDE_ADC_H_


#include "inc/tm4c123gh6pm.h"
#include <stdio.h>
#include <stdint.h>


void ADC1_init(){
    /*Enable the ADC1 Clock*/
        SYSCTL_RCGCADC_R |= SYSCTL_RCGCADC_R1;

    /* initialize PE3 for AIN0 input  */
        GPIO_PORTE_AFSEL_R |= 0x00000008;       /* enable alternate function for PE3 */
        GPIO_PORTE_DEN_R &= ~(1<<3);        /* disable digital function */
        GPIO_PORTE_AMSEL_R |= (1<<3);       /* enable analog function */

    /* initialize sample sequencer3 */
        ADC1_ACTSS_R &= ~(1<<3);        /* disable SS3 during configuration */
        ADC1_EMUX_R |= ADC_EMUX_EM3_TIMER;    /* Timer interrupt trigger conversion */
        ADC1_SSMUX3_R = ADC_SSMUX3_MUX0_S;         /* get input from channel 1 */
        ADC1_SSCTL3_R |= (1<<1)|(1<<2);     //Enable last 2 bits DEC=3   /* take one sample at a time, set flag at 1st sample */
        ADC1_IM_R |= ADC_IM_MASK3;          // Enable interrupt mask for SS3 interrupt
        ADC1_ACTSS_R |= (1<<3);         /* enable ADC1 sequencer 3 */
        NVIC_EN1_R |= (1<<19);   // Enable interrupt for the ADC1 SS3
//        ADC1_PSSI_R |= (1<<3);        /* Enable SS3 conversion or start sampling data from AN0 */

}


#endif /* INCLUDE_ADC_H_ */
