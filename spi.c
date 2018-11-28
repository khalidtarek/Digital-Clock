 /******************************************************************************
 *
 * Module: SPI
 *
 * File Name: spi.c
 *
 * Description: Source file for the SPI AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#include "spi.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void SPI_initMaster(void) 
{
	/******** Configure SPI Master Pins *********
	 * SS(PB4)   --> Output
	 * MOSI(PB5) --> Output 
	 * MISO(PB6) --> Input
	 * SCK(PB7) --> Output
	 ********************************************/
	SET_BIT(SPI_PORT_DIR,SS);
	SET_BIT(SPI_PORT_DIR,MOSI);
	CLEAR_BIT(SPI_PORT_DIR,MISO);
	SET_BIT(SPI_PORT_DIR,SCK);

	SPCR = (1<<SPE) | (1<<MSTR) | (1<<CPHA) ; // enable SPI + enable Master + choose SPI clock = Fosc/4
}

void SPI_initSlave(void)
{ 
	/******** Configure SPI Slave Pins *********
	 * SS(PB4)   --> Input
	 * MOSI(PB5) --> Input
	 * MISO(PB6) --> Output
	 * SCK(PB7) --> Input
	 ********************************************/
	CLEAR_BIT(SPI_PORT_DIR,SS);
	CLEAR_BIT(SPI_PORT_DIR,MOSI);
	SET_BIT(SPI_PORT_DIR,MISO);
	CLEAR_BIT(SPI_PORT_DIR,SCK);
	SPCR = (1<<SPE); // just enable SPI + choose SPI clock = Fosc/4
}

uint8 SPI_TrancieveByte(const uint8 data)
{
	SPDR = data;
	while(BIT_IS_CLEAR(SPSR,SPIF)){} //wait until SPI interrupt flag=1(data is receive correctly)
	return SPDR; //return the received byte from SPI data register
}


void SPI_sendByte(const uint8 data) 
{
	SPDR = data; //send data by SPI
	while(BIT_IS_CLEAR(SPSR,SPIF)){} //wait until SPI interrupt flag=1 (data is sent correctly)
}

uint8 SPI_recieveByte(void)
{
   while(BIT_IS_CLEAR(SPSR,SPIF)){} //wait until SPI interrupt flag=1(data is receive correctly)
   return SPDR; //return the received byte from SPI data register
}

void SPI_SendString(const uint8 *Str)
{
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		SPI_sendByte(Str[i]);
		i++;
	}
}

void SPI_ReceiveString(char *Str)
{
	unsigned char i = 0;
	Str[i] = SPI_recieveByte();
	while(Str[i] != '#')
	{
		i++;
		Str[i] = SPI_recieveByte();
	}
	Str[i] = '\0';
}
