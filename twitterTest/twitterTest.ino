#include <WiFi.h>
#include <WiFiClient.h>
#include <Temboo.h>
#include "TembooAccount.h" // Contains Temboo account information

WiFiClient client;
int numRuns = 1;   // Execution count, so this doesn't run forever
int maxRuns = 10;   // Maximum number of times the Choreo should be executed
int mode=0;
int TEMP=2,LEFT=23,RIGHT=24,LIGHT=6,HOT=1,COLD=-1,NORMAL=0;
void getTemp(){
    return .1*(analogRead(TEMP)*1460/4095-500);
}
int getLeft(){
  return analogRead(LEFT);
}
int getRight(){
  return analogRead(RIGHT);
  }
int getLight(){
  return analogRead(LIGHT);
}

int interpretTemp(int temp){
  if (temp>27){
   return HOT;
  }
  if (temp<20){
   return COLD;
  }
  return NORMAL;
}

int checkTemp(){
initial = interpretTemp(getTemp());
if(initial){
  delay(50);
  if (initial==interpretTemp(getTemp())){
    return initial;
    }
  }
  return NORMAL;
}

boolean interpretForce(int force){
  if (force<2700 && force>1900){return true;}
  return false;
}

boolean checkLeft(){
initial = interpretForce(getLeft());
if(initial){
  delay(50);
  if (initial==interpretForce(getLeft())){
    return initial;
    }
  }
return false;
}

boolean checkRight(){
initial = interpretForce(getRight());
if(initial){
  delay(50);
  if (initial==interpretForce(getRight())){
    return initial;
    }
  }
 return false;
}

boolean interpretLight(int light){
  if (light<4000){return true;}
  return false;
}

boolean checkLight(){
initial = interpretLight(getLight());
if(initial){
  delay(50);
  if (initial==interpretLight(getLight())){
    return initial;
    }
  }
return false;
}



void setup() {
  
  
  Serial.begin(9600);
  /*Serial.println("Wi-Fi SSID:");
  while(!Serial.available()){}
  delay(100);
  char WifiID[Serial.available()];
  Serial.readBytes(WifiID,Serial.available());
  Serial.read();
  Serial.read();
  Serial.println(WifiID);
  Serial.println("Wi-Fi Passkey:");
  while(!Serial.available()){}
  delay(100);
  char passkey[Serial.available()];
  Serial.readBytes(passkey,Serial.available());
  Serial.read();
  Serial.read();
  Serial.println(passkey);*/
  pinMode(PUSH1,INPUT);
  int wifiStatus = WL_IDLE_STATUS;

  // Determine if the WiFi Shield is present.
  Serial.print("\n\nShield:");
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("FAIL");

    // If there's no WiFi shield, stop here.
    while(true);
  }

  Serial.println("OK");

  // Try to connect to the local WiFi network.
  while(wifiStatus != WL_CONNECTED) {
    Serial.print("WiFi:");
    wifiStatus = WiFi.begin(WIFI_SSID, WPA_PASSWORD);

    if (wifiStatus == WL_CONNECTED) {
      Serial.println("OK");
    } else {
      Serial.println("FAIL");
    }
    delay(5000);
  }

  Serial.println("Setup complete.\n");
}

void loop() {
  if (numRuns <= maxRuns) {
    while(!digitalRead(PUSH1)){}
    Serial.println("Running StatusesUpdate - Run #" + String(numRuns++));

    TembooChoreo StatusesUpdateChoreo(client);

    // Invoke the Temboo client
    StatusesUpdateChoreo.begin();

    // Set Temboo account credentials
    StatusesUpdateChoreo.setAccountName(TEMBOO_ACCOUNT);
    StatusesUpdateChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    StatusesUpdateChoreo.setAppKey(TEMBOO_APP_KEY);

    // Set Choreo inputs
    String AccessTokenValue = "2839646316-ASFFiuSpojXgQ8L4BQrA8Q1SxzSlBtqbtgZqbjL";
    StatusesUpdateChoreo.addInput("AccessToken", AccessTokenValue);
    String AccessTokenSecretValue = "ReIRIIZ0rqZUYcR0Sew9OhbFrn7RAClkh8Dl0YAeVs5TI";
    StatusesUpdateChoreo.addInput("AccessTokenSecret", AccessTokenSecretValue);
    String ConsumerSecretValue = "S0F6zt7V30Uw1xoSAE3HvS6jK1a5MvkCc001BDOHAnA5cs5lzV";
    StatusesUpdateChoreo.addInput("ConsumerSecret", ConsumerSecretValue);
    String StatusUpdateValue = "Test tweet Hello World :) "+String(millis());
    StatusesUpdateChoreo.addInput("StatusUpdate", StatusUpdateValue);
    String ConsumerKeyValue = "RGHeKMBcdJwM56beejtuTVU00";
    StatusesUpdateChoreo.addInput("ConsumerKey", ConsumerKeyValue);

    // Identify the Choreo to run
    StatusesUpdateChoreo.setChoreo("/Library/Twitter/Tweets/StatusesUpdate");

    // Run the Choreo; when results are available, print them to serial
    StatusesUpdateChoreo.run();

    while(StatusesUpdateChoreo.available()) {
      char c = StatusesUpdateChoreo.read();
      Serial.print(c);
    }
    StatusesUpdateChoreo.close();
  }

  Serial.println("\nWaiting...\n");
  //delay(30000); // wait 30 seconds between StatusesUpdate calls
}
