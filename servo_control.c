/*******************************************************
This program was created by the CodeWizardAVR V4.02
Automatic Program Generator
© Copyright 1998-2024 Pavel Haiduc, HP InfoTech S.R.L.
http://www.hpinfotech.ro

Project : servo
Version :
Date    :  5/17/2024
Author  :
Company :
Comments:


Chip type               : ATmega164A
Program type            : Application
AVR Core Clock frequency: 20.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*******************************************************/

// I/O Registers definitions
#include <mega164a.h>

// Define the frequency of the clock
#define F_CPU 20000000UL
#include <delay.h>
#include <io.h>

void timer_config()
{
    // TOP value for 50Hz PWM with 20MHz clock and prescaler 8     
    // perioada 20ms  
    // ICR1 = 49999
    ICR1L = 0xC3;
    ICR1H = 0x4F;

    // 1 ms start PWM            
    OCR1AL = 0xC3;
    OCR1AH = 0x09;

    // Timer/Counter 1 initialization PRESCALER 8 (CS11), non-inverted PWM
    TCCR1A = (1 << WGM11) | (1 << COM1A1);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
}

// USART0 initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART0 Receiver: On
// USART0 Transmitter: On
// USART0 Mode: Asynchronous
// USART0 Baud Rate: 9600
void usart0_init(void) {
    UCSR0A = 0x00;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    UBRR0H = 0x00;
    UBRR0L = 0x81; // 9600 baud rate for 20 MHz clock
}

// Function to receive a character from USART0
char usart0_receive(void) {
    while (!(UCSR0A & (1 << RXC0))); // Wait until data is received
    return UDR0; // Return received data
}

// Function to transmit a character via USART0
void usart0_transmit(char data) {
    while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
    UDR0 = data; // Send data
}

void main(void) {
    usart0_init();
    timer_config();
    // PWM port pin 19
    DDRD = (1 << DDD5);
    #asm("sei")
        while (1) {
            char received_char = usart0_receive(); // Receive character
            if (received_char == 'a') {
                usart0_transmit('a');
                usart0_transmit(' ');
                // 1.5 ms
                OCR1AL = 0xA5;
                OCR1AH = 0x0E;
                delay_ms(1000);
            }
            else if (received_char == 'b') {
                usart0_transmit('b');
                usart0_transmit(' ');
                // 1 ms 
                OCR1AL = 0xC3;
                OCR1AH = 0x09;
                delay_ms(1000);
            }
            else if (received_char == 'c') {
                usart0_transmit('c');
                usart0_transmit(' ');
                // 2 ms  
                OCR1AL = 0x87;
                OCR1AH = 0x13;
                delay_ms(1000);
            }
        }
}
