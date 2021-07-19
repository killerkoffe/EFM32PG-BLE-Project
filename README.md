# EFM32PG-BLE-Project
Bluetooth Low Energy Embedded project using the Pearl Gecko EFM32(ARM Cortex-M series processor) micro-controller.  

This repository contains the source code files for a BLE temperature sensor. It uses an external Bluetooth module that communicates with the MCU using UART and LDMA, and an on-board temperature sensor that communicates with the MCU over I2C. The MCU is put to sleep in energy mode 3 (EM3) the lowest possible level that can be woken up with interrupts. DMA is used to receive a UART packet while the processor is asleep and minimize the time the processor is in active mode/ maximize the time it is asleep. When the processor is awake, I run it at the maximum frequency possible so it can complete its work faster and thus minimize the time it is in active mode. Additionally, some tips I learned in developing a low energy ARM Cortex-M micro-controller include:
 * Make use of clock trees that can disable sections of the micro-controller that are not being used
 * Make use of peripherals that can operate while the processor is in a sleep mode
 * Leakage current of all unused/unconnected GPIO pins can be minimized by explicitly setting its I/O setting to disabled
 * The energy consumed by the digital communication process can be higher than the energy of the peripheral itself
 * The ARM cortex-M Sleep on exit feature should only be enabled at the very end of peripheral initialization


