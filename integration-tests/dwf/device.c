#include "device.h"

extern int FDwfDeviceConfigOpen(int idxDev, int idxCfg, HDWF *phdwf);
extern int FDwfDeviceCloseAll();
extern int FDwfGetLastErrorMsg(char szError[512]);

static HDWF device = -1;

/**
 * Get currently open device
 *
 * @return DWF device handler
 */
HDWF get_device() {
  if (device < 0) {
    printf("Device is not yet opened\n");
  }
  return device;
}

/**
 * Connect to DWF device
 *
 * @return 1 if successful and 0 otherwise
 */
int open_device() {
  printf("Open automatically the first available device\n");
  if (!FDwfDeviceConfigOpen(-1, 0, &device)) {
    print_last_error("Device open failed");
    return 0;
  }

  return 1;
}

/**
 * Close DWF device connection
 *
 * @param device DWF device handler
 */
void close_device() { FDwfDeviceCloseAll(device); }

/**
 * Print last received error
 *
 * @param prefix prefix to add to the printed message
 */
void print_last_error(char *prefix) {
  char szError[512] = {0};
  FDwfGetLastErrorMsg(szError);
  printf("%s\n\t%s", prefix, szError);
}