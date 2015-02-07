
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
