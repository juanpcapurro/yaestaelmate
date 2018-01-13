#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <DNSServer.h>
#include <WiFiManager.h>

#include "pinout.h"

const char* NOTIFICATION_SERVER_URL = "http://192.168.0.17:3000/json_rpc";

void setup(void){
    WiFiManager wifiManager;
    Serial.begin(115200);
    wifiManager.autoConnect("gaucho");
    Serial.print("Connected succesfully. IP: ");
    Serial.println(WiFi.localIP());

    while (true){
        HTTPClient http;
        http.begin(NOTIFICATION_SERVER_URL);
        http.addHeader("Content-Type","application/json");
        int http_code = http.POST("{\"jsonrpc\": \"2.0\", \"method\": \"ya_esta_el_mate\", \"params\": [\"ya esta el mate\"]}");
        Serial.printf("POSTed. return value: %i\n",http_code);
        delay(5000);
    }
}

void loop(void){
}
