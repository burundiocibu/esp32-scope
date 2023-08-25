// -*- coding: utf-8 -*-
// hardware: picos3

#include <Wire.h>
#include <SPI.h>
#include <esp_wifi.h>
#include "driver/adc.h"

#include <Arduino_GFX_Library.h>
#include <INA226.h>
#include <ADS1220_WE.h>


// waveshare 2" IPS LCD module, ST7789V Controller
// purple vcc, white gnd
#define TFT_RST 6      // brown
#define TFT_BL 7         // grey
#define TFT_CS 34        // yellow
#define TFT_SCLK 36      // orange
#define TFT_MOSI 35      // green
#define TFT_DC 1         // blue
#define TFT_WIDTH 240
#define TFT_HEIGHT 320

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

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
// See https://github.com/moononournation/Arduino_GFX/blob/master/examples/PDQgraphicstest/Arduino_GFX_dev_device.h
Arduino_DataBus *bus = new Arduino_ESP32SPI(
    TFT_DC,
    TFT_CS,
    SCK, 
    MOSI,
    MISO);

Arduino_GFX *gfx = new Arduino_ST7789(
    bus,
    TFT_RST, 
    1,
    true,
    TFT_WIDTH,
    TFT_HEIGHT,
    0,0,0,0);

INA226 INA(0x40);

ADS1220_WE ads(ADS1220_CS, ADS1220_DR_);

void printConfig()
{
    Serial.print("LSB:\t");
    Serial.println(INA.getCurrentLSB(), 10);
    Serial.print("LSB_uA:\t");
    Serial.println(INA.getCurrentLSB_uA(), 3);
    Serial.print("shunt:\t");
    Serial.println(INA.getShunt(), 3);
    Serial.print("maxCur:\t");
    Serial.println(INA.getMaxCurrent(), 3);
    Serial.print("Conv time:\t");
    Serial.println(INA.getBusVoltageConversionTime());
    Serial.print("avg:\t");
    Serial.println(INA.getAverage());
}

void setup()
{
    Serial.begin(115200);
    while(!Serial.availableForWrite())
        delay(10);
    delay(100);
    Serial.println(__FILENAME__);

    Wire.begin(PICOS3_SDA, PICOS3_SCL);
    if (!INA.begin() )
        Serial.println("INA.begin() failed!");

    INA.setMaxCurrentShunt(.01, 1, false);
    INA.setBusVoltageConversionTime(7);
    INA.setShuntVoltageConversionTime(7);
    printConfig();

    if(!ads.init())
        Serial.println("ADS1220.init failed");
    ads.bypassPGA(false);
    Serial.printf("PGA bypass %d\n", ads.isPGABypassed());
    ads.setCompareChannels(ADS1220_MUX_3_2);
    ads.setGain(ADS1220_GAIN_32);
    Serial.printf("register 0: %x\n", ads.getRegister(0));

    if (!gfx->begin())
        Serial.println("gfx->begin() failed!");
    gfx->fillScreen(BLACK);

    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    gfx->setCursor(0, 0);
    gfx->setTextColor(WHITE);
    gfx->println(__FILENAME__);
}


void gfx_snprintf(int x, int y, int text_size, const char * fmt, ...)
{
    uint32_t t0 = micros();
    char buff[128];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buff, sizeof(buff), fmt, args);
    va_end(args);

    gfx->setTextSize(text_size);
    int16_t x1,y1;
    uint16_t w,h;
    gfx->getTextBounds(buff, x, y, &x1, &y1, &w, &h);
    gfx->fillRect(x1, y1, w, h, BLACK);
    gfx->setCursor(x, y);
    gfx->print(buff);

    int dt = micros()-t0;
    snprintf(buff, sizeof(buff), "%d", dt);
    gfx->setTextSize(1);
    x += w+5;
    gfx->getTextBounds(buff, x, y, &x1, &y1, &w, &h);
    gfx->fillRect(x1, y1, w, h, BLACK);
    gfx->setCursor(x, y);
    gfx->print(buff);

}


void loop()
{
    float bus_v = INA.getBusVoltage();
    float shunt_mv = INA.getShuntVoltage_mV();
    float shunt_ma = INA.getCurrent_mA();
    float power_mw = INA.getPower_mW();

    gfx_snprintf(0,  0, 2, "%8.3f mA", shunt_ma);
    gfx_snprintf(0, 20, 2, "%8.3f mW", power_mw);
    gfx_snprintf(0, 40, 2, "%7.3f V", bus_v);

    ads.setCompareChannels(ADS1220_MUX_3_2);
    gfx_snprintf(0, 60, 2, "%7.3f mV (%0x)", ads.getVoltage_mV(), ads.getRawData());
    gfx_snprintf(0, 100, 2, "%7.3f C", ads.getTemperature());

    delay(500);
}