#include "cryotimer.h"
#include "em_cryotimer.h"
#include "cmu.h"
#include "gpio.h"
#include "em_gpio.h"
#include "capsense.h"
#include "em_letimer.h"
#include "em_lesense.h"
#include "capsense.c"
#include "si7021.h"
//***********************************************************************************
//	Function: cryotimer_init
//	Inputs:
//
//	Initializes the cryotimer peripheral, and sets the boolean global used to
//	toggle the LETIMER0 in cap_sense
//***********************************************************************************
void cryotimer_init(){
	CRYOTIMER_Init_TypeDef cryotimer_init = CRYOTIMER_INIT_DEFAULT;
	cryotimer_init.enable = false;
	cryotimer_init.osc = cryotimerOscULFRCO;
	cryotimer_init.presc = cryotimerPresc_1; 		//divide clock by 1
	cryotimer_init.period = cryotimerPeriod_1k;		//interrupt after 1000 clock cycles. 1 second.
	CRYOTIMER_Init(&cryotimer_init);
	CRYOTIMER->IEN |= CRYOTIMER_IEN_PERIOD;
	NVIC_EnableIRQ(CRYOTIMER_IRQn);
	CAPSENSE_Init();
	just_toggled = false;
	CRYOTIMER_Enable(true);
}//end cryotimer_init
//***********************************************************************************
//	Function: cap_sense
//	Inputs:
//
//	turns the LETIMER off and on based on global variable "just_toggled" and if capacitor button0
//	is pressed. If the LETIMER was running, extra considerations to reset the count must
//	be taken to ensure proper operation if it is started up again
//***********************************************************************************
void cap_sense(){
	CAPSENSE_Sense();
	if (!(CAPSENSE_getPressed(BUTTON0_CHANNEL)) && just_toggled ) 
		just_toggled = false;
	if((CAPSENSE_getPressed(BUTTON0_CHANNEL)) && !just_toggled )
	{
		if(LETIMER0->STATUS == 1)					//status = 1 when letimer is running
		{ 
			if(((LETIMER0->COMP0) < (LETIMER0->CNT)) && ((LETIMER0->CNT) < (LETIMER0-> COMP1)))
				LPM_Disable();
			LETIMER_Enable(LETIMER0, false);
			LETIMER0->CMD = LETIMER_CMD_CLEAR; 		//resets the letimer count. This worked better for me than directly setting CNT to 0
			while(LETIMER0->SYNCBUSY);
			just_toggled = true;
		}
		else if(LETIMER0->STATUS == 0 )
		{
			LETIMER_Enable(LETIMER0, true);
			while(LETIMER0->SYNCBUSY);
			just_toggled = true;
		}
	}
	Sensing = false;
}//end cap_sense
//***********************************************************************************
//	Function: CRYOTIMER_IRQHandler
//	Inputs:
//
//	Sets the global variable "Sensing", that will start the process to check if a capacitor button
//	is being pressed the next time "Sensing" is checked in main.c
//***********************************************************************************
void CRYOTIMER_IRQHandler(){
	uint32_t int_flag;
	int_flag = (CRYOTIMER->IF & CRYOTIMER->IEN);
	CRYOTIMER->IFC = int_flag;
	if (int_flag & CRYOTIMER_IF_PERIOD)
		Sensing = true;
	}//end CRYOTIMER_IRQHandler







