/*
 * It2_reciever_impTester.cpp
 *
 * Created: 1/05/2020 10:56:13 PM
 * Author : Matt
 */ 

#include <avr/io.h>

uint8_t TakeSample (uint8_t Sample)
{
	TCNT0 = 0xE8;
	TCCR0 = 0x03;
	uint8_t timerOverflowCount = 0;
	while (timerOverflowCount < Sample)
	{
		while ((TIFR & 0x01) == 0);
		TCNT0 = 0xE8;
		TIFR=0x01;
		timerOverflowCount++;
	}
	uint8_t i = PINC;
	if (i != 0)
	{
		i = 1;
	}
	
	return i;
}

void sync (uint8_t Sample)
{
	uint8_t s1 = 1; //current sample
	uint8_t s2 = 1; //previous sample
	while (s1 <= s2)
	{
		s2 = s1;
		
		s1 = TakeSample (Sample);
	}
}

uint8_t bitread (uint8_t NumSam,uint8_t Sample)
{
	uint8_t temp[NumSam] = {0};
	for (volatile uint8_t a=0;a<NumSam;a++)
	{
		temp[a] = TakeSample (Sample);
	}
	
	for (uint8_t a=0;a<NumSam;a++)
	{
		if(temp[a] == 1)
		{
			if(temp[a + 1] == 0)
			{
				return a + 1;
				a = NumSam;
			}
		}
	}
}

int main(void)
{
    DDRB = 0xFF;
    PORTB = 0x00;
    DDRC = 0x00;
    PORTC = 0xFF;
	
	uint8_t Sample = 2;
	uint8_t Range = 2;
	
	uint8_t T = 40;
	uint8_t Low = 30;
	uint8_t Null = 20;
	uint8_t High = 10;
	
	uint8_t Ns = T/Sample;
	uint8_t Nn = 3;
	uint8_t Nb = 8;
	
	uint8_t Nullpoint = Null/Sample;
	uint8_t Highpoint = High/Sample;
	uint8_t Lowpoint = Low/Sample;
	
	uint8_t a;
	uint8_t i;
	uint8_t data;
	
	uint8_t exval = 0x34;
	
	
    while (1) 
    {
		PORTB = 0x00;
		//////////////////////////////////////////////////////////////////////////
		/////////////////////////////Synchronize//////////////////////////////////
		sync(Sample);
		
		//////////////////////////////////////////////////////////////////////////
		/////////////////////////////Detect Nulls/////////////////////////////////
		i = 0;
		while (i < Nn)
		{
			//PORTB = 0x01; //debugging
			a = bitread (Ns, Sample);
			if (a >= Nullpoint - Range && a <= Nullpoint + Range)
			{
				i++;
			}
			else
			{
				i = 0;
			}
			
		}
		
		//////////////////////////////////////////////////////////////////////////
		/////////////////////////////Read Data////////////////////////////////////
		
		PORTB = 0x00;
		i = 0;
		data = 0x00;
		while (i < Nb)
		{
			//PORTB = 0x02; //debugging
			a = bitread (Ns, Sample);
			if (a >= (Highpoint - Range) && a <= (Highpoint + Range))
			{
				data = data | (0x01 << i);
				//PORTB = 0b10101010; //debugging
				i++;
			}else if (a >= (Lowpoint - Range) && a <= (Lowpoint + Range))
			{
				//PORTB = 0b01010101; //debugging
				i++;
			}else if (a >= (Nullpoint - Range) && a <= (Nullpoint + Range))
			{
				//i = 0;
				if (i > 0)
				{
					i=Nb+10;
					PORTB = 0b10000001; //debugging
				}
			}else
			{
				i = Nb+10; //read failed, exit
				PORTB = 0b10000001; //debugging
			}
			
			
		}
		PORTB = 0x00;
		//////////////////////////////////////////////////////////////////////////
		///////////////////Test Data Against Expected Value///////////////////////
		if (i == Nb)
		{
						
			if (data == exval)
			{
				
				PORTB = 0b10101010;
				
				
			}
			else
			{
				PORTB = 0b01010101;
				
			}
		}
		//////////////////////////////////////////////////////////////////////////
		/////////////////////////////////Delay////////////////////////////////////
		for(i=0;i<20;i++){bitread(Ns,Sample);}	//delay to show correct output
		//////////////////////////////////////////////////////////////////////////
		////////////////////////////////Restart///////////////////////////////////
    }
	
}

