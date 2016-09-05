#include <avr/io.h>
#include <stdlib.h>
#include "config.h"
#include "memory.h"

extern volatile uint8_t* data;

void Memory_init(void){
	MCUCR |= (1 << SRE);
	SFIOR &= ~((1 << XMM2) | (1 << XMM1) | (1 << XMM0));
	
	data = (uint8_t*) malloc(DATA_SIZE);
}