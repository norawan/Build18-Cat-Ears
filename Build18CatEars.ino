/**
 * @file Build18CatEars.ino
 *
 * @brief Arduino sketch to control pan-tilt servo motors that move cat ears 
 *        according to user's brain activity
 *
 *        States
 *          - Relaxed: Ears drooped down
 *          - Neutral: Ears perked up
 *          - Excited: Ears wiggle around
 * */

#include <Servo.h>

// Macros
#define L_PAN_PIN 10
#define L_TILT_PIN 11
#define R_PAN_PIN 5
#define R_TILT_PIN 6

#define BAUDRATE 115200

// Neutral Position
#define L_T_NEUTRAL 60
#define R_T_NEUTRAL 60
#define L_P_NEUTRAL 90
#define R_P_NEUTRAL 90

#define MAX_COUNT 500

// Enum for speed
typedef enum { FAST, SLOW } speed_t;

// Create servo objects for each servo
Servo l_pan;
Servo l_tilt;
Servo r_pan;
Servo r_tilt;

// Variables
int l_t_pos;
int r_t_pos;
int l_p_pos;
int r_p_pos;

int state = 0; // Tracks the state. 0 = Relaxed, 1 = Neutral, 2 = Excited
int count = 0; // Counts the number of times no new status has been sent

void setup() {
  // Initialize pins
  l_pan.attach(L_PAN_PIN);
  l_tilt.attach(L_TILT_PIN);
  r_pan.attach(R_PAN_PIN);
  r_tilt.attach(R_TILT_PIN);

  // Initialize servo positions
  updatePosition(L_T_NEUTRAL, R_T_NEUTRAL, L_P_NEUTRAL, R_P_NEUTRAL);
  l_tilt.write(l_t_pos);
  r_tilt.write(r_t_pos);
  l_pan.write(l_p_pos);
  r_pan.write(r_p_pos);
  
  delay(1000);

  // Test movements on startup
  wiggleEars(3);
  delay(1000);
  droopEars();
  delay(1000);
  neutralEars();
  delay(1000);

  // Begin Serial Communication
  Serial.begin(BAUDRATE);
  Serial.print("Connecting to serial port...");
  Serial.print("\n");
}

void loop() {

  // Data received from serial port
  if (Serial.available()){
    Serial.print("Data received: ");
    String data = Serial.readStringUntil('\n');
    
    Serial.print(data);
    Serial.print("\n");

    float val = data.toInt();
    if (val == 2) {
      state = 2;
      wiggleEars(3);
    }
    else if (val ==1)  {
      state = 1;
      // Move ears to neutral position
      neutralEars();
    }
    else if (val == 0)  {
      state = 0;
      // Droop ears accoringly
      droopEars();
    }
    delay(100);
  }
  else {
    count += 1;
    if (count == MAX_COUNT) {
      count = 0;
      if (state == 2) {
        state = 2;
        wiggleEars(3);
      }
      else if (state ==1)  {
        // Move ears to neutral position
        // TODO: add a neutral wiggle
        neutralEars();
      }
      else if (state == 0)  {
        // Droop ears accoringly
        // TODO: add a drooped wiggle
        droopEars();
      }
      delay(100);
    }
  }
}

/**
 * @brief Sets the position variables to the input positions
 *
 * @param[in] lt Angle position for left tilt servo
 * @param[in] rt Angle position for right tilt servo
 * @param[in] lp Angle position for left pan servo
 * @param[in] rp Angle position for right pan servo
 */
void updatePosition(int lt, int rt, int lp, int rp) {
  l_t_pos = lt;
  r_t_pos = rt;
  l_p_pos = lp;
  r_p_pos = rp;
}

/**
 * @brief Get the dif to move servos by
 *
 * @param[in] start_pos Starting angle
 * @param[in] end_pos Ending angle
 * @return 1, -1, or 0 depending on the difference between start and end
 */
int dif(int start_pos, int end_pos) {
  if (start_pos < end_pos) {
    return 1;
  }
  if (start_pos == end_pos) {
    return 0;
  }
  return -1;
}

/**
 * @brief Sweeps servos to desired position
 *
 * @param[in] lt Angle position for left tilt servo
 * @param[in] rt Angle position for right tilt servo
 * @param[in] lp Angle position for left pan servo
 * @param[in] rp Angle position for right pan servo
 * @param[in] speed Speed of movement
 */
void sweepToPosition(int lt, int rt, int lp, int rp, speed_t speed) {
  int d_lp = dif(l_p_pos, lp);
  int d_lt = dif(l_t_pos, lt);
  int d_rp = dif(r_p_pos, rp);
  int d_rt = dif(r_t_pos, rt);
  while (l_t_pos!=lt || r_t_pos!=rt || l_p_pos!=lp || r_p_pos!=rp) {
    // Update positions
    if (l_t_pos!=lt) l_t_pos = l_t_pos + d_lt;
    if (r_t_pos!=rt) r_t_pos = r_t_pos + d_rt;
    if (l_p_pos!=lp) l_p_pos = l_p_pos + d_lp;
    if (r_p_pos!=rp) r_p_pos = r_p_pos + d_rp;

    l_tilt.write(l_t_pos);
    r_tilt.write(r_t_pos);
    l_pan.write(l_p_pos);
    r_pan.write(r_p_pos);

    if (speed == FAST) {
      delay(1);
    }
    else {
      delay(12);
    }
  }
  
}

/**
 * @brief Sets the servo positions to wiggle ears a certain amount of times
 *
 * @param[in] count Number of times to wiggle ears
 */
void wiggleEars(int count) {
  for (int i = 0; i < count; i++) {
    sweepToPosition(40, 40, 50, 130, FAST);
    delay(150);

    sweepToPosition(60, 60, 90, 90, FAST);
    delay(150);
  }
}

/**
 * @brief Sets the servo positions to droop ears down when relaxed
 */
void droopEars() {
  sweepToPosition(100, 100, 30, 150, SLOW);
  delay(100);
}

/**
 * @brief Sets the servo positions to the neutral position
 */
void neutralEars() {
  sweepToPosition(L_T_NEUTRAL, R_T_NEUTRAL, L_P_NEUTRAL, R_P_NEUTRAL, SLOW);
  delay(100);
}
