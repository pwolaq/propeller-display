#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "config.h"
#include "leds.h"
#include "timer.h"
#include "tachometer.h"
#include "bluetooth.h"

extern volatile uint8_t *data;
volatile char *usart_char;
volatile unsigned int _index = 0;
volatile uint8_t progress[LEDS_NUM*3];
volatile unsigned char percent;

void Bluetooth_init(void)
{
    #define BAUD 9600
    #include <util/setbaud.h>
    
    UCSRB = (1<<TXEN) | (1<<RXEN) | (1<<RXCIE); 
    UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);
     
    UBRRH = UBRRH_VALUE;
    UBRRL = UBRRL_VALUE;
    
    Bluetooth_reset();
}

void Bluetooth_send(char *buffer){
    while (!(UCSRA & (1<<UDRE))); 

    usart_char = buffer;
    
    UCSRB |= (1<<UDRIE);
}

void Bluetooth_reset(void){
    percent = 0;
    
    for(int i=0;i<LEDS_NUM*3;i+=3){
        progress[i] = 0;
        progress[i+1] = 31;
        progress[i+2] = 0;
    }
    
    Leds_set( (uint8_t*) progress );
}

ISR(USART_UDRE_vect){
    if(*usart_char != 0){
        UDR = *(usart_char++);
    } else {
        UCSRB &= ~(1<<UDRIE);
    }
}


ISR(USART_RX_vect)
{
    int step = ANGLES*3;
    
    uint8_t received;
    received = UDR;
    
    if(_index == 0){
        Bluetooth_reset();
        Timer_stop();
    }
    
    data[_index++] = received;
    
    if(_index >= step * percent){
        uint8_t *ptr = (uint8_t*) &progress[percent*3];
        *(ptr++) = 31;
        *(ptr++) = 0;
        *ptr = 0;
        percent++;
        Leds_set((uint8_t*) progress);
    }
    
    if(_index == DATA_SIZE){
        _index = 0;
        _delay_us(10);
        Timer_start();
    }
}