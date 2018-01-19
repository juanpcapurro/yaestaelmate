#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <FS.h>

#include "pinout.h"

String NOTIFICATION_SERVER_URL;
const int BUTTON_PIN = D7;
std::unique_ptr<ESP8266WebServer> web_server;

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

void change_sever_ip(String ip){
    const String SERVER_PREFIX = "http://";
    const String SERVER_SUFFIX = ":3000/json_rpc";
    NOTIFICATION_SERVER_URL = SERVER_PREFIX + ip + SERVER_SUFFIX;
}

void handle_index() {
    SPIFFS.begin();
    Serial.println("handling index");
    File file = SPIFFS.open("/index.html","r");
    web_server->streamFile(file, "text/html");
    file.close();
    SPIFFS.end();
}

void handle_not_found(){
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += web_server->uri();
    message += "\nMethod: ";
    message += (web_server->method() == HTTP_GET)?"GET":"POST";
    message += "\nArguments: ";
    message += web_server->args();
    message += "\n";
    for (uint8_t i=0; i<web_server->args(); i++){
        message += " " + web_server->argName(i) + ": " + web_server->arg(i) + "\n";
    }
    web_server->send(404, "text/plain", message);
}

void handle_settings(){
    Serial.println("handling /settings");
    if(!web_server->hasArg("cebador_ip")){
        Serial.println("incorrect argument format!");
        web_server->send(400,"text/plain", "incorrect argument format!");
        return;
    }
    String cebador_ip = web_server->arg("cebador_ip");
    Serial.print("cebador_ip: ");
    Serial.println(cebador_ip);
    change_sever_ip(cebador_ip);
    web_server->send(200,"text/plain", "Cebador's IP changed to " + cebador_ip + " successfully");
}

void setup(void){
    WiFiManager wifiManager;
    Serial.begin(115200);
    wifiManager.autoConnect("gaucho");
    Serial.print("Connected succesfully. IP: ");
    Serial.println(WiFi.localIP());
    change_sever_ip("192.168.0.17");//default ip

    web_server.reset(new ESP8266WebServer(80));
    web_server->on("/", handle_index);
    web_server->on("/settings", handle_settings);
    web_server->onNotFound(handle_not_found);
    web_server->begin();

    Debounced_pushbutton button = Debounced_pushbutton(BUTTON_PIN);
    long last_notification_time = 0;

    while (true){
        button.update();
        web_server->handleClient();
        // ensure the cebador isn't spammed
        if(button.pressed() && last_notification_time + 5000 < millis()){
            last_notification_time = millis();
            HTTPClient http;
            http.begin(NOTIFICATION_SERVER_URL);
            http.addHeader("Content-Type","application/json");
            int http_code = http.POST("{\"jsonrpc\": \"2.0\", \"method\": \"ya_esta_el_mate\", \"params\": [\"ya esta el mate\"]}");
            Serial.printf("POSTed to %s\nreturn value: %i\n", NOTIFICATION_SERVER_URL.c_str(), http_code);
            http.end();
        }
        delay(10);
    }
}

void loop(void){
}
