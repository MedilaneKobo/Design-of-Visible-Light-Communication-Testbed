/*
 * LIFI_TX.c
 *
 * Created: 2022/09/07 15:49:20
 * Author : MARY MALUNGANA
 */ 

/*
 * TransmitCode.c
 *
 * Created: 2022/09/04 19:06:04
 * Author : MEDILANE KOBO
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LED 6
#define BUTTON_PIN A0
#define PERIOD 2.5

//#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 8UL))) - 1)


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



void send_byte(char my_byte)
{
	PORTD &= ~(1<<LED);
	_delay_ms(PERIOD);

	//transmission of bits
	for(int i = 0; i < 8; i++)
	{
		
		if ((my_byte&(0x01 << i))!= 0)
		{
			PORTD |= 1<<LED;
		}
		else
		PORTD &= ~(1<<LED);
		
		_delay_ms(PERIOD);
	}

	PORTD |= (1<<LED);
	_delay_ms(PERIOD);

}

int main(void)
{
	char* string ="DEAR MAMA\n";
	int string_length;
	
	string_length = strlen(string);
	
	DDRD |=1<<LED;
	
	
	while (1)
	{
		
		
			for(int i = 0; i < string_length; i ++)
			{
				send_byte(string[i]);
			}
			//_delay_ms(2000);
				
	}
}