//***********************************************************************************
// Include Files
//***********************************************************************************
//***********************************************************************************
// Definitions
//***********************************************************************************
//***********************************************************************************
// Global Variables
//***********************************************************************************
//***********************************************************************************
// Function Prototypes
//***********************************************************************************
void si7021_init();
void i2cWrite();
uint32_t i2cRead();
void LPM_Enable();
void LPM_Disable();
float sensorMeasurementTemp();