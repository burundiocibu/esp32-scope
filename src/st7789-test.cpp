// -*- coding: utf-8 -*-
// hardware: picos3

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

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

Adafruit_ST7789 tft(TFT_CS,  TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup()
{
    Serial.begin(115200);
  
    while (!Serial)
        delay(10);  // Wait until serial console is opened
    Serial.println(__FILENAME__);

    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    tft.init(TFT_WIDTH, TFT_HEIGHT);
    tft.setRotation(1);
    Serial.println("TFT initialized");

    tft.fillScreen(ST77XX_RED);
    delay(100);
    tft.fillScreen(ST77XX_GREEN);
    delay(100);
    tft.fillScreen(ST77XX_BLUE);
    delay(100);
    tft.fillScreen(ST77XX_BLACK);
}

void loop() {
    delay(10);
     uint16_t color;

    color = ST77XX_BLACK;
    tft.fillTriangle(200, 45, 200, 85, 220, 65, color);

    color = ST77XX_WHITE;
    tft.fillTriangle(120, 45, 120, 85, 100, 65, color);
 
    color = ST77XX_GREEN;
    tft.fillCircle(40, 100, 20, color);
}
