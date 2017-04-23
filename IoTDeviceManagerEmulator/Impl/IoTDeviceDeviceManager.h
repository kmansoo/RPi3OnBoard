#pragma once

#include <string> 

class IoTDeviceDeviceManager
{
public:
    IoTDeviceDeviceManager();
    ~IoTDeviceDeviceManager();

public:
    enum CONST_VALUE {
        CV_MAX_BULB_COUNT = 3,
        CV_MAX_DOORLOCK_COUNT = 3,
    };

    std::string bulbName_[CV_MAX_BULB_COUNT];
    bool    BulbStatus_[CV_MAX_BULB_COUNT];
    std::string doorLockName1_;
    bool    DoorLockStatus1_;

};
