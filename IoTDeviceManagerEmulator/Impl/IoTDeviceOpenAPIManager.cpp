#include "IoTDeviceOpenAPIManager.h"

#include <iostream>
#include <functional>

#include "ccCore/ccString.h"

#include "json/value.h"
#include "json/writer.h"
#include "json/reader.h"

IoTDeviceOpenAPIManager::IoTDeviceOpenAPIManager(IoTDeviceManagerIF& agent) : IoTDeviceManagerIF_(agent) {

    addAPI(std::string("/v1/accessory/lightbulb"), std::bind(&IoTDeviceOpenAPIManager::accessory_lightbulb, this, std::placeholders::_1, std::placeholders::_2));
    addAPI(std::string("/v1/accessory/lightbulb/*"), std::bind(&IoTDeviceOpenAPIManager::accessory_lightbulb_name, this, std::placeholders::_1, std::placeholders::_2));
    addAPI(std::string("/v1/accessory/doorlock/*"), std::bind(&IoTDeviceOpenAPIManager::accessory_doorlock_name, this, std::placeholders::_1, std::placeholders::_2));
}

IoTDeviceOpenAPIManager::~IoTDeviceOpenAPIManager() {
}

bool IoTDeviceOpenAPIManager::accessory_lightbulb(std::shared_ptr<Luna::ccWebServerRequest> request, std::shared_ptr<Luna::ccWebServerResponse> response) {
    std::cout << "enter IoTDeviceOpenAPIManager::accessory_lightbulb, query_string=" << request->get_query_string() << std::endl;

    std::string result_data =
        "{\n" \
        "  \"result_code\": 0\n" \
        "}";

    switch (request->get_method()) {
    case Luna::ccWebServerRequest::HttpMethod_Post: { //  Create
        response->send_status(403, std::string("Forbidden"));
        response->send_content_type("Content-Type: application/x-www-form-urlencoded", (size_t)0);

        return false;
    }

    case Luna::ccWebServerRequest::HttpMethod_Get: {   //  Read
        std::string json_data;

        if (IoTDeviceManagerIF_.getBulbStatus("all_lightbulbs", json_data)) {
            response->send_status(200, std::string("OK"));
            response->send_content_type("application/json", json_data.length());
            response->send_content(json_data);

            return true;
        }

        response->send_status(403, std::string("Forbidden"));
        response->send_content_type("Content-Type: application/x-www-form-urlencoded", (size_t)0);
        return false;
    }

    case Luna::ccWebServerRequest::HttpMethod_Put: {
        std::string     accessory_name = "all_lightbulbs";

        Json::Reader    json_reader;
        Json::Value     json_protocol;
        std::string     json_data;

        json_data.reserve(1024);

        request->get_content_body(json_data);

        if (!json_reader.parse(json_data, json_protocol)) {
            response->send_status(403, std::string("Forbidden"));
            response->send_content_type("Content-Type: application/x-www-form-urlencoded", (size_t)0);
            return false;
        }

        bool powerStatus = false;
        int brightLevel = 100;

        try {
            powerStatus = json_protocol["power_status"].asBool();
            brightLevel = json_protocol["bright_level"].asInt();
        }
        catch (...) {

        }

        if (IoTDeviceManagerIF_.setBulbStatus(accessory_name, powerStatus)) {
            response->send_status(200, std::string("OK"));
            response->send_content_type("application/json", json_data.length());
            response->send_content(json_data);

            return true;
        }


        response->send_status(403, std::string("Forbidden"));
        response->send_content_type("Content-Type: application/x-www-form-urlencoded", (size_t)0);

        return false;

    }

    case Luna::ccWebServerRequest::HttpMethod_Delete: {
        response->send_status(403, std::string("Forbidden"));
        response->send_content_type("Content-Type: application/x-www-form-urlencoded", (size_t)0);

        return false;
    }

    default:
        response->send_status(403, std::string("Forbidden"));
        response->send_content_type("Content-Type: application/x-www-form-urlencoded", (size_t)0);
        break;
    }

    return false;
}

bool IoTDeviceOpenAPIManager::accessory_lightbulb_name(std::shared_ptr<Luna::ccWebServerRequest> request, std::shared_ptr<Luna::ccWebServerResponse> response) {
    std::cout << "enter IoTDeviceOpenAPIManager::accessory_lightbulb, query_string=" << request->get_query_string() << std::endl;

    std::string result_data =
        "{\n" \
        "  \"result_code\": 0\n" \
        "}";

    switch (request->get_method()) {
    case Luna::ccWebServerRequest::HttpMethod_Get: {   //  Read
        std::string accessory_name = request->get_resource();
        std::string json_data;

        if (IoTDeviceManagerIF_.getBulbStatus(accessory_name, json_data)) {
            response->send_status(200, std::string("OK"));
            response->send_content_type("application/json", json_data.length());
            response->send_content(json_data);

            return true;
        }
        
        response->send_status(403, std::string("Forbidden"));
        response->send_content_type("Content-Type: application/x-www-form-urlencoded", (size_t)0);
        return false;
    }
    break;

    case Luna::ccWebServerRequest::HttpMethod_Put: {
        std::string     accessory_name = request->get_resource();

        Json::Reader    json_reader;
        Json::Value     json_protocol;
        std::string     json_data;

        json_data.reserve(1024);

        request->get_content_body(json_data);

        if (!json_reader.parse(json_data, json_protocol)) {
            response->send_status(403, std::string("Forbidden"));
            response->send_content_type("Content-Type: application/x-www-form-urlencoded", (size_t)0);
            return false;
        }

        bool powerStatus = false;
        int brightLevel = 100;

        try {
            powerStatus = json_protocol["power_status"].asBool();
            brightLevel = json_protocol["bright_level"].asInt();
        }
        catch (...) {

        }

        if (IoTDeviceManagerIF_.setBulbStatus(accessory_name, powerStatus)) {
            response->send_status(200, std::string("OK"));
            response->send_content_type("application/json", json_data.length());
            response->send_content(json_data);

            return true;
        }

        response->send_status(404, std::string("Not Found"));
        response->send_content_type("Content-Type: application/x-www-form-urlencoded", (size_t)0);
        return false;
    }

    default:
        response->send_status(403, std::string("Forbidden"));
        response->send_content_type("Content-Type: application/x-www-form-urlencoded", (size_t)0);
        break;
    }

    return false;
}

bool IoTDeviceOpenAPIManager::accessory_doorlock_name(std::shared_ptr<Luna::ccWebServerRequest> request, std::shared_ptr<Luna::ccWebServerResponse> response) {
    std::string result_data =
        "{\n" \
        "  \"result_code\": 0\n" \
        "}";

    switch (request->get_method()) {
    case Luna::ccWebServerRequest::HttpMethod_Get: {   //  Read
        std::string accessory_name = request->get_resource();
        std::string json_data;

        if (IoTDeviceManagerIF_.getDoorLockStatus(accessory_name, json_data)) {
            response->send_status(200, std::string("OK"));
            response->send_content_type("application/json", json_data.length());
            response->send_content(json_data);

            return true;
        }

        response->send_status(403, std::string("Forbidden"));
        response->send_content_type("Content-Type: application/x-www-form-urlencoded", (size_t)0);
        break;
    }

    case Luna::ccWebServerRequest::HttpMethod_Put: {
        std::string     accessory_name = request->get_resource();

        Json::Reader    json_reader;
        Json::Value     json_protocol;
        std::string     json_data;

        json_data.reserve(1024);

        request->get_content_body(json_data);

        if (!json_reader.parse(json_data, json_protocol)) {
            response->send_status(403, std::string("Forbidden"));
            response->send_content_type("Content-Type: application/x-www-form-urlencoded", (size_t)0);
            return false;
        }

        std::string lock_status = json_protocol["lock_status"].asString();

        if (IoTDeviceManagerIF_.setDoorLockStatus(accessory_name, lock_status)) {
            response->send_status(200, std::string("OK"));
            response->send_content_type("application/json", json_data.length());
            response->send_content(json_data);

            return true;
        }

        response->send_status(404, std::string("Not Found"));
        response->send_content_type("Content-Type: application/x-www-form-urlencoded", (size_t)0);
        break;
    }

    default:
        response->send_status(403, std::string("Forbidden"));
        response->send_content_type("Content-Type: application/x-www-form-urlencoded", (size_t)0);
        break;
    }

    return false;
}
