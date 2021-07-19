#include <stdbool.h>
#include "em_leuart.h"
#include "leuart.h"
#include "main.h"
#include "em_core.h"
#include "ldma.h"
#define ASCII_AT 			0x4154
#define ASCII_A 			0x41
#define ASCII_T				0x54
#define ASCII_NAME 			0x4E414D45
#define ASCII_ffll			0x6B796B69
#define TEMP_BUFFER_SIZE 	11
//***********************************************************************************
//	Function: leuart0_init
//	Inputs:
//
//	Initializes the Low Energy UART peripheral
//***********************************************************************************
void leuart0_init(){
	LEUART_Init_TypeDef leuart_init;
	leuart_init.enable = false;
	leuart_init.refFreq = 0;
	leuart_init.baudrate = 9600;
	leuart_init.databits = leuartDatabits8;
	leuart_init.parity = leuartNoParity;
	leuart_init.stopbits = leuartStopbits1;
	LEUART_Init(LEUART0, &leuart_init);
	LEUART0->ROUTELOC0 |= LEUART_ROUTELOC0_RXLOC_LOC18 | LEUART_ROUTELOC0_TXLOC_LOC18;
	LEUART0->ROUTEPEN  |= LEUART_ROUTEPEN_RXPEN | LEUART_ROUTEPEN_TXPEN;
	LEUART0->IEN |= LEUART_IEN_RXOF|LEUART_IEN_SIGF;
	LEUART0->STARTFRAME = 0x3f;
	LEUART0->SIGFRAME = 0x23;
	LEUART_FreezeEnable(LEUART0, true);
	LEUART0->CTRL |= LEUART_CTRL_SFUBRX;
	LEUART0->CMD = LEUART_CMD_RXBLOCKEN;
	LEUART_FreezeEnable(LEUART0, false);
	TXSTATE = TXSGN;
	RXSTATE = RXSTART;
	TX_en = false;
	temp_unit = 'C';
	NVIC_EnableIRQ(LEUART0_IRQn);
}//end leuart0_init
//***********************************************************************************
//	Function: leuart_write_char
//	Inputs: character to write (A)
//
//	writes the character to be sent via leuart0
//***********************************************************************************
void leuart_write_char(char A){
	LEUART0->TXDATA = A;
}//end leuart_write_char
//***********************************************************************************
//	Function: leuart_write_char_LDMA
//	Inputs: character to write (A), position in the TXBuffer to fill
//
//	just to make it clear LDMA is being used for TXBuffer
//***********************************************************************************
void leuart_write_char_LDMA(char A, int32_t pos){
	TXBuffer[pos] = A;
}
//***********************************************************************************
//	Function: leuart_write
//	Inputs: character array (charArr[])
//
//	writes the characters in the character array one at a time to the leuart0.
//	The length of the character array must be defined in the TEMP_BUFFER_SIZE constant.
//***********************************************************************************
void leuart_write(char charArr[]){
	for(int i =0; i< TEMP_BUFFER_SIZE; i++)
		leuart_write_char(charArr[i]);
}//end leuart_write
//***********************************************************************************
//	Function: leuart_loopback
//	Inputs:
//
//	Debug test function
//***********************************************************************************
void leuart_loopback(){
	while(!(LEUART0->IF & LEUART_IF_TXBL));
	LEUART0->TXDATA = ASCII_A;
	while(!(LEUART0->IF & LEUART_IF_TXBL));
	LEUART0->TXDATA = ASCII_T;
	while(!(LEUART0->IF & LEUART_IF_RXDATAV));
	data = LEUART0->RXDATA;
	while(!(LEUART0->IF & LEUART_IF_RXDATAV));
	data = data<<8;
	data = data + LEUART0->RXDATA;	//data value returned is decimal 16724 which is 0x4154 which is ASCII AT
}//end leuart_loopback
//***********************************************************************************
//	Function: leuart_temp
//	Inputs:
//
//	Handles transmission of temperature through polling rather than the interrupt handler.
//***********************************************************************************
void leuart_temp(){
	char temp[TEMP_BUFFER_SIZE];
	for(int i=0; i< TEMP_BUFFER_SIZE-8; i++)
		temp[i]=0x20;
	int32_t tempInt=tempCelsius*10;
	if(tempCelsius > 0)
		temp[TEMP_BUFFER_SIZE-8]= '+';
	else
	{
		temp[TEMP_BUFFER_SIZE-8]= '-';
		tempInt = tempInt*(-1);
	}
	temp[TEMP_BUFFER_SIZE-7] = (tempInt/1000)+0x30;
	if(temp[TEMP_BUFFER_SIZE-7]==0x30)
		temp[TEMP_BUFFER_SIZE-7] = 0x20;
	tempInt =tempInt%1000;
	temp[TEMP_BUFFER_SIZE-6] = (tempInt/100)+0x30;
	if(temp[TEMP_BUFFER_SIZE-6]==0x30)
		temp[TEMP_BUFFER_SIZE-6] = 0x20;
	tempInt=tempInt%100;
	temp[TEMP_BUFFER_SIZE-5] = (tempInt/10)+0x30;
	temp[TEMP_BUFFER_SIZE-4]='.';
	tempInt = tempInt%10;
	temp[TEMP_BUFFER_SIZE-3] = (tempInt) + 0x30;
	temp[TEMP_BUFFER_SIZE-2]='C';
	temp[TEMP_BUFFER_SIZE-1] = '\n';
	leuart_write(temp);
}//end leuart_temp
//***********************************************************************************
//	Function: update_temp_var
//	Inputs:
//
//	Updates the temperature character array temp[], designed to make transmission easier.
//***********************************************************************************
void update_temp_var(){
	tempFahrenheit = tempCelsius*9/5 + 32;
	if(temp_unit == 'F')
		tempInt = 10*tempFahrenheit;
	else
		tempInt = 10*tempCelsius;
	if(tempInt > 0)
		sgn = '+';
	else
	{
		sgn = '-';
		tempInt = tempInt*(-1);
	}
	temp[0] = (tempInt/1000)+0x30;
	tempInt = tempInt%1000;
	temp[1] = (tempInt/100)+0x30;
	tempInt = tempInt%100;
	temp[2] = (tempInt/10)+0x30;
	tempInt = tempInt%10;
	temp[3] = tempInt+0x30;
	if(temp[0]==0x30)
		bit1= ' ';
	else
		bit1 = temp[0];
	if((temp[1]==0x30) && (temp[0]==0x30))
		bit2 = ' ';
	else
		bit2 = temp[1];
	point = '.';
	bit3 = temp [2];
	bit4 = temp[3];
	newline = '\n';
	TXBuffer[0] = sgn;
	TXBuffer[1] = bit1;
	TXBuffer[2] = bit2;
	TXBuffer[3] = bit3;
	TXBuffer[4] = point;
	TXBuffer[5] = bit4;
	TXBuffer[6] = temp_unit;
	TXBuffer[7] = newline;
}//end update_temp_var
//***********************************************************************************
//	Function: leuart_read
//	Inputs:
//
//	Handles the execution of commands received
//	Currently the only ones are display farhenheit (df) case insensitive, and
//	display celcius (dc) case insensitive. Commands are recognized by starting with '?'
//	and ending with '#'. A valid command would be sent as: ?df# or ?dc# 
//***********************************************************************************
void leuart_read(){
	if ((read[2]=='f')||(read[2]=='F'))
		temp_unit = 'F';
	else if((read[2]=='c')||(read[2]=='C'))
		temp_unit = 'C';
	update_temp_var();
	clear_read();
	ldma_RX();
}//end leuart_read
//***********************************************************************************
//	Function: clear_read
//	Inputs:
//
//	Clears the read[] character array defined in leuart.h
//***********************************************************************************
void clear_read(){
	for(int i=0; i<READ_SIZE; i++ )
		read[i] = RX_EMPTY;
}//end clear_read
//***********************************************************************************
//	Function: decode_commad
//	Inputs:
//
//	checks the RXBuffer character array used by DMA for valid commands, then updates
//	the read character array buffer with the command. leuart_read() handles the command
//***********************************************************************************
void decode_command(){
	for (int i=0; (i< RXBUFFER_SIZE) && (read[3]!='#'); i++)
	{
		switch(RXSTATE)
		{
			case RXSTART:
				read[0] = RXBuffer[i];
				if(read[0] != '?')
				{
					clear_read();
					RXSTATE=RXSTART-1;
				}
				break;
			case RXC1:
				read[1]= RXBuffer[i];
				if(!((read[1]== 'D' )|| (read[1]== 'd')))
					RXSTATE--;
				break;
			case RXC2:
				read[2]= RXBuffer[i];
				if(!((read[2]== 'C' )|| (read[2]== 'c')|| (read[2]== 'F')|| (read[2]== 'f')))
				{
					clear_read();
					RXSTATE=RXSTART-1;
				}
				break;
			case RXSTOP:
				read[3] = RXBuffer[i];
				if(read[3] != '#')
					RXSTATE--;
				break;
		}
		RXSTATE++;
	}//end for loop
	if (read[3] == '#')
	{
		leuart_read();
		RXSTATE = RXSTART;
	}
	else
	{
		clear_read();
		RXSTATE=RXSTART;
	}
}//end decode_command
//***********************************************************************************
//	Function: update_ldma_TXBuffer
//	Inputs: 
//
//	Updates the TXBuffer using the temperature value sensed (temp[])
//	by calling leuart_write_char_LDMA()
//***********************************************************************************
void update_ldma_TXBuffer(){
	bool isDone = false;
	while (!isDone)
	{
		switch(TXSTATE)
		{
			case TXSGN:
				leuart_write_char_LDMA(sgn,TXSTATE);
				break;
			case TXBIT1:
				if(temp[0]==0x30)
					leuart_write_char_LDMA(' ',TXSTATE);
				else
					leuart_write_char_LDMA(temp[0],TXSTATE);
				break;
			case TXBIT2:
				if((temp[1]==0x30) && (temp[0]==0x30))
					leuart_write_char_LDMA(' ',TXSTATE);
				else
					leuart_write_char_LDMA(temp[1],TXSTATE);
				break;
			case TXBIT3:
					leuart_write_char_LDMA(temp[2],TXSTATE);
				break;
			case TXPOINT:
				leuart_write_char_LDMA('.',TXSTATE);
				break;
			case TXBIT4:
					leuart_write_char_LDMA(temp[3],TXSTATE);
				break;
			case TXCELCIUS:
				leuart_write_char_LDMA(temp_unit,TXSTATE);
				break;
			case TXNEWLINE:
				leuart_write_char_LDMA('\n',TXSTATE);
				TXSTATE=TXSGN-1;
				break;
		}
		TXSTATE++;
		if (TXSTATE==TXSGN)
			isDone = true;
	}
}
//***********************************************************************************
//	Function: LEUART0_IRQHandler
//	Inputs:
//
//	handles the leuart0 interrupts
//***********************************************************************************
void LEUART0_IRQHandler(){
	uint32_t int_flag;
	int_flag = (LEUART0->IF & LEUART0->IEN);
	LEUART0->IFC = LEUART_IFC_SIGF | LEUART_IFC_RXOF| LEUART_IFC_TXC;
	if(int_flag & LEUART_IF_RXOF)
		clear_read();
	if (int_flag & LEUART_IF_SIGF)
		decode_command();
	if (int_flag & LEUART_IF_TXC)
	{
		LEUART0->IEN &= ~LEUART_IEN_TXC;
		unblockSleepMode(EM3);
	}
}//end LEUART0_IRQHandler
