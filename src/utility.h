#ifndef UTILITYHEADER_INCLUDED
#define UTILITYHEADER_INCLUDED

typedef enum { portb, portd, portc } port;
typedef enum { p_set = 1, p_unset = 2, p_toggle = 3 } action;
typedef enum { output = 1, input = 0 } type;

void set(port p, int pin);
void unset(port p, int pin);
void toggle(port p, int pin);

void make_output(port p, int pin);
void make_input(port p, int pin);
void make_outputs(port p, int from, int to);
void make_inputs(port p, int from, int to);
void make_pullup(port p, int pin);

int read(port p, int pin);
void wait_btn(port p, int pin);

/* Shift register  */

typedef struct {
  int MR;
  int SH_CP;
  int ST_CP;
  int OE;
  int DS;
  int port;
} ShiftRegister;

void sr_step(ShiftRegister *sr);
void sr_step_times(ShiftRegister *sr, int times);
void sr_toggle_output(ShiftRegister *sr);
void sr_reset(ShiftRegister *sr);

#endif
