#include "em_i2c.h"
#include "em_gpio.h"
#include "gpio.h"
#include "main.h"
#define READ_USER_REG1   	0xE7
#define WRITE_USER_REG1   	0xE6
#define CMD_MEASURE_TEMP  	0xE3
#define SI7021_DEVICE_ID   	0x40
#define CMD_WRITE 			0u;
#define CMD_READ  			1u;
//***********************************************************************************
//	Function: si7021_init
//	Inputs:
//
//	The temperature sensor does not require setup.
//***********************************************************************************
void si7021_init(){
}//end si7021_init
//***********************************************************************************
//	Function: i2cRead
//	Inputs:
//
//	Reads and returns user register 1 on the si7021
//***********************************************************************************
uint32_t i2cRead(){
	I2C0->CMD = I2C_CMD_START;
	I2C0->TXDATA = (SI7021_DEVICE_ID << 1) | CMD_WRITE;
	while(!(I2C0->IF & I2C_IF_ACK));
	I2C0->IFC = I2C_IFC_ACK;
	I2C0->TXDATA = READ_USER_REG1;
	while(!(I2C0->IF & I2C_IF_ACK));
	I2C0->IFC = I2C_IFC_ACK;
	I2C0->CMD = I2C_CMD_START;
	I2C0->TXDATA = (SI7021_DEVICE_ID << 1u) + CMD_READ;
	while(!(I2C0->IF & I2C_IF_ACK));
		I2C0->IFC = I2C_IFC_ACK;
	while(!(I2C0->IF & I2C_IF_RXDATAV));
	I2C0->CMD = I2C_CMD_NACK;
	I2C0->CMD = I2C_CMD_STOP;
	return I2C0->RXDATA;
}//end i2cRead
//***********************************************************************************
//	Function: i2cWrite
//	Inputs:
//
//	Uses the I2C protocol to configure the temperature resolution to 14 bits
//***********************************************************************************
void i2cWrite(){
	uint32_t USER_REG1;
	for(int i=0; i<500; i++);
	USER_REG1 = i2cRead();
	for(int i=0; i<500; i++);
	I2C0->IFC = I2C_IFC_ACK;
	I2C0->CMD = I2C_CMD_START;
	I2C0->TXDATA = (SI7021_DEVICE_ID << 1u) + CMD_WRITE;
	while(!(I2C0->IF & I2C_IF_ACK));
	I2C0->IFC = I2C_IFC_ACK;
	for(int i=0; i<500; i++);
	I2C0->TXDATA = WRITE_USER_REG1;
	while(!(I2C0->IF & I2C_IF_ACK));
	for(int i=0; i<500; i++);
	I2C0->IFC = I2C_IFC_ACK;
	I2C0->TXDATA = (USER_REG1 | (0u<<7u & 0u));
	I2C0->CMD = I2C_CMD_ACK;
	I2C0->CMD = I2C_CMD_STOP;
}//end i2cWrite
//***********************************************************************************
//	Function: LPM_Enable
//	Inputs:
//
//	Powers on the Temperature sensor and enables the I2C SDA and SCL ports in the GPIO
//***********************************************************************************
void LPM_Enable(){
	GPIO_PinModeSet(I2C0_SDA_port,I2C0_SDA_pin, gpioModeWiredAnd, 1);
	GPIO_PinModeSet(I2C0_SCL_port,I2C0_SCL_pin, gpioModeWiredAnd, 1);
	GPIO_PinModeSet(SENSOR_ENABLE_port, SENSOR_ENABLE_pin, gpioModePushPull, 1);
}//end LPM_Enable
//***********************************************************************************
//	Function: LPM_Disable
//	Inputs:
//
//	Turns off the sensor pin and I2C SDA and SCL ports in the GPIO
//***********************************************************************************
void LPM_Disable(){
	GPIO_PinModeSet(I2C0_SDA_port,I2C0_SDA_pin, gpioModeDisabled, 0);
	GPIO_PinModeSet(I2C0_SCL_port,I2C0_SCL_pin, gpioModeDisabled, 0);
	GPIO_PinModeSet(SENSOR_ENABLE_port, SENSOR_ENABLE_pin, gpioModePushPull, 0);
}//end LPM_Disable()
//***********************************************************************************
//	Function:  sensorMeasurementTemp
//	Inputs:
//
//	Uses the I2C protocol to send the measure temperature command and returns the value
//	in degrees Celsius.
//***********************************************************************************
float sensorMeasurementTemp(){
	int32_t tempMS;
	int32_t tempLS;
	float tempCel;
	I2C0->CMD = I2C_CMD_START;
	I2C0->TXDATA = (SI7021_DEVICE_ID << 1) | CMD_WRITE;
	while(!(I2C0->IF & I2C_IF_ACK));
	I2C0->IFC = I2C_IFC_ACK;
	I2C0->TXDATA = CMD_MEASURE_TEMP;
	while(!(I2C0->IF & I2C_IF_ACK));
	I2C0->IFC = I2C_IFC_ACK;
	I2C0->CMD = I2C_CMD_START;
	I2C0->TXDATA = (SI7021_DEVICE_ID << 1u) + CMD_READ;
	while(!(I2C0->IF & I2C_IF_ACK));
		I2C0->IFC = I2C_IFC_ACK;
	// Auto clockstretch
	while(!(I2C0->IF & I2C_IF_RXDATAV));
	tempMS = I2C0->RXDATA;
	I2C0->CMD = I2C_CMD_ACK;
	while(!(I2C0->IF & I2C_IF_RXDATAV));
	tempLS = I2C0->RXDATA;
	//optional checksum
	I2C0->CMD = I2C_CMD_NACK;
	I2C0->CMD = I2C_CMD_STOP;
	tempMS = tempMS<<8;
	tempMS = tempMS + tempLS;
	tempCel = ((175.72 * tempMS)/65536) -46.85;
	return tempCel;
}//end sensorMeasurementTemp

