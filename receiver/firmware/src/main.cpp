/*
 * @Author  : Simon Fojtu
 * @Date    : 16.07.2016
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>


/* Global variables */
/* time since startup in 1/TIMER0_OVF_FREQ s */
volatile long mtime;
/* device status */
//status_t stat;

static char mtc;

//!
// Drive DC motor using two signals
//
// e.g. design 1 from http://www.talkingelectronics.com/projects/H-Bridge/H-Bridge-1.html
//
// The class ensures that only one of the pins is HIGH at any time (to avoid shoot through)
// in the H-bridge.
//
class MotorDriver {
    public:
        enum Status {
            stopped,
            forward,
            reverse
        };

        MotorDriver(volatile uint8_t* out, volatile uint8_t* ddr, uint8_t pinA, uint8_t pinB) {
            _OUT = out;
            _PINA = pinA;
            _PINB = pinB;

            *_OUT &= !(1 << _PINA | 1 << _PINB);
            *ddr |= (1 << _PINA) | (1 << _PINB);

            _status = stopped;
        }

        void fwd() {
            if (_status == reverse) {
               *_OUT &= ~(1 << _PINB);
               _delay_ms(10);
            }
            *_OUT |= (1 << _PINA);
            _status = forward;
        }

        void rev() {
            if (_status == forward) {
               *_OUT &= ~(1 << _PINA);
               _delay_ms(10);
            }
            *_OUT |= (1 << _PINB);
            _status = reverse;
        }

        void stop() {
            *_OUT &= ~((1 << _PINA) | (1 << _PINB));
            _status = stopped;
        }

    private:
        Status _status;
        uint8_t _PINA, _PINB;
        volatile uint8_t* _OUT;
};

MotorDriver motorDriver(&PORTB, &DDRB, PB3, PB4);

/*!
 * Button
 */
class Button {
    public:
        Button(volatile uint8_t* in, volatile uint8_t* ddr, uint8_t pin) {
            _in = in;
            _pin = pin;

            *ddr &= ~(1 << _pin);
        }

        bool isPressed() const {
            return (*_in & (1 << _pin));
        }
    private:
        uint8_t _pin;
        volatile uint8_t* _in;
};

Button mainButton(&PINB, &DDRB, PB2);


/*!
 * Timer0 overflow interrupt handling routine
 */
ISR(TIMER0_OVF_vect)
{
        /* frequency TIMER0_OVF_FREQ Hz */
        mtime++;
}


void main_init(void)
{
//        spi_init();
//
//        hw_init();
//
//        timer0_init();

        mtime = 0;
        mtc = 0;
}

void main_tick(void)
{
        /* Update time */
//        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
//                stat.ltime = mtime;
//        }
//        stat.sec = stat.ltime / TIMER0_OVF_FREQ;

        /* Process buttons */
    if (mainButton.isPressed()) {
        motorDriver.fwd();
    } else {
        motorDriver.stop();
    }
//        Keyboard_tick(&stat);

        /* Slow tick */
        if (mtc == 255) {
//                hw_read_tick(&stat);
//
//                hw_write_tick(&stat);
        }

        mtc++;
}

int main(void)
{

        main_init();

//        timer0_start();

        sei();


        while (1) {
            main_tick();
        }

        return(0);
}

