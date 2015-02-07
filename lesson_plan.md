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
  gamby.clearScreen();
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

Urg, sorry. Now that we''ve wrapped that pretty gnarly code up in a function though, we can forget about how it works!


## Lesson 7 - Drawing our game room.

We now need to draw our 'game room' - the screen that we''re going to play the snake game in.

This consists of a box for the snake to roam around in, and some text to tell us what the score
and hiscore is.

We need to get our blockMode block palette into our sketch, so:

* Save your sketch as 'Lesson7-GameRoom'
* Open up the _Templates->GambyBlockMode example
* Cut and paste the following sections over into your sketch:

```
#include <avr/pgmspace.h>

PROGMEM prog_uint16_t palette[] = {
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
```

In the `setup()` function, add the following line at the start, to make the pallet above available:

```
  gamby.palette = palette;
```

Now, lets make our game room.

Gamby provides a useful 'box' function that will draw us the outline of a box on the
screen, in a 'block' type of our choice.

````
const byte ROOM_WIDTH = 16;
const byte ROOM_HEIGHT = 14;

void drawRoom() {
  gamby.clearDisplay();
  gamby.box(0, 0, ROOM_WIDTH+1, ROOM_HEIGHT+1, 2);   // x1, y1, x2, y2, palette
}
````

Add this to the main loop, so it becomes:

````
void loop () {
  showInitialSplashScreen();
  waitForButtonPress();
  drawRoom();
  delay(500);
  waitForButtonPress();
}
````

Lets worry about the scoreboard later. Note that we''ve left some space for it on the right.

Hit Save!

## Lesson 8 - How do we move something about in our room?

Save our current sketch as 'Lesson8-Spider'

Before we think about a snake, let''s just move a 'spider' around the room. He''s simpler, as we
can show him with a single block.

Our 'room', inside the box, is 16 blocks wide and 14 blocks high. So, a bit of maths:

* we have 16 x 14 = 224 'squares' in our room.

This is nice, because it means we can store the location of our spider in a really small
amount of memory -- a 'byte'. The Arduino only has 2048 bytes of memory, so we have to be
pretty conservative about how we use it.

So, let''s draw our spider:

Feel free to just type this in:

````
byte spiderLocation = 112; // somewhere in the middle.


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

````

And don''t forget to actually call `drawSpider()` in our main loop:

````
void loop() {
  showInitialSplashScreen();
  waitForButtonPress();
  drawRoom();
  drawSpider();
  delay(1000);
  waitForButtonPress();
}
````

Now let''s figure out how to move her around!

Similar to the function we created when we wanted to know when a button was pressed. we
call upon gamby.readInputs again to find out if directions have been pressed. This time though,
we return a value from our function to let our calling code know what direction we would like
to go.

Add the following code to the end of your sketch:

````
const byte LEFT = 1;
const byte RIGHT = 2;
const byte UP = 3;
const byte DOWN = 4;

byte lastInputs = 0;

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
````

As well as knowing which directions have been pressed, we also need to know how the spiderLocation
changes with each direction. There''s a little bit of maths to this, brace yourself.

````
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
````

We can now use the above functions to actually move our spider around, which forms the basis
of our 'Game' so far. So, let''s make a 'playGame' function where we loop moving around our spider.

````
byte spiderDirection = 0;
bool weAreAlive = true;

void emptyLocation(byte location) {
  drawBlock()
}

void emptyLocation(byte pos) {
  gamby.drawBlock(getScreenX(pos), getScreenY(pos), EMPTY_BLOCK);
}

void moveSpider() {
  byte nextSpiderLocation = getRelativePosition(spiderLocation, spiderDirection);
  emptyLocation(spiderLocation);
  spiderLocation = nextSpiderLocation;
  drawSpider();
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
````

Finally, in our main `loop()` at the top, replace `drawSpider()` with `playGame()` to include our game logic in the main application loop!

Upload, and move that spider!!!

Save :)


## Lesson 9 - Colliding with the edges!

Save your sketch now as 'Lesson9-Colliding'.

You've probably noticed that we can move through walls, which isn't very realistic!

Because of the way that we have created our room, we need a special bit of logic to work out if we are trying to escape from it:

````
bool willWeCollideWithEdge(byte loc, byte dir) {
  if ( dir == LEFT  && getRoomX(loc) == 0 ) { return true; }
  if ( dir == RIGHT && getRoomX(loc) == ROOM_WIDTH - 1 ) { return true; }
  if ( dir == UP    && getRoomY(loc) == 0 ) { return true; }
  if ( dir == DOWN  && getRoomY(loc) == ROOM_HEIGHT - 1 ) { return true; }
  return false;
}
````

We can now update our moveSpider() function to kill our spider
if it collides with the wall!

````
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
````

We should also probably add a bit of text to tell our player what has happened when the spider dies!

````
void displayDeathMessage() {
  gamby.setPos(16,5);
  gamby.print("SQUASHED!");
  tone(9, 110, 500);
}
````

And then this after `playGame()` in our main loop:

````
void loop () {
  showInitialSplashScreen();
  waitForButtonPress();
  drawRoom();
  playGame();
  displayDeathMessage();
  delay(1000);
  waitForButtonPress();
}
````

***OH NOES! WE HAVE A BUG!***

You might be noticing now that restarting the game immmediately kills our spider :(

**Can you think why?**


Reason is that we are not resetting our global variables before we call playGame() again, so our spider and direction are the same as they were when we died before.

Let''s add a new function that resets these to the start-of-game values:

````
void initializeGame() {
  spiderLocation = 104;
  spiderDirection = 0;
  weAreAlive = true;
}
````

And add this to the `loop()`.



**Save!!!**


## Lesson 11 - Eating fruit.

We need something to eat. We''re a hungry spider (and when we''re a snake, we need something to
make us get longer!)

So, lets plop some fruit around that we have to munch. This also brings the concept of scoring
nicely into out game!

We need to pick a square for the fruit to be placed, and of course it can''t go where something
is already. At the moment, that''s just our spider -- but it''ll get a bit more complicated when
we have a long snake!

````
byte fruitSquare;
````

````
byte pickFruitSquare() {
  byte potentialFruitSquare = random(ROOM_HEIGHT*ROOM_WIDTH);
  while ( inForbiddenLocation(potentialFruitSquare) ) {
    potentialFruitSquare = random(ROOM_HEIGHT*ROOM_WIDTH); // pick another
  }
  return potentialFruitSquare;
}

bool inForbiddenLocation(byte loc) {
  if ( loc == spiderLocation ) { return true; }
  return false;
}

void drawFruit() {
  fruitSquare = pickFruitSquare();
  gamby.drawBlock(getScreenX(fruitSquare), getScreenY(fruitSquare), 1);
}
````

And let''s draw a fruit into our game:

````
void loop () {
  showInitialSplashScreen();
  waitForButtonPress();
  initializeGame();
  drawRoom();
  drawFruit();
  playGame();
  displayDeathMessage();
  delay(1000);
  waitForButtonPress();
}
````


Upload this sketch, you should now be able to run over the fruit, and eat it.

Thinking about it though, we haven't added any code to say that we've collided with the fruit!

***We''re actually just erasing it as we pass over it :(***

Let's add a bit of logic to actually munch the fruit!


Add this code snippet to the end of your `moveSpider()` function:

```
  if ( spiderLocation == fruitSquare ) {
    score += 10;
    tone(9, 440, 10);
    drawFruit();
    updateInfoDisplay();
  } 
```

We also want to make our `updateInfoDisplay()` function, and define our variables for scoring:

```
const byte SCOREPOS_X = 74;
int score = 0;
int hiscore = 0;
```

```
void updateInfoDisplay() {
  if ( score > hiscore ) {
    hiscore = score;
  }
  gamby.setPos(SCOREPOS_X,0);
  gamby.print("Hi");
  gamby.setPos(SCOREPOS_X,1);
  gamby.print(hiscore);
  gamby.setPos(SCOREPOS_X,5);
  gamby.print("Score");
  gamby.setPos(SCOREPOS_X,7);
  gamby.print(score);
}
````

And of course, now we have a score, we need to reset it (but not the hiscore!) when we initialize the game:

````
void initializeGame() {
  spiderLocation = 104;
  spiderDirection = 0;
  weAreAlive = true;
  score = 0;
}
````


Hit SAVE!

## Lesson 10 - Making a snakey snake.

Save your sketch as 'Lesson10-SnakeySnake'

Ok, we''re now onto the tricky bit. Sorry about that!

Firstly, let''s think a little about the snake and what it means for updating our screen:

* The snake head moves onto a different square.
* The snake body stays in the same place
* The snake tail moves onwards; old location gets erased.

If we eat a fruit, then the head moves on, but the tail remains in place.

Here it hopefully becomes a little clearer as to why we reference the locations as a simple byte rather
than x and y co-ordinates: It makes tracking the snake lots easier!

What we are going to do, is create what is called a **ring buffer** for the snake.

Think of a set of boxes in a row:

      0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17
    -------------------------------------------------------------------------
    |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
    -------------------------------------------------------------------------

In our case above, we have 18 'positions' in our set of boxes, accessible via references 0 to 17.

What we''ll be doing is storing the snake head and tail positions in these boxes, and
keeping an idea of where each of them are.

So, let''s say our snake head is at location 120 and the tail at 119, just to the left of it.

      0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17
    -------------------------------------------------------------------------
    |   |   |   |   |   |   |   |   |119|120|   |   |   |   |   |   |   |   |
    -------------------------------------------------------------------------
    snakeTailPos == 8
    snakeHeadPos == 9

Now, when our snake moves right, the head moves to position 121, and the tail to position 120:

      0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17
    -------------------------------------------------------------------------
    |   |   |   |   |   |   |   |   |   |120|121|   |   |   |   |   |   |   |
    -------------------------------------------------------------------------
    snakeTailPos == 9
    snakeHeadPos == 10

But notice what happened here. We stored the new head location in position 10, and moved the
pointers to the positions up one. The value in position 9 is unaffected, but is now the tail
of our snake!

Now imagine we''ve eaten some fruit. We want the snake to grow longer. Moving up now we
move the head to position (121-16) == 105.

      0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17
    -------------------------------------------------------------------------
    |   |   |   |   |   |   |   |   |   |120|121|105|   |   |   |   |   |   |
    -------------------------------------------------------------------------
    snakeTailPos == 9
    snakeHeadPos == 11

The trick here is that while we move the snake head position pointer onwards, we simply do
not move the tail pointer.

**The multi-box structure we have been using above is called an ARRAY. They are really useful.**

So, now for some code implementing the above:

Define our buffer array, and some variables/constants to help us manage it:

````
const byte SNAKE_HEAD_BLOCK = 1;
const byte INITIAL_HEAD_SQUARE = 120;
const byte INITIAL_TAIL_SQUARE = 119;
const byte SNAKE_BUFFER_SIZE = 64;
const byte MAX_SNAKE_LENGTH = 62;
byte snakeBuffer[SNAKE_BUFFER_SIZE];
byte snakeHeadBufferPosition;
byte snakeTailBufferPosition;
byte snakeDirection;
````

And update our initializeGame function to manage the snake:

````
void initializeGame() {
  snakeTailBufferPosition = 0;
  snakeHeadBufferPosition = 1;
  snakeBuffer[snakeTailBufferPosition] = INITIAL_TAIL_SQUARE;
  snakeBuffer[snakeHeadBufferPosition] = INITIAL_HEAD_SQUARE;
  weAreAlive = true;
  score = 0;
}
````

And we want to make an updated version of our moveSpider code that moves the snake instead:

````
void drawSnakeHead() {
  byte headSquare = snakeBuffer[snakeHeadBufferPosition];
  gamby.drawBlock(getScreenX(headSquare), getScreenY(headSquare), SNAKE_HEAD_BLOCK);
}

void moveSnake() {
  byte snakeHeadSquare = snakeBuffer[snakeHeadBufferPosition];
  byte nextHeadSquare = getRelativePosition(snakeHeadSquare, snakeDirection);
  
  if ( willWeCollideWithEdge(snakeHeadSquare, snakeDirection) ) {
    weAreAlive = false;
  } else {

    if ( nextHeadSquare == fruitSquare ) {
      score += 10;
      tone(9, 440, 10);
      drawFruit();
      updateInfoDisplay();
    } else {
      emptyLocation(snakeBuffer[snakeTailBufferPosition]);
      snakeTailBufferPosition = getRealBufferPosition(snakeTailBufferPosition + 1);
    }
    updateSnakeHeadSquare(nextHeadSquare);
    drawSnakeHead();
  }
}

void updateSnakeHeadSquare(byte snakeHeadSquare) {
  snakeHeadBufferPosition = getRealBufferPosition(snakeHeadBufferPosition + 1);
  snakeBuffer[snakeHeadBufferPosition] = snakeHeadSquare;
}

byte getRealBufferPosition(byte bufPos) {
  return ( bufPos ) % SNAKE_BUFFER_SIZE;
}
````

Note that we have a function being called - `getRealBufferPosition()`. This is a little bit of magic to handle the case that the buffer is of a fixed size, so handles the cases where our snakeHead/Tail BufferPosition moves off the end of the array and to the start.


Finally, to hook all this in, replace moveSpider in our main game:

````
void playGame() {
  while (weAreAlive) {
    byte intendedDirection = checkForDirectionButtonPress();
    if ( intendedDirection ) {
      snakeDirection = intendedDirection;
    }
    moveSnake();
    delay(100);
  }
}
````




## Lesson 11 - Colliding with ourselves

You might have noticed a couple of problems with our snake.

1. We can reverse over ourself!
2. We can move through ourself!


Ok, let's tackle not reversing:

```
bool reversingDirection(byte actualDirection, byte intendedDirection) {
  if ( actualDirection == LEFT && intendedDirection == RIGHT ) { return true; }
  if ( actualDirection == RIGHT && intendedDirection == LEFT ) { return true; }
  if ( snakeDirection == UP && intendedDirection == DOWN )     { return true; }
  if ( snakeDirection == DOWN && intendedDirection == UP )     { return true; }
  return false;
}
```

And so we then insert that into our `moveSnake()` code:

```
...
 if ( intendedDirection ) {
      if (! reversingDirection(snakeDirection, intendedDirection) ) {
        snakeDirection = intendedDirection;
      }
    }
 }
```

But how do we check that we're not crashing into ourself?

Well, let's just check what positions are in our snakeBuffer!

````
byte snakeLength() {
  if ( snakeHeadBufferPosition > snakeTailBufferPosition ) {
    return snakeHeadBufferPosition - snakeTailBufferPosition + 1;
  } else {
    return ( snakeHeadBufferPosition + SNAKE_BUFFER_SIZE ) - snakeTailBufferPosition + 1;
  }
}

bool checkIfSquareIsWithinSnake(byte square) {
  byte i = 0;
  while ( i < snakeLength() ) {
    if ( square == snakeBuffer[getRealBufferPosition(snakeTailBufferPosition + i)] ) {
      return true;
    }
    i++;
  }
  return false;
}
````

And then insert some code to check if we've collided in the main `moveSnake()` function:

````

  if ( willWeCollideWithEdge(snakeHeadSquare, snakeDirection) ) {
    weAreAlive = false;
  } else if ( checkIfSquareIsWithinSnake(nextHeadSquare) ) {
    weAreAlive = false;
  } else {
     ...  
````



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

