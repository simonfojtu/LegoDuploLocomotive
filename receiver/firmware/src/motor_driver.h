
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

        Status getStatus() const {
            return _status;
        }

    private:
        Status _status;
        uint8_t _PINA, _PINB;
        volatile uint8_t* _OUT;
};
