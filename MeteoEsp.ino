#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <UTFT.h>
#include <DHT.h>
#include <SFE_BMP180.h>
#include <Wire.h>
#include <RTClib.h>
#include <SparkFunHTU21D.h>
#include <BH1750.h>

#include "JsonConfig.h"
#include "WebCommon.h"
#include "Common.h"

RTC_DS1307 rtc;

BH1750 lightMeter;
uint16_t lightness;
String lightnessStr;

HTU21D sht21;
SensorData data3;

ESP8266WebServer WebServer(80);
const int maxConnectAttempts = 20;

JsonConfig config;

#define DHTPIN 12
#define DHTTYPE DHT22
DHT dht22(DHTPIN, DHTTYPE);
SensorData data1;

SFE_BMP180 bmp180;
#define ALTITUDE 20 //meters
SensorData data2;

extern uint8_t BigFont[];
UTFT myGLCD(ILI9341_S5P, 15, 5, 4);
const int width = 320;
const int height = 240;
int fontHeight;
const int valueX = 140;

bool bmp180initialized = false;
bool dht22initialized = false;
bool rtcInitialized = false;
bool meetWithServer = false;

unsigned long previousMillis = 0;
bool isRebooting = false;
int currentSensorCycle = 0;
const int ONE_SECOND = 1000;

//declaration
void renderWiFiStatus(String status, int r, int g, int b);
void renderServerStatus(String status, int r, int g, int b);
void renderAPStatus(String status, int r, int g, int b);

bool isRtcInitialized()
{
    return rtcInitialized && rtc.isrunning();
}

void renderDateTime()
{
    myGLCD.setColor(255, 255, 0);
    if (isRtcInitialized())
        myGLCD.print(getDateTimeString(rtc.now()), 1, getRowY(7, fontHeight));
    else
        myGLCD.print("RTC Off", 1, getRowY(7, fontHeight));
    myGLCD.setColor(0, 255, 0);
}

void renderHeader()
{
    myGLCD.clrScr();
    myGLCD.setColor(0, 0, 0);
    myGLCD.fillRect(0, 0, width, height);

    myGLCD.setColor(255, 255, 255);
    myGLCD.print(String("Wi-Fi  :"), 1, getRowY(0, fontHeight));
    myGLCD.print(String("Server :"), 1, getRowY(1, fontHeight));
    myGLCD.print(String("AP     :"), 1, getRowY(2, fontHeight));

    myGLCD.setColor(0, 255, 0);
    myGLCD.print(String("T1,   C:"), 1, getRowY(3, fontHeight));
    myGLCD.print(String("H,    %:"), 1, getRowY(4, fontHeight));
    myGLCD.print(String("P, mmHg:"), 1, getRowY(5, fontHeight));
    myGLCD.print(String("T2,   C:"), 1, getRowY(6, fontHeight));
}

void rebootESP()
{
    myGLCD.setColor(0, 0, 0);
    myGLCD.fillRect(0, 0, width, height);
    myGLCD.setColor(255, 255, 255);

    int periods = atoi(config.reboot_delay); // how many times will change the text
    for (int i = 0; i < periods; i++)
    {
        myGLCD.print(String("Reboot in ") + String(periods - i) + " sec(s) ", 1, getRowY(0, fontHeight));
        Serial.printf("Reboot ESP in %d sec.\r\n", (periods - i));
        delay(ONE_SECOND);
    }

    //ignore all messages to display while rebooting
    isRebooting = true;

    ESP.restart();
}

void webRoot()
{
    Serial.println("\r\nServer: request ROOT");

    WebServer.send(200, "text/html", String("") +
        FPSTR(headStart) + FPSTR(styles) + FPSTR(scripts) + FPSTR(headEnd) + FPSTR(bodyStart) + FPSTR(mainMenu) +
        "<h2>Welcome to " + config.module_name + "</h2>" +
        "<div class='container'>" +
        "<div class='row'><div class='label'>Module IP:</div><div class='value'>" + getIpString(WiFi.localIP()) + "</div></div>" +
        "<div class='row'><div class='label'>Module Name:</div><div class='value'>" + config.module_name + "</div></div>" +
        "<div class='row'><div class='label'>Module MAC:</div><div class='value'>" + getMacString() + "</div></div>" +

        "<div class='row'><div class='label'>Time:</div><div class='value'>" + (isRtcInitialized() ? getDateTimeString(rtc.now()) : "-") + "</div></div>" +
        "<div class='row'><div class='label'>Uptime:</div><div class='value'>" + getUptimeData() + "</div></div>" +

        "<div class='row'><div class='label'>Temp 1, C:</div><div class='value'>" + data1.tempStr + "</div></div>" +
        "<div class='row'><div class='label'>RH 1, %:</div><div class='value'>" + data1.humidityStr + "</div></div>" +
        "<div class='row'><div class='label'>Pressure 1, mmHg:</div><div class='value'>" + data1.pressureStr + "</div></div>" +

        "<div class='row'><div class='label'>Temp 2, C:</div><div class='value'>" + data2.tempStr + "</div></div>" +
        "<div class='row'><div class='label'>RH 2, %:</div><div class='value'>" + data2.humidityStr + "</div></div>" +
        "<div class='row'><div class='label'>Pressure 2, mmHg:</div><div class='value'>" + data2.pressureStr + "</div></div>" +

        "<div class='row'><div class='label'>Temp 3, C:</div><div class='value'>" + data3.tempStr + "</div></div>" +
        "<div class='row'><div class='label'>RH 3, %:</div><div class='value'>" + data3.humidityStr + "</div></div>" +
        "<div class='row'><div class='label'>Pressure 3, mmHg:</div><div class='value'>" + data3.pressureStr + "</div></div>" +

        "<div class='row'><div class='label'>Illumination, lx:</div><div class='value'>" + lightnessStr + "</div></div>" +

        "<div class='row'><div class='label'>Free memory:</div><div class='value'>" + getFreeMemory() + "</div></div>" +
        "</div>" +
        FPSTR(bodyEnd)
        );
    
    Serial.println("Server: request ROOT sent");
}

void webSetup()
{
    Serial.println("\r\nServer: request SETUP");

    bool config_changed = false;
    String payload = WebServer.arg("module_id");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.module_id, sizeof(config.module_id));
        config_changed = true;
    }
    payload = WebServer.arg("module_name");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.module_name, sizeof(config.module_name));
        config_changed = true;
    }
    payload = WebServer.arg("sta_ssid");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.sta_ssid, sizeof(config.sta_ssid));
        config_changed = true;
    }
    payload = WebServer.arg("sta_pwd");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.sta_pwd, sizeof(config.sta_pwd));
        config_changed = true;
    }
    payload = WebServer.arg("add_data_url");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.add_data_url, sizeof(config.add_data_url));
        config_changed = true;
    }

    payload = WebServer.arg("static_ip_mode");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.static_ip_mode, sizeof(config.static_ip_mode));
        config_changed = true;
    }
    payload = WebServer.arg("static_ip");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.static_ip, sizeof(config.static_ip));
        config_changed = true;
    }
    payload = WebServer.arg("static_gateway");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.static_gateway, sizeof(config.static_gateway));
        config_changed = true;
    }
    payload = WebServer.arg("static_subnet");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.static_subnet, sizeof(config.static_subnet));
        config_changed = true;
    }

    payload = WebServer.arg("sensor_bmp180_on");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.sensor_bmp180_on, sizeof(config.sensor_bmp180_on));
        config_changed = true;
    }
    payload = WebServer.arg("sensor_dht22_on");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.sensor_dht22_on, sizeof(config.sensor_dht22_on));
        config_changed = true;
    }
    payload = WebServer.arg("sensor_sht21_on");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.sensor_sht21_on, sizeof(config.sensor_sht21_on));
        config_changed = true;
    }
    payload = WebServer.arg("sensor_bh1750_on");
    if (payload.length() > 0)
    {
        payload.toCharArray(config.sensor_bh1750_on, sizeof(config.sensor_bh1750_on));
        config_changed = true;
    }

    WebServer.send(200, "text/html", String("") +
        FPSTR(headStart) + FPSTR(styles) + FPSTR(scripts) + FPSTR(headEnd) + FPSTR(bodyStart) + FPSTR(mainMenu) +
        "<h2>Module Setup</h2>" +
        "<div class='container'>" +
        "<div class='row'><div class='label'>Module ID:</div><div class='value'><input type='text' id='module_id' value='" + config.module_id + "' /></div></div>" +
        "<div class='row'><div class='label'>Module Name:</div><div class='value'><input type='text' id='module_name' value='" + config.module_name + "' /></div></div>" +

        "<div class='row'><div class='label'>SSID:</div><div class='value'><input type='text' id='sta_ssid' value='" + config.sta_ssid + "' /></div></div>" +
        "<div class='row'><div class='label'>Password:</div><div class='value'><input type='text' id='sta_pwd' value='" + config.sta_pwd + "' /></div></div>" +

        "<div class='row'><div class='label'>Static IP Mode:</div><div class='value'><input type='text' id='static_ip_mode' value='" + config.static_ip_mode + "' /></div></div>" +
        "<div class='row'><div class='label'>Static IP:</div><div class='value'><input type='text' id='static_ip' value='" + config.static_ip + "' /></div></div>" +
        "<div class='row'><div class='label'>Gateway:</div><div class='value'><input type='text' id='static_gateway' value='" + config.static_gateway + "' /></div></div>" +
        "<div class='row'><div class='label'>Subnet:</div><div class='value'><input type='text' id='static_subnet' value='" + config.static_subnet + "' /></div></div>" +

        "<div class='row'><div class='label'>Add Data URL:</div><div class='value'><input type='text' id='add_data_url' value='" + config.add_data_url + "' /></div></div>" +

        "<div class='row'><div class='label'>BMP180 On:</div><div class='value'><input type='text' id='sensor_bmp180_on' value='" + config.sensor_bmp180_on + "' /></div></div>" +
        "<div class='row'><div class='label'>DHT22 On:</div><div class='value'><input type='text' id='sensor_dht22_on' value='" + config.sensor_dht22_on + "' /></div></div>" +
        "<div class='row'><div class='label'>SHT21 On:</div><div class='value'><input type='text' id='sensor_sht21_on' value='" + config.sensor_sht21_on + "' /></div></div>" +
        "<div class='row'><div class='label'>BH1750 On:</div><div class='value'><input type='text' id='sensor_bh1750_on' value='" + config.sensor_bh1750_on + "' /></div></div>" +

        "<div class='footer'><input type='button' value='Save' onclick='saveFormData(\"/setup\");'/></div>" +
        "</div>" +
        FPSTR(bodyEnd)
        );

    if (config_changed)
    {
        config.saveConfig();
    }

    Serial.println("Server: request SETUP sent");
}

void webReboot()
{
    Serial.println("\r\nServer: request REBOOT");

    WebServer.send(200, "text/html", String("") +
        FPSTR(headStart) + FPSTR(styles) + FPSTR(rebootScripts) + FPSTR(scripts) + FPSTR(headEnd) + FPSTR(bodyStart) + FPSTR(mainMenu) +
        "<h2 id='info'>Module Reboot in " + config.reboot_delay + " sec(s)</h2>" +
        FPSTR(bodyEnd)
        );

    Serial.println("Server: request REBOOT sent");
    rebootESP();
}

void webTime()
{
    Serial.println("\r\nServer: request TIME");

    DateTime now = rtc.now();
    int years = now.year();
    int months = now.month();
    int days = now.day();
    int hours = now.hour();
    int minutes = now.minute();
    int seconds = now.second();

    bool time_changed = false;
    String payload = WebServer.arg("year");
    if (payload.length() > 0)
    {
        years = atoi(payload.c_str());
        time_changed = true;
    }
    payload = WebServer.arg("month");
    if (payload.length() > 0)
    {
        months = atoi(payload.c_str());
        time_changed = true;
    }
    payload = WebServer.arg("day");
    if (payload.length() > 0)
    {
        days = atoi(payload.c_str());
        time_changed = true;
    }
    payload = WebServer.arg("hour");
    if (payload.length() > 0)
    {
        hours = atoi(payload.c_str());
        time_changed = true;
    }
    payload = WebServer.arg("minute");
    if (payload.length() > 0)
    {
        minutes = atoi(payload.c_str());
        time_changed = true;
    }
    payload = WebServer.arg("second");
    if (payload.length() > 0)
    {
        seconds = atoi(payload.c_str());
        time_changed = true;
    }    

    WebServer.send(200, "text/html", String("") +
        FPSTR(headStart) + FPSTR(styles) + FPSTR(scripts) + FPSTR(headEnd) + FPSTR(bodyStart) + FPSTR(mainMenu) +
        "<h2>Setting Module Time</h2>" +
        "<div class='container'>" +
        "<div class='row'><div class='label'>Year:</div><div class='value'><input type='text' id='year' value='" + years + "' /></div></div>" +
        "<div class='row'><div class='label'>Month:</div><div class='value'><input type='text' id='month' value='" + months + "' /></div></div>" +
        "<div class='row'><div class='label'>Day:</div><div class='value'><input type='text' id='day' value='" + days + "' /></div></div>" +
        "<div class='row'><div class='label'>Hour:</div><div class='value'><input type='text' id='hour' value='" + hours + "' /></div></div>" +
        "<div class='row'><div class='label'>Minute:</div><div class='value'><input type='text' id='minute' value='" + minutes + "' /></div></div>" +
        "<div class='row'><div class='label'>Second:</div><div class='value'><input type='text' id='second' value='" + seconds + "' /></div></div>" +
        "<div class='footer'><input type='button' value='Save' onclick='saveFormData(\"/time\");'/></div>" +
        "</div>" +
        FPSTR(bodyEnd)
        );

    if (time_changed)
    {
        Serial.println("Server: setting new TIME");
        rtc.adjust(DateTime(years, months, days, hours, minutes, seconds));
    }

    Serial.println("Server: request TIME sent");
}

void handleNotFound()
{
    Serial.println("\r\nServer: not found");

    WebServer.send(404, "text/html", String("") +
        FPSTR(headStart) + FPSTR(styles) + FPSTR(headEnd) + FPSTR(bodyStart) + FPSTR(mainMenu) +
        "<h2 id='info'>Page " + WebServer.uri() + ", arguments " + WebServer.args() + " not found.</h2>" +
        FPSTR(bodyEnd)
        );
}

void initWebServer()
{
    Serial.println("Server: starting");
    WebServer.on("/", webRoot);
    WebServer.on("/setup", webSetup);
    WebServer.on("/time", webTime);
    WebServer.on("/reboot", webReboot);
    WebServer.onNotFound(handleNotFound);
    WebServer.begin();
    Serial.println("Server: started");
}

int connectWiFi()
{
    Serial.println("Wifi: connecting");

    int connectAttempts = 0;

    while (connectAttempts < maxConnectAttempts)
    {
        Serial.printf("Wifi: connecting, attempt %d\r\n", connectAttempts);
        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("Wifi: connected");
            renderWiFiStatus("On", 255, 255, 255);
            return 1;
        }

        delay(500);
        connectAttempts++;

        yield();
    }

    Serial.println("Wifi: timeout");
    renderWiFiStatus("Timeout", 255, 0, 0);
    return 0;
}

void handleWiFiEvent(WiFiEvent_t event)
{
    switch (event)
    {
    case WIFI_EVENT_STAMODE_CONNECTED:
        Serial.println("Wifi event: WIFI_EVENT_STAMODE_CONNECTED");
        renderWiFiStatus("Linking", 255, 255, 0);
        break;
    case WIFI_EVENT_STAMODE_DISCONNECTED:
        Serial.println("Wifi event: WIFI_EVENT_STAMODE_DISCONNECTED");
        renderWiFiStatus("Off", 255, 0, 0);
        break;
    case WIFI_EVENT_STAMODE_AUTHMODE_CHANGE:
        Serial.println("Wifi event: WIFI_EVENT_STAMODE_AUTHMODE_CHANGE");
        break;
    case WIFI_EVENT_STAMODE_GOT_IP:
        Serial.println("Wifi event: WIFI_EVENT_STAMODE_GOT_IP");
        Serial.print("Wifi: connected, IP = ");
        Serial.print(WiFi.localIP());
        Serial.println();
        renderWiFiStatus("On", 255, 255, 255);
        break;
    case WIFI_EVENT_STAMODE_DHCP_TIMEOUT:
        Serial.println("Wifi event: WIFI_EVENT_STAMODE_DHCP_TIMEOUT");
        break;
    case WIFI_EVENT_SOFTAPMODE_STACONNECTED:
        Serial.println("Wifi event: WIFI_EVENT_SOFTAPMODE_STACONNECTED");
        renderAPStatus("Connected", 255, 255, 255);
        break;
    case WIFI_EVENT_SOFTAPMODE_STADISCONNECTED:
        Serial.println("Wifi event: WIFI_EVENT_SOFTAPMODE_STADISCONNECTED");
        renderAPStatus("Off", 255, 255, 255);
        break;
    case WIFI_EVENT_SOFTAPMODE_PROBEREQRECVED:
        break;
    case WIFI_EVENT_MAX:
        Serial.println("Wifi event: WIFI_EVENT_MAX");
        break;
    }
}

void initWiFi()
{
    Serial.println("Wifi: starting");

    renderWiFiStatus("Off", 255, 0, 0);
    renderServerStatus("-", 255, 255, 255);
    renderAPStatus("Off", 255, 255, 255);

    delay(1000);
    WiFi.mode(WIFI_AP_STA);
    WiFi.onEvent(handleWiFiEvent);
    WiFi.begin(config.sta_ssid, config.sta_pwd);
    if (atoi(config.static_ip_mode) == 1)
    {
        Serial.println("Wifi: use static IP");
        IPAddress staticIP = stringToIp(config.static_ip);
        IPAddress staticGateway = stringToIp(config.static_gateway);
        IPAddress staticSubnet = stringToIp(config.static_subnet);
        WiFi.config(staticIP, staticGateway, staticSubnet);
    }
    else
    {
        Serial.println("Wifi: using DHCP");
    }

    Serial.println(String("Wifi: connect to '") + config.sta_ssid + "' with password '" + config.sta_pwd + "'");

    connectWiFi();

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println(String("Wifi: connected, creating AP ") + config.module_name);
        WiFi.softAP(config.module_name);
        Serial.print("Wifi: connected, IP = ");
        Serial.print(WiFi.localIP());
        Serial.println();
    }
    else
    {
        Serial.println(String("Wifi: not connected, creating AP ") + config.module_name);
        WiFi.mode(WIFI_AP);
        WiFi.softAP(config.module_name);
    }

    Serial.println("Wifi: started\r\n");

    initWebServer();

    if (!MDNS.begin(config.module_name))
    {
        while (1)
        {
            delay(1000);
            yield();
        }
    }
    MDNS.addService("http", "tcp", 80);
}

void initSensors()
{
    dht22.begin();

    if (bmp180.begin())
    {
        bmp180initialized = true;
    }

    sht21.begin();

    lightMeter.begin();
}

void initLcd()
{
    myGLCD.InitLCD();
    myGLCD.setFont(BigFont);
    fontHeight = myGLCD.getFontYsize();

    renderHeader();
}

void initRtc()
{
  if (!rtc.begin())
  {
    Serial.println("RTC: couldn't find");
  }
  else
  {
    rtcInitialized = true;
  }

  if (!rtc.isrunning())
  {
    Serial.println("RTC: isn't running, setting time");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void setup()
{
    Serial.begin(115200);
    isRebooting = false;

    Serial.println("\r\nStarting module...");

    if (!SPIFFS.begin())
    {
        Serial.println("Config: failed to mount file system");
    }
    else
    {
        if (!config.loadConfig())
        {
            Serial.println("Config: failed to load");
        }
        else
        {
            Serial.println("Config: loaded");
        }

        config.printConfig();
    }

    initRtc();
    initLcd();
    initWiFi();
    initSensors();

    Serial.println("\r\nStarting complete.");
}

SensorData getSht21Data()
{
    float h = sht21.readHumidity();
    float t = sht21.readTemperature();
    SensorData data;
    data.humidity = h;
    data.temp = t;
    data.pressure = 0;
    return data;
}

SensorData getDht22Data()
{
    float h = dht22.readHumidity();
    float t = dht22.readTemperature();
    SensorData data;
    data.humidity = h;
    data.temp = t;
    data.pressure = 0;
    return data;
}

uint16_t getLightness()
{
    uint16_t lux = lightMeter.readLightLevel();
    return lux;
}

SensorData getBmp180Data()
{
    char status;
    double T, P, p0, a;

    // If you want to measure altitude, and not pressure, you will instead need
    // to provide a known baseline pressure. This is shown at the end of the sketch.
    // You must first get a temperature measurement to perform a pressure reading.  
    // Start a temperature measurement:
    // If request is successful, the number of ms to wait is returned.
    // If request is unsuccessful, 0 is returned.
    status = bmp180.startTemperature();

    if (status != 0)
    {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed temperature measurement:
        // Note that the measurement is stored in the variable T.
        // Function returns 1 if successful, 0 if failure.
        status = bmp180.getTemperature(T);

        if (status != 0)
        {
            // Start a pressure measurement:
            // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
            // If request is successful, the number of ms to wait is returned.
            // If request is unsuccessful, 0 is returned.
            status = bmp180.startPressure(3);

            if (status != 0)
            {
                // Wait for the measurement to complete:
                delay(status);

                // Retrieve the completed pressure measurement:
                // Note that the measurement is stored in the variable P.
                // Note also that the function requires the previous temperature measurement (T).
                // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
                // Function returns 1 if successful, 0 if failure.
                status = bmp180.getPressure(P, T);

                if (status != 0)
                {
                    // The pressure sensor returns abolute pressure, which varies with altitude.
                    // To remove the effects of altitude, use the sealevel function and your current altitude.
                    // This number is commonly used in weather reports.
                    // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
                    // Result: p0 = sea-level compensated pressure in mb

                    p0 = bmp180.sealevel(P, ALTITUDE);

                    // On the other hand, if you want to determine your altitude from the pressure reading,
                    // use the altitude function along with a baseline pressure (sea-level or other).
                    // Parameters: P = absolute pressure in mb, p0 = baseline pressure in mb.
                    // Result: a = altitude in m.

                    a = bmp180.altitude(P, p0);
                }
            }
        }
    }

    SensorData data;
    if (bmp180initialized)
    {
        data.temp = T;
        data.pressure = p0;
        data.humidity = 0;
    }    
    return data;
}

void renderRowValue(String value, int row, int r = 0, int g = 255, int b = 0)
{
    if (isRebooting)
    {
        //do nothing if rebooting
        return;
    }

    while (value.length() < 10)
    {
        value += " ";
    }
    myGLCD.setColor(r, g, b);
    myGLCD.print(value, valueX, getRowY(row, fontHeight));
    myGLCD.setColor(0, 255, 0);
}

void renderWiFiStatus(String status, int r, int g, int b)
{
    renderRowValue(status, 0, r, g, b);
}

void renderServerStatus(String status, int r, int g, int b)
{
    renderRowValue(status, 1, r, g, b);
}

void renderAPStatus(String status, int r, int g, int b)
{
    renderRowValue(status, 2, r, g, b);
}

void renderSensorValues()
{
    if (atoi(config.sensor_dht22_on) == 1)
    {
        data1 = getDht22Data();
        data1.tempStr = floatToString(data1.temp, VALUE_TEMP);
        data1.humidityStr = floatToString(data1.humidity, VALUE_HUMIDITY);
        data1.pressureStr = floatToString(data1.pressure * 0.0295333727 * 25.4, VALUE_PRESSURE, 3, 0);
        renderRowValue(data1.tempStr, 3);
        renderRowValue(data1.humidityStr, 4);
        Serial.println(String("Temp 1    : " + data1.tempStr));
        Serial.println(String("RH 1      : " + data1.humidityStr));
        Serial.println(String("Pressure 1: " + data1.pressureStr));
    }

    if (atoi(config.sensor_bmp180_on) == 1)
    {
        data2 = getBmp180Data();
        data2.tempStr = floatToString(data2.temp, VALUE_TEMP);
        data2.humidityStr = floatToString(data2.humidity, VALUE_HUMIDITY);
        data2.pressureStr = floatToString(data2.pressure * 0.0295333727 * 25.4, VALUE_PRESSURE, 3, 0);
        renderRowValue(data2.pressureStr, 5);
        renderRowValue(data2.tempStr, 6);
        Serial.println(String("Temp 2    : " + data2.tempStr));
        Serial.println(String("RH 2      : " + data2.humidityStr));
        Serial.println(String("Pressure 2: " + data2.pressureStr));
    }

    if (atoi(config.sensor_sht21_on) == 1)
    {
        data3 = getSht21Data();
        data3.tempStr = floatToString(data3.temp, VALUE_TEMP);
        data3.humidityStr = floatToString(data3.humidity, VALUE_HUMIDITY);
        data3.pressureStr = floatToString(data3.pressure * 0.0295333727 * 25.4, VALUE_PRESSURE, 3, 0);
        Serial.println(String("Temp 3    : " + data3.tempStr));
        Serial.println(String("RH 3      : " + data3.humidityStr));
        Serial.println(String("Pressure 3: " + data3.pressureStr));
    }

    if (atoi(config.sensor_bh1750_on) == 1)
    {
        lightness = getLightness();
        lightnessStr = floatToString(lightness, VALUE_ILLUMINATION, 5, 0);
        Serial.println(String("Light     : " + lightnessStr));
    }

    if (isRtcInitialized())
        Serial.println(String("RTC       : ") + getDateTimeString(rtc.now()));
    else
        Serial.println("RTC       : -");
}

void parseServerResponse(String payload)
{
    StaticJsonBuffer<2048> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(payload.c_str());
    int years = root["year"];
    int months = root["month"];
    int days = root["day"];
    int hours = root["hour"];
    int minutes = root["minute"];
    int seconds = root["second"];
    Serial.println(String("HTTPClient: server time ") + hours + ":" + minutes + ":" + seconds);

    if (!isRtcInitialized())
    {
        Serial.println("HTTPClient: RTC off, skip setting time");
        return;
    }

    DateTime now = rtc.now();
    int years1 = now.year();
    int months1 = now.month();
    int days1 = now.day();
    int hours1 = now.hour();
    int minutes1 = now.minute();
    int seconds1 = now.second();

    if (years1 != years || months1 != months || days1 != days || hours1 != hours || minutes1 != minutes || seconds1 != seconds)
    {
        //set server time
        Serial.println("HTTPClient: set server time");
        rtc.adjust(DateTime(years, months, days, hours, minutes, seconds));
    }
}

float getTempForJson(float value)
{
    return (isnan(value) || value > 70) ? 0 : value;
}

float getPressureForJson(float value)
{
    return isnan(value) ? 0 : value;
}

float getHumidityForJson(float value)
{
    return (isnan(value) || value > 100) ? 0 : value;
}

float getIlluminationForJson(float value)
{
    return (isnan(value) || value > 50000) ? 0 : value;
}

String getSensorsDataJson()
{
    StaticJsonBuffer<1024> jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();

    json["moduleid"] = atoi(config.module_id);
    json["modulename"] = config.module_name;

    json["temperature1"] = getTempForJson(data1.temp);
    json["humidity1"] = getHumidityForJson(data1.humidity);
    json["pressure1"] = getPressureForJson(data1.pressure);

    json["temperature2"] = getTempForJson(data2.temp);
    json["humidity2"] = getHumidityForJson(data2.humidity);
    json["pressure2"] = getPressureForJson(data2.pressure);

    json["temperature3"] = getTempForJson(data3.temp);
    json["humidity3"] = getHumidityForJson(data3.humidity);
    json["pressure3"] = getPressureForJson(data3.pressure);

    json["illumination"] = getIlluminationForJson(lightness);

    //send handshake and info about module
    if (!meetWithServer)
    {
        json["meet"] = 1;
        json["ip"] = getIpString(WiFi.localIP());
        json["mac"] = getMacString();
    }

    char buffer[2048];
    json.printTo(buffer, sizeof(buffer));

    return String(buffer);
}

void saveSensorsData()
{
    Serial.println("\r\nHTTPClient: starting");

    String payload = getSensorsDataJson();
    HTTPClient http;

    Serial.println(String("HTTPClient: request URL ") + config.add_data_url + " with payload " + payload);
    http.begin(config.add_data_url);
    Serial.println("HTTPClient: URL requested");

    int httpCode = http.POST(payload);

    if (httpCode > 0)
    {
        if (httpCode == HTTP_CODE_OK)
        {
            String payload = http.getString();
            parseServerResponse(payload);
            Serial.println("HTTPClient: " + payload);
            renderServerStatus(String("OK (") + httpCode + ")   ", 255, 255, 255);
            Serial.printf("HTTPClient: server OK %d\r\n", httpCode);
        }
        else
        {
            renderServerStatus(String("Error (") + httpCode + ")", 255, 0, 0);
            Serial.printf("HTTPClient: server ERROR %d\r\n", httpCode);
        }
    }
    else
    {
        Serial.println("HTTPClient: server OFF");
        renderServerStatus("Off (404)  ", 255, 0, 0);
    }

    http.end();

    Serial.println("HTTPClient: ended");

    yield();
}

void loop()
{
    WebServer.handleClient();

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= ONE_SECOND)
    {
        currentSensorCycle++;
        previousMillis = currentMillis;
        renderDateTime();
        
        if (currentSensorCycle % atoi(config.get_data_delay) == 0)
        {
            Serial.println("\r\nGetting sensors data...");
            renderSensorValues();
            if (WiFi.status() == WL_CONNECTED)
            {
                saveSensorsData();
            }
        }
    }
}

