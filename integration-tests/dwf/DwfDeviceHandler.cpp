#include "DwfDeviceHandler.hpp"
#include "dwf.h"
#include <iostream>
#include <stdexcept>

using namespace std;

DwfDeviceHandler::DwfDeviceHandler(HDWF device) : device{device} {}

/**
 * Connect to first found DWF device
 */
DwfDeviceHandler DwfDeviceHandler::openFirst() {
  cout << "Open automatically the first available device\n";

  HDWF device = -1;

  if (!FDwfDeviceConfigOpen(-1, 0, &device)) {
    throw std::runtime_error(getLastError("Device open failed"));
  }

  return DwfDeviceHandler(device);
}

/**
 * Print last received error
 *
 * @param prefix prefix to add to the returned message
 */
string DwfDeviceHandler::getLastError(string prefix) {
  char szError[512] = {0};
  FDwfGetLastErrorMsg(szError);
  string errorString(prefix);
  errorString = errorString + ": " + szError;

  return errorString;
}

/**
 * Get currently open device
 *
 * @return DWF device handler
 */
HDWF DwfDeviceHandler::getDevice() const {
  if (device < 0) {
    throw std::runtime_error("Device is not yet opened");
  }

  return device;
}

/**
 * Close DWF device connection
 */
void DwfDeviceHandler::close() { FDwfDeviceClose(device); }