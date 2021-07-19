#include "gpio.h"
#include "em_gpio.h"
//***********************************************************************************
//	Function: gpio_init
//	Inputs:
//
//	Initializes the General Purpose Input/Output peripheral
//***********************************************************************************
void gpio_init(void){
	// Set LED ports to be standard output drive with default off (cleared)
	// LED0
	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, LED0_default);
	// LED1
	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, LED1_default);
	// I2C SDA
	GPIO_PinModeSet(I2C0_SDA_port,I2C0_SDA_pin, gpioModeDisabled, I2C0_SDA_default);
	// I2C SCL
	GPIO_PinModeSet(I2C0_SCL_port,I2C0_SCL_pin, gpioModeDisabled, I2C0_SCL_default);
	// SI7021 Temperature Sensor Enable
	GPIO_PinModeSet(SENSOR_ENABLE_port, SENSOR_ENABLE_pin, gpioModePushPull, SENSOR_ENABLE_default);
	// UART TX
	GPIO_DriveStrengthSet(LEUART0_TX_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LEUART0_TX_port, LEUART0_TX_pin, gpioModePushPull, LEUART0_TX_default);
	// UART RX
	GPIO_DriveStrengthSet(LEUART0_RX_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LEUART0_RX_port, LEUART0_RX_pin, gpioModePushPull, LEUART0_RX_default);
}//end gpio_init
