#pragma once

#include "ccWebServer/ccRESTfulApi.h"

class IoTDeviceManagerIF {
public:
    virtual bool    setBulbStatus(std::string name, bool on) = 0;
    virtual bool    getBulbStatus(std::string name, std::string& status_json) = 0;
    virtual bool    getAllBulbsStatus(std::string& status_json) = 0;

    virtual bool    setDoorLockStatus(std::string name, std::string status) = 0;
    virtual bool    getDoorLockStatus(std::string name, std::string& status_json) = 0;
    virtual bool    getAllDoorLockStatus(std::string& status_json) = 0;
};

class IoTDeviceOpenAPIManager : public Luna::ccRESTfulApi
{
public:
    IoTDeviceOpenAPIManager(IoTDeviceManagerIF& agent);
    ~IoTDeviceOpenAPIManager();

protected:
    IoTDeviceManagerIF&   IoTDeviceManagerIF_;

protected:
    bool    accessory_lightbulb(std::shared_ptr<Luna::ccWebServerRequest> request, std::shared_ptr<Luna::ccWebServerResponse> response);
    bool    accessory_lightbulb_name(std::shared_ptr<Luna::ccWebServerRequest> request, std::shared_ptr<Luna::ccWebServerResponse> response);
    bool    accessory_doorlock_name(std::shared_ptr<Luna::ccWebServerRequest> request, std::shared_ptr<Luna::ccWebServerResponse> response);
};

