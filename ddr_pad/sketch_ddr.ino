/* FSR testing sketch.

Connect one end of FSR to 5V, the other end to Analog 0.
Then connect one end of a 10K resistor from d 0 to ground
Connect LED from pin 11 through a resistor to ground

For more information see www.ladyada.net/learn/sensors/fsr.html */
// #include <Keyboard.h>
#include <FastLED.h>
#include <Keyboard.h>
// int fsrAnalogPin = 0;     // FSR is connected to analog 0
// int LEDpin = 11;          // connect Red LED to pin 11 (PWM pin)
// int fsrReading;           // the analog reading from the FSR resistor divider
// bool keyPressed = false;  // Track key state
// const int pressThreshold = 500;
// const int releaseBuffer = 20;
// int buttonPin = 9;
// bool buttonDown = false;
// CRGB PINK = CRGB(217, 85, 1);
// CRGB BLUE = CRGB(3, 252, 152);
// CRGB PINK = CRGB(217, 85, 1);
// CRGB BLUE = CRGB(3, 252, 152);

#include "DDRPad.h";

#define LED_PIN 6          // Pin where your WS2811 data line is connected
#define NUM_LEDS_PER_PAD 9 // Total number of LEDs in the strip

#define BRIGHTNESS 200
#define COLOR_ORDER GRB // WS2811s typically use GRB

CRGB leds[NUM_LEDS_PER_PAD * 4];

DDRPad<NUM_LEDS_PER_PAD, LED_PIN> pad(leds);

void setup(void)
{
  pinMode(3, INPUT_PULLUP);
  Serial.begin(9600);
  pad.Begin();
  Keyboard.begin();

  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
}

void loop(void)
{
  int buttonPress1 = analogRead(A0);
  int buttonPress2 = analogRead(A1);
  int buttonPress3 = analogRead(A2);
  int buttonPress4 = analogRead(A3);

  pad.right.loop(buttonPress1);
  pad.down.loop(buttonPress2);
  pad.left.loop(buttonPress3);
  pad.up.loop(buttonPress4);
  // Serial.println("Reading All:");
  // Serial.println("=========");
  // Serial.println(buttonPress1);
  // Serial.println(buttonPress2);
  // Serial.println(buttonPress3);
  // Serial.println(buttonPress4);
  // Serial.println("=========");
  // Serial.println("");

  // delay(50);
}

// void bootSequence(void) {
//   for (int i = 0; i < NUM_LEDS; i++) {
//     leds[i] = BLUE;
//   }
//   FastLED.setBrightness(0);
//   FastLED.show();

//   for (int i = 0; i < 150; i++) {
//     FastLED.setBrightness(i);
//     FastLED.show();
//     delay(5);
//   }

//   for (int i = 0; i < 150; i++) {
//     FastLED.setBrightness(149 - i);
//     FastLED.show();
//     delay(1);
//   }

//   delay(5000);
// for (int i = 0; i <= 4; i++) {
//   for (int j = i * 9; j < i * 9 + 9; j++) {
//     leds[j] = BLUE;  //i % 0 == 0 ? BLUE : PINK;
//   }
//   FastLED.show();
//   delay(1000);
// }

// for (uint8_t fade = 0; fade <= 255; fade++) {
//   for (int i = 0; i <= 4; i++) {
//     for (int j = i * 9; j < i * 9 + 9; j++) {
//       leds[j] = blend(leds[j], CRGB::Black, fade);
//     }
//   }

//   FastLED.show();
//   delay(20);
// }

// for (uint8_t fade2 = 0; fade2 <= 255; fade2++) {
//   for (int i = 0; i < 4; i++) {
//     for (int j = i * 9; j < i * 9 + 9; j++) {
//       leds[j] = CRGB::Black;
//       leds[j] = blend(leds[j], BLUE, fade2);
//     }
//   }

//   FastLED.show();
//   delay(20);
// }
// FastLED.setBrightness(150);

// FastLED.clear();
// FastLED.show();
// }