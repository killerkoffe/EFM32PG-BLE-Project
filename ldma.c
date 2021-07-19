#include "ldma.h"
#include "em_ldma.h"
#include "em_cmu.h"
#include "em_leuart.h"
#include "leuart.h"
#include "main.h"
LDMA_Descriptor_t ldmaTXDescriptor, ldmaRXDescriptor;
LDMA_TransferCfg_t ldmaTXConfig, ldmaRXConfig;
//	Notes:
//	After each transfer, the SRC address increments by the amount set in SRCINC
//	For TX, we do not want to increment the DST. DSTINC should be zero. 
//	We do want to wait for TXBL before the next DMA transfer though.
//	After a complete transfer we will want to reset the SRC address
//	Probably want to set it up as single transfer descriptor, transfer the set amount, then reset.
//	TOTAL_BYTES should be set to TXBUFFER_SIZE
//***********************************************************************************
//	Function: ldma_init
//	Inputs:
//
//	Initializes the LDMA peripheral
//***********************************************************************************
void ldma_init(){
	CMU_ClockEnable(cmuClock_LDMA, true);
	LDMA_Init_t ldma_init = LDMA_INIT_DEFAULT;
	LDMA_Init(&ldma_init);
	LDMA->IEN |= (1 << TXCHANNEL)| (1 << RXCHANNEL);
	NVIC_EnableIRQ(LDMA_IRQn);
	ldma_RX();
}//end ldma_init
//***********************************************************************************
//	Function: ldma_TX
//	Inputs:
//
//	Initializes the LDMA transmit to LEUART from the character array TXBuffer defined
//	in leuart.h. After each transfer the SRC address increments by the 
//	ammount set in SRCINC.
//***********************************************************************************
void ldma_TX(){
	ldmaTXDescriptor = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_SINGLE_M2P_BYTE(TXBuffer, &(LEUART0->TXDATA), TXBUFFER_SIZE);
	ldmaTXConfig = (LDMA_TransferCfg_t)LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_LEUART0_TXBL);
	LDMA_StartTransfer(TXCHANNEL, &ldmaTXConfig, &ldmaTXDescriptor);
}//end ldma_TX
//***********************************************************************************
//	Function: ldma_RX
//	Inputs:
//
//	Initializes the LDMA receive from LEUART, and stores the result in the character
//	array RXBuffer defined in leuart.h.
//***********************************************************************************
void ldma_RX(){
	LEUART0->CTRL |= LEUART_CTRL_RXDMAWU;
	while(LEUART0->SYNCBUSY);
	ldmaRXDescriptor = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_SINGLE_P2M_BYTE(&LEUART0->RXDATA, RXBuffer, RXBUFFER_SIZE);
	ldmaRXConfig = (LDMA_TransferCfg_t)LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_LEUART0_RXDATAV);
	LDMA_StartTransfer(RXCHANNEL, &ldmaRXConfig, &ldmaRXDescriptor);
}//end ldma_RX
//***********************************************************************************
//	Function: LDMA_IRQHandler
//	Inputs:
//
//	Handles the LDMA interrupts.
//***********************************************************************************
void LDMA_IRQHandler(){
	uint32_t int_flag = LDMA_IntGet() & LDMA->IEN;
	if(int_flag & (1 << TXCHANNEL) )
	{
		LDMA_IntClear(1 << TXCHANNEL);
		LEUART0->CTRL &= ~LEUART_CTRL_TXDMAWU;
		LEUART0->IEN |= LEUART_IEN_TXC;
	}
	if(int_flag & (1 << RXCHANNEL) )
	{
		LDMA_IntClear(1 << RXCHANNEL);
		decode_command();
	}
}//end LDMA_IRQHandler

