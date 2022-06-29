#include <util/delay.h>
#include <utility.h>

#define __AVR_ATmega328P__ 1
#include <avr/io.h>

// Hex values for a common anode 7 segment display, order (DP)GFEDCBA, from 0-F(+DP)
const uint8_t DISPLAY_VALUES[17] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
                                    0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E, 0x0E};

int main()
{
  ShiftRegister sr = {PORTC4, PORTC3, PORTC2, PORTC1, PORTC0, portc};
  sr_init(&sr);
  sr_reset(&sr);

  DistanceSensor ds = {PORTB0, PORTB1, portb};
  ds_init(&ds);
  ds_measure(&ds);

  while (1) {
    int distance = ds_measure(&ds);

    if (distance > 16)
      sr_push_val(&sr, DISPLAY_VALUES[16]);
    else
      sr_push_val(&sr, DISPLAY_VALUES[distance]);

    _delay_ms(10);
  }
}
