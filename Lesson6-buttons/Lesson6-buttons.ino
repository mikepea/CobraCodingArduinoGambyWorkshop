
#include <Gamby.h>

// Bring in the font from the 'font' tab (font.ino)
extern prog_int32_t font[];

// Each of Gamby's 'modes' are wrapped in a class, a self-contained unit
// that bundles together all the required functionality. To use a mode,
// you must first create an 'instance' of its class -- a sort of working 
// copy. Your sketch should only use one.
GambyBlockMode gamby;

void setup () {
  // Set the font. You generally need to do this only once, usually just after
  // initializing Gamby. You could, however, do this elsewhere in your 
  // sketch -- for example, if you wanted to change fonts on the fly.
  gamby.font = font;

  // That's all that's required! From here on, it's up to you. All of Gamby's
  // functionality is contained in the 'gamby' object, which you use like
  // this:
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
