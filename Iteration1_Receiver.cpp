/*
 * Iteration1_reciever.cpp
 *
 * Created: 31/03/2020 9:37:50 PM
 * Author : Matt
 */ 

#include <avr/io.h>


int main(void)
{
    
	DDRB = 0xff;
    DDRC = 0x00;
   
	PORTB = 0x00;
    PORTC = 0xff;
    
    while(1)
    { 
		int a = PINC;
		if(a & 1<<0)
		{
			
			PORTB = 0x81; // off state
		}
		else
		{
			PORTB = 0x7E; // on state
		}
    }
    return 0;
}

