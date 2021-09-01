#include <avr/io.h>
#include <util/delay.h>

#include "utility.h"

/* Helper */

static void do_port(port p, action a, int pin) {
    switch (p) {
    case portb:
	if (a == p_set)
	    PORTB |= (1 << pin);
	else if (a == p_unset)
	    PORTB &= (0 << pin);
	else
	    PORTB ^= (1 << pin);
	break;
    case portd:
	if (a == p_set)
	    PORTD |= (1 << pin);
	else if (a == p_unset)
	    PORTD &= (0 << pin);
	else
	    PORTD ^= (1 << pin);
	break;
    case portc:
	if (a == p_set)
	    PORTC |= (1 << pin);
	else if (a == p_unset)
	    PORTC &= (0 << pin);
	else
	    PORTC ^= (1 << pin);
	break;
    }
}

static void do_ddr(port p, type t, int pin) {
    switch (p) {
    case portb:
	if (t == output)
	    DDRB |= (1 << pin);
	else if (t == input)
	    DDRB &= (0 << pin);
	break;
    case portd:
	if (t == output)
	    DDRD |= (1 << pin);
	else if (t == input)
	    DDRD &= (0 << pin);
	break;
    case portc:
	if (t == output)
	    DDRC |= (1 << pin);
	else if (t == input)
	    DDRC &= (0 << pin);
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
    switch(p) {
    case portb:
	return PINB & (1 << pin);
    case portd:
	return PIND & (1 << pin);
    case portc:
	return PINC & (1 << pin);
    }

    return 1;
}


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

		if (counter > 25)
		    break;
	    }

	    break;
	}
    }
}
