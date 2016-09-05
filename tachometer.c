#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "bluetooth.h"
#include "tachometer.h"
#include "config.h"
#include "timer.h"
#include <avr/interrupt.h>

#define EDGES 1
#define CYCLES 64

volatile unsigned int overflows;
volatile unsigned char cycle;

volatile unsigned int ICR1_A;
volatile unsigned int ICR1_B;

extern volatile unsigned int angle;

ISR(TIMER1_CAPT_vect){
    static unsigned int ICR1_Current;
    
    ICR1_Current = ICR1;
    
    angle = 0;
    
    if(cycle == 0){
    
        TIFR |= (1<<TOV1);
        TIMSK |= (1<<TOIE1);

        overflows = 0;
        ICR1_A = ICR1_Current;
        
    } else if(cycle ==  CYCLES * EDGES - 1){

        TIMSK &= ~( (1<<TICIE1) | (1<<TOIE1) );
        ICR1_B  = ICR1_Current;
       
        unsigned long int ticks = (65536 * overflows - ICR1_A + ICR1_B) / CYCLES;
        Timer_set(ticks);
    }

    cycle++;
}

ISR(TIMER1_OVF_vect){
    overflows++;
}

void Tachometer_run(void){
    cycle = 0;
    overflows = 0;
    TIMSK |= (1<<TICIE1);
}

void Tachometer_init(void){
    PORTE |= (1<<PE0);
    TCCR1A = 0;
    TCCR1B = (1<<CS10);
}