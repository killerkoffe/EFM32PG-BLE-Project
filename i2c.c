#include "em_i2c.h"
#include "em_gpio.h"
#include "em_device.h"
#include "gpio.h"
//***********************************************************************************
//	Function: i2c0_init()
//	Inputs:
//
//	Initializes the  Inter-Integrated Circuit Interface (I2C) peripheral
//***********************************************************************************
void i2c0_init(){
	I2C_Init_TypeDef i2c0_init;
 	i2c0_init.enable =	true;                  // Enable when initialization done.           
	i2c0_init.master =	true;                  // Set to master mode.
	i2c0_init.freq	=	I2C_FREQ_FAST_MAX;     // Use currently configured reference clock.
	i2c0_init.refFreq = 0;
	i2c0_init.clhr =i2cClockHLRAsymetric;
	I2C_Init(I2C0, &i2c0_init);
	for (int i=0;i<9;i++) 
	{
		GPIO_PinOutClear(I2C0_SCL_port, I2C0_SCL_pin);
		GPIO_PinOutSet(I2C0_SCL_port, I2C0_SCL_pin);
	}
	I2C0->CMD = I2C_CMD_ABORT;
	I2C0->ROUTELOC0 |= I2C_ROUTELOC0_SDALOC_LOC15 | I2C_ROUTELOC0_SCLLOC_LOC15;
	I2C0->ROUTEPEN  |= I2C_ROUTEPEN_SCLPEN | I2C_ROUTEPEN_SDAPEN;
	I2C0->IFC = I2C_IFC_ACK;
	I2C0->IEN = I2C_IEN_ACK | I2C_IEN_RXDATAV;
}//end i2c0_init()


