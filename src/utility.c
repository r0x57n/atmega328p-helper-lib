#define __AVR_ATmega328P__ 1

#include <avr/io.h>
#include <util/delay.h>

#include "utility.h"

/* Helper */

static void do_port(port p, action a, int pin) {
  switch (p) {
    case portb:
      switch (a) {
        case p_set:
          PORTB |= (1 << pin);
          break;
        case p_unset:
          PORTB &= ~(1 << pin);
          break;
        case p_toggle:
          PORTB ^= (1 << pin);
          break;
      }
    break;
  case portd:
      switch (a) {
        case p_set:
          PORTD |= (1 << pin);
          break;
        case p_unset:
          PORTD &= ~(1 << pin);
          break;
        case p_toggle:
          PORTD ^= (1 << pin);
          break;
      }
    break;
  case portc:
      switch (a) {
        case p_set:
          PORTC |= (1 << pin);
          break;
        case p_unset:
          PORTC &= ~(1 << pin);
          break;
        case p_toggle:
          PORTC ^= (1 << pin);
          break;
      }
    break;
  }
}

static void do_ddr(port p, type t, int pin) {
  switch (p) {
    case portb:
      switch (t) {
        case output:
          DDRB |= (1 << pin);
          break;
        case input:
          DDRB &= ~(1 << pin);
          break;
      }
    break;
  case portd:
      switch (t) {
        case output:
          DDRD |= (1 << pin);
          break;
        case input:
          DDRD &= ~(1 << pin);
          break;
      }
    break;
  case portc:
      switch (t) {
        case output:
          DDRC |= (1 << pin);
          break;
        case input:
          DDRC &= ~(1 << pin);
          break;
      }
    break;
  }
}

/* PORTx */

void set(port p, int pin) { do_port(p, p_set, pin); }
void unset(port p, int pin) { do_port(p, p_unset, pin); }
void toggle(port p, int pin) { do_port(p, p_toggle, pin); }

/* DDRx */

void make_output(port p, int pin) { do_ddr(p, output, pin); }
void make_input(port p, int pin) { do_ddr(p, input, pin); }

void make_outputs(port p, int from, int to) {
  for (int i = from; i <= to; i++) {
    do_ddr(p, output, i);
  }
}

void make_inputs(port p, int from, int to) {
  for (int i = from; i <= to; i++) {
    do_ddr(p, input, i);
  }
}

/* Pull-up */

void make_pullup(port p, int pin) {
  make_input(p, pin);
  set(p, pin);
}

/* PINx */

int read(port p, int pin) {
  switch (p) {
  case portb:
    return PINB & (1 << pin);
  case portd:
    return PIND & (1 << pin);
  case portc:
    return PINC & (1 << pin);
  }

  return 1;
}

/*
 *
 * Quality of life functions.
 *
 **/

/* Waits for PINx to change value */

void wait_btn(port p, int pin) {

  while (1) {
    if (read(p, pin) == 0) {
      int counter = 0;

      // debounce
      while (1) {
        int val = read(p, pin);

        if (val > 0) {
          counter++;
        }

        _delay_ms(1);

        if (counter > 5)
          break;
      }

      break;
    }
  }
}


/*
**
** Shift register handling.
**
*/

#define SR_WAIT_MS 50

void sr_step(ShiftRegister *sr) {
  set(sr->port, sr->SH_CP);
  set(sr->port, sr->ST_CP);
  _delay_ms(SR_WAIT_MS);
  unset(sr->port, sr->SH_CP);
  unset(sr->port, sr->ST_CP);
  _delay_ms(SR_WAIT_MS);
}

void sr_step_times(ShiftRegister *sr, int times) {
  for (int i = times; i >= 0; i--) {
    set(sr->port, sr->SH_CP);
    set(sr->port, sr->ST_CP);
    _delay_ms(SR_WAIT_MS);
    unset(sr->port, sr->SH_CP);
    unset(sr->port, sr->ST_CP);
    _delay_ms(SR_WAIT_MS);
  }
}

void sr_reset(ShiftRegister *sr) {
  unset(sr->MR, sr->port);
  sr_step(sr);
  set(sr->MR, sr->port);
  sr_step(sr);
}

void sr_toggle_output(ShiftRegister *sr) {
}
