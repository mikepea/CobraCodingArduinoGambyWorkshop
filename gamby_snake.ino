/*
   Snake Game.   
*/

#include <Gamby.h>
#include <avr/pgmspace.h>

boolean playing;    // 'true' when the game is playing, 'false' after game over

// block mode -- saves memory
GambyBlockMode gamby;

const byte ROOML_WIDTH = 20;
const byte ROOM_HEIGHT = 15;

const byte INPUT_DELAY = 50;

// the 'block' bitmaps we can use.
PROGMEM prog_uint16_t palette[] = {
  0x0000, //  0: Empty
  0xffff, //  1: Full
};

unsigned long lastInputTime; // The time at which gamby.readInputs() was last called
byte lastInputs;    // The state of the inputs the last time they were checked.

void setup() {
  gamby.palette = palette;
  showSplashScreen();
  randomSeed(millis());
  startGame();  
}

void loop() {
  if (playing) {
    if (millis() > lastInputTime) {
      gamby.readInputs();
      // do stuff with inputs
      lastInputTime = millis() + INPUT_DELAY;
    }
  } else {
    delay(100);
    gamby.readInputs();
    if (gamby.inputs)
      startGame();
  }
}

void startGame() {
  gamby.clearScreen();
  playing = true;
}

// Display the splash screen and wait for the user to press a button.
void showSplashScreen() {
  gamby.clearDisplay();

  // Wait for a button to be pressed before continuing
  gamby.readInputs();
  byte initialInputs = gamby.inputs;
  while (gamby.inputs == initialInputs) {
    delay(100);
    gamby.readInputs();
  }
}
