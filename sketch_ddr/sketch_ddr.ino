/* FSR testing sketch. 

Connect one end of FSR to 5V, the other end to Analog 0.
Then connect one end of a 10K resistor from Analog 0 to ground
Connect LED from pin 11 through a resistor to ground 
 
For more information see www.ladyada.net/learn/sensors/fsr.html */

#include <Keyboard.h>

int fsrAnalogPin = 0;  // FSR is connected to analog 0
int LEDpin = 11;       // connect Red LED to pin 11 (PWM pin)
int fsrReading;        // the analog reading from the FSR resistor divider
int LEDbrightness;
bool keyPressed = false;  // Track key state
const int pressThreshold = 500; 
const int releaseBuffer = 20;

void setup(void) {
  Serial.begin(9600);  // We'll send debugging information via the Serial monitor
  Keyboard.begin();
}

void loop(void) {
  fsrReading = analogRead(fsrAnalogPin);
  Serial.print("Analog reading = ");
  Serial.println(fsrReading);

    if (fsrReading > pressThreshold && !keyPressed) {
        Keyboard.press('A');  // Change 'A' to your desired key
        keyPressed = true;
    } 
    else if (fsrReading < (pressThreshold - releaseBuffer) && keyPressed) {
        Keyboard.release('A');
        keyPressed = false;
    }
}