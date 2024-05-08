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

Based on the build setup, connect TX/RX or use serial port control via ST-Link USB.

### Command syntax
```sh
wave_type frequency_hz
```
Where 
```sh
wave_type: sine | square | saw | triangle
frequency_hz: [20, 20000]
```

### Examples
```sh
% sine 440
Generating 440Hz sine wave
```

```sh
% square 200
Generating 200Hz square wave
```

```sh
% saw 100
Generating 100Hz saw wave
```

```sh
% triangle 50
Generating 50Hz triangle wave
```

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
