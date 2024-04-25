#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"

#include <ezButton.h>

#define VRX_PIN  32 // ESP32 pin GIOP36 (ADC0) connected to VRX pin
#define VRY_PIN  35 // ESP32 pin GIOP39 (ADC0) connected to VRY pin
#define SW_PIN   33 // ESP32 pin GIOP17 connected to SW  pin

ezButton button(SW_PIN);

int valueX = 0; // to store the X-axis value
int valueY = 0; // to store the Y-axis value
int bValue = 0; // To store value of the button



//  Variable for Y tracking 
int checValY = 1;
int preValY = 1;
int valY = 1;
int currValY =1;
int cgTrakY = 0;

// Variable for X traking
int checVal = 1;
int preVal = 1;
int val = 1;
int currVal =1;
int cgTrak = 0;


String spellWord = "";
String morose_code = "";
String Sentence = "";
bool DotCheck = true;

String mores_ind_Find[36] = {".-", "-...","-.-.", "-..", ".","..-.", "--.", "....","..", ".---", "-.-",".-..", "--", "-.","---", ".--.",
                         "--.-",".-.", "...", "-","..-", "...-", ".--","-..-", "-.--", "--..",".----", "..---", "...--","....-",
                          ".....", "-....","--...", "---..", "----.","-----"};

char Find_Word[36] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','1','2','3','4','5','6','7','8','9','0'};



#define I2S_DOUT      25
#define I2S_BCLK      27
#define I2S_LRC       26

Audio audio;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600) ;

  // Setup For Audio
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin( "Brown_Munde-24GHz", "9r@@mies");

  while (WiFi.status() != WL_CONNECTED)
    delay(1500);

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(100); 

}

void loop() {
  // put your main code here, to run repeatedly:
  audio.loop();
  button.loop();
  
  // MUST call the loop() function first

  valueX = analogRead(VRX_PIN);
  valueY = analogRead(VRY_PIN);


  //  Value Traking For X
  if(valueX < 10){
    currVal = 0;
  }else if( (1890 < valueX) && (valueX < 1910)){
    currVal = 1;
  }else if(4050 < valueX){
    currVal = 2;
  }
  
  // Serial.println(valueX);
  if(currVal != val){
    // Serial.println(currVal);
    cgTrak += 1;
    checVal = preVal;
    preVal = val;
    val = currVal;
  }

  // value Tracking for Y
  if(valueY < 10){
    currValY = 0;
  }else if((1890 < valueY ) && (valueY < 1910)){
    currValY = 1;
  }else if(4050 < valueY){
    currValY = 2;
  }

  if(currValY != valY){
    cgTrakY += 1;
    checValY = preValY;
    preValY = valY;
    valY = currValY;
  }

  // Functionilaty of X
  if (val == 1 && checVal == 1 && cgTrak == 2){
    if(preVal == 0){
      if(morose_code == " "){
        morose_code = '.';
        Serial.print(".");
      }else{
        morose_code += '.';
        Serial.print(".");
      }
      cgTrak = 0;
    }else if(preVal == 2){
      if(morose_code == " "){
        morose_code = '-';
        Serial.print("-");
      }else{
        morose_code += '-';
        Serial.print("-");
      }
      cgTrak = 0;
    }
    // Serial.println(morose_code);
  }
  int idx = -1;
  // Functionilaty For Y
  if (valY == 1 && checValY == 1 && cgTrakY == 2){
    if(preValY == 0){
      // String stringOne = spellWord.c_str();
      Serial.print(spellWord);
      Sentence += spellWord;
      Sentence += " ";

      audio.connecttospeech(spellWord.c_str(),"en");
      Serial.print("Speaking...\n");
      cgTrakY = 0;
      spellWord = " ";

    }else if(preValY == 2){

      idx = -1;

      for(int i=0;i<36;i++){

        String myString = String(mores_ind_Find[i]);

        if(morose_code == myString){
          spellWord += Find_Word[i];
          Serial.println("");
          Serial.println(spellWord);
          idx = i;
          break;
        }
      }

      morose_code = " ";

      if(idx == -1){
        Serial.println("Wrong");
      }
      cgTrakY = 0;
    }
  }
  



  // Read the button value
  bValue = button.getState();

  if (button.isPressed()) {
    // TODO do something here
    audio.connecttospeech(Sentence.c_str(),"en");
    if(!((Sentence == "")||(Sentence == " "))){
      Serial.println(Sentence);
      Sentence = "";
    }
  }

  if (button.isReleased()) {
    if(!((Sentence == "")||(Sentence == " "))){
      Serial.println(Sentence);
      Sentence = "";
    }
    spellWord = " ";
    morose_code = " ";
  }
  // y - 0 to 4095 mid == {188_}  lim (10 - 3400) assign val == {0 1 2}
  // x - 0 to 4095 mid == {190_}  lim (10 -- 3400) assign val == {0 1 2}

}


