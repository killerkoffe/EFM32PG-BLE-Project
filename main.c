#include <stdint.h>
#include <stdbool.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_core.h"
#include "em_letimer.h"
#include "bsp.h"
#include "main.h"
#include "gpio.h"
#include "cmu.h"
#include "letimer0.h"
#include "sleep.h"
#include "i2c.h"
#include "em_i2c.h"
#include "si7021.h"
#include "leuart.h"
#include "em_leuart.h"
#include "ldma.h"
#include "cryotimer.h"

int main(void)
{
    EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_DEFAULT;
    CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_DEFAULT;
    CHIP_Init();                                            // Chip errata 
    EMU_EM23Init_TypeDef em23Init = EMU_EM23INIT_DEFAULT;   // Init DCDC regulator and HFXO with kit specific parameters 
    EMU_DCDCInit(&dcdcInit);
    em23Init.vScaleEM23Voltage = emuVScaleEM23_LowPower;
    EMU_EM23Init(&em23Init);
    CMU_HFXOInit(&hfxoInit);
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);        // Switch HFCLK to HFXO and disable HFRCO 
    CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);
    cmu_init();                                             // Initialize Clocks 
    gpio_init();                                            // Initialize GPIO 
    CORE_ATOMIC_IRQ_ENABLE();
    i2c0_init();                                            // Initialize I2C 
    si7021_init();                                          // Initialize Temperature Sensor 
    letimer0_init();                                        // Initialize LETIMER 
    LETIMER_Enable(LETIMER0, true);
    leuart0_init();                                         // Initialize LEUART 
    LEUART_Enable(LEUART0,leuartEnable);
    ldma_init();                                            // Initialize LDMA 
    cryotimer_init();                                       // Initialize CRYOTIMER 
    while (1)
    {
        if(Sensing)
            cap_sense();
        Enter_Sleep();
    }
}



