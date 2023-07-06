#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <DHTesp.h>
#include <BH1750.h>
#include <ThingsBoard.h>

#define WIFI_SSID "ALVIN"
#define WIFI_PASSWORD "alvin123"

#define TOKEN "Alvin2502000413"
#define THINGSBOARD_SERVER "demo.thingsboard.io"

#define DHT_PIN 19

DHTesp dht;
BH1750 lightMeter;
WiFiClient wifiClient;
ThingsBoard tb(wifiClient);

void WifiConnect()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  if (WiFi.waitForConnectResult() == WL_CONNECTED){
    Serial.println("Connected to the WiFi network");
  }
  else{
    Serial.println("Connection Failed! Rebooting...");
    delay(10000);
    ESP.restart();
  }  
}

void setup() {
  Serial.begin(115200);
  dht.setup(DHT_PIN, DHTesp::DHT11);
  Wire.begin();
  lightMeter.begin(); 
  WifiConnect();
  if (tb.connect(THINGSBOARD_SERVER,TOKEN)){
    Serial.println("Connected to ThingsBoard");
  }
  else{
    Serial.println("Error connecting to ThingsBoard");
  }
}

void loop() {
  if (millis() % 5000 ==0) //delay 5 detik
  {
    float fHumidity = dht.getHumidity();
    float fTemperature = dht.getTemperature();
    float lux = lightMeter.readLightLevel();
    Serial.printf("Humidity: %.2f, Temperature: %.2f, Light: %.2f \n",
       fHumidity, fTemperature, lux);
    tb.sendTelemetryFloat("humidity", fHumidity);
    tb.sendTelemetryFloat("temperature", fTemperature);
    tb.sendTelemetryFloat("light", lux);
    // Serial.println("masok");
  }
  tb.loop();
}