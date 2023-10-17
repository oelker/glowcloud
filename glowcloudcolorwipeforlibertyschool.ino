// normally we would also include "SPI.h" but don't do that in thise case, it's not for Trinket or Gemma. 
// notes on how to program this board: you need arduino 1.6.x of some sort, and enable contributor boards.
// this project was done on the Adafruit Gemma NOT the Arduino Gemma, make sure you've chosen the right board. 
// Adafruit Gemma requires the USBtinyISP programmer, again, NOT the Arduino Gemma programmer.

#include "Adafruit_WS2801.h"

//this if statement allows for differences in which microprocessor is being used, yay.

#ifdef __AVR_ATtiny85__
 #include <avr/power.h>
#endif

// thankfully 12mm flat Adafruit Pixels use RGB color, some don't.
uint8_t dataPin  = 0;    // Yellow wire on the 12mm flat Adafruit Pixels
uint8_t clockPin = 1;    // Green wire on the 12mm flat Adafruit Pixels
 
// Digital IO pin connected to the button.  This will be
// driven with a pull-up resistor so the switch should
// pull the pin to ground momentarily.  On a high -> low
// transition the button press logic will execute.
uint8_t SWITCH  = 2;    

// button debouncer, how many ms to debounce, 5+ ms is usually plenty
// this should be a define statement and NOT a variable, right? 
  
uint8_t DEBOUNCE = 10; 

// this is the variable we will iterate after the switch is pressed 
// Don't forget to connect the ground wire to Arduino ground,
// and the +5V wire to a +5V supply

uint8_t SETTING = 0;


Adafruit_WS2801 strip = Adafruit_WS2801(25, dataPin, clockPin);

//Adafruit_WS2801 strip = Adafruit_WS2801(25); uncomment this if you do want to use SPI, we do not.

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket (or on Gemma? I think so so I'm leaving it in)
#endif
  pinMode(SWITCH, INPUT);   
  digitalWrite(SWITCH, HIGH); //this statement define the pullup resistor
  
  strip.begin(); //this initializes the strip
  strip.show();  //this updates the LED instructions to a starting position of "off"
  //putting procedures under strip.show() has it run through these procedures in order, not in respond to switch presses.
  
  
//  The actual definition of colorwipe as a helper function is further down, after the first void loop which 
//  is the one the board will actually run.
//  The following are some other procedures we could add to the switch
//  colorWipe(Color(0, 0, 255), 50);
//  rainbow(20);
//  rainbowCycle(20);
} 
// colorwipe fills the dots one after the other with said color
// putting procedures in the loop makes them run over and over again in sequence, not in response to the switch. 
// put procedures after the setting == 2 statement to line them up available to the switch presses. 

void loop(){
if (SETTING == 0){ 
    colorWipe(Color(25, 25, 112), 50); //this is a midnight blue colorWipe
  
  }
  if (digitalRead(SWITCH)==LOW){  
 //   delay(DEBOUNCE);
    SETTING = (SETTING+1) % 4;
  }
  if (SETTING == 1){
     colorWipe(Color(139, 0, 0), 50); //this is a maroon colorWipe
  }
  if (SETTING == 2){
    colorWipe(Color(99, 1, 3), 50); //setting starting colorwipe on a rosy red 
  } 
  if (SETTING == 3){
    colorWipe(Color(0, 255, 0), 50); //this is an electric green
  }
}

// SEE https://github.com/adafruit/Adafruit-WS2801-Library/blob/master/examples/strandtest/strandtest.pde for 
// original strand test code for the source of these procedures and helper functions.
// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain

void colorWipe(uint32_t c, uint8_t wait) {   //i think this is where colorWipe is defined?
 
  int i;   
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}


void rainbowCycle(uint8_t wait) {
  int i, j;
  
  for (j=0; j < 256; j++) {     // 5 cycles of all 25 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}
/* Helper functions */

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
