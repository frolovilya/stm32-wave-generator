#include "Level.hpp"
#include "../peripherals/Peripherals.hpp"

double getPeakToPeak(double signalLevelVolts) {
  return signalLevelVolts * 2 / adcInstance.getVdda() *
         dacInstance.getMaxValue();
}