#include <iostream>

#include "ccCore/ccCoreAPI.h"
#include "ccCore/ccString.h"
#include "ccNetwork/ccNetworkManager.h"
#include "ccWebServer/ccWebServerManager.h"
#include "ccMongooseServer/ccMongooseWebServerObjectFactory.h"

#include "../Impl/IoTDeviceDeviceManager.h"
#include "../Impl/IoTDeviceOpenAPIManager.h"

class IoTDeviceManagerEmulator : public IoTDeviceManagerIF {

public:
    IoTDeviceManagerEmulator();
    ~IoTDeviceManagerEmulator();

protected:
    virtual bool    setBulbStatus(std::string name, bool on);
    virtual bool    getBulbStatus(std::string name, std::string& status_json);
    virtual bool    getAllBulbsStatus(std::string& status_json);

    virtual bool    setDoorLockStatus(std::string name, std::string status);
    virtual bool    getDoorLockStatus(std::string name, std::string& status_json);
    virtual bool    getAllDoorLockStatus(std::string& status_json);    

private:
    IoTDeviceDeviceManager  oDeviceManager_;
    IoTDeviceOpenAPIManager oOpenAPIManager_;
};

//  Implements
IoTDeviceManagerEmulator::IoTDeviceManagerEmulator() : oOpenAPIManager_(*this) {

    //  for Network
    Luna::ccNetworkManager::instance().init();

    //  Start Open API Server
    Luna::ccWebServerManager::instance().attach_factory(std::make_shared<Luna::ccMongooseWebServerObjectFactory>());
    Luna::ccWebServerManager::instance().create_web_server("IoT Device Manager Server", "8000", ".");

    Luna::ccWebServerManager::instance().add_restful_api(std::make_shared<IoTDeviceOpenAPIManager>(*this));
    Luna::ccWebServerManager::instance().start();
}

IoTDeviceManagerEmulator::~IoTDeviceManagerEmulator() {

}

bool IoTDeviceManagerEmulator::setBulbStatus(std::string name, bool on) {

    if (name == "all_lightbulbs") {
        for (int count = 0; count < 3; count++) {
            oDeviceManager_.BulbStatus_[count] = on;

            std::cout << "SET [Bulb/" << oDeviceManager_.BulbName_[count] << "] : " << oDeviceManager_.BulbStatus_[count] << std::endl;
        }

        return true;
    }
    else {
        for (int count = 0; count < oDeviceManager_.CV_MAX_BULB_COUNT; count++) {
            if (oDeviceManager_.BulbName_[count] == name) {
                oDeviceManager_.BulbStatus_[count] = on;
                std::cout << "SET [Bulb/" << oDeviceManager_.BulbName_[count] << "] : " << oDeviceManager_.BulbStatus_[count] << std::endl;
                return true;
            }
        }
    }

    return false;
}

bool IoTDeviceManagerEmulator::getBulbStatus(std::string name, std::string& status_json) {
    if (name == "all_lightbulbs") {
        return getAllBulbsStatus(status_json);
    }
    else {
        for (int count = 0; count < oDeviceManager_.CV_MAX_BULB_COUNT; count++) {
            if (oDeviceManager_.BulbName_[count] == name) {
                Luna::ccString::format(
                    status_json,
                    "{\n" \
                    "  \"power_status\": %d,\n" \
                    "  \"bright_level\": 100\n" \
                    "}",
                    oDeviceManager_.BulbStatus_[count]);

                return true;
            }
        }
    }

    return false;
}

bool IoTDeviceManagerEmulator::getAllBulbsStatus(std::string& status_json) {
    std::string accessory_status;

    status_json =
        "{\n" \
        "  \"accessories\": [\n";

    for (int count = 0; count < oDeviceManager_.CV_MAX_BULB_COUNT; count++) {
        Luna::ccString::format(
            accessory_status,
            "    {\n" \
            "      \"name\": \"%s\",\n" \
            "      \"power_status\": %d,\n" \
            "      \"bright_level\": 100\n" \
            "    }",
            oDeviceManager_.BulbName_[count].c_str(),
            oDeviceManager_.BulbStatus_[count]);

        status_json += accessory_status;

        if (count == oDeviceManager_.CV_MAX_BULB_COUNT - 1)
            status_json += "\n";
        else
            status_json += ",\n";
    }

    status_json +=
        "  ]\n" \
        "}";

    return true;
}

bool IoTDeviceManagerEmulator::setDoorLockStatus(std::string name, std::string status) {

    if (oDeviceManager_.DoorLockName1_ == name.c_str()) {

        if (status == "closed")
            oDeviceManager_.DoorLockStatus1_ = true;
        else
            oDeviceManager_.DoorLockStatus1_ = false;

        std::cout << "SET [DoorLock/" << oDeviceManager_.DoorLockName1_ << "] : " << oDeviceManager_.DoorLockStatus1_ << std::endl;

        return true;
    }

    return false;
}

bool IoTDeviceManagerEmulator::getDoorLockStatus(std::string name, std::string& status_json) {

    if (oDeviceManager_.DoorLockName1_ == name.c_str()) {
        Luna::ccString::format(
            status_json,
            "    {\n" \
            "      \"name\": \"%s\",\n" \
            "      \"lock_status\": \"%s\"\n" \
            "    }",
            oDeviceManager_.DoorLockName1_.c_str(),
            (oDeviceManager_.DoorLockStatus1_) ? "closed" : "open");

        return true;
    }

    return false;
}

bool IoTDeviceManagerEmulator::getAllDoorLockStatus(std::string& status_json) {

    std::string accessory_status;

    status_json =
        "{\n" \
        "  \"accessories\": [\n";

    Luna::ccString::format(
        accessory_status,
        "    {\n" \
        "      \"name\": \"%s\",\n" \
        "      \"lock_status\": %d\n" \
        "    }\n",
        oDeviceManager_.DoorLockName1_.c_str(),
        oDeviceManager_.DoorLockStatus1_);

    status_json += accessory_status;

    status_json +=
        "  ]\n" \
        "}";

    return true;
}

int main(int argc, char* argv[]) {
    IoTDeviceManagerEmulator oEmulator;

    while (true)
        Luna::sleep(1);

    return 0;
}