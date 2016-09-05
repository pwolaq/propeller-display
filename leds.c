#include <avr/io.h>
#include <avr/interrupt.h>
#include "config.h"
#include "leds.h"

void Leds_init(void){
	LEDS_DDR_A |= _BV(LEDS_PIN_A);
	LEDS_DDR_B |= _BV(LEDS_PIN_B);
	LEDS_DDR_C |= _BV(LEDS_PIN_C);
}

void inline Leds_set(uint8_t *data)
{
    uint8_t ctr, reg;
	uint8_t current_a, current_b, current_c;
    
    uint8_t length = LEDS_NUM;
	
    uint8_t hi_a = _BV(LEDS_PIN_A);
    uint8_t hi_b = _BV(LEDS_PIN_B);
    uint8_t hi_c = _BV(LEDS_PIN_C);
	
	uint8_t lo_a = ~hi_a & LEDS_PORT_A;
	uint8_t lo_b = ~hi_b & LEDS_PORT_B;
	uint8_t lo_c = ~hi_c & LEDS_PORT_C;
  
    hi_a |= LEDS_PORT_A;
	hi_b |= LEDS_PORT_B;
	hi_c |= LEDS_PORT_C;
    
    reg = SREG;
	
    cli();

    while(length--){
        current_a = *data;
		current_b = *(data+30);
		current_c = *(data+60);
		
		data++;
    
        asm volatile(
        "    ldi   %0,8              \n\t"
        "loop%=:                     \n\t"
        "    out   %[port_a],%[hi_a] \n\t"
        "    out   %[port_b],%[hi_b] \n\t"
        "    out   %[port_c],%[hi_c] \n\t"
        "    sbrs  %[data_a],7       \n\t"
        "    out   %[port_a],%[lo_a] \n\t"
        "    sbrs  %[data_b],7       \n\t"
        "    out   %[port_b],%[lo_b] \n\t"
        "    sbrs  %[data_c],7       \n\t"
        "    out   %[port_c],%[lo_c] \n\t"
        "    lsl   %[data_a]         \n\t"
        "    lsl   %[data_b]         \n\t"
        "    lsl   %[data_c]         \n\t"
        "    out   %[port_a],%[lo_a] \n\t"
        "    out   %[port_b],%[lo_b] \n\t"
        "    out   %[port_c],%[lo_c] \n\t"
        "    dec   %0                \n\t"
        "    brne  loop%=            \n\t"
        :	"=&d" (ctr)
        :	[data_a] "r" (current_a), [port_a] "I" (_SFR_IO_ADDR(LEDS_PORT_A)), [hi_a] "r" (hi_a), [lo_a] "r" (lo_a),
			[data_b] "r" (current_b), [port_b] "I" (_SFR_IO_ADDR(LEDS_PORT_B)), [hi_b] "r" (hi_b), [lo_b] "r" (lo_b),
			[data_c] "r" (current_c), [port_c] "I" (_SFR_IO_ADDR(LEDS_PORT_C)), [hi_c] "r" (hi_c), [lo_c] "r" (lo_c)
        );
    }
  
    SREG = reg;
}