#include "tm4c123gh6pm.h"
#include <stdint.h>

uint8_t UART0Rx (void);
void delayMs (uint32_t n);

int main(void)
{
    SYSCTL_RCGCUART_R |= 1; // provide clock for UART0;
    SYSCTL_RCGC2_R |= 1; // enable clock to port A
    SYSCTL_RCGC2_R |= 0x20; // enable clock for port F

    UART0_CTL_R = 0; // disable UART0
    UART0_IBRD_R = 104; // integer divisor baud rate, for 16 MHz and 9600 baud rate
    UART0_FBRD_R = 11; // fraction part
    UART0_CC_R = 0; // use the system clock for UART0
    UART0_LCRH_R = 0x60; // 8bit, no parity, 1stop bit, no FIFO
    UART0_CTL_R = 0x301; // enable UART0. TXE, RXE

    GPIO_PORTA_DEN_R = 0x03; // Make PA0 and PA1 as digital
    GPIO_PORTA_AFSEL_R = 0x03; // Use PA0, PA1 alternate function
    GPIO_PORTA_PCTL_R = 0x11; // Configure PA0 and PA1 for UART

    GPIO_PORTF_DIR_R = 0x0E; // configure port F to control the LEDs
    GPIO_PORTF_DEN_R = 0x0E;
    GPIO_PORTF_DATA_R = 0;

    delayMs(1);

    uint8_t character;
    for(;;)
    {

        character = UART0Rx(); // get a character from UART
        GPIO_PORTF_DATA_R = character << 1; // shift left and write it to LEDs
    }
    return 0;
}

uint8_t UART0Rx (void)
{
    uint8_t character;
    while((UART0_FR_R & 0x10) != 0); // wait until the buffer is not empty
    character = UART0_DR_R; // read the received data
    return character; // return the value received
}

void delayMs (uint32_t n)
{
    uint32_t i, j;
    for(i=0; i<n ; i++)
    {
        for(j=0; j<3180 ; j++)
        {

        }
    }
}
