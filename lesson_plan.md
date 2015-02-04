Gamby Snake Workshop
-----------

## Setup

* Install the Arduino software

## Lesson 1 - Talking to the Arduino

* Connect the Arduino using the USB cable. A green power light should come on.
* In the Arduino software, ensure Tools->Board->Arduino Uno
* Click on File->Examples->01.Basics->Blink
* Click the (=>) 'Upload' button.

The software should say 'compiling', then 'uploading' briefly, and then tell you how big the 'sketch' is.

You should now have a flashing amber light!

This is the 'blink' demo, pretty much the simplest thing you can do with an Arudino, and a useful test for us to see that everything is working as expected.

Let''s have a look at the code:

* /* comments */
* // comments
* setup()
* loop()
* pinMode
* digitalWrite()

## Lesson 2 - variables, a bit of maths, and a bit of logic.

Open Blink again, and tweak it to look like:

````
int blink_delay = 10;

void setup() {
  pinMode(13, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(13, HIGH);
  delay(blink_delay);
  digitalWrite(13, LOW);
  delay(blink_delay);
  //blink_delay = blink_delay + 10;
  //if ( blink_delay > 500 ) {
  //  blink_delay = 10;
  //}
}
````

## Lesson 4 - Making it easier to read: Functions and Constants

As we add code, it gets harder and harder to remember what different bits do, and makes it
difficult to understand the flow of the program.

Also, we start to get 'magic numbers' littered all over the place, which are confusing to read.

We have 'magic numbers' of 10, 13 & 500 in our code. What are their purposes?

Also, we have a function in our code too - the 'blinkLed' code.

So, we can update our sketch to be:

````
const int INITIAL_BLINK_DELAY = 10;
const int BLINK_DELAY_INCREMENT = 10;
const int MAX_BLINK_DELAY = 500;
const int LED_PIN = 13;

int blink_delay = INITIAL_BLINK_DELAY;

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  blinkLed(blink_delay);
  blink_delay = blink_delay + BLINK_DELAY_INCREMENT;
  if ( blink_delay > MAX_BLINK_DELAY ) {
    blink_delay = INITIAL_BLINK_DELAY;
  }
}

void blinkLed(int blink_ms) {
  digitalWrite(LED_PIN, HIGH);
  delay(blink_ms);
  digitalWrite(LED_PIN, LOW);
  delay(blink_ms);
}
````

What have we done here?

* Created 'const' constant variables (!) to give meaning to our magic numbers. We now can adjust them in one place, and when we see them in the code we know what they are.
* Split out the blinkLed code into its own function. A function is a small snippet of code that has a very small and specific purpose. It helps us understand our code later, and can also help us *reuse* code in multiple places.

NB: demonstrate adding a second instance of blinkLed(MAX_BLINK_DELAY - blink_delay)



-------------------

## Lesson 5 - Installing Gamby Library

TODO figure out how to install gamby library on each of:

* Linux
* WinXP
* Win7
* Win8
* MacOS

So, now we have the Gamby library installed, lets test loading some example sketches:

* File->Examples->Gamby->First Test

Should display the Gamby splash screen

* File->Examples->Gamby->GambyGraphicsMode->GambyGraphicsSpeedTest

little demo of some of the graphics capability!


### Gamby Modes

The Gamby has 3 modes:

* Text
* Block
* Graphics

Text is purely for writing text to the screen. It uses very little memory, but not so useful for games.

Graphics mode is quite advanced, and lets us fully control every pixel in the display, drawing shapes, etc. It uses quite a lot of the arduino memory.

We're going to concentrate on Block Mode. This gives us acces to a grid of 24 by 16 'blocks' of 4x4 pixels. It's really good for games like snake, or tetris, or candy crush. The benefit for us it's it's very easy to get going with, and works well for our snake game.

Open up _Templates->BlockModeTemplate

Click 'upload'. An error occurs! We have a bug!!!

(FIX: change 'pallette' to 'block' as the const variable name)

Notes on this sketch:

* `#include <Gamby.h>` -- include the Gamby library code
* `PROGMEM` -- a special type of variable, stored in flash memory rather than RAM (32K vs 2K).
   Slower than normal vars though, and harder to write to.
* `GambyBlockMode gamby` -- 'gamby' here is a special variable called an 'Instance' (of a Class
  'GambyBlockMode') -- we use it to access the Gamby library code. It makes our lives really easy.

## Lesson 5 - Make a Splash Screen

A splash screen tells our player that they are about to start our game, and what to do to start.

Open up _Templates->TextModeTemplate

Notice that this has also opened up a second tab in your Arduino app, 'font'. This is a special
'include' file that includes extra code that we wish to use, but is separate from our main
program. In this case, it is defines the shapes of the characters used in our text font.

Save this as 'GambySplashScreen', so we can edit our own copy.

Feel free to delete the comments, to fit more code on the screen.

Run the program, and you''ll see that we get 'Hello GAMBY!' on the screen.

This is in TextMode though. How about BlockMode?

Change the line:

    GambyTextMode gamby;

to:

    GambyBlockMode gamby;

and then upload again. Does your program work?

Even though we''re running in BlockMode, we are still able to write a font to the screen!

**How do we position the text on the screen though?**


Put `gamby.setPos(0,2);` before the `println` call. Upload!

The text moves down to the *third* line.

Now update this to `gamby.setPos(8,2)`. The text moves across to the 8th *pixel*.

Gamby text positioning is a little odd!

Now lets make our splash screen. Put the following code in it's own section at the end of the file, as we're making a function!

````
void showInitialSplashScreen() {
  gamby.clearDisplay();
  delay(500);

  gamby.setPos(0,2);
  gamby.println("SNAAAAAAKE!");
  delay(500);

  gamby.setPos(0,7);
  gamby.println("Press any button to begin!");
}	
````

Upload your code now. Nothing happens!

We need to *call* our function in the main code...

At the start of the loop(), put the following line:

```
showInitialSplashScreen();
```

Now upload again.

The splash screen is sort of flickering - why?

This is because our loop is repeating, and each time is clearing the screen to re-paste the text onto it.

We don't need to worry about this though, as now we're going to add some code to wait for a key press, just like we say on our splash screen!

## Lesson 6 - Getting Input from the buttons

Save your code again -- this time as 'Lesson6-Buttons'

Add the following function:

````
void waitForButtonPress() {
  gamby.readInputs();
  byte initialInputs = gamby.inputs;
  while (gamby.inputs == initialInputs) {
    delay(100);
    gamby.readInputs();
  }
}
````

And then call the function after your call to `showInitialSplashScreen` in `loop()`

Now, what is going on in our function? This is pretty complicated unfortunately.

(EXPLAIN, but MOVE ON QUICKLY)

The gamby library can 'scan' which buttons are currently pressed. This populates a variable called 'gamby.inputs'. We take a copy of this, so we know what buttons were *already pressed*.

Then, in a while-loop, we keep reading the inputs variable until it is *different* from what we initially read - indicating that a new button has been pressed.

Urg, sorry. Now that we've wrapped that pretty gnarly code up in a function though, we can forget about how it works!


## Lesson 7 - Drawing our game room.

````
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
````

## Lesson 7 - How do we move something about in our room?


## Lesson 8 - Colliding with the edges!

## Lesson 9 - Making our snake snakey.

## Lesson 10 - Colliding with ourselves

--------------------------------------------------------------

## Further Improvements...

### Add 'internal walls' to the room to make the game more difficult.

* You could then maybe introduce the notion of 'game levels', each getting harder.
* Maybe once the snake has eaten 50 fruit at each level, they move onto the next.

### Storing the hiscore(s) in EEPROM

* The Arduino chip has EEPROM - memory that survives across powering it off and on.
* You could simply store the hi-score in there, or maybe implement a way of storing names too!
* See Examples->EEPROM for how to work with it.

### Make the snake head and/or tail pointy

* so we know the direction of travel

