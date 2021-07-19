#include "letimer0.h"
#include "em_letimer.h"
#include "em_cmu.h"
#include "gpio.h"
#include "main.h"
#include "si7021.h"
#include "leuart.h"
#include "ldma.h"
//***********************************************************************************
//	Function: letimer0_init
//	Inputs:
//
//	initializes the low energy timer peripheral, the setup persists down to EM3.
//***********************************************************************************
void letimer0_init(void){
	uint32_t vcomp0, vcomp1;
	uint32_t prescale = 0u;
	uint32_t prescale_clk;
	uint32_t prescale_freq;
	LETIMER_Init_TypeDef	letimer0_init;
	blockSleepMode(LOWEST_ALLOWED_EM +1);
	if(LOWEST_ALLOWED_EM < EM3)							// setup prescale
		prescale_freq = LFXO_FREQ;
	else
		prescale_freq= ULFRCO_FREQ;
	prescale_clk = LETIMER_PERIOD*prescale_freq-1u;
	while(prescale_clk > 65535u)
	{
		prescale_clk = prescale_clk >> 1u;
		prescale_freq = prescale_freq >> 1u;
		prescale++;
	}
	if(prescale > 0)
		prescale = 1u << prescale; 						// 2^ prescale. Modifying the prescale variable to work properly with CMU_ClockPrescSet
	CMU_ClockPrescSet(cmuClock_LETIMER0, prescale ); 	// second input is the number you divide by, not the actual prescale itself. 
	vcomp0 = prescale_freq*LETIMER_PERIOD -1u;
	vcomp1 = vcomp0-(prescale_freq * (LED_ON_TIME));
	letimer0_init.bufTop = false;
	letimer0_init.comp0Top = true;
	letimer0_init.debugRun = false;
	letimer0_init.enable = 0;
	letimer0_init.out0Pol =0;
	letimer0_init.repMode = letimerRepeatFree;
	letimer0_init.topValue = 0;
	letimer0_init.ufoa0 = letimerUFOANone;
	letimer0_init.ufoa1 = letimerUFOANone;
	LETIMER_Init(LETIMER0, &letimer0_init);
	LETIMER0->IFC = LETIMER_IFC_COMP0 | LETIMER_IFC_COMP1;
	LETIMER0->IEN = LETIMER_IEN_COMP0 | LETIMER_IEN_COMP1;
	LETIMER_CompareSet(LETIMER0,0,vcomp0);
	LETIMER_CompareSet(LETIMER0,1,vcomp1);
	NVIC_EnableIRQ(LETIMER0_IRQn);
}//end letimer0_init
//***********************************************************************************
//	Function: LETIMER0_IRQHandler
//	Inputs:
//
//	Controls the functionality of interrupts COMP0 and COMP1
//***********************************************************************************
void LETIMER0_IRQHandler(void){
	uint32_t int_flag;
	int_flag = LETIMER_IntGet (LETIMER0);
	LETIMER0->IFC = int_flag;
	if((int_flag & LETIMER_IFC_COMP0) != false)
		LPM_Enable();
	else if((int_flag & LETIMER_IFC_COMP1) != false)
	{
		for (int i=0;i<9;i++) 
		{
			GPIO_PinOutClear(I2C0_SCL_port, I2C0_SCL_pin);
			GPIO_PinOutSet(I2C0_SCL_port, I2C0_SCL_pin);
		}
		I2C0->CMD = I2C_CMD_ABORT;
		tempCelsius=sensorMeasurementTemp();
		update_temp_var();
		LEUART0->CTRL |= LEUART_CTRL_TXDMAWU;
		LPM_Disable();
		ldma_TX();
	}
	unblockSleepMode(LOWEST_ALLOWED_EM +1);
}//end LETIMER0_IRQHandler