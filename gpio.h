//***********************************************************************************
// Include Files
//***********************************************************************************
//***********************************************************************************
// Definitions
//***********************************************************************************
// LED0 
#define	LED0_port		gpioPortF
#define LED0_pin		4
#define LED0_default		false 	// off
// LED1 
#define LED1_port		gpioPortF
#define LED1_pin		5
#define LED1_default		false	
// I2C SDA
#define I2C0_SDA_port 		gpioPortC
#define I2C0_SDA_pin		10
#define I2C0_SDA_default	false
// I2C SCL
#define I2C0_SCL_port 		gpioPortC
#define I2C0_SCL_pin		11
#define I2C0_SCL_default	false
// SI7021 Temperature Sensor Enable 
#define SENSOR_ENABLE_port 	gpioPortB
#define SENSOR_ENABLE_pin	10
#define SENSOR_ENABLE_default	false
// UART TX
#define LEUART0_TX_port		gpioPortD
#define LEUART0_TX_pin 		10
#define LEUART0_TX_default  	false
// UART RX
#define LEUART0_RX_port		gpioPortD
#define LEUART0_RX_pin 		11
#define LEUART0_RX_default  	false
//***********************************************************************************
// Global Variables
//***********************************************************************************
//***********************************************************************************
// Function Prototypes
//***********************************************************************************
void gpio_init(void);

