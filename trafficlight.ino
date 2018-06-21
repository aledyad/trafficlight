#include <MsTimer2.h>

const byte PIN_TF1_RED = 2;
const byte PIN_TF1_YELLOW = 3;
const byte PIN_TF1_GREEN = 4;
const byte PIN_TF2_RED = 5;
const byte PIN_TF2_YELLOW = 6;
const byte PIN_TF2_GREEN = 7;

// tl1 red tl2 green
const byte PIN_MODE_0 = 8;
// tl1 red+yellow tl2 yellow | tl1 yellow tl2 red+yellow
const byte PIN_MODE_1 = 9;
// tl1 green tl2 red
const byte PIN_MODE_2 = 10;
// auto mode
const byte PIN_MODE_3 = 11;
// night mode
const byte PIN_MODE_4 = 12;

// r - red
// y - yellow
// g - green
// a - red + yellow
// b - blink green
// c - blink yellow
const char *tf1MapByMode[] = {
  "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr",
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
  "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
  "gggggggggggggggggggggggggggggggggggg",
  "rrrrrrrrrrrrrraaagggggggbbbbbyyyrrrr",
  "cccccccccccccccccccccccccccccccccccc"};
const char *tf2MapByMode[] = {
  "gggggggggggggggggggggggggggggggggggg",
  "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
  "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr",
  "rrrrrrrrrrrrrraaagggggggbbbbbyyyrrrr",
  "cccccccccccccccccccccccccccccccccccc"};
// 012345678901234567890123456789012345

struct TrafficLight {
  byte stateIndex;
  byte modeIndex;
  char **mapByMode;
  byte pinRed;
  byte pinYellow;
  byte pinGreen;
};

TrafficLight tl1, tl2;
int blinkState = LOW;
int timerCounter = 0;

void draw_tl(TrafficLight* tl) {
  char *stateMap = tl->mapByMode[tl->modeIndex];
  char state = stateMap[tl->stateIndex];
  
  int red = LOW, yellow = LOW, green = LOW;
  switch(state) {
    case 'r':
      red = HIGH;
      break;
    case 'y':
      yellow = HIGH;
      break;
    case 'c':
      yellow = blinkState;
      break;
    case 'g':
      green = HIGH;
      break;
    case 'b':
      green = blinkState;
      break;
    case 'a':
      red = HIGH;
      yellow = HIGH;
      break;
  }

  digitalWrite(tl->pinRed, red);
  digitalWrite(tl->pinYellow, yellow);
  digitalWrite(tl->pinGreen, green);
}

void clear_tl(TrafficLight* tl) {
  digitalWrite(tl->pinRed, LOW);
  digitalWrite(tl->pinYellow, LOW);
  digitalWrite(tl->pinGreen, LOW);
}

void update_tl_state(TrafficLight* tl) {
  tl->stateIndex++;
  if (tl->stateIndex >= 36) {
    tl->stateIndex = 0;
  }
}

void update_tl_mode(TrafficLight* tl) {
  if (digitalRead(PIN_MODE_0) == LOW)
    tl->modeIndex = 0;
  if (digitalRead(PIN_MODE_1) == LOW) {
    if (tl->modeIndex == 0)
      tl->modeIndex = 1;
    if (tl->modeIndex == 3)
      tl->modeIndex = 2;
  }
  if (digitalRead(PIN_MODE_2) == LOW)
    tl->modeIndex = 3;
  if (digitalRead(PIN_MODE_3) == LOW)
    tl->modeIndex = 4;
  if (digitalRead(PIN_MODE_4) == LOW)
    tl->modeIndex = 5;
}

void update_blink_state() {
  if (blinkState == LOW) {
    blinkState = HIGH;
  } else {
    blinkState = LOW;
  }
}

void on_timer() {
  timerCounter++;

  // every 1000 ms
  if (timerCounter >= 100) {
    timerCounter = 0;
  }

  // every 500 ms
  if ((timerCounter % 50) == 0) {
    update_blink_state();

    update_tl_state(&tl1);
    update_tl_state(&tl2);
  }

  draw_tl(&tl1);
  draw_tl(&tl2);
  // duty ratio 5%
  delayMicroseconds(500);
  clear_tl(&tl1);
  clear_tl(&tl2);

  update_tl_mode(&tl1);
  update_tl_mode(&tl2);
}

void setup() {
  pinMode(PIN_TF1_RED, OUTPUT);
  pinMode(PIN_TF1_YELLOW, OUTPUT);
  pinMode(PIN_TF1_GREEN, OUTPUT);
  pinMode(PIN_TF2_RED, OUTPUT);
  pinMode(PIN_TF2_YELLOW, OUTPUT);
  pinMode(PIN_TF2_GREEN, OUTPUT);

  pinMode(PIN_MODE_0, INPUT_PULLUP);
  pinMode(PIN_MODE_1, INPUT_PULLUP);
  pinMode(PIN_MODE_2, INPUT_PULLUP);
  pinMode(PIN_MODE_3, INPUT_PULLUP);
  pinMode(PIN_MODE_4, INPUT_PULLUP);

  tl1.stateIndex = 0;
  tl1.modeIndex = 4;
  tl1.mapByMode = tf1MapByMode;
  tl1.pinRed = PIN_TF1_RED;
  tl1.pinYellow = PIN_TF1_YELLOW;
  tl1.pinGreen = PIN_TF1_GREEN;
  tl2.stateIndex = 18;
  tl2.modeIndex = 4;
  tl2.mapByMode = tf2MapByMode;
  tl2.pinRed = PIN_TF2_RED;
  tl2.pinYellow = PIN_TF2_YELLOW;
  tl2.pinGreen = PIN_TF2_GREEN;

  MsTimer2::set(10, on_timer);
  MsTimer2::start();
}

void loop() {
}
