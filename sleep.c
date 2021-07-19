/* This code is originally Silicon Labs and copy righted by Silicon Labs’ in 2015 and Silicon Labs’ grants
 * permission to anyone to use the software for any purpose, including commercial applications, and to alter
 * it, and redistribute it freely subject that the origins is not miss represented, altered source version must
 * be plainly marked, and this notice cannot be altered or removed from any source distribution.
 *
 * Names of routines have been changed to conform to the naming convention of the application developer.
 *
 * Routines include:
 *
 * void Enter_Sleep();
 * void blockSleepMode(uint32_t EM);
 * void unblockSleepMode(uint32_t EM);
 *
 */
#include "main.h"
#include "em_emu.h"
#include "sleep.h"
#include "em_core.h"
//***********************************************************************************
//	Function: Enter_Sleep
//	Inputs:
//
//	Puts the PG to sleep based on the sleepMode[] state array, which is set in
//	the blockSleepMode and unblockSleepMode functions.
//***********************************************************************************
void Enter_Sleep(){
	if(sleepMode[EM0] > 0)
	{
		return;
	}
	else if(sleepMode[EM1] > 0)
	{
		return;
	}
	else if(sleepMode[EM2] > 0)
	{
		EMU_EnterEM1();
		return;
	}
	else if( sleepMode[EM3] > 0)
	{
		EMU_EnterEM2(false);
		return;
	}
	else
	{
		EMU_EnterEM3(false);
		return;
	}
}//end Enter_Sleep
//***********************************************************************************
//	Function: blockSleepMode
//	Inputs: uint32_t EM
//
//	blocks the sleep mode (input EM) in the sleepMode[] state array.
//	Initially called with the LOWEST_ALLOWED_SLEEP_MODE constant defined in main.h
//***********************************************************************************
void blockSleepMode(uint32_t EM){
	CORE_ATOMIC_IRQ_DISABLE();
	sleepMode[EM]++;
	CORE_ATOMIC_IRQ_ENABLE();
}//end blockSleepMode
//***********************************************************************************
//	Function: unblockSleepMode
//	Inputs: uint32_t EM
//
//	unblocks the sleep mode (input EM) in the sleepMode[] state array
//***********************************************************************************
void unblockSleepMode(uint32_t EM){
	CORE_ATOMIC_IRQ_DISABLE();
	if(sleepMode[EM] > 0)
		sleepMode[EM]--;
	CORE_ATOMIC_IRQ_ENABLE();
}//end unblockSleepMode

