// STM32 Blue Pill SPI LCD Example

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <DHT.h>

#define TFT_DC PB7
#define TFT_CS PB6
const int DHTPIN = A3;
const int DHTTYPE = DHT22; 

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
DHT dht(DHTPIN, DHTTYPE);

float h;
float t;
void setup() {
  Serial.begin(115200);

  tft.begin();
  dht.begin();
  h = dht.readHumidity(); //Leitura umidade relativa do sensor
  t = dht.readTemperature(); //Lê temperatura
  
  tft.setCursor(10, 120);
  tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(2);
  tft.print("Temperature : ");
   tft.println(t);

  tft.setCursor(10, 160);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(2);
  tft.print("Humidity: ");
  tft.print(h);
  tft.println(" %");
}

void loop() {
  
  
  float nh = dht.readHumidity(); 
  float nt = dht.readTemperature();
  if (nh!=h or nt!=t)
  {
    h=nh;
    t=nt;
      tft.fillScreen(ILI9341_BLACK);
      tft.setCursor(10, 120);
  tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(2);
  tft.print("temp : ");
   tft.println(t);

  tft.setCursor(10, 160);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(2);
  tft.print("humi: ");
   tft.print(h);
  tft.println("%");
  }
    Serial.println("temp : ");
    Serial.println(t); // print temperatura
    Serial.println("humi : ");
    Serial.println(h); // print umidade

  delay(200);
}