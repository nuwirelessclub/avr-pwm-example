#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

#define LED_DDR DDRB
#define LED_PORT PORTB
#define LED_PIN 1

volatile uint8_t ledValue = 0;
volatile bool isUp = true;

int main(void) {
    // Setup code here
    LED_DDR |= (1 << LED_PIN); // pinMode(ledPin, OUTPUT);
    
    // Init a timer for PWM
    TCCR1A |= _BV(COM1A1); // Enable output on OC1A
    TCCR1A |= _BV(WGM10); // Set timer in 8 bit fast PWM mode TCCR1A
    TCCR1B |= _BV(WGM12); // Set timer in 8 bit fast PWM mode TCCR1A
    TCCR1B |= _BV(CS11); // Enable clock with div by 8 prescaler
    
    // Init another timer to handle changing the LED value
    TCCR0A |= _BV(WGM01); // Put timer in CTC mode
    TCCR0B |= _BV(CS02); // Enable clock with div by 256 prescaler
    TIMSK0 |= _BV(OCIE0A); // Enable output compare 1 interrupt
    OCR0A = 188;
    
    sei(); // Enabled global interrupts
    
    for(;;) {
        asm("nop;");
        // Calculate pi here without any delays!
	}
	return 0; // never reached
}

ISR(TIMER0_COMPA_vect) {
    if(isUp) {
        if(ledValue == 255) {
            isUp = false;
            ledValue--;
        }
        else {
            ledValue++;
        }
    }
    else {
        if(ledValue == 0) {
            isUp = true;
            ledValue++;
        }
        else {
            ledValue--;
        }
    }
    OCR1A = ledValue; // Set the led value
}