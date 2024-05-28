#include "UARTPeripheral.hpp"
#include <cstring>
#include <string>

void UARTPeripheral::configure() {
  configureRCC();
  configureGPIO();

  getPeripheral()->CR1 |= USART_CR1_UE;     // enable USART
  getPeripheral()->CR1 |= USART_CR1_RXNEIE; // enable RX interrupt
  getPeripheral()->CR1 |= USART_CR1_IDLEIE; // enable IDLE interrupt
  getPeripheral()->CR1 &= ~USART_CR1_M;     // 8-bit data word length
  getPeripheral()->CR2 &= ~USART_CR2_STOP;  // 1 stop bit
  // baud rate = f_clock / (16 * USARTDIV)
  // USARTDIV = 42MHz / (16 * 115200) = 22,7864
  getPeripheral()->BRR =
      0x16C; // Mantissa = 22 (0x16); Fraction ~= 16 * 0,7864 = 13 (0xD)

  configureNVIC();
}

/**
 * Start UART data transfer and receival
 */
void UARTPeripheral::start() {
  getPeripheral()->CR1 |=
      USART_CR1_TE | USART_CR1_RE; // start transmitter and receiver
}

/**
 * Send UART message
 * 
 * @param data data to send
 * @param appendNewLine whether to append '\n' to the data
 */
void UARTPeripheral::send(std::string data, bool appendNewLine) {
  if (appendNewLine) {
    data += "\n";
  }
  for (size_t i = 0; i < data.length(); i++) {
    getPeripheral()->DR = data.at(i);
    // wait DR to be ready for next data
    while (!(getPeripheral()->SR & USART_SR_TXE))
      ;
  }

  // wait for transfer to be completed
  while (!(getPeripheral()->SR & USART_SR_TC))
    ;
}

/**
 * Register a callback to receive data via UART
 *
 * @param callback callback fired on data receival
 */
void UARTPeripheral::receive(std::function<UART_RX_Handler> callback) {
  rxCallback = callback;
}

/**
 * Common UART interrupt handler.
 * Triggering RX callback on receiving '\n' last character.
 *
 * MESSAGE\n -> c-string MESSAGE\0 -> std::string
 */
void UARTPeripheral::handleInterrupt() {
  // fill buffer
  while (getPeripheral()->SR & USART_SR_RXNE) {
    if (rxBufferIndex > rxBufferSize) {
      rxBufferIndex = 0;
    }
    uint32_t receivedData = getPeripheral()->DR;
    char receivedChar = (char)receivedData;
    if (receivedChar != '\0') {
      rxBuffer[rxBufferIndex++] = receivedChar;

      // trigger callback on LF
      if (receivedChar == '\n') {
        // replace \n with \0 and convert to string
        rxBuffer[rxBufferIndex - 1] = '\0';
        std::string receivedString(rxBuffer, rxBufferIndex - 1);
        rxBufferIndex = 0;

        rxCallback(receivedString);
      }
    }
  }
}