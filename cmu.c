#include "cmu.h"
#include "main.h"
#include "em_cmu.h"
//***********************************************************************************
//	Function: cmu_init
//	Inputs:
//
//	Initializes the Clock Management Peripheral
//***********************************************************************************
void cmu_init(void){
	CMU_ClockSelectSet(cmuClock_HFPER, cmuSelect_HFRCO);
	CMU_OscillatorEnable(cmuOsc_HFRCO, true, true);
	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_OscillatorEnable(cmuOsc_LFRCO, false, false);		// By default, LFRCO is enabled, but we want to use LFXO instead
	CMU_OscillatorEnable(cmuOsc_ULFRCO, true, true);		
	CMU_OscillatorEnable(cmuOsc_LFXO, true, true);			
	CMU_HFXOAutostartEnable(true,false,false);
	if(LOWEST_ALLOWED_EM < EM3)								// Route LF clock to the LFA clock tree 
		CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);	
	else
		CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO);	 
	CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);		// Route LF clock to the LFB clock tree
	CMU_ClockEnable(cmuClock_LFB, true);
	CMU_ClockEnable(cmuClock_LFA, true);
	CMU_ClockEnable(cmuClock_CORELE, true);
	CMU_ClockEnable(cmuClock_GPIO, true);					// Peripheral clocks enabled
	CMU_ClockEnable(cmuClock_LETIMER0, true);
	CMU_ClockEnable(cmuClock_I2C0, true);
	CMU_ClockEnable(cmuClock_LEUART0, true);
	CMU_ClockEnable(cmuClock_CRYOTIMER, true);
}//end cmu_init



