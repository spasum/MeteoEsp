#ifndef COMMON_H
#define COMMON_H

#include <Arduino.h>
#include <RTClib.h>
#include <ESP8266WiFi.h>
#include <IPAddress.h>

int getRowY(int row, int fontHeight);
String twoDigits(int value);
String getDateTimeString(DateTime now);
IPAddress stringToIp(String strIp);
bool isIPValid(const char * IP);
String floatToString(float f, int digits = 4, int decimals = 1);
String getUptimeData();
String getFreeMemory();
String getIpString(IPAddress ip);
String getMacString();

#endif
