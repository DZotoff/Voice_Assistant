#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include <ArduinoJson.h>
#include "Audio.h"

const char* ssid = "MB210-G";
const char* password = "studentMAMK";
const char* chatgpt_bearer = "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";
const char* temperature = "0";
String Message = "";
String New_message = "";
long start;

bool speak = false;
const int MAX_TOKENS = 60;
const char* openAPIendPoint = "https://api.openai.com/v1/chat/completions";
const char* server = "api.openai.com";
#define PORT 443                               // The port you'll connect to on the server - this is standard.
#define SERVER_RESPONSE_WAIT_TIME (15 * 1000) 


#define I2S_DOUT      25
#define I2S_BCLK      27
#define I2S_LRC       26

Audio audio;

void setup()
{
  Serial.begin(115200);
  
  // wait for WiFi connection
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(100);
  audio.connecttospeech("Starting", "en");
  //start = millis();
}

void loop()
{
 
 
  if(speak){
    audio.loop();
    if ((millis()-start) == 15000){
      
      audio.stopSong();
      speak = false;
    }
  }
    
  if (!speak){
    check_for_new_messages();
    if (New_message != Message ){
      send_to_chat_GPT(New_message);
      Message=New_message;
      speak = true;
      start = millis();
    }
  }    
}
void check_for_new_messages(){
  HTTPClient https;
  if (https.begin("http://172.20.49.58:3002/")) 
  {  
    
    https.addHeader("Content-Type", "application/json");
    int httpCode = https.GET();
    String payload = https.getString();
    New_message=payload;
    Serial.println(New_message);
  }
  else {
    Serial.printf("[HTTPS] Unable to connect\n");
  } 
}


void send_to_chat_GPT(String Message){
  HTTPClient https;

  if (https.begin("https://api.openai.com/v1/chat/completions")) 
  {  
    
    https.addHeader("Content-Type", "application/json");
    https.addHeader("Authorization", String("Bearer ") + chatgpt_bearer);
    
    int httpCode = https.POST(   String("{\"model\": \"gpt-3.5-turbo\", \"messages\": [{\"role\": \"user\", \"content\": \"")+ Message + String("\"}],\"temperature\": 0.7, \"max_tokens\": 60}")   );
    
    if (httpCode > 0){
      String payload = https.getString();
      Serial.println(payload);
      DynamicJsonDocument doc(1024);

      deserializeJson(doc, payload);
      String Answer = doc["choices"][0]["message"]["content"];
      Answer = Answer.substring(0);
      Serial.print("Answer : "); Serial.println(Answer);

      audio.connecttospeech(Answer.c_str(), "en");
      
    }
    else {
      Serial.printf("[HTTPS] GET... failed, error: %s\n");
      String payload = https.getString();
      audio.connecttospeech("Sorry, no answer", "en");
      Serial.println(httpCode);
    }
    https.end();
  }
   else {
    Serial.printf("[HTTPS] Unable to connect\n");
  } 

}
