//GROUP MEMBERS:
//Mercado, Irvin Gil
//Hinayas, Michelle Ann
//Diacosta, Lyndon
//Basa, Benjie
//Crisostomo, Mark Neil
//Mayor, Renante

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <MD_UISwitch.h>

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10

// HARDWARE SPI
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// SOFTWARE SPI
//MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// Turn on debug statements to the serial output
#define  DEBUG_ENABLE  0

#if  DEBUG_ENABLE
#define DEBUG(s, x) { Serial.print(F(s)); Serial.print(x); }
#define DEBUGS(x) Serial.print(F(x))
#define DEBUGX(x) Serial.println(x, HEX)
#else
#define DEBUG(s, x)
#define DEBUGS(x)
#define DEBUGX(x)
#endif

// User interface pin and switch definitions
const uint8_t SPEED_IN = A5;      // control the speed with an external pot
const uint8_t JUSTIFY_SET = 6;    // change the justification
const uint8_t INVERSE_SET = 9;    // set/reset the display to inverse

uint8_t uiPins[] = { JUSTIFY_SET, INVERSE_SET };

const uint16_t PAUSE_TIME = 1000; // in milliseconds
const uint8_t SPEED_DEADBAND = 5; // in analog units

// Global variables
uint8_t  curString = 0;
const char *msg[] =
{
 ""
};

MD_UISwitch_Digital uiSwitches(uiPins, ARRAY_SIZE(uiPins));

// Sprite Definitions
const uint8_t F_PMAN1 = 6;
const uint8_t W_PMAN1 = 8;
const uint8_t PROGMEM pacman1[F_PMAN1 * W_PMAN1] =  // gobbling pacman animation
{
  0x00, 0x81, 0xc3, 0xe7, 0xff, 0x7e, 0x7e, 0x3c,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c,
  0x3c, 0x7e, 0xff, 0xff, 0xff, 0xff, 0x7e, 0x3c,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c,
};

const uint8_t F_PMAN2 = 6;
const uint8_t W_PMAN2 = 18;
const uint8_t PROGMEM pacman2[F_PMAN2 * W_PMAN2] =  // ghost pursued by a pacman
{
  0x00, 0x81, 0xc3, 0xe7, 0xff, 0x7e, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x3c, 0x7e, 0xff, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
};


const uint8_t F_HEART = 5;
const uint8_t W_HEART = 9;
const uint8_t PROGMEM heart[F_HEART * W_HEART] =  // beating heart
{
  0x0e, 0x11, 0x21, 0x42, 0x84, 0x42, 0x21, 0x11, 0x0e,
  0x0e, 0x1f, 0x33, 0x66, 0xcc, 0x66, 0x33, 0x1f, 0x0e,
  0x0e, 0x1f, 0x3f, 0x7e, 0xfc, 0x7e, 0x3f, 0x1f, 0x0e,
  0x0e, 0x1f, 0x33, 0x66, 0xcc, 0x66, 0x33, 0x1f, 0x0e,
  0x0e, 0x11, 0x21, 0x42, 0x84, 0x42, 0x21, 0x11, 0x0e,
};

const uint8_t F_INVADER = 2;
const uint8_t W_INVADER = 10;
const uint8_t PROGMEM invader[F_INVADER * W_INVADER] =  // space invader
{
  0x0e, 0x98, 0x7d, 0x36, 0x3c, 0x3c, 0x36, 0x7d, 0x98, 0x0e,
  0x70, 0x18, 0x7d, 0xb6, 0x3c, 0x3c, 0xb6, 0x7d, 0x18, 0x70,
};

const uint8_t F_ROCKET = 2;
const uint8_t W_ROCKET = 11;
const uint8_t PROGMEM rocket[F_ROCKET * W_ROCKET] =  // rocket
{
  0x18, 0x24, 0x42, 0x81, 0x99, 0x18, 0x99, 0x18, 0xa5, 0x5a, 0x81,
  0x18, 0x24, 0x42, 0x81, 0x18, 0x99, 0x18, 0x99, 0x24, 0x42, 0x99,
};


struct 
{
  const uint8_t *data;
  uint8_t width;
  uint8_t frames;
} 
sprite[] =
{
  
  { invader, W_INVADER, F_INVADER },
  { heart, W_HEART, F_HEART },
  { rocket, W_ROCKET, F_ROCKET },
  
};

void doUI(void)
{
  // set the speed if it has changed - Analog read
  {
    int16_t speed = 35;

    if ((speed >= ((int16_t)P.getSpeed() + SPEED_DEADBAND)) ||
        (speed <= ((int16_t)P.getSpeed() - SPEED_DEADBAND)))
    {
      P.setSpeed(speed);
      DEBUG("\nChanged speed to ", P.getSpeed());
    }
  }

  // now process the switch digital inputs
  if (uiSwitches.read() == MD_UISwitch::KEY_PRESS) // a switch was pressed!
  {
    switch (uiSwitches.getKey())
    {
      case JUSTIFY_SET: // TEXT ALIGNMENT - nothing on initialise
      {
        static uint8_t  curMode = 1;
        textPosition_t  textAlign[] =
        {
          PA_CENTER,
          PA_LEFT,
          PA_RIGHT
        };

        DEBUG("\nChanging alignment to ", curMode);
        P.setTextAlignment(textAlign[curMode]);
        P.displayReset();
        curMode = (curMode + 1) % ARRAY_SIZE(textAlign);
      }
      break;

      case INVERSE_SET:  // INVERSE
        P.setInvert(!P.getInvert());
      break;
    }
  }
}

void setup(void)
{
#if DEBUG_ENABLE
  Serial.begin(57600);
  DEBUGS("[Parola Test]");
#endif

  // user interface switches
  uiSwitches.begin();

  // Parola object
  P.begin();
  P.displayText(msg[curString], PA_CENTER, P.getSpeed(), PAUSE_TIME, PA_SPRITE, PA_SPRITE);
  P.setSpriteData(invader, W_INVADER, F_INVADER,rocket, W_ROCKET, F_ROCKET );
  curString++;
}

void loop(void)
{
  static uint8_t  curFX = 0;

  doUI();

  if (P.displayAnimate())
  {
    if (curString == ARRAY_SIZE(msg))
    {
      P.setSpriteData(sprite[curFX].data, sprite[curFX].width, sprite[curFX].frames,  // entry sprite
                      sprite[curFX].data, sprite[curFX].width, sprite[curFX].frames); // exit sprite
      curFX = (curFX + 1) % ARRAY_SIZE(sprite);
      curString = 0;
    }

    P.setTextBuffer(msg[curString]);
    P.displayReset();
    curString++;
  }
}
