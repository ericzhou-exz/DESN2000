// A basic everyday NeoPixel strip test program.

// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)

#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN 5

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 80

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int analogPin = A0; // Solar panel connected to analog pin 0
int sensorValue = 0; //variable to store the read value
float RedLEDs = 1.2;  //voltage value for red LED activation
float GreenLEDs = 1.7; // voltage value for green LED activation

// LED Track pixel number values ------------------------------------------

int GreenTrack1START = 0; // Starting value of the first green LED (track 1)
int GreenTrack1END = 18; // Ending value of the first green LED (track 1)
int GreenTrack2START = 34; // Starting value of the first green LED (track 2)
int GreenTrack2END = 43; // Ending value of the first green LED (track 2)
int GreenTrack3START = 60; // Starting value of the first green LED (track 3)
int GreenTrack3END = 73; // Ending value of the first green LED (track 3)

int RedTrack1START = 18; // Starting value of the first red LED (track 1)
int RedTrack1END = 34; // Ending value of the first red LED (track 1)
int RedTrack2START = 43; // Starting value of the first red LED (track 2)
int RedTrack2END = 59; // Ending value of the first red LED (track 2)
int RedTrack3START = 73; // Starting value of the first red LED (track 3)
int RedTrack3END = 80; // Ending value of the first red LED (track 3)


// setup() function -- runs once at startup --------------------------------

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);       // For voltage reading of solar panel
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(255); // Set BRIGHTNESS to about 1/5 (max = 255)
}


// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {
  sensorValue = analogRead(analogPin);  //read the input pin
  float V_panel = sensorValue * (5.0/1024.0);
  Serial.print( "Voltage value : ");
  // Do a theater marquee effect in various colors...
 // theaterChase(strip.Color(0,   255,   0), 100); // Red, half brightness
  //strip.setBrightness(255);
  if(V_panel < RedLEDs ){
      Red_theaterChase(strip.Color(255,   0,   0),100); //RED TRACK (Solar)
  }else if ( V_panel  > GreenLEDs){
       Green_theaterChase(strip.Color(  0, 255,   0), 100); // GREEN Track (Grid)

  } else {
      theaterChaseBOTH(strip.Color(0 ,   0,   255),100); //Blue Track (Cloudy)
      
  }
  Serial.println(V_panel);
  float pin0 = 0;
  float pin1 = 0;
  float current = 0;
  pin0 = analogRead(A0);
  current = pin0 - pin1;
  float voltage = analogRead(A2);
  voltage = (voltage / 1024.0) * 5.0 * 6.0;
  current = (current / 1024.0) * 5.0;
  current = current / 75.0;
  int power = current * voltage * 500;
  //if (availableForWrite() > 0) {
    mySerial.write(power);
    Serial.println(power);
    Serial.println(current);
    Serial.println(voltage);
  //}
  //delay(100);
}



// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.

//Green LED track that represent solar power --------------
void Green_theaterChase(uint32_t color, int wait) {
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3
     for(int c=b; c < strip.numPixels (); c += 3) {
        if ( c > GreenTrack1START && c < GreenTrack1END ){
          strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
          }
        if ( c > GreenTrack2START && c < GreenTrack2END){
          strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
        }
        if ( c > GreenTrack3START && c < GreenTrack3END){
          strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
        }
        
        }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
    
}
void theaterChaseBOTH(uint32_t color, int wait) {
  //   strip.setBrightness(255);
      //REVERSE
    for(int d=strip.numPixels(); d>(strip.numPixels()-3); d--) {   //  'b' counts from 2 to 0...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'e' counts down from 'numPixels' to half of the strip in steps of 3...
    for(int e= d; e > strip.numPixels()/2-1 ; e -= 3) {
       strip.setPixelColor(strip.numPixels() - e, color);
       strip.setPixelColor(e, color); // Set pixel 'c' to value 'color'
       
      }
      
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
    
}


// Red LED tracks that represent electricity grid -----------------------
void Red_theaterChase(uint32_t color, int wait) {
    //strip.setBrightness(255);
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of LED strip
     for(int c=b; c < strip.numPixels (); c += 3) {
        if ( c > RedTrack1START && c < RedTrack1END ){
          strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
          }
        if ( c > RedTrack2START && c < RedTrack2END){
          strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
        }
        if ( c > RedTrack3START && c < RedTrack3END){
          strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
        }
        
        }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
    
}




    
