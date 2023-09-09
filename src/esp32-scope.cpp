// -*- coding: utf-8 -*-
// hardware: picos3

#include <esp_wifi.h>
#include "driver/adc.h"
#include <WiFi.h>
#include <WiFiUdp.h>

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

#include "secrets.h"

const char *udpAddress = "192.168.86.102";
const int udpPort = 10000;

WiFiUDP udp;

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        delay(10);
    delay(100);
    Serial.println(__FILENAME__);

    WiFi.begin(ssid, pwd);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop()
{
    static long i = 0;
    int v0 = analogRead(A1);
    uint8_t buff[50];
    size_t len = sprintf((char *)buff, ">v1:%d i:%d<", v0, i);
    i += 1;
    if (i > 50)
        i = 0;

    udp.beginPacket(udpAddress, udpPort);
    udp.write(buff, len);
    udp.endPacket();
    delay(20);
}