#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <string>
#include "dwf.h"

class DwfDeviceHandler {
public:
  static DwfDeviceHandler openFirst();
  static std::string getLastError(const std::string prefix);
  
  explicit DwfDeviceHandler(HDWF device);

  HDWF getDevice() const;
  void close();

private:
  HDWF device = -1;
};

#endif
