//***********************************************************************************
// Include Files
//***********************************************************************************
#include <stdint.h>
#include <stdbool.h>
//***********************************************************************************
// Definitions
//***********************************************************************************
enum State{EM0,EM1,EM2,EM3,EM4};// Only way to wake from EM4 is with the reset button.
#define LOWEST_ALLOWED_EM EM3	// EM3 is the lowest that can still wake from interrupts.
#define MAX_MODES 5
//***********************************************************************************
// Global Variables
//***********************************************************************************
uint32_t sleepMode[MAX_MODES];
uint32_t readchk;
uint8_t TX_en;
float tempCelsius;
float tempFahrenheit;
//***********************************************************************************
// Function Prototypes
//***********************************************************************************
