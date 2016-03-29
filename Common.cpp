#include "Common.h"

int getRowY(int row, int fontHeight)
{
    return 1 + (10 + fontHeight) * row;
}

String twoDigits(int value)
{
    if (value < 10)
        return String("0") + value;
    return String(value);
}

String getDateTimeString(DateTime now)
{
    String dt = String(twoDigits(now.hour())) + ":" + twoDigits(now.minute()) + ":" + twoDigits(now.second()) + " " + now.day() + "/" + now.month() + "/" + now.year();
    return dt;
}

IPAddress stringToIp(String strIp)
{
    String temp;
    IPAddress ip;

    int count = 0;
    for (int i = 0; i <= strIp.length(); i++)
    {
        if (strIp[i] != '.')
        {
            temp += strIp[i];
        }
        else
        {
            if (count < 4)
            {
                ip[count] = atoi(temp.c_str());
                temp = "";
                count++;
            }
        }
        if (i == strIp.length())
        {
            ip[count] = atoi(temp.c_str());
        }
    }
    return ip;
}

bool isIPValid(const char * IP)
{
    //limited size
    int internalcount = 0;
    int dotcount = 0;
    bool previouswasdot = false;
    char c;

    if (strlen(IP) > 15 || strlen(IP) == 0)
    {
        return false;
    }
    //cannot start with .
    if (IP[0] == '.')
    {
        return false;
    }

    //only letter and digit
    for (int i = 0; i < strlen(IP); i++)
    {
        c = IP[i];
        if (isdigit(c))
        {
            //only 3 digit at once
            internalcount++;
            previouswasdot = false;
            if (internalcount > 3)
            {
                return false;
            }
        }
        else if (c == '.')
        {
            //cannot have 2 dots side by side
            if (previouswasdot)
            {
                return false;
            }
            previouswasdot = true;
            internalcount = 0;
            dotcount++;
        }//if not a dot neither a digit it is wrong
        else
        {
            return false;
        }
    }
    //if not 3 dots then it is wrong
    if (dotcount != 3)
    {
        return false;
    }
    //cannot have the last dot as last char
    if (IP[strlen(IP) - 1] == '.')
    {
        return false;
    }
    return true;
}

String floatToString(float f, int valueType, int digits, int decimals)
{
    if (isnan(f) || f == 0.0 || f < 0.0)
    {
        return "-";
    }
    if (valueType == VALUE_TEMP)
    {
        if (f > 70) return "-";
    }
    if (valueType == VALUE_HUMIDITY)
    {
        if (f > 100) return "-";
    }
    if (valueType == VALUE_ILLUMINATION)
    {
        if (f > 50000) return "-";
    }

    char c[10];
    dtostrf(f, digits, decimals, c);
    return String(c);
}

String getUptimeData()
{
    long Day = 0;
    int Hour = 0;
    int Minute = 0;
    int Second = 0;
    int HighMillis = 0;
    int Rollover = 0;
    char value_buff[120];

    //** Making Note of an expected rollover *****//
    if (millis() >= 3000000000)
    {
        HighMillis = 1;
    }
    //** Making note of actual rollover **//
    if (millis() <= 100000 && HighMillis == 1)
    {
        Rollover++;
        HighMillis = 0;
    }

    long secsUp = millis() / 1000;

    Second = secsUp % 60;
    Minute = (secsUp / 60) % 60;
    Hour = (secsUp / (60 * 60)) % 24;
    Day = (Rollover * 50) + (secsUp / (60 * 60 * 24));  //First portion takes care of a rollover [around 50 days]

    sprintf_P(value_buff, (const char *)F("%dd %02d:%02d"), Day, Hour, Minute);
    Serial.print(F("Uptime: "));  Serial.print(value_buff);  Serial.print(F(":"));  Serial.print(Second / 10);  Serial.println(Second % 10);

    return value_buff;
}

String getFreeMemory()
{
    return String(ESP.getFreeHeap());
}

String getIpString(IPAddress ip)
{
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    return ipStr;
}

String getMacString()
{
    char value_buff[120];

    uint8_t macData[6];
    WiFi.macAddress(macData);
    sprintf_P(value_buff, (const char *)F("%x:%x:%x:%x:%x:%x"), macData[0], macData[1], macData[2], macData[3], macData[4], macData[5]);
    return String(value_buff);
}
