# stm32-wave-generator

STM32F4 analog wave generator with precise UART control. Uses built-in DAC to produce waves of various form and frequency.


## Build

Install [ARM GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads). `arm-none-eabi-gcc` compiler is required.

On Mac could be done with _brew_ as well:
```sh
brew install --cask gcc-arm-embedded
```

Install [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html) to flash STM32 via ST-Link.

There's also an all-in-one [STM32CubeCLT](https://www.st.com/en/development-tools/stm32cubeclt.html?rt=um&id=UM3088) command tools package available.

Build the project using CMake:

```sh
mkdir build; cd build

# Using USART2 by default if -DUSE_USART3 flag isn't provided
cmake ../ -DPROGRAMMER_CLI=/PATH/TO/STM32_Programmer_CLI \
    -DUSE_USART3

make

# Upload to the device
make flash
```

## Connectivity

DAC output pin is **A4**.

Using USART2 by default: 
TX **PA2**, RX **PA3**. Note that on Nucleo boards USART2 is routed to ST-Link USB instead.

Using USART3 when built with `-DUSE_USART3`:
TX **C10**, RX **C11**.


## Usage

### Command Syntax
```sh
sine|square|saw|triangle 20..20000LF
```
Note that `LF(\n)` at the end is required.

### Sending Commands via Serial Port

When using Nucleo board or any UART to USB adapter it's possible to send commands via a serial port.

```sh
# Install picocom serial port terminal
brew install picocom

# Find your connected device
ls /dev/tty.*

# Start session (press Ctrl+a Ctrl+x to exit)
picocom --echo --omap crlf --imap lfcrlf -b 115200 /dev/tty.usbmodem14203
```

### Examples
```sh
sine 440
Generating 440Hz sine wave
```
![sine-440](https://github.com/frolovilya/stm32-wave-generator/assets/271293/2d8baa18-3032-44dd-9d15-85e10a6b2b6a)

```sh
square 200
Generating 200Hz square wave
```
![square-200](https://github.com/frolovilya/stm32-wave-generator/assets/271293/21c3fa6d-4aa2-480e-95d7-ad2351046e58)

```sh
saw 300
Generating 300Hz saw wave
```
![saw-300](https://github.com/frolovilya/stm32-wave-generator/assets/271293/c1e2de59-1d40-44a9-82bd-15b46a5f6384)

```sh
triangle 600
Generating 600Hz triangle wave
```
![triangle-600](https://github.com/frolovilya/stm32-wave-generator/assets/271293/79b30d67-fb2d-41dc-8336-3c95391298d8)

## Integration Tests

Tests are using [Digilent WaveForms SDK](https://digilent.com/reference/software/waveforms/waveforms-sdk/start) and require a compatible Analog Discovery device to be plugged-in and connected to the corresponding SMT32F4 pins.

```sh
# Build and launch tests
cd ./integration-tests
mkdir build; cd build

# The following pins configuration is default
cmake ../ -DSCOPE_CHANNEL 1 \
    -DDIGITAL_TX_PIN 0 \
    -DDIGITAL_RX_PIN 1

make

./integration-tests
```
