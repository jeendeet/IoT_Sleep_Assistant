#include "data.h"
void led_setup() {
  Serial.println("[LED] Start Setup");
  ledcSetup(ledChanelBlue   ,ledFreq,ledResolution);
  ledcSetup(ledPinYellow ,ledFreq,ledResolution);
  ledcAttachPin(ledPinBlue     ,ledChanelBlue);
  ledcAttachPin(ledPinYellow   ,ledChanelYellow);
  ledcWrite(ledChanelBlue  ,255);
  ledcWrite(ledChanelYellow,100);

}
void led_change(int chanel, int percent) {
  ledcWrite(chanel, ledConvert(percent));
  Serial.println("[LED] Set led " + String(chanel) + " at " + String(percent));
}

void led_test_mode(int control){
  if(control > 500) control = 500;
  ledcWrite(ledChanelBlue  ,ledConvert(control/2));
  ledcWrite(ledChanelYellow,ledConvert(255-control/2));
}

void data_setup(){
  database.control_mode = 0;
  database.blue_bright = 100;
  database.yellow_bright=50;
}

void dht_setup() {
  Serial.println("[DHT] task begin");
}
void dht_get_data()
{
  DHT.read(DHT11_PIN);
  Serial.print("temp:");
  Serial.print(DHT.temperature);
  Serial.print("  humi:");
  Serial.println(DHT.humidity);
  database.temperature = DHT.temperature;
  database.humid = DHT.humidity;
}
//87-0-182-13!

void uart_setup(){
  Serial2.begin(9600);
  pinMode(uartPincontrol,OUTPUT);
  digitalWrite(uartPincontrol,1);
}
//void data_processing(std :: string Data){
//    char *stringData = Data.c_str();
//    beatAvg = atoi(strtok(stringData,"-"));
//    ACC_count = atoi(strtok(NULL,"-"));
//    ACC_intensity_mean = atof(strtok(NULL,"!"));
////    Serial.print("Data: ", beatAvg, ACC_count, ACC_intensity_mean);
//}

void uart_readdata(char* datarev){
  char string[20];
  int count = 0;
  int bstatus =1;
  unsigned int startime = millis();
  int isTimeout = 0;
  Serial.println("UART Start read");
  digitalWrite(uartPincontrol,0);
  while(bstatus == 1){
    while(Serial2.available() && bstatus ==1){
      string[count] = char(Serial2.read());
      if(string[count] =='!')
      {
        digitalWrite(uartPincontrol,1);
        bstatus = 0;
        break;
      }
      count ++;
    }
    if(millis() -startime > 2000){
      bstatus = 0;
      isTimeout = 1;
    }
  }
  Serial.println("end");
  if(isTimeout ==0){
    Serial.println("string:");
    Serial.println(string);
    char* p;
    p = strtok(string,"-");
    database.heartrate = atoi(p);
    Serial.println(database.heartrate);
    
    p = strtok(NULL,"-");
    count = atoi(p);
    Serial.println(count);
  
    p = strtok(NULL,"-");
    database.movement = atoi(p);
    Serial.println(database.movement);
    
    p = strtok(NULL,"!");
    mode2control = atoi(p);
    Serial.println(mode2control);
  }
  else Serial.println("Serial timeout!:");
}

void ir_setup(){
  // Set up what we want to send.
  // See state_t, opmode_t, fanspeed_t, swingv_t, & swingh_t in IRsend.h for
  // all the various options.
  ac.next.model = 1;  // Some A/Cs have different models. Try just the first.
  ac.next.mode = stdAc::opmode_t::kCool;  // Run in cool mode initially.
  ac.next.celsius = true;  // Use Celsius for temp units. False = Fahrenheit
  ac.next.degrees = 25;  // 25 degrees.
  ac.next.fanspeed = stdAc::fanspeed_t::kMedium;  // Start the fan at medium.
  ac.next.swingv = stdAc::swingv_t::kOff;  // Don't swing the fan up or down.
  ac.next.swingh = stdAc::swingh_t::kOff;  // Don't swing the fan left or right.
  ac.next.light = false;  // Turn off any LED/Lights/Display that we can.
  ac.next.beep = false;  // Turn off any beep from the A/C if we can.
  ac.next.econo = false;  // Turn off any economy modes if we can.
  ac.next.filter = false;  // Turn off any Ion/Mold/Health filters if we can.
  ac.next.turbo = false;  // Don't use any turbo/powerful/etc modes.
  ac.next.quiet = false;  // Don't use any quiet/silent/etc modes.
  ac.next.sleep = -1;  // Don't set any sleep time or modes.
  ac.next.clean = false;  // Turn off any Cleaning options if we can.
  ac.next.clock = -1;  // Don't set any current time if we can avoid it.
  ac.next.power = false;  // Initially start with the unit off.

  decode_type_t protocol = (decode_type_t)10;
  ac.next.protocol = protocol;  // Change the protocol used.
  Serial.println("Protocol " + String(protocol) + " / " + typeToString(protocol) + " is used");
}
void ir_control(int isOn){
  if(isOn == 1){
    ac.next.power = true;  // We want to turn on the A/C unit.
    Serial.println("Sending a message to turn ON the A/C unit.");
    ac.sendAc();  // Have the IRac class create and send a message.
  }
  else{
    ac.next.power = false;  // We want to turn on the A/C unit.
    Serial.println("Sending a message to turn OFF the A/C unit.");
    ac.sendAc();  // Have the IRac class create and send a message.
  }
}
