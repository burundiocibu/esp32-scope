// -*- coding: utf-8 -*-
// hardware: picos3

#include <Wire.h>
#include <SPI.h>
#include <esp_wifi.h>
#include "driver/adc.h"

#include <INA226.h>
#include <ADS1220_WE.h>


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

#define ADS1220_CS 2
#define ADS1220_DR_ 3


INA226 INA(0x40);

ADS1220_WE ads(ADS1220_CS, ADS1220_DR_);

void setup()
{
    Serial.begin(115200);
    while(!Serial)
        delay(10);
    delay(100);
    Serial.println(__FILENAME__);

    Wire.begin(PICOS3_SDA, PICOS3_SCL);
    if (!INA.begin() )
        Serial.println("INA.begin() failed!");

    INA.setMaxCurrentShunt(.01, 1, false);
    INA.setBusVoltageConversionTime(7);
    INA.setShuntVoltageConversionTime(7);

    if(!ads.init())
        Serial.println("ADS1220.init failed");
    ads.bypassPGA(true);
}


void loop()
{
    ads.setCompareChannels(ADS1220_MUX_3_2);

    //Serial.printf(">%.3f %.3f<\n", ads.getVoltage_mV(), INA.getCurrent_mA());
    Serial.printf(">%.3f<\n", ads.getVoltage_mV());
}