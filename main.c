/* TM4C123G Tiva C Series ADC Example */

/* This Program measures analog voltage from analog Channel zero */
/* Also sends measured voltage value to computer virtual terminal using UART */
#include "inc/tm4c123gh6pm.h"
#include "include/ADC.h"
void Delay(unsigned long counter);
void UART0_init(void);
void UART0_Transmitter(unsigned char data);
void printstring(char *str);
char mesg[11];
float voltage;
unsigned int adc_value;
void ADC1_SS3_Handler(void){
    adc_value = (ADC1_SSFIFO3_R) & (0X00000FFF) ; /* read adc conversion result from SS3 FIFO*/
    ADC1_ISC_R = ADC_ISC_IN3;          /* clear conversion clear flag bit*/
    ADC1_PSSI_R |= (1<<3);        /* Enable SS3 conversion or start sampling data from AN0 */
}


int main(void)
{

       UART0_init();
    /* Enable Clock to ADC0 and GPIO pins*/
//       SYSCTL_RCGCADC_R |= SYSCTL_RCGCADC_R1;    /* AD1 clock enable*/


    /* initialize PE3 for AIN0 input  */
       SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;   /* Enable Clock to GPIOE -or PE3/AN0- */
       GPIO_PORTE_AFSEL_R |= 0x00000008;       /* enable alternate function for PE3 */
       GPIO_PORTE_DEN_R &= ~(1<<3);        /* disable digital function */
       GPIO_PORTE_AMSEL_R |= (1<<3);       /* enable analog function */

    /* Initialization of PORTF - LED 1(Red LED)*/
       SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;   /* Enable Clock to GPIOF */
       GPIO_PORTF_AMSEL_R  =   0X0000;       // Disable analog Function
       GPIO_PORTF_DEN_R |= 0X0000001F ;
       GPIO_PORTF_PCTL_R   =   0X00000000; // GPIO Clear Bit PCTL
       GPIO_PORTF_DIR_R |= 0X0000000F ;

    /* Initialize the ADC*/
       ADC1_init();

    /* Initialization of NVIC Interrupt*/
       NVIC_EN1_R |= (1<<19);   // Enable interrupt for the ADC1 SS3

    while(1)
    {
        ADC1_ACTSS_R |= (1<<3);         /* enable ADC1 sequencer 3 */
//        while((ADC1_RIS_R & 8) == 0) ;   /* Wait until sample conversion completed*/
//        adc_value = (ADC1_SSFIFO3_R) & (0X00000FFF) ; /* read adc conversion result from SS3 FIFO*/
//        ADC1_ISC_R = ADC_ISC_IN3;          /* clear conversion clear flag bit*/
        if(adc_value > 2040){
            GPIO_PORTF_DATA_R = 0x02;
        }else{
            GPIO_PORTF_DATA_R = 0x04;
        }
              /* convert digital value back into voltage */
             voltage = (adc_value * 0.0008);
//            sprintf(mesg, "\r\nVoltage = %0.3f Volts", voltage);
             mesg[0] = 'H';
             mesg[1] = 'e';
             mesg[2] = 'l';
             mesg[3] = 'l';
             mesg[4] = 'o';
             mesg[5] = ' ';
             mesg[6] = 'W';
             mesg[7] = 'o';
             mesg[8] = 'r';
             mesg[9] = 'l';
             mesg[10] = 'd';
             printstring(mesg);
//      Delay(2000);
    }
}

void UART0_init(void)
{
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0;  /* enable clock to UART5 */
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0;  /* enable clock to PORTE for PE4/Rx and PE5/Tx */
    Delay(1);
    /* UART0 initialization */
    UART0_CTL_R = 0;         /* UART5 module disable */
    UART0_IBRD_R = 8;      /* for 9600 baud rate, integer = 104 */
    UART0_FBRD_R = 44;       /* for 9600 baud rate, fractional = 11*/
    UART0_CC_R= UART_CC_CS_SYSCLK;          /*select system clock*/
    UART0_LCRH_R = UART_LCRH_WLEN_8;     /* data length 8-bit, not parity bit, no FIFO */
    UART0_CTL_R = (UART_CTL_RXE)|(UART_CTL_TXE);     /* Enable UART5 module, Rx and Tx */

    /* UART5 TX5 and RX5 use PE4 and PE5. Configure them digital and enable alternate function */
    GPIO_PORTA_DEN_R = 0x03;      /* set PE4 and PE5 as digital */
    GPIO_PORTA_AMSEL_R = 0;    /* Turn off analog function*/
}
void UART0_Transmitter(unsigned char data)
{
    while((UART0_FR_R & (1<<5)) != 0); /* wait until Tx buffer not full */
    UART0_DR_R = data;                  /* before giving it another byte */
}

void printstring(char *str)
{
  while(*str)
    {
        UART0_Transmitter(*(str++));
    }
}
void Delay(unsigned long counter)
{
    unsigned long i = 0;

    for(i=0; i< counter*1000; i++);
}
