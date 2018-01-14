#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <DNSServer.h>
#include <WiFiManager.h>

#include "pinout.h"

const char* NOTIFICATION_SERVER_URL = "http://192.168.0.17:3000/json_rpc";
const int BUTTON_PIN = D1;

class Debounced_pushbutton{
    public:
    Debounced_pushbutton(int pin){
        pinMode(pin, INPUT_PULLUP);
        this->pin              = pin;
        this->last_update_time = millis();
        this->last_status      = digitalRead(this->pin);
    }
    bool pressed(){
        this->update();
        return !last_status; //because of pullup
    }
    void update(){
        int current_status = digitalRead(this->pin);
        if( (current_status != last_status) && (last_update_time + DEBOUNCE_TIME < millis()) )
            last_status = current_status;
    }

    private:
    int pin;
    long last_update_time;
    bool last_status;
    static const int DEBOUNCE_TIME=50;
};

void setup(void){
    WiFiManager wifiManager;
    Serial.begin(115200);
    wifiManager.autoConnect("gaucho");
    Serial.print("Connected succesfully. IP: ");
    Serial.println(WiFi.localIP());
    long last_notification_time = 0;

    Debounced_pushbutton button = Debounced_pushbutton(BUTTON_PIN);

    while (true){
        button.update();
        if(button.pressed() && last_notification_time + 5000 < millis()){
            last_notification_time = millis();
            HTTPClient http;
            http.begin(NOTIFICATION_SERVER_URL);
            http.addHeader("Content-Type","application/json");
            int http_code = http.POST("{\"jsonrpc\": \"2.0\", \"method\": \"ya_esta_el_mate\", \"params\": [\"ya esta el mate\"]}");
            Serial.printf("POSTed. return value: %i\n",http_code);
        }
        delay(10);
    }
}

void loop(void){
}
