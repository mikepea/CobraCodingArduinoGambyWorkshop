
#include <Gamby.h>
#include <avr/pgmspace.h>

// Bring in the font from the 'font' tab (font.ino)
extern prog_int32_t font[];

PROGMEM prog_uint16_t blocks[] = {
  0x0000, //  0 All 'white' (block 0 should almost always be 0x0000) 
  0xffff, //  1 All 'black'
  0x5a5a, //  2 50% gray dither
  0xfaf5, //  3 75% gray dither
  0x050a, //  4 25% gray dither
  0xedb7, //  5 Light diagonal left line
  0x1248, //  6 Dark diagonal left line
  0x7bde, //  7 Light diagonal right line
  0x8421, //  8 Dark diagonal right line
  0x888f, //  9 Dark-on-light solid grid lines
  0x1110, // 10 Light-on-dark solid grid lines
  0xa080, // 11 Light-on-dark dotted grid lines
  0x5f1f, // 12 Dark-on-light dotted grid lines
  0x33cc, // 13 Checker pattern 
  0xcc33, // 14 Mirrored checker pattern
  0x0001  // 15 Single pixel (upper right)
};


const byte ROOM_WIDTH = 16;
const byte ROOM_HEIGHT = 14;

byte spiderLocation = 120; // somewhere in the middle.

const byte LEFT = 1;
const byte RIGHT = 2;
const byte UP = 3;
const byte DOWN = 4;

byte lastInputs = 0;

byte spiderDirection = 0;
bool weAreAlive = true;

// Each of Gamby's 'modes' are wrapped in a class, a self-contained unit
// that bundles together all the required functionality. To use a mode,
// you must first create an 'instance' of its class -- a sort of working 
// copy. Your sketch should only use one.
GambyBlockMode gamby;

void setup () {

  gamby.font = font;
  gamby.palette = blocks;


  gamby.println("Hello, GAMBY!");
}

void loop () {
  showInitialSplashScreen();
  waitForButtonPress();
  drawRoom();
  playGame();
  displayDeathMessage();
  delay(500);
  waitForButtonPress();
}

void showInitialSplashScreen() {
  gamby.clearScreen();
  delay(500);

  gamby.setPos(0,2);
  gamby.println("SNAAAAAAKE!");
  delay(500);

  gamby.setPos(0,7);
  gamby.println("Press any button to begin!");
}

void waitForButtonPress() {
  gamby.readInputs();
  byte initialInputs = gamby.inputs;
  while (gamby.inputs == initialInputs) {
    delay(100);
    gamby.readInputs();
  }
}

void drawRoom() {
  gamby.clearDisplay();
  gamby.box(0, 0, ROOM_WIDTH+1, ROOM_HEIGHT+1, 2);   // x1, y1, x2, y2, palette
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

void drawSpider() {
  byte headSquare = spiderLocation;
  gamby.drawBlock(getScreenX(headSquare), getScreenY(headSquare), 1);
}


byte checkForDirectionButtonPress() {
  gamby.readInputs();
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
  return 0;
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


void emptyLocation(byte pos) {
  gamby.drawBlock(getScreenX(pos), getScreenY(pos), 0);
}

void moveSpider() {
  byte nextSpiderLocation = getRelativePosition(spiderLocation, spiderDirection);
  emptyLocation(spiderLocation);
  if ( willWeCollideWithEdge(spiderLocation, spiderDirection) ) {
    weAreAlive = false;
  } else {
    spiderLocation = nextSpiderLocation;
    drawSpider();
  }
}
void playGame() {
  while (weAreAlive) {
    byte intendedDirection = checkForDirectionButtonPress();
    if ( intendedDirection ) {
      spiderDirection = intendedDirection;
    }
    moveSpider();
    delay(100); // 10 frames per second
  }
}

bool willWeCollideWithEdge(byte loc, byte dir) {
  if ( dir == LEFT  && getRoomX(loc) == 0 ) { return true; }
  if ( dir == RIGHT && getRoomX(loc) == ROOM_WIDTH - 1 ) { return true; }
  if ( dir == UP    && getRoomY(loc) == 0 ) { return true; }
  if ( dir == DOWN  && getRoomY(loc) == ROOM_HEIGHT - 1 ) { return true; }
  return false;
}

void displayDeathMessage() {
  gamby.setPos(16,5);
  gamby.print("SQUASHED!");
  tone(9, 110, 500);
}
