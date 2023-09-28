// -*- coding: utf-8 -*-
// hardware: picos3

#include <esp_wifi.h>
#include "driver/adc.h"
#include <WiFi.h>
#include <WiFiUdp.h>

#include "wifi_setup.hpp"


#define PICOS3_LED_DATA 18
#define PICOS3_LED_PWR 17
#define PICOS3_5V_PRES 33
#define PICOS3_VBAT 10
#define PICOS3_BOOT 0
#define PICOS3_SDA 8
#define PICOS3_SCL 9
#define PICOS3_SCLK 36
#define PICOS3_MOSI 35
#define PICOS3_MISO 37
#define PICOS3_CS 34

const char *udpAddress = "192.168.1.138"; // 197 is wilderer 
const int udpPort = 10000;

WiFiUDP udp;

const char* hostname = "esp32-scope";


void setup()
{
    Serial.begin(115200);
    //while (!Serial) delay(10);
    delay(100);
    Serial.println(__FILENAME__);

    wifi_setup(hostname);
}

void loop()
{
    ArduinoOTA.handle();

    static long i = 0;
    static unsigned long t0 = millis();
    int v0 = analogRead(A10);
    uint8_t buff[50];
    size_t len = sprintf((char *)buff, ">v1:%d i:%d<", v0, i);
    i += 1;
    unsigned long t1 = millis();
    if (t1 - t0 >= 1000)
    {
        i = 0;
        t0 = t1;
    }
    
    udp.beginPacket(udpAddress, udpPort);
    udp.write(buff, len);
    if (!udp.endPacket())
    {
        Serial.println("TxErr");
        delay(500);
    }
    delay(20);
}