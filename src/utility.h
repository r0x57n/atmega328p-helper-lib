
typedef enum { portb, portd, portc } port;
typedef enum { p_set = 1, p_unset = 0, p_toggle } action;
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
