#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "config.h"
#include "tachometer.h"
#include "bluetooth.h"
#include "memory.h"
#include "timer.h"
#include "leds.h"

volatile uint8_t *data;
volatile unsigned int angle = 0;
volatile unsigned int prev = 0;

int main(void)
{
	Leds_init();
	Memory_init();
    Bluetooth_init();
	Tachometer_init();
	Timer_init();
	
	sei();
	
	Tachometer_run();
	
	while(1){
		if(angle != prev){
			prev = angle;
			Leds_set((uint8_t*) &data[angle * LEDS_NUM * 3]);
		}
	}
	
	return 0;
}