#include <avr/io.h>
#include <util/delay.h>

#include "utility.h"

/* Helper */

static void do_port(port p, action a, int pin) {
    switch (p) {
    case portb:
	if (a != p_toggle)
	    PORTB = a << pin;
	else
	    PORTB ^= (1 << pin);
	break;
    case portd:
	if (a != p_toggle)
	    PORTD = a << pin;
	else
	    PORTD ^= (1 << pin);
	break;
    case portc:
	if (a != p_toggle)
	    PORTC = a << pin;
	else
	    PORTC ^= (1 << pin);
	break;
    }
}

static void do_ddr(port p, type t, int pin) {
    switch (p) {
    case portb:
	PORTB = t << pin;
	break;
    case portd:
	PORTD = t << pin;
	break;
    case portc:
	PORTC = t << pin;
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
	return PORTB & (1 << pin);
    case portd:
	return PORTD & (1 << pin);
    case portc:
	return PORTC & (1 << pin);
    }

    return 0;
}

/* Waits for PINx to change value */
void wait_for(port p, int pin) {
    int old_val = read(p, pin);

    while (1) {
	if (old_val != read(p, pin)) {
	    break;
	}
    }
}
