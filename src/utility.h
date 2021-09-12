#ifndef UTILITYHEADER_INCLUDED
#define UTILITYHEADER_INCLUDED

#include <stdint.h>

typedef enum { portb, portd, portc } Port;
typedef enum { p_set = 1, p_unset = 2, p_toggle = 3 } Action;
typedef enum { output = 1, input = 0 } Type;

void set(Port p, int pin);
void unset(Port p, int pin);
void toggle(Port p, int pin);

void make_output(Port p, int pin);
void make_input(Port p, int pin);
void make_outputs(Port p, int from, int to);
void make_inputs(Port p, int from, int to);
void make_pullup(Port p, int pin);

int read(Port p, int pin);
void wait_btn(Port p, int pin);


/*
 * Shift register (SN74HC595N).
 */

typedef struct {
  int MR;
  int SH_CP;
  int ST_CP;
  int OE;
  int DS;
  Port port;
} ShiftRegister;

void sr_init(ShiftRegister *sr);
void sr_step(ShiftRegister *sr);
void sr_step_times(ShiftRegister *sr, int times);
void sr_toggle(ShiftRegister *sr);
void sr_reset(ShiftRegister *sr);
void sr_push_val(ShiftRegister *sr, uint8_t value);


/*
 * Distance sensor (HY-SRF05).
 */

typedef struct {
  int TRIG;
  int ECHO;
  Port port;
} DistanceSensor;

void ds_init(DistanceSensor *ds);
float ds_measure(DistanceSensor *ds);

#endif
