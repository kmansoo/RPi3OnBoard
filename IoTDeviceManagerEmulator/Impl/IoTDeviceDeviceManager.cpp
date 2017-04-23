#include "IoTDeviceDeviceManager.h"

IoTDeviceDeviceManager::IoTDeviceDeviceManager() {
    bulbName_[0] = "lightbulb1";
    bulbName_[1] = "lightbulb2";
    bulbName_[2] = "lightbulb3";

    doorLockName1_ = "main_door";

    for (int count = 0; count < CV_MAX_BULB_COUNT; count++)
        BulbStatus_[count] = rand() % 2;

    DoorLockStatus1_ = true;
}

IoTDeviceDeviceManager::~IoTDeviceDeviceManager() {
}

