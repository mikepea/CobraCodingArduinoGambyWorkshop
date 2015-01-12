/*
   Snake Game.
*/

#include <Gamby.h>
#include <avr/pgmspace.h>

boolean playing;    // 'true' when the game is playing, 'false' after game over

// block mode -- saves memory, and snake is pretty blocky!
GambyBlockMode gamby;

const byte ROOM_WIDTH = 18;
const byte ROOM_HEIGHT = 16;

const byte SCORELABEL_X = ROOM_WIDTH*4+2;
const byte SCORELABEL_Y = 6;

const byte SCOREPOS_X = SCORELABEL_X;
const byte SCOREPOS_Y = SCORELABEL_Y + 1;

const byte HISCORELABEL_X = SCORELABEL_X;
const byte HISCORELABEL_Y = 0;

const byte HISCOREPOS_X = SCORELABEL_X;
const byte HISCOREPOS_Y = HISCORELABEL_Y + 1;

const byte START_X = 8;
const byte START_Y = 8;

const byte INPUT_DELAY = 50;

// the 'block' bitmaps we can use.
PROGMEM prog_uint16_t palette[] = {
  0x0000, //  0: Empty
  0xffff, //  1: Full
};

// friendly names for common palette blocks:
const byte EMPTY_BLOCK = 0;
const byte WALL_BLOCK = 1;

unsigned long lastInputTime; // The time at which gamby.readInputs() was last called
byte lastInputs;    // The state of the inputs the last time they were checked.

// Bring in the font from the other tab (font.ino)
extern prog_int32_t font[];

int score = 0;
int hiscore = 50;

void setup() {
  gamby.palette = palette;
  gamby.font = font;
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
  byte i;
  gamby.clearScreen();

  // The room sides
  for (i=0; i<ROOM_HEIGHT; i++) {
    gamby.setBlock(0,i,WALL_BLOCK);
    gamby.setBlock(ROOM_WIDTH-1, i, WALL_BLOCK);
  }
  // The room top and bottom
  for (i=0; i<ROOM_WIDTH; i++) {
    gamby.setBlock(i, 0, WALL_BLOCK);
    gamby.setBlock(i, ROOM_HEIGHT-1, WALL_BLOCK);
  }

  // setBlock doesn't actually draw. .update does.
  gamby.update(0,0,ROOM_WIDTH-1,ROOM_HEIGHT-1);

  // Print the info display
  setupInfoDisplay();

  playing = true;
}

void updateInfoDisplay() {
  gamby.setPos(HISCOREPOS_X,HISCOREPOS_Y);
  gamby.print(hiscore);
  gamby.setPos(SCOREPOS_X,SCOREPOS_Y);
  gamby.print(score);
}

void setupInfoDisplay() {
  gamby.setPos(HISCORELABEL_X,HISCORELABEL_Y);
  gamby.print("Hi");
  gamby.setPos(SCORELABEL_X,SCORELABEL_Y);
  gamby.print("Score");
  updateInfoDisplay();
}

// Display the splash screen and wait for the user to press a button.
void showSplashScreen() {
  gamby.clearDisplay();

  gamby.setPos(0,2);
  gamby.print("SNAAAAAAKE!");

  gamby.setPos(0,7);
  gamby.print("Press any button to begin!");

  // Wait for a button to be pressed before continuing
  gamby.readInputs();
  byte initialInputs = gamby.inputs;
  while (gamby.inputs == initialInputs) {
    delay(100);
    gamby.readInputs();
  }
}
