#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <FastLED.h>

#define PIN 13
#define NUM_LEDS 120

CRGB leds[NUM_LEDS];

unsigned long constantModeDelay = 0;
unsigned long loopDelay = 0;

const char* ssid = "Chump1";
const char* password = "CrkZ#77RmZ89";
const int port = 43433;

uint32_t primaryColor;
uint32_t secondaryColor;
int intensity;
bool secondaryColorExists;
bool animation;

DynamicJsonDocument doc(1024);

WiFiServer server(port);
String cn_ipadr = "192.168.1.15";
String cn_ipmsk = "255.255.255.0";
String cn_ipgtw = "192.168.1.1";

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

void ConstantMode(uint32_t primaryColor,uint32_t secondaryColor,int intensity,bool secondaryColorExists, bool animation)
{
   FastLED.setBrightness(intensity);
  if(secondaryColorExists)
  {
    if(animation)
    {
      if(millis() - constantModeDelay > 750)
      {
        Serial.print("test  2 \n");
    
        for (int i = 0; i < NUM_LEDS; i++)
        {
          if (i % 2 == 0)
          {
            leds[i] = CRGB(primaryColor);
          }
          else
          {
            leds[i] = CRGB(secondaryColor);
          }
        }
        FastLED.show();
       }
      if(millis() - constantModeDelay > 1500)
      {
        constantModeDelay = millis();
        for (int i = 0; i < NUM_LEDS; i++)
        {
          if (i % 2 == 0)
          {
            leds[i] = CRGB(secondaryColor);
          }
          else
          {
            leds[i] = CRGB(primaryColor);
          }
        }
        FastLED.show();
      }
    }
    else
    {
      Serial.print("test");
      for (int i = 0; i < NUM_LEDS; i++)
        {
          if (i % 2 == 0)
          {
            leds[i] = CRGB(primaryColor);
          }
          else
          {
            leds[i] = CRGB(secondaryColor);
          }
        }
        FastLED.show();
    }
  }
  else
  {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CRGB(primaryColor);
      
    }
    FastLED.show();
  }
  
  
}

void setup()
{
  Serial.begin(115200);
  Serial.println();

  FastLED.addLeds<WS2812B, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalSMD5050 );
  FastLED.setBrightness(128);
  pinMode(13, OUTPUT);

  IPAddress ip = strToIP(cn_ipadr); 
  IPAddress subnet = strToIP(cn_ipmsk); 
  IPAddress gateway = strToIP(cn_ipgtw); 
  WiFi.config(ip, gateway, subnet);

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  server.begin();
  Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());
  Serial.print("setup complete");
}

void testFunc( ){
   FastLED.clear();
    
    leds[1] = CRGB(0,0,255);   
    FastLED.show();
    delay(100);
}

void loop()
{
  
  
  WiFiClient client = server.available();
  // wait for a client (web browser) to connect
  if (client)
  {
    Serial.println("\n[Client connected]");
    while (client.connected())
    {
      // read line by line what the client (web browser) is requesting
      if (client.available())
      {
        
        String line = client.readStringUntil('\r');
        DeserializationError error = deserializeJson(doc,line);
         if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
          return;
        }
        primaryColor = doc["primaryColor"];
        secondaryColor = doc["secondaryColor"];
        intensity = doc["intensity"];
        secondaryColorExists = doc["secondaryColorExists"];
        animation = doc["animation"];

      
        
        Serial.print(primaryColor);
        Serial.print('\n');
         Serial.print(intensity);
         Serial.print('\n');
         Serial.print((String)animation);
        Serial.print('\n');
        Serial.print((String)doc["primaryColor"]);
        Serial.print((String)doc["animation"]);
        Serial.print(53);
        
        Serial.print(line);
        client.print(200);
      }
      
    }

    while (client.available()) {
      // but first, let client finish its request
      // that's diplomatic compliance to protocols
      // (and otherwise some clients may complain, like curl)
      // (that is an example, prefer using a proper webserver library)
      client.read();
    }

    // close the connection:
    client.stop();
    Serial.println("[Client disconnected]");

    
  }
  
  if(millis() - loopDelay > 50){
    loopDelay = millis();
   if((String)doc["mode"] == "constantMode")
        {
          ConstantMode(primaryColor,
          secondaryColor,
          intensity,
          secondaryColorExists,
          animation);
        }}
        

        
       
}

