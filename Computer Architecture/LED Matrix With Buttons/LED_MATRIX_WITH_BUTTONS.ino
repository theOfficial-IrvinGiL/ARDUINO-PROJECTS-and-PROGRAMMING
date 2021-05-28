//GROUP MEMBERS:
//Mercado, Irvin Gil
//Hinayas, Michelle Ann
//Diacosta, Lyndon
//Basa, Benjie
//Crisostomo, Mark Neil
//Mayor, Renante

//VIDEO LINK: 


#include <MD_Parola.h>

#include <MD_MAX72xx.h>

#include <SPI.h>

#define sw1 6  //set pins
#define sw2 7

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4  // Number of modules connected
#define CLK_PIN   13   // SPI SCK pin on UNO
#define DATA_PIN  11   // SPI MOSI pin on UNO
#define CS_PIN    10   // connected to pin 10 on UNO


int directionOfText = 0;  // used to hold the slider analog value
int scroll_speed;   // used when changing scroll speed

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);  // SPI config

int scrollSpeed = 50;  // used to set text scroll speed in Parola at start

// sets scrolling direction if slider in middle at start
textEffect_t scrollEffect = PA_SCROLL_LEFT;

textPosition_t scrollAlign = PA_LEFT;  // how to aligh the text

int scrollPause = 0; // ms of pause after finished displaying message

#define  BUF_SIZE  75  // Maximum of 75 characters
char curMessage[BUF_SIZE] = { "CPE 3RDYR" };  // used to hold current message


int left = 0;
int right = 0;


void setup()
{
  Serial.begin(9600);
  pinMode(sw1, INPUT);
  pinMode(sw2, INPUT);

  P.begin();  // Start Parola

  // configure Parola
  P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);


}

void loop() {
  if (digitalRead(sw1) == HIGH) {
    left = 0;
    right = 1;

  }
  else if (digitalRead(sw2) == HIGH) {
    left = 1;
    right = 0;
  
  }
  else{
    left = 0;
    right = 0;
    }

  if (right == 1) {
    scrollEffect = (scrollEffect == PA_SCROLL_RIGHT ? PA_SCROLL_RIGHT : PA_SCROLL_RIGHT);
    P.setTextEffect(scrollEffect, scrollEffect);
    P.displayClear();
    strcpy(curMessage,"Right");
    P.displayReset();  // Reset and display it again
    Serial.println("RIGHT");
  }
  else if(left == 1) {
    scrollEffect = (scrollEffect == PA_SCROLL_LEFT ? PA_SCROLL_LEFT  : PA_SCROLL_LEFT);
    P.setTextEffect(scrollEffect, scrollEffect);
    P.displayClear();
    strcpy(curMessage,"Left");
    P.displayReset();  // Reset and display it again
     Serial.println("LEFT");
  }
  else{
    Serial.println("none");
    
    }

  if (P.displayAnimate()) // If finished displaying message
  {
    P.displayReset();  // Reset and display it again
  }

}
