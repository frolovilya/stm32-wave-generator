# stm32-wave-generator

STM32F4 wave signal generator with UART control. Uses built-in DAC to produce sin, square and saw waves of a given frequency.


## Build and Configuration

## Connectivity

DAC output pin **A4**.

Using USART2 by default: 
TX **PA2**, RX **PA3**. Note that on Nucleo boards the pins are routed to ST-Link USB.

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
wave_type: sin | square | saw
frequency_hz: [20, 20000]
```

### Examples
```sh
% sin 440
Generating 440Hz sin wave
```

```sh
% square 200
Generating 200Hz square wave
```

```sh
% saw 100
Generating 100Hz saw wave
```

## Integration Tests

Tests are using Digilent WaveForms SDK and require a compatible Analog Discovery device to be plugged-in and connected to the corresponding SMT32F4 pins. The project must be built with `-DUSE_USART3` flag.

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
