#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include "fl1.htm"
#include "svg.h"

#define Ind0 13
#define Ind1 5
#define Ind2 16
#define Ind3 14
#define Ind4 12
#define Irst 4

ESP8266WebServer server(80);
int cntWIFIConnect = 0;
int arrDO[] = {0,0,0,0};

//---- Config ----
String cn_apmod;
String cn_ssid;
String cn_pass;
String cn_ipadr;
String cn_ipmsk;
String cn_ipgtw;
//----------------

void(* resetFunc) (void) = 0; // объявляем функцию reset

void defaultConfig(){
  cn_apmod = "softap";
  cn_ssid  = "esp8266";
  cn_pass  = "12345678";
  cn_ipadr = "192.168.4.1";
  cn_ipmsk = "255.255.255.0";
  cn_ipgtw = "192.168.4.1";
  Serial.println("Set Config to default");  
}

void readConfig() {
  EEPROM.begin(255);
  if(char(EEPROM.read(0))!='{'){
    defaultConfig();
  }
  else
  {
    int i = 0;
    char ch;
    String conf;
    do {
      ch = char(EEPROM.read(i));
      conf +=ch;
      i++;
    }
    while(i<255 && ch!='\0');

    StaticJsonDocument<255> root;
    deserializeJson(root,conf);
    
    cn_apmod = (const char*) root["apmod"];
    cn_ssid  = (const char*) root["ssid"];
    cn_pass  = (const char*) root["pass"];
    cn_ipadr = (const char*) root["ipadr"];
    cn_ipmsk = (const char*) root["ipmsk"];
    cn_ipgtw = (const char*) root["ipgtw"];
  }
  EEPROM.end();

  Serial.println("---- readConfig ----");
  Serial.print("apmod:"); Serial.println(cn_apmod);
  Serial.print("ssid:");  Serial.println(cn_ssid);
  Serial.print("pass:");  Serial.println(cn_pass);
  Serial.print("ipadr:"); Serial.println(cn_ipadr);
  Serial.print("ipmsk:"); Serial.println(cn_ipmsk);
  Serial.print("ipgtw:"); Serial.println(cn_ipgtw);
  Serial.println("----");
}

void writeConfig() {
  StaticJsonDocument<255> root;

  root["apmod"] = cn_apmod;
  root["ssid"]  = cn_ssid;
  root["pass"]  = cn_pass;
  root["ipadr"] = cn_ipadr;
  root["ipmsk"] = cn_ipmsk;
  root["ipgtw"] = cn_ipgtw;
  
  String jsn;
  serializeJson(root,jsn);
  
  EEPROM.begin(255);
  for(uint16 i=0;i<jsn.length();i++){
    EEPROM.write(i,jsn[i]);
  }
  EEPROM.write(jsn.length(),0);
  EEPROM.end();
  
  Serial.println("---- writeConfig ----");
  Serial.print("apmod:"); Serial.println(cn_apmod);
  Serial.print("ssid:");  Serial.println(cn_ssid);
  Serial.print("pass:");  Serial.println(cn_pass);
  Serial.print("ipadr:"); Serial.println(cn_ipadr);
  Serial.print("ipmsk:"); Serial.println(cn_ipmsk);
  Serial.print("ipgtw:"); Serial.println(cn_ipgtw);
  Serial.println("----");
}

void setInd(){
  digitalWrite(Ind1,arrDO[0]); delay(1);
  digitalWrite(Ind2,arrDO[1]); delay(1);
  digitalWrite(Ind3,arrDO[2]); delay(1);
  digitalWrite(Ind4,arrDO[3]); delay(1);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void handleRoot() {
  Serial.println("Recive /");
  server.send_P(200, "text/html", INDEX_HTML);
}

void handlePropSvg() {
  Serial.println("Recive /propsvg");
  server.send_P(200, "image/svg+xml", PROP_SVG);
}

void handleSetDO() {
  String num = server.arg("do");

  if (num=="0") arrDO[0]=(arrDO[0]==0)?1:0;
  if (num=="1") arrDO[1]=(arrDO[1]==0)?1:0;
  if (num=="2") arrDO[2]=(arrDO[2]==0)?1:0;
  if (num=="3") arrDO[3]=(arrDO[3]==0)?1:0;

  setInd();

  String resp = "{\"do\":[";
  for(int i=0;i<4;i++){
    resp +=(arrDO[i]==0)?"0":"1";
    resp +=(i<3)?",":"";
  }
  resp +="]}";

  server.send(200,"text/plain",resp);

  if(num!="00"){
    Serial.print("Recive /setdo?do=");
    Serial.println(num);
    Serial.print("  arrDO:");
    Serial.println(resp);
  }
}

void handleSetProp(){
  Serial.println("Recive /setprop");
  String cn_apmod_old = cn_apmod;
  
  if(server.args()>0){
    cn_apmod = server.arg("apmod");
    cn_ssid  = server.arg("ssid");
    cn_pass  = server.arg("pass");
    cn_ipadr = server.arg("ipadr");
    cn_ipmsk = server.arg("ipmsk");
    cn_ipgtw = server.arg("ipgtw");
    writeConfig();
  }
  
  StaticJsonDocument<255> root;

  root["apmod"] = cn_apmod;
  root["ssid"]  = cn_ssid;
  root["pass"]  = cn_pass;
  root["ipadr"] = cn_ipadr;
  root["ipmsk"] = cn_ipmsk;
  root["ipgtw"] = cn_ipgtw;
  
  String jsn;
  serializeJson(root,jsn);
  server.send(200,"text/plain",jsn);
  
  yield();
  if(server.args()>0) resetFunc();
}

IPAddress strToIP(String IPAdr){
int Parts[4] = {0,0,0,0};
int Part = 0;
for (uint16 i=0; i<IPAdr.length(); i++ ){
  char ch = IPAdr[i];
  if ( ch == '.' )
  {
    Part++;
    continue;
  }
  Parts[Part] *= 10;
  Parts[Part] += ch - '0';
}
IPAddress ip( Parts[0], Parts[1], Parts[2], Parts[3] ); 
return ip; 
}

void WIFIConnectSta(void) {
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);

//  cn_ssid="Chump1";
//  cn_pass="CrkZ#77RmZ89";
//  cn_ipadr="192.168.55.13";
//  cn_ipmsk="255.255.255.0";
//  cn_ipgtw="192.168.55.1";

  // ---- WIFI.config ----
  IPAddress ip = strToIP(cn_ipadr); 
  IPAddress subnet = strToIP(cn_ipmsk); 
  IPAddress gateway = strToIP(cn_ipgtw); 
  WiFi.config(ip, gateway, subnet);

  Serial.print("WIFI Config IP: ");
  Serial.print(ip);
  Serial.print(" Gateway:");
  Serial.print(gateway);
  Serial.print(" Mask:");
  Serial.println(subnet);
  Serial.println("");

  const char* c_ssid = cn_ssid.c_str();
  const char* c_pass = cn_pass.c_str();

  WiFi.begin(c_ssid,c_pass);
  Serial.print("WIFI mode Client : ");
  Serial.print(c_ssid);
  Serial.print("/");
  Serial.println(c_pass);
 
  digitalWrite(Ind0, 0);
  for (int i = 0; i < 60; i++)
  {
    if ( WiFi.status() != WL_CONNECTED ) {
      digitalWrite(Ind0, digitalRead(Ind0) == 1 ? 0 : 1);
      Serial.print ( "." );
      yield();
      delay(250);
    }
  }
  digitalWrite(Ind0, 0);

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("WIFI Connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("WIFI Not Connected");
  }
}

void WIFIStartAP(void) {
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);

//  defaultConfig();

  // ---- WIFI.config ----
  IPAddress ip = strToIP(cn_ipadr); 
  IPAddress subnet = strToIP(cn_ipmsk); 
  IPAddress gateway = strToIP(cn_ipgtw); 
  WiFi.softAPConfig(ip, gateway, subnet);

  Serial.print("WIFI Config IP: ");
  Serial.print(ip);
  Serial.print(" Gateway:");
  Serial.print(gateway);
  Serial.print(" Mask:");
  Serial.println(subnet);
  Serial.println("");

  const char* c_ssid = cn_ssid.c_str();
  const char* c_pass = cn_pass.c_str();

  WiFi.softAP(c_ssid,c_pass);
  Serial.print("WIFI mode Point : ");
  Serial.print(c_ssid);
  Serial.print("/");
  Serial.println(c_pass);
  Serial.print("SoftAP_IP :");
  cn_ipadr = WiFi.softAPIP().toString();
  Serial.println(cn_ipadr);
}

void ServerStart(void) {
  if (MDNS.begin("esptest")) {
    Serial.println("MDNS responder started - esptest");
  }

  server.on("/", handleRoot);
  server.on("/propsvg", handlePropSvg);
  server.on("/setdo", handleSetDO);
  server.on("/setprop", handleSetProp);
  server.onNotFound(handleNotFound);
  server.begin();
  
  Serial.println("HTTP server started : esptest.local");
  MDNS.addService("http", "tcp", 80);
}

void setup(void) {
  Serial.begin(57600);
  pinMode(Ind0, OUTPUT); // объявляем пин Ind0 как выход
  pinMode(Ind1, OUTPUT); 
  pinMode(Ind2, OUTPUT); 
  pinMode(Ind3, OUTPUT); 
  pinMode(Ind4, OUTPUT);
  pinMode(Irst, INPUT);
    
  digitalWrite(Ind0,1);
  delay(3000);
  digitalWrite(Ind0,0);
  
  if(digitalRead(Irst)!=0){
    defaultConfig();
    writeConfig(); 
  }
  
  readConfig();
  
  if(cn_apmod=="apsta")
  {
    WIFIConnectSta();
  }
  else
  {
    WIFIStartAP();
  }
  
  ServerStart();
}

void loop(void) {
  if (!(millis() % 1000)) {
    digitalWrite(Ind0, digitalRead(Ind0) == 1 ? 0 : 1); delay(1);

    if (cn_apmod=="apsta"){
      if(WiFi.status() != WL_CONNECTED) {
        if (cntWIFIConnect<5) WIFIConnectSta();
        cntWIFIConnect++;
        if (cntWIFIConnect>40) cntWIFIConnect = 0;
      }
      else
      {
        cntWIFIConnect=0;
      }
    }
  }

  server.handleClient();
  yield();
}