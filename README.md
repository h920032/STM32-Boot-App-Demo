# STM32 Secure Boot and Application Demo

## Overview
This project demonstrates a bootloader and application setup for the STM32H723 microcontroller. The repository contains both bootloader and application code that work together to provide a reliable firmware update mechanism. The bootloader is implemented in the **H723_Boot_testing** folder, and the application firmware is in **H723_App_testing**.

## Project Structure
- `H723_Boot_testing`: Bootloader project for STM32H723.
- `H723_App_testing`: Application project for STM32H723.
- `postbuild.sh`: Script to combine bootloader and application hex files.

## Hardware Requirements
- STM32H723 development board (e.g., NUCLEO-H723ZG)
- ST-Link programmer (or equivalent)
- LEDs and a push button:
  - **LED_GREEN** on PB0
  - **LED_RED** on PB14
  - **LED_YELLOW** on PE1
  - **BUTTON** on PC13
- *Add any additional hardware requirements*

## Features
- Boot/Application partition architecture with a dedicated bootloader.
- Firmware verification and version checking prior to starting the application.
- LED indicators show system status.
- Manual firmware update capability via bootloader commands.
- *Add your key features here*

## Memory Layout
The STM32H723 flash memory is divided into two main sections:
- **Bootloader**: Located at the beginning of flash memory at `0x08000000`.
- **Application**: Located at offset `0x08005000`.

## Getting Started
1. Clone this repository.
2. Open the project in STM32CubeIDE.
3. Build the bootloader project in **H723_Boot_testing/** and the application project in **H723_App_testing/**.
4. Flash the bootloader first, then flash the application firmware.
5. Reset the board to see the system in action.

## Build Instructions

### Step 1: Build Bootloader Project (`H723_Boot_testing`)

1. Open STM32CubeIDE.
2. Import the `H723_Boot_testing` project.
3. Select the `Debug` build configuration.
4. Build the project (`Project` → `Build Project`).
5. Verify that the bootloader hex file (`H723_Boot_testing.hex`) is generated in the build output directory (`Debug` ).

### Step 2: Build Application Project (`H723_App_testing`)

1. Import the `H723_App_testing` project into STM32CubeIDE.
2. Select the `Debug` build configuration.
3. Build the project (`Project` → `Build Project`).
4. Verify that the application hex file (`Boot-App-Demo.hex`) is generated in the project directory.

### Step 3: Combine Bootloader and Application Hex Files

Run the provided `postbuild.sh` script to combine the bootloader and application hex files into a single hex file:

```bash
./postbuild.sh <path_to_bootloader_hex> <path_to_application_hex> <output_combined_hex>
```

### Step 4: Flash the Combined Hex File

Use STM32CubeProgrammer or your preferred flashing tool to program the combined hex file (`Combined.hex`) onto your STM32H723 device.

## Development Workflow
1. Modify the application code (located in `/H723_App_testing/Core/Src/`) as needed.
2. Rebuild the application project after making changes.
3. Use the bootloader update mechanism (triggered via button press or command interface) to load updated firmware.
4. The bootloader performs version checking and integrity verification before transferring execution to the application.
5. Use the bootloader to update the application through

## Configuration
- **Communication & Boot Mode**: The bootloader and application share system configuration registers (via SYSCFG, NVIC, etc.) to manage boot addresses and version flags.
- **Clock and NVIC Settings**: Configured in the respective .ioc files for bootloader and application projects.
- **Compiler & Linker Settings**: Managed by STM32CubeIDE with GCC; see the project files for detailed configurations.

## References
- [STM32H723 Datasheet](https://www.st.com/resource/en/datasheet/stm32h723zg.pdf)
- [Application Note AN2606: STM32 MCU System Memory Boot Mode](https://www.st.com/resource/en/application_note/cd00167594-stm32-microcontroller-system-memory-boot-mode-stmicroelectronics.pdf)