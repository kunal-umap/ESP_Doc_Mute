#include <google-tts.h>
#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"

#define I2S_DOUT      25
#define I2S_BCLK      27
#define I2S_LRC       26

Audio audio;


void setup()
{

  Serial.begin(115200);

  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin( "realme narzo 20", "ganesh123");
  Serial.println("he;;o");
  while (WiFi.status() != WL_CONNECTED)
    delay(1500);

  TTS tts;
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(100);
  Serial.println(tts.getSpeechUrl("こんにちは、世界！", "ja"));
  String stringOne = tts.getSpeechUrl("Hello, World!");
  audio.connecttohost(stringOne.c_str());
  Serial.println(tts.getSpeechUrl("Hello, World!"));
  
}

void loop()

{
  TTS tts;
  // Serial.println(tts.getSpeechUrl("Hello, World!"));
  audio.loop();

}

