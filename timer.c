#include <avr/interrupt.h>
#include <stdio.h>
#include "config.h"
#include "timer.h"
#include "leds.h"
#include "tachometer.h"
#include "bluetooth.h"

extern volatile unsigned int angle;

void Timer_init(void){
    TCCR0 |= (1<<WGM01);
    TIMSK |= (1<<OCIE0);
}

void Timer_start(void){
    TCCR0 |= (1<<CS01)|(1<<CS00);
}

void Timer_stop(void){
    TCCR0 &= ~((1<<CS01)|(1<<CS00));
}

void Timer_set(unsigned long int ticks){
    Timer_stop();
    uint8_t val = ((ticks/ANGLES/64) & 255);
    if(val != 0){
        OCR0 = val;
    }
    Timer_start();
    Tachometer_run();
}

ISR(TIMER0_COMP_vect){
    if(angle >= ANGLES-1){
        angle = 0;
    } else {
        angle++;
    }
}