#ifndef PERIPHERAL_HPP
#define PERIPHERAL_HPP

template <typename T>
class Peripheral {
public:
  virtual ~Peripheral() {};

  virtual T *getPeripheral() const = 0;
  virtual void configure() = 0;
};

#endif