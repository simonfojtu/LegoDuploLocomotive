/*
 * @Author  : Simon Fojtu
 * @Date    : 16.07.2016
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <util/atomic.h>

#include "motor_driver.h"


MotorDriver motorDriver(&PORTB, &DDRB, PB3, PB4);

// set to 1 if button at PB2 is pressed long enough
volatile int longpress = 0;

volatile uint8_t timer0_counter = 0;
ISR(TIMER0_OVF_vect)
{
    timer0_counter++;
    if (timer0_counter == 5) {
        longpress = 1;
        timer0_counter = 0;
        TIMSK &= ~(1 << TOIE0); // disable Timer/Counter0 overflow interrupt
    }
}

// state of PB2 when INT0 interrupt fires
volatile int PB2_state;

// motor driver status
volatile MotorDriver::Status motorStatus = motorDriver.getStatus();

ISR(PCINT0_vect) {
    PB2_state = PINB & (1 << PB2);

    if (PB2_state) { // button pressed
        motorStatus = motorDriver.getStatus();
        motorDriver.stop();
        longpress = 0;
        TCNT0 = 0; // reset Timer/Counter0
        TIMSK |= 1 << TOIE0; // enable Timer/Counter0 overflow interrupt
        // set idle sleep mode, where the timer/counter0 is still running
        MCUCR &= ~(1 << SM1 | 2 << SM0); // Idle sleep mode
    } else { // button released
        if (motorStatus == MotorDriver::stopped) {
            if (longpress == 1)
                motorDriver.rev();
            else
                motorDriver.fwd();
        }
        // set power down sleep mode, where only pcint0 and watchdog can wake us
        MCUCR |= (1 << SM1); // Power-down sleep mode
    }
    // keep watchdog happy
    wdt_reset();
}

volatile uint8_t watchdog_counter = 0;
ISR(WDT_vect) {
    watchdog_counter++;
    // Watchdog fires every 8 seconds.
    // 8 * 15 = 120s, thus after 2 minutes the motor is stopped.
    if (watchdog_counter == 15) {
        motorDriver.stop();
        watchdog_counter = 0;
    }
}

int main(void)
{
    // set up external interrupt on PB2 - PCINT2
    PCMSK |= 1 << PCINT2; // Pin change enable mask
    GIMSK |= 1 << PCIE; // Pin change interrupt enable

    // set up watchdog
    WDTCR |= 1 << WDIE; // enable watchdog interrupt
    WDTCR |= (1 << WDP3) | (1 << WDP0); // select prescaler: 1024K cycles -> 8s at 5V [p46]

    // set up button press timer
    TCCR0B |= (1 << CS02) | (1 << CS00); // prescaler 1/1024 -> overflow 3.81Hz at 1MHz clock

    sei(); // enable interrupts

    while (1) {
        MCUCR |= (1 << SE); // sleep enable
        sleep_mode(); //Go to sleep
    }

    return(0);
}

