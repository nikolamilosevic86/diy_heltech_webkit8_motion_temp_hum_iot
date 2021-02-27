// This example just provide basic function test;
// For more informations, please vist www.heltec.cn or mail to support@heltec.cn

#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

#include <dht.h>

dht DHT;

#define DHT11_PIN 15

ESP8266WiFiMulti wifiMulti;
#include "heltec.h" // alias for `#include "SSD1306Wire.h"`

int sensorPin = 13;
String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") + \
         String(ipAddress[1]) + String(".") + \
         String(ipAddress[2]) + String(".") + \
         String(ipAddress[3])  ;
}
void setup() {
  // put your setup code here, to run once:
  pinMode(sensorPin, INPUT);
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Serial Enable*/);
  Heltec.display->init();
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_10);

  Heltec.display->drawString(0, 0, "Hello Nikola!");

  Heltec.display->display();
  delay(5000);

  WiFi.mode(WIFI_STA);
  wifiMulti.addAP("Vodafone-83AC", "MfN9rHpHPnsFzMLx");
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Connecting Wifi...");
  Heltec.display->display();
  delay(1000);

  if (wifiMulti.run() == WL_CONNECTED) {
    Heltec.display->clear();
    Heltec.display->drawString(0, 0, "WiFi connected");

    Heltec.display->drawString(0, 10, "IP address: ");
    String broadCast = IpAddress2String(WiFi.localIP());
    Heltec.display->drawString(0, 20, broadCast);
    Heltec.display->display();
  }

}

void loop() {
  if (wifiMulti.run() != WL_CONNECTED) {
    Heltec.display->clear();
    Heltec.display->drawString(10, 0, "WiFi not connected!");
    delay(1000);
  } else {
    //Heltec.display->clear();
    //Heltec.display->drawString(0,0,"WiFi connected");

    //Heltec.display->drawString(0,10,"IP address: ");
    String broadCast = IpAddress2String(WiFi.localIP());
    //Heltec.display->drawString(0,20,broadCast);
    //Heltec.display->display();
    int chk = DHT.read11(DHT11_PIN);
    String statuss = "";
    switch (chk)
    {
      case DHTLIB_OK:
        statuss = "OK" ;
        break;
      case DHTLIB_ERROR_CHECKSUM:
        statuss = "checksum error";
        break;
      case DHTLIB_ERROR_TIMEOUT:
        statuss = "timeout error" ;
        break;
      default:
        statuss = "unknown error";
        break;
    }
    // DISPLAY DATA
    String humidity = String(DHT.humidity, 2) + "%";
    String temperature = String(DHT.temperature, 2) + "°C";
    Heltec.display->clear();
    Heltec.display->drawString(0, 0, "Temp: " + temperature);
    Heltec.display->drawString(0, 10, "Humidity: " + humidity);
    int sensor = digitalRead(sensorPin);
    String motion = String(sensor,2);
    //Heltec.display->drawString(0, 20, "IP: " + broadCast);
    Heltec.display->drawString(0, 20, "Motion: " + motion);
    Heltec.display->display();
    



    WiFiClient client;

    HTTPClient http;
    String url = "http://myhomenikola.pythonanywhere.com/add_data?temp=" + temperature + "&humid=" + humidity + "&ip=" + broadCast + "&motion=" + motion;

    if (http.begin(client, url)) {  // HTTP

      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
        }
      }

      http.end();
    }
    delay(1000);
  }
  // Now it needs to send data to http://myhomenikola.pythonanywhere.com/add_data?temp=10*C&humid=43%&ip=172.20.123.23&motion=11
  // put your main code here, to run repeatedly:

}
