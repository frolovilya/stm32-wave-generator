# stm32-wave-generator

STM32F4 analog wave generator with precise UART control. Uses built-in DAC to produce waves of various form and frequency.


## Build and Configuration

## Connectivity

DAC output pin **A4**.

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

Tests are using Digilent WaveForms SDK and require a compatible Analog Discovery device to be plugged-in and connected to the corresponding SMT32F4 pins.

```sh
# Build the main project with USART3 enabled
mkdir build & cd build
cmake ../ -DUSE_USART3
make
make flash

# Build and launch tests
# The following pins configuration is default
cd ../integration-tests
mkdir build & cd build
cmake ../ -DSCOPE_CHANNEL 1 \
    -DDIGITAL_TX_PIN 0 \
    -DDIGITAL_RX_PIN 1
make
./integration-tests
```
