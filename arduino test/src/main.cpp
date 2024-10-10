#include <Arduino.h>
#define NUM_LEDS 120
#include "FastLED.h"
#define PIN 13
#include <ezButton.h>

CRGB leds[NUM_LEDS];
byte counter;
byte counter3;
double counter2;
unsigned long myMillis1 = 0;
unsigned long myMillis2 = 0;
unsigned long myMillis3 = 0;
unsigned long myMillis4 = 0;
unsigned long myMillis5 = 0;
int period = 2;
int j = 0;
int direction1 = 1;
int direction2 = 1;
int lineBright;
int brightDelta1= 11;
int brightDelta2 = 11;
int masterCounter = 0;
double k = 0.002;
String MySer = "";
ezButton button(7); 




void LightOff()
{
    FastLED.clear();
    FastLED.show();
  
}

void TestRGB()
{
  
  leds[0] = CRGB(1,0,0);
    
  leds[1] = CRGB(0,1,0);

  leds[2] = CRGB(0,0,1);
    
  
  FastLED.show();
}

void RedYellowHSV()
{
  if(millis() - myMillis1 > period)
  {
    myMillis1 = millis();
    // 
    int br = (int)round(((sin(counter2*2-(PI/2))+1)/2)*255);
    if(br <= 24)
    {
      br = 0;
    }
  for (int i = 0; i < NUM_LEDS; i++)
  {
    
    if (i % 2 == 0)
    {
      leds[i] = CHSV(0,255,br);
    }
    else
    {
      leds[i] =CHSV(30,255,br);
    }
  }
  if(counter2 >= 2*PI)
  {
    counter2 = 0;
  }
  counter2 += k;
  FastLED.show();
  }
}

void RedYellowAlternate()
{
  if(millis() - myMillis2 > 750)
  {
    
    for (int i = 0; i < NUM_LEDS; i++)
    {
      if (i % 2 == 0)
      {
        leds[i] = CHSV(0,255,160);
      }
      else
      {
        leds[i] =CHSV(30,255,160);
      }
    }
    FastLED.show();
  }
  if(millis() - myMillis2 > 1500)
  {
    myMillis2 = millis();
  for (int i = 0; i < NUM_LEDS; i++)
    {
      if (i % 2 == 0)
      {
        leds[i] =CHSV(30,255,160);
      }
      else
      {
        leds[i] =CHSV(0,255,160);
      }
    }
    FastLED.show();
  }
  
}

void RainbowLine()
{
  if(millis() - myMillis3 > 50)
  {
    myMillis3 = millis();
  if(direction1 == 1)
  {
    lineBright = 35;
  }
  else if (direction1 == -1)
  {
    lineBright = 255;
  }
  FastLED.clear();
  for(int i = 0; i < 20; i++)
  {
    if(i+j < 120 && i+j >= 0)
    {
    leds[i+j] = CHSV(counter3,255,lineBright);
    }
    lineBright += brightDelta1;
    
  }
  counter3++;
  FastLED.show();
  j+=direction1;
  if(j>=119)
  {
    direction1 = -1;
    brightDelta1 *=-1;
  }
  if(j <= -20)
  {
    direction1 = 1;
    brightDelta1 *=-1;
  }
  }
  
}

void RedLine()
{
  if(millis() - myMillis4 > 50)
  {
    myMillis4 = millis();
  if(direction2 == 1)
  {
    lineBright = 35;
  }
  else if (direction2 == -1)
  {
    lineBright = 255;
  }
  FastLED.clear();
  for(int i = 0; i < 20; i++)
  {
    if(i+j < 120 && i+j >= 0)
    {
    leds[i+j] = CHSV(0,255,lineBright);
    }
    lineBright += brightDelta2;
    
  }
  FastLED.show();
  j+=direction2;
  if(j>=119)
  {
    direction2 = -1;
    brightDelta2 *=-1;
  }
  if(j <= -20)
  {
    direction2 = 1;
    brightDelta2 *=-1;
  }
  }
  
}

void YellowLine()
{
  if(millis() - myMillis5 > 50)
  {
    myMillis5 = millis();
  lineBright = 35;
  FastLED.clear();
  for(int i = 0; i < 20; i++)
  {
    int n = i + j;
    if(n > 119)
    {
      n -= 119;
    }
    leds[n] = CHSV(30,255,lineBright);
    lineBright += 11;
  }
  FastLED.show();
  j++;
  if(j > 119)
  {
    j = 0;
  }
  }
  
}

void RainbowLoop()
  {
    
    
    for (int i = 0; i < NUM_LEDS; i++ ) 
    {         // от 0 до первой трети
    leds[i] = CHSV(counter + i * 2, 255, 255);  // HSV. Увеличивать HUE (цвет)
    // умножение i уменьшает шаг радуги
    }
     counter++;        // counter меняется от 0 до 255 (тип данных byte)
     FastLED.show();
   
    }
  

void SrLoop()
{
  while (Serial.available()>0)
  {
    char ch = Serial.read();
    Serial.write(ch);
    if (ch != char(13))
    {
      MySer += ch;
    }
    else
    {
      MySer.trim();
       if (MySer == "1")
       {
        masterCounter++;
        if (masterCounter > 6)
        {
          masterCounter = 0;
        }
        Serial.println("Changed Mode");
       }
      Serial.flush();
      MySer.remove(0);
    }
  }
}

void Master (int n)
{
  switch (n)
  {
    case 0:
        LightOff();
        break;
    case 1:
    RedYellowHSV();
    break;
    case 2:
    RedYellowAlternate();
    break;
    case 3:
    
    RainbowLine();
    break;
    case 4:
    
    RedLine();
    break;
    case 5:
    
    YellowLine();
    break;
    case 6:
    RainbowLoop();
    break;
    default:
    LightOff();
    break;
  }
}
  
void setup() {
  
  /*Serial.begin(115200);
  Serial.println("Input 1 to switch mode");
  */
  button.setDebounceTime(50);
  FastLED.addLeds<WS2812B, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalSMD5050 );
  FastLED.setBrightness(128);
  pinMode(13, OUTPUT);
  LightOff();
}

  void loop() {
  button.loop();
  if(button.isPressed())
  {
    masterCounter++;
    if(masterCounter > 6)
    {
      masterCounter = 0;
    }
  }
  Master(masterCounter);
}

