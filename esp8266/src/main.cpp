#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>

MDNSResponder mdns;
ESP8266WebServer server(80);

void handle_index() {
  Serial.println("handling index");
  File file = SPIFFS.open("/index.html","r");
  server.streamFile(file, "text/html");
  file.close();
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void handle_get_ssid_and_password(){
  if(!server.hasArg("ssid") || !server.hasArg("password"))
    Serial.println("incorrect argument format!");
  const char* SSID = server.arg("ssid").c_str();
  const char* password = server.arg("password").c_str();
  Serial.print("SSID: ");
  Serial.println(SSID);
  Serial.print("password: ");
  Serial.println(password);
  WiFi.softAPdisconnect();
  delay(200);
  WiFi.begin(SSID,password);
  for(int i=0; i<5 && WiFi.status() != WL_CONNECTED; i++){
    delay(1000);
  }

  if(WiFi.status() == WL_CONNECTED){
    Serial.print("Connected to ");
    Serial.println(SSID);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    if (mdns.begin("esp8266", WiFi.localIP())) {
      Serial.println("MDNS responder started");
    }
    server.stop();
    server.close();
    server.onNotFound([]{
      server.send(418, "text/plain", "I'm a teapot.\nWell, not actually a teapot, I'm a mate.\nAnd I hold no affiliation with the desktop environment.");
    });
    server.begin();
    Serial.println("HTTP server restarted");
  }
  else{
    Serial.println("Invlid SSID/password. Check for typos");
    server.send(200,"text/plain","Invlid SSID/password. Check for typos");
  }
}

void setup(void){
  Serial.begin(115200);
  SPIFFS.begin();
  WiFi.mode(WIFI_AP);

  WiFi.softAPConfig(IPAddress(192,168,0,1),IPAddress(192,168,0,1),IPAddress(255,255,255,0));
  const char* ssid = "yaestaelmate";
  WiFi.softAP(ssid);
  Serial.print(WiFi.softAPIP());
  Serial.println(" Is the local IP. Waiting for configuration");
  WiFi.printDiag(Serial);
  Serial.println("");
  
  server.on("/", handle_index);
  server.on("/get_ssid_and_password", handle_get_ssid_and_password);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
