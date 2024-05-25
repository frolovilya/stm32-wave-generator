#ifndef PERIPHERAL_HPP
#define PERIPHERAL_HPP

template <typename T>
class Peripheral {
public:
  virtual T *getPeripheral() = 0;
  virtual void configure() = 0;
};

#endif