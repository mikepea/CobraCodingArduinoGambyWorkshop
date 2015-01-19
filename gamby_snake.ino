/*
   Snake Game.
*/

#include <Gamby.h>
#include <avr/pgmspace.h>

// block mode -- saves memory, and snake is pretty blocky!
GambyBlockMode gamby;

const byte ROOM_WIDTH = 16;
const byte ROOM_HEIGHT = 14;

const byte SCORELABEL_X = (ROOM_WIDTH+2)*4+2;
const byte SCORELABEL_Y = 6;

const byte SCOREPOS_X = SCORELABEL_X;
const byte SCOREPOS_Y = SCORELABEL_Y + 1;

const byte HISCORELABEL_X = SCORELABEL_X;
const byte HISCORELABEL_Y = 0;

const byte HISCOREPOS_X = SCORELABEL_X;
const byte HISCOREPOS_Y = HISCORELABEL_Y + 1;

const byte INPUT_DELAY = 50;

const byte LEFT = 1;
const byte RIGHT = 2;
const byte UP = 3;
const byte DOWN = 4;

const byte INITIAL_HEAD_SQUARE = 120;
const byte INITIAL_TAIL_SQUARE = 119;
const int  INITIAL_MOVE_DELAY = 200;

// the 'block' bitmaps we can use.
PROGMEM prog_uint16_t palette[] = {
  0x0000, //  0: Empty
  0xffff, //  1: Full
};

// friendly names for common palette blocks:
const byte EMPTY_BLOCK = 0;
const byte WALL_BLOCK = 1;
const byte SNAKE_HEAD_BLOCK = 1;

unsigned long lastInputTime; // The time at which gamby.readInputs() was last called
byte lastInputs;    // The state of the inputs the last time they were checked.

// Bring in the font from the other tab (font.ino)
extern prog_int32_t font[];

int score = 0;
int hiscore = 50;

byte snakeHeadBufferPosition;
byte snakeTailBufferPosition;
int  snakeMoveDelay = INITIAL_MOVE_DELAY;
long lastSnakeMoveTime = 0;
byte snakeDirection = RIGHT;
bool snakeIsAlive = 1;

const byte SNAKE_BUFFER_SIZE = 32;
const byte MAX_SNAKE_LENGTH = 16;
byte snakeBuffer[SNAKE_BUFFER_SIZE];

void setup() {
  gamby.palette = palette;
  gamby.font = font;
  showInitialSplashScreen();
  delay(1000);
}

void loop() {
  randomSeed(millis());
  showGameStartScreen();

  startGame();

  playGame();

  checkForAnyButtonPressWithDelay(100);

}

bool checkForAnyButtonPressWithDelay(int ms_to_wait) {
  delay(ms_to_wait);
  gamby.readInputs();
  if (gamby.inputs)
    return 1;
  else
    return 0;
}

byte checkForDirectionButtonPress() {

  if (millis() > lastInputTime) {
    gamby.readInputs();
    lastInputTime = millis() + INPUT_DELAY;
    if (gamby.inputs != lastInputs) {
      lastInputs = gamby.inputs;
      if (gamby.inputs & DPAD_LEFT) {
        return LEFT;
      } else if (gamby.inputs & DPAD_RIGHT) {
        return RIGHT;
      } else if (gamby.inputs & DPAD_DOWN) {
        return DOWN;
      } else if (gamby.inputs & DPAD_UP) {
        return UP;
      }
    }
  }
  return 0;

}

void showGameStartScreen() {
  gamby.clearScreen();
  drawRoom();
  setupInfoDisplay();
}

void drawRoom() {
  // The room sides
  for (byte i=0; i<(ROOM_HEIGHT+2); i++) {
    gamby.setBlock(0,i,WALL_BLOCK);
    gamby.setBlock(ROOM_WIDTH+1, i, WALL_BLOCK);
  }
  // The room top and bottom
  for (byte i=0; i<(ROOM_WIDTH+2); i++) {
    gamby.setBlock(i, 0, WALL_BLOCK);
    gamby.setBlock(i, ROOM_HEIGHT+1, WALL_BLOCK);
  }
  // setBlock doesn't actually draw. .update does.
  gamby.update(0,0,ROOM_WIDTH+1,ROOM_HEIGHT+1);
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
void showInitialSplashScreen() {
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

void startGame() {
  snakeMoveDelay = INITIAL_MOVE_DELAY;
  snakeTailBufferPosition = 0;
  snakeHeadBufferPosition = 1;
  snakeBuffer[snakeTailBufferPosition] = INITIAL_TAIL_SQUARE;
  snakeBuffer[snakeHeadBufferPosition] = INITIAL_HEAD_SQUARE;
  snakeDirection = RIGHT;
}

bool reversingDirection(byte actualDirection, byte intendedDirection) {
  if ( actualDirection == LEFT && intendedDirection == RIGHT ) { return true; }
  if ( actualDirection == RIGHT && intendedDirection == LEFT ) { return true; }
  if ( snakeDirection == UP && intendedDirection == DOWN )     { return true; }
  if ( snakeDirection == DOWN && intendedDirection == UP )     { return true; }
  return false;
}

void playGame() {
  while ( snakeIsAlive ) {
    byte intendedDirection = checkForDirectionButtonPress();
    if ( intendedDirection ) {
      if (! reversingDirection(snakeDirection, intendedDirection) ) {
        snakeDirection = intendedDirection;
      }
    }
    if ( millis() - lastSnakeMoveTime > snakeMoveDelay ) {
      lastSnakeMoveTime = millis();
      moveSnake();
    }
  }
  snakeDeath();
}

void snakeHasDied() {
  snakeIsAlive = 0;
}

byte getRealBufferPosition(byte bufPos) {
  return ( bufPos ) % SNAKE_BUFFER_SIZE;
}

void updateSnakeHeadSquare(byte snakeHeadSquare) {
  snakeHeadBufferPosition = getRealBufferPosition(snakeHeadBufferPosition + 1);
  snakeBuffer[snakeHeadBufferPosition] = snakeHeadSquare;
}

bool snakeWillCollideWithEdge(byte head, byte dir) {
  if ( snakeDirection == LEFT  && getRoomX(head) == 0 ) { return true; }
  if ( snakeDirection == RIGHT && getRoomX(head) == ROOM_WIDTH - 1 ) { return true; }
  if ( snakeDirection == UP    && getRoomY(head) == 0 ) { return true; }
  if ( snakeDirection == DOWN  && getRoomY(head) == ROOM_HEIGHT - 1 ) { return true; }
  return false;
}

byte getRelativePosition(byte pos, byte dir) {
  if ( dir == LEFT ) {
    return (pos - 1);
  } else if ( dir == RIGHT ) {
    return (pos + 1);
  } else if ( dir == UP ) {
    return (pos - ROOM_WIDTH);
  } else if ( dir == DOWN ) {
    return (pos + ROOM_WIDTH);
  } else {
    return pos; // invalid direction provided.
  }
}

void moveSnake() {
  byte snakeHeadSquare = snakeBuffer[snakeHeadBufferPosition];

  if ( snakeWillCollideWithEdge(snakeHeadSquare, snakeDirection) ) {
    snakeHasDied();
    return;
  }

  byte nextHeadSquare = getRelativePosition(snakeHeadSquare, snakeDirection);
  updateSnakeHeadSquare(nextHeadSquare);

  if ( moveTailSquare() ) {
    emptyLocation(snakeBuffer[snakeTailBufferPosition]);
    snakeTailBufferPosition = getRealBufferPosition(snakeTailBufferPosition + 1);
  }
  drawSnakeHead();

}

byte snakeLength() {
  if ( snakeHeadBufferPosition > snakeTailBufferPosition ) {
    return snakeHeadBufferPosition - snakeTailBufferPosition + 1;
  } else {
    return ( snakeHeadBufferPosition + SNAKE_BUFFER_SIZE ) - snakeTailBufferPosition + 1;
  }
}

bool moveTailSquare() {
  if ( snakeLength() >= MAX_SNAKE_LENGTH ) {
    return true;
  }
  if ( random(16) <= 1 ) {
     return false;
  } else {
    return true;
  }
}

byte getRoomX(byte pos) {
  return (pos % ROOM_WIDTH);
}

byte getScreenX(byte pos) {
  return getRoomX(pos) + 1;
}

byte getRoomY(byte pos) {
  return (pos / ROOM_WIDTH);
}

byte getScreenY(byte pos) {
  return getRoomY(pos) + 1;
}

void emptyLocation(byte pos) {
  gamby.drawBlock(getScreenX(pos), getScreenY(pos), EMPTY_BLOCK);
}

void drawSnakeHead() {
  byte headSquare = snakeBuffer[snakeHeadBufferPosition];
  gamby.drawBlock(getScreenX(headSquare), getScreenY(headSquare), SNAKE_HEAD_BLOCK);
}

void snakeDeath() {
  gamby.setPos(16,5);
  gamby.print("SQUASHED!");
  delay(1000);
  snakeIsAlive = 1;
}

void showScore() {
}
