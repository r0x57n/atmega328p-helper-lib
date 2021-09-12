#define __AVR_ATmega328P__ 1

#include <avr/io.h>
#include <util/delay.h>

#include "utility.h"

/*
 * Library helper functions, statics.
 *
 * Used inside of the library only, not exposed to end user.
 */

static void do_port(Port p, Action a, int pin)
{
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

static void do_ddr(Port p, Type t, int pin) {
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


/*
 * Accessible functions.
 *
 * Functions exposed to the user of the library.
 */


/*
 * DDRx handling.
 */

void make_output(Port p, int pin) { do_ddr(p, output, pin); }
void make_input(Port p, int pin) { do_ddr(p, input, pin); }

void make_outputs(Port p, int from, int to) {
  for (int i = from; i <= to; i++) {
    do_ddr(p, output, i);
  }
}

void make_inputs(Port p, int from, int to) {
  for (int i = from; i <= to; i++) {
    do_ddr(p, input, i);
  }
}

// Create a pull up port by making it an input port and setting it high.
void make_pullup(Port p, int pin) {
  make_input(p, pin);
  set(p, pin);
}


/*
 * PORTx handling (setting/unsetting outputs).
 */

void set(Port p, int pin) { do_port(p, p_set, pin); }
void unset(Port p, int pin) { do_port(p, p_unset, pin); }
void toggle(Port p, int pin) { do_port(p, p_toggle, pin); }


/*
 * PINx handling (reading inputs).
 */

int read(Port p, int pin) {
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


// Waits for PINx to change value.
void wait_btn(Port p, int pin) {

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
 * Shift register functions to make handling easier.
 */

#define SR_WAIT_MS 0

void sr_init(ShiftRegister *sr) {
  make_output(sr->port, sr->MR);
  make_output(sr->port, sr->SH_CP);
  make_output(sr->port, sr->ST_CP);
  make_output(sr->port, sr->OE);
  make_output(sr->port, sr->DS);
}

void sr_step(ShiftRegister *sr) {
  set(sr->port, sr->SH_CP);
  set(sr->port, sr->ST_CP);
  _delay_ms(SR_WAIT_MS);
  unset(sr->port, sr->SH_CP);
  unset(sr->port, sr->ST_CP);
  _delay_ms(SR_WAIT_MS);
}

void sr_step_times(ShiftRegister *sr, int times) {
  for (int i = times; i > 0; i--) {
    set(sr->port, sr->SH_CP);
    set(sr->port, sr->ST_CP);
    _delay_ms(SR_WAIT_MS);
    unset(sr->port, sr->SH_CP);
    unset(sr->port, sr->ST_CP);
    _delay_ms(SR_WAIT_MS);
  }
}

void sr_reset(ShiftRegister *sr) {
  unset(sr->port, sr->MR);
  sr_step(sr);
  set(sr->port, sr->MR);
  sr_step(sr);
}

void sr_toggle(ShiftRegister *sr) {
  toggle(sr->port, sr->OE);
}

/*
 * Pushes the value to the shift register, lowest bit will be output on Q7
 * and so on.
 */
void sr_push_val(ShiftRegister *sr, uint8_t value) {
  for (uint8_t i = 0; i < 8; i++) {
    uint8_t bit = (value & 1);

    if (bit)
      set(sr->port, sr->DS);
    else
      unset(sr->port, sr->DS);

    value >>= 1;
    sr_step(sr);
  }
}


/*
 * Distance sensor functions to make handling easire.
 */

#define SOUND_VELOCITY 340

void ds_init(DistanceSensor *ds) {
  make_output(ds->port, ds->TRIG);
  make_input(ds->port, ds->ECHO);
  unset(ds->port, ds->TRIG);
  unset(ds->port, ds->ECHO);
}

float ds_measure(DistanceSensor *ds) {
  set(ds->port, ds->TRIG);
  _delay_ms(0.02);
  unset(ds->port, ds->TRIG);

  while(!read(ds->port, ds->ECHO)) {}

  float elapsed_time = 0;
  while(read(ds->port, ds->ECHO)) {
    _delay_ms(0.1);
    elapsed_time += 0.1;
  }

  // distance (in cm) = (elapsed time * sound velocity) / 100 / 2
  // divide distance by 2 because sensor returns the round trip time
  return ( ( elapsed_time * SOUND_VELOCITY ) / 100 ) / 2;
}
