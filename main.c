/* TM4C123G Tiva C Series ADC Example */

/* This Program measures analog voltage from analog Channel zero */
/* Also sends measured voltage value to computer virtual terminal using UART */
#include "inc/tm4c123gh6pm.h"
#include "include/ADC.h"
#include "String.h"
#include "include/Timer_init.h"
#include "include/LED_init.h"
#include "include/UART_init.h"


unsigned long SW1,SW2;  // User btn to take input form PF4, PF0
volatile uint32_t ui32loop;

void Delay(unsigned long counter);
char mesg[30];
float voltage;
unsigned int adc_value, adc_value_p;

void ADC1_SS3_Handler(void){
    adc_value = (ADC1_SSFIFO3_R) & (0X00000FFF) ; /* read adc conversion result from SS3 FIFO*/
    if(adc_value_p = adc_value){
        adc_value_p = adc_value;
    }
    ADC1_ISC_R = ADC_ISC_IN3;          /* clear conversion clear flag bit*/
    ADC1_PSSI_R |= (1<<3);        /* Enable SS3 conversion or start sampling data from AN0 */
}

void UART5Handler(void){
    unsigned char rx_data = 0;
    UART0_ICR_R &= ~(0x010); // Clear receive interrupt
    rx_data = UART5_DR_R ; // get the received data byte
    UART5_Transmitter(rx_data); // send data that is receive
}



int main(void)
{



    /* initialize PE3 for AIN0 input  */
       SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;   /* Enable Clock to GPIOE -or PE3/AN0- */
       GPIO_PORTE_AFSEL_R |= 0x00000008;       /* enable alternate function for PE3 */
       GPIO_PORTE_DEN_R &= ~(1<<3);        /* disable digital function */
       GPIO_PORTE_AMSEL_R |= (1<<3);       /* enable analog function */

       leds_init();


    /* Timers Initialization*/
       GPIO_PORTF_DATA_R = 0x08;
    Timer_init();
    Delay(1000);
    /* Timers Initialization*/
    GPIO_PORTF_DATA_R = 0x0C;
    Delay(1000);

    /* Initialize the ADC*/
    GPIO_PORTF_DATA_R = 0x02;
    ADC1_init();
    Delay(1000);
    /* Initialize UART 5*/
    UART5_init();


    while(1)
    {

//        SW1 =   GPIO_PORTF_DATA_R & 0X10;   //Listening To The PF4 Pin and Get the state of Switch
//        SW2 =   GPIO_PORTF_DATA_R & 0X01;   //Listening To The PF0 Pin and Get the state of Switch

//        if(adc_value > 2040){
//            GPIO_PORTF_DATA_R = 0x02;       // 0x02 --> RED,  0x04 -->  Blue, 0x08 --> Green
//        }else{
//            GPIO_PORTF_DATA_R = 0x0E;
//        }
              /* convert digital value back into voltage */
             voltage = (adc_value * 0.0008);
//            sprintf(mesg, "\r\nVoltage = %f Volts", voltage);

             printstring(" Message Echos back");
//             if(adc_value > 2050){
//                     GPIO_PORTF_DATA_R   =   0X04;
//                 }
//                 else{
//                     GPIO_PORTF_DATA_R   =   0X02;   // Turn on the led
//                 }
//      Delay(2000);
    }
}


void Delay(unsigned long counter)
{
    unsigned long i = 0;

    for(i=0; i< counter*1000; i++);
}
