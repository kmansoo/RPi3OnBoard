#include "IoTDeviceDeviceManager.h"

#include <stdlib.h>

IoTDeviceDeviceManager::IoTDeviceDeviceManager() {
    BulbName_[0] = "lightbulb1";
    BulbName_[1] = "lightbulb2";
    BulbName_[2] = "lightbulb3";

    DoorLockName1_ = "main_door";

    for (int count = 0; count < CV_MAX_BULB_COUNT; count++)
        BulbStatus_[count] = rand() % 2;

    DoorLockStatus1_ = true;
}

IoTDeviceDeviceManager::~IoTDeviceDeviceManager() {
}

