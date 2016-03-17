#ifndef JSONCONFIG_H
#define JSONCONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "FS.h"

class DhtData
{
public:
    float temp;
    float humidity;
    String tempStr;
    String humidityStr;
};

class BmpData
{
public:
    float temp;
    float pressure;
    String tempStr;
    String pressureStr;
};

class JsonConfig
{
public:
    bool saveConfig();
    bool loadConfig();
    bool printConfig();

    char sensor_id[32] = "1";
    char module_name[32] = "MeteoESP1";
    char sta_ssid[32] = "Keenetic-8095";
    char sta_pwd[32] = "89168561950";

    char static_ip_mode[32] = "0";
    char static_ip[32] = "192.168.1.55";
    char static_gateway[32] = "192.168.1.1";
    char static_subnet[32] = "255.255.255.0";

    char get_data_delay[32] = "10";
    char reboot_delay[32] = "10";

    char add_data_url[32] = "http://192.168.1.44/add.php";

private:
};

#endif
