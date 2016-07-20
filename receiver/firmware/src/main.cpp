/*
 * @Author  : Simon Fojtu
 * @Date    : 16.07.2016
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <util/atomic.h>

#include "motor_driver.h"


MotorDriver motorDriver(&PORTB, &DDRB, PB3, PB4);

volatile int longpress = 0;
volatile uint8_t timer0_counter = 0;
ISR(TIMER0_OVF_vect)
{
    timer0_counter++;
    if (timer0_counter == 8) {
        longpress = 1;
        timer0_counter = 0;
        TIMSK &= ~(1 << TOIE0); // disable Timer/Counter0 overflow interrupt
    }
}

volatile int PB2_state; // state of PB2 when INT0 interrupt fires
volatile MotorDriver::Status motorStatus = motorDriver.getStatus();

ISR(INT0_vect) {
    PB2_state = PINB & (1 << PB2);

    if (PB2_state) { // button pressed
        motorStatus = motorDriver.getStatus();
        motorDriver.stop();
        longpress = 0;
        TCNT0 = 0; // reset Timer/Counter0
        TIMSK |= 1 << TOIE0; // enable Timer/Counter0 overflow interrupt
        // TODO start timer to measure how long it is pressed
    } else { // button released
        if (motorStatus == MotorDriver::stopped) {
            if (longpress == 1)
                motorDriver.rev();
            else
                motorDriver.fwd();
        }
    }
    wdt_reset();
}

ISR(WDT_vect) {
    // Watchdog fired
    motorDriver.stop();
}


void main_init(void)
{
    // set up external interrupt on PB2 - INT0
    //MCUCR |= 1 << ISC01; // The falling edge of INT0 generates an interrupt request [p51]
    MCUCR |= 1 << ISC00; // Any logical change on INT0 generates an interrupt request [p51]
    GIMSK |= 1 << INT0; // Enable external interrupt request 0

    // set up watchdog
    WDTCR |= 1 << WDIE; // enable watchdog interrupt
    WDTCR |= (1 << WDP3) | (1 << WDP0); // select prescaler: 1024K cycles -> 8s at 5V [p46]

    // set up button press timer
    TCCR0B |= (1 << CS02) | (1 << CS00); // prescaler 1/1024 -> overflow 3.81Hz at 1MHz clock

}


int main(void)
{

        main_init();

        sei();

        while (1) {
            // sleep();
        }

        return(0);
}

