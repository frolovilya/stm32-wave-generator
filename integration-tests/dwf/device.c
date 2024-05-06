#include "device.h"

extern int FDwfDeviceConfigOpen(int idxDev, int idxCfg, HDWF *phdwf);
extern int FDwfDeviceCloseAll();
extern int FDwfGetLastErrorMsg(char szError[512]);

HDWF open_device() {
    HDWF hdwf;

    printf("Open automatically the first available device\n");
    if(!FDwfDeviceConfigOpen(-1, 0, &hdwf)) {
        print_last_error("Device open failed");
        return -1;
    }

    return hdwf;
}

void close_device(HDWF device) {
    FDwfDeviceCloseAll(device);
}

void print_last_error(char *prefix) {
    char szError[512] = {0};
    FDwfGetLastErrorMsg(szError);
    printf("%s\n\t%s", prefix, szError);
}