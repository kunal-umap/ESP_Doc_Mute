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
   WiFi.begin( "Brown_Munde-24GHz", "9r@@mies");

  while (WiFi.status() != WL_CONNECTED)
    delay(1500);

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(100);
  // audio.connecttohost("http://mp3.ffh.de/radioffh/hqlivestream.mp3");
  // Answer.c_str()
  // audio.connecttospeech("Hello I am Kunal","en");
  delay(15000);
}
bool a = true;
void loop()

{

  audio.loop();
  if(a){
    a = false;
    audio.connecttospeech("Hello I am Kunal","en");
  }
  
  // delay(15000);

}

void audio_info(const char *info) {
  Serial.print("audio_info: "); Serial.println(info);
}