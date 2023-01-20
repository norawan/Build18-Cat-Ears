/**
 * @file Build18CatEars.ino
 *
 * @brief Arduino sketch to control pan-tilt servo motors that move cat ears 
 *        according to user's
 */

#include <Servo.h>

// Macros
#define L_PAN_PIN 10
#define L_TILT_PIN 11
#define R_PAN_PIN 5
#define R_TILT_PIN 6

#define BAUDRATE 115200

#define ATTN_THRESHOLD 0.60

// Create servo objects for each servo
Servo l_pan;
Servo l_tilt;
Servo r_pan;
Servo r_tilt;

// Variables
int potpin = 0;
int val;

int l_pan_pos;
int l_tilt_pos;
int r_pan_pos;
int r_tilt_pos;

void setup() {
  // Initialize pins
  l_pan.attach(L_PAN_PIN);
  l_tilt.attach(L_TILT_PIN);
  r_pan.attach(R_PAN_PIN);
  r_tilt.attach(R_TILT_PIN);

  // Initialize servo positions
  l_tilt.write(90);
  l_pan.write(90);
  r_tilt.write(90);
  r_pan.write(90);

  l_tilt_pos = 90;
  l_pan_pos = 90;
  r_tilt_pos = 90;
  r_pan_pos = 90;

  // Begin Serial 
  Serial.begin(BAUDRATE);
  Serial.print("Connecting to serial port...");
  Serial.print("\n");
}

void loop() {

  // Data received from MuseLSL
  if (Serial.available()){
    Serial.print("Data received: ");
    String data = Serial.readStringUntil('\n');
    Serial.print(data);
    Serial.print("\n");

    float val = data.toFloat();
    if (val > ATTN_THRESHOLD) {
      wiggleEars(3);
    }
    delay(100);
  }
}

/**
 * @brief Sets the servo positions to wiggle ears a certain amount of times
 *
 * @param[in] count Number of times to wiggle ears
 */
void wiggleEars(int count) {
  for (int i = 0; i < count; i++) {
    l_tilt.write(100);
    l_pan.write(50);
    delay(100);
    l_tilt.write(80);
    l_pan.write(90);
    delay(100);
  }
}
