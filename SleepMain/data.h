#ifndef DATA_H
#define DATA_H
#include <DFRobot_DHT11.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRutils.h>

const uint16_t kIrLed = 13;  // The ESP GPIO pin to use that controls the IR LED.
IRac ac(kIrLed);  // Create a A/C object using GPIO to sending messages with.

typedef struct{
  int control_mode;
  int blue_bright; 
  int yellow_bright;
  int alarm_h;
  int alarm_m;
  int temperature;
  int humid;
  int heartrate;
  int movement;
  int ircontrol;
} f_data;
volatile f_data database;

#define ledPinBlue      2
#define ledChanelBlue   0
#define ledPinYellow    15
#define ledChanelYellow 1
#define ledFreq         500
#define ledResolution   8
#define ledConvert(percent) (((percent)*255)/100)

DFRobot_DHT11 DHT;
#define DHT11_PIN 4

#define uartPincontrol 5
char uartdata[20];

int mode2control = 0; 
#endif
