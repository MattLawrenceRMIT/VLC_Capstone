/*
 * It2_Transmitter.cpp
 *
 * Created: 10/04/2020 2:20:26 PM
 * Author : Matt
 */ 

#include <avr/io.h>


int main(void)

{
	uint8_t timestart = 0xE8;
	volatile int data = 0x34;
    DDRB = 0xff; //config as output
	PORTB = 0x00;
	
	volatile uint8_t N = 8; //size of data in bits
	volatile uint8_t nl = 3; //number of nulls before data starts
	
	volatile uint8_t timerOverflowCount;
	volatile uint8_t i;
	volatile char j;
	volatile uint8_t k;
	
	
	
	
	uint8_t T = 40;
	uint8_t High = 10;
	uint8_t Null = 20;
	uint8_t Low = 30;
	volatile int temp = 0;
	
	while(1)
	{
		
			TCNT0 = timestart; //sets timer start pos to 0
			TCCR0 = 0x03; //sets clock source to clk/256
			timerOverflowCount = 0;
			
			i = 1;
			j = 0b1;
			k = 1;
			while(k<=nl)
			{
				while ((TIFR & 0x01) == 0);
				TCNT0 = timestart;
				TIFR=0x01; //clear timer1 overflow flag
				timerOverflowCount++;
				
				if (timerOverflowCount==Null)
				{
					PORTB = 0x01;
				}
				if (timerOverflowCount==T)
				{
					PORTB = 0x00;
					timerOverflowCount = 0;
					k++;
				}
			}
			
			TCNT0 = timestart;
			while(i<=N)
			{
		
				while ((TIFR & 0x01) == 0);
				TCNT0 = timestart;
				TIFR=0x01; //clear timer1 overflow flag
		
				timerOverflowCount++;
				temp = j&data;
				
						
				if (timerOverflowCount==High)
				{
			
					if(temp > 0x00)
					{	 
						
						PORTB = 0x01;
						
					}
				}
				
				if (timerOverflowCount==Low)
				{
					if(temp == 0x00)
					{ 
						
						PORTB = 0x01;
						
					}
			
				}
				if (timerOverflowCount==T)
				{
					PORTB = 0x00;
					timerOverflowCount = 0;
					j = (0x01<<i);
					i++;
				}
		
			
			
		}
		
		
	
		/*
		PORTB = 0xFF;
		TCNT0 = 0x00;
		timerOverflowCount=0;
		
		
		while(timerOverflowCount < 184)
		{
			while((TIFR & 0x01) == 0);
			TCNT0 = 0x00;
			TIFR = 0x01;
			timerOverflowCount++;
		}
	*/
	
	
	
	
	
	}
	
}

