#include <Arduino.h>
#include "data.h"
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "datvg"
#define WIFI_PASSWORD "asdfghjkl"

// Insert Firebase project API Key
#define API_KEY "AIzaSyBQ8bKLOCgR1CHBP60mO8kk2lj-QrexecI"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://espnewest-default-rtdb.firebaseio.com/" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

void firebase_setup(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println("Successful");
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("[Firebase] Signup ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Serial.println("[Firebase] End setup");
}

void firebase_send(String path, int value){
  while(1){
    if (Firebase.ready()){
      if (Firebase.RTDB.setInt(&fbdo, path, value)){
        Serial.println("PASSED");
        break;
      }
      else {
        Serial.println("FAILED: " + fbdo.errorReason());
      }
    }
  }  
}
void firebase_readdata(){
    Serial.println("[FIREBASE]READ FIREBASE DATA");
    if (Firebase.ready()){
      if(Firebase.RTDB.get(&fbdo,"Sleep/controlmode"))
        database.control_mode = fbdo.intData();
      Serial.println("[FIREBASE]controlmode :" + String(database.control_mode));
    }
    if(database.control_mode == 1){
      Firebase.RTDB.get(&fbdo,"Sleep/bright/blue");
      database.blue_bright = fbdo.intData();
      Serial.println("[FIREBASE]bright blue :" + String(database.blue_bright));
      
      Firebase.RTDB.get(&fbdo,"Sleep/bright/yellow");
      database.yellow_bright = fbdo.intData();
      Serial.println("[FIREBASE]bright yellow :" + String(database.yellow_bright));
      
      Firebase.RTDB.get(&fbdo,"Sleep/ircontrol/ordernow");
      database.ircontrol = fbdo.intData();
      Serial.println("[FIREBASE]IR control mode :" + String(database.ircontrol));
    }
}
void firebase_updatedata()
{ 
  Serial.println("[FIREBASE] Update new data");
  dht_get_data();
  Serial.print("[FIREBASE]humid :");
  Serial.println(database.humid);
  firebase_send("Sleep/environment/humid", database.humid);
  Serial.print("[FIREBASE]temperature :");
  Serial.println(database.temperature);
  firebase_send("Sleep/environment/temperature", database.temperature);
//  Serial.print("[FIREBASE]heartrate :");
//  Serial.println(database.heartrate);
//  firebase_send("Sleep/health/heartrate", database.heartrate);
//  Serial.print("[FIREBASE]movement :");
//  Serial.println(database.movement);
//  firebase_send("Sleep/health/movement", database.movement);
}
/****************************************************/
/*                  APP TASK                        */
/****************************************************/
int system_count = 0;
void setup(){
    Serial.begin(115200);
//    uart_setup();
    led_setup();
    dht_setup();
    data_setup();
    ir_setup();
    firebase_setup();
    firebase_readdata();
}
void loop(){
    firebase_readdata();
//    uart_readdata(uartdata);
    if(database.control_mode == 1) //Handle mode
    {
      led_change(ledChanelBlue, database.blue_bright);
      led_change(ledChanelYellow, database.yellow_bright);
      if(database.ircontrol < 2){
        ir_control(database.ircontrol);
        database.ircontrol = 2;
        firebase_send("Sleep/ircontrol/ordernow", database.ircontrol);
      }
    }
    else if(database.control_mode == 2) //Test mode
    {
      Serial.print("TEST mode: ");
      Serial.println(mode2control);
      led_test_mode(mode2control);
    }
    if(system_count == 0){
      firebase_updatedata();
    }
    system_count ++;
    if(system_count > 10) system_count = 0; 
}
