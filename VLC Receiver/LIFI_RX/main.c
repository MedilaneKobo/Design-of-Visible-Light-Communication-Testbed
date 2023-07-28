/*
 * LIFI_RX.c
 *
 * Created: 9/7/2022 4:52:32 PM
 * Author : maputle
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
//#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>



#define THRESHOLD 2
#define PERIOD 1.024
#define PD_PIN 0

//volatile int setBit;//=false;

/*ISR (INT0_vect)
{
	setBit = 1;
	_delay_ms(PERIOD/10);
	setBit=0;
}*/

/*ISR(INT1_vect)
{
	resetBit=true;
}*/

void ADC_Init()
{
	DDRA =0x0;        //set PORTA as input
	ADMUX= (1<<REFS0);      //Vref: AVCC
	ADCSRA = (1<<ADEN) | (1<<ADPS0);
}

bool ADC_Read(int channel)
{
	int Logic;
	channel &= 0x03;
	ADMUX= (ADMUX& 0xFC) | channel;    //Set as an input
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC))   ;       //start conversion
	Logic = ADC*5/1024;
	
	return Logic > THRESHOLD ? true : false;
}


//#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)


void UART_init(long USART_BAUDRATE)
{
	UCSRB |= (1 << RXEN) | (1 << TXEN);// Turn on transmission and reception
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);// Use 8-bit character sizes
	UBRRL = BAUD_PRESCALE;		// Load lower 8-bits of the baud rate value
	UBRRH = (BAUD_PRESCALE >> 8);	// Load upper 8-bits
}

unsigned char UART_RxChar()
{
	while ((UCSRA & (1 << RXC)) == 0);/* Wait till data is received */
	return(UDR);			/* Return the byte*/
}

void UART_TxChar(char ch)
{
	while (! (UCSRA & (1<<UDRE)));	/* Wait for empty transmit buffer*/
	UDR = ch ;
}

void UART_SendString(char *str)
{
	unsigned char j=0;
	
	while (str[j]!=0)		/* Send string till null */
	{
		UART_TxChar(str[j]);
		j++;
	}
}


char get_byte()
{
	char ret = 0;
	_delay_ms(PERIOD*1.2);
	for(int i = 0; i < 8; i++)
	{
		ret = ret | ADC_Read(PD_PIN) << i;
		//ret = ret | setBit << i;
		_delay_ms(PERIOD);
	}
	return ret;
}

void print_byte(char my_byte)
{
	char buff[194];
	sprintf(buff, "%c", my_byte);
	UART_SendString(buff);
}

int main(void)
{   
	//DDRD |= (0<<PD_PIN);
	//PORTD= (1<<PD_PIN);
	
	UART_init(57600);
	ADC_Init();
	
	bool previous_state=true;
	bool current_state;
	
	/*GICR = 1<<INT0 ;    //Enable Interrupt
	MCUCR |= (1<<ISC01) | (1<<ISC00);   //Trigger on rising edge
	sei();
	*/
	
    while (1) 
    {			
			
				current_state = ADC_Read(PD_PIN);
				//current_state = setBit;
				if(!current_state && previous_state)
				{
					print_byte(get_byte());
				}
				previous_state = current_state;
				
				//if(setBit==false)
				//{
				//	print_byte(get_byte());
				//}
				//setBit=true;
	
    }
}

