#include <MsTimer2.h>

const byte PIN_SELECT_MODE = 9;
const byte PIN_TF1_RED = 2;
const byte PIN_TF1_YELLOW = 3;
const byte PIN_TF1_GREEN = 4;
const byte PIN_TF2_RED = 5;
const byte PIN_TF2_YELLOW = 6;
const byte PIN_TF2_GREEN = 7;

// r - red
// y - yellow
// g - green
// a - red + yellow
// b - blink green
// c - blink yellow
const char tfMap[] = "rrrrrrrrrrrrrraaggggggggbbbbyyrrrr";
// -------------------0123456789012345678901234567890123456789

struct TrafficLight {
  byte stateIndex;
  byte pinRed;
  byte pinYellow;
  byte pinGreen;
};

TrafficLight tl1, tl2;
int blinkState = HIGH;

void update_tl_state(TrafficLight* tl) {
  if (blinkState == LOW) {
    tl->stateIndex++;
    if (tl->stateIndex >= sizeof(tfMap) - 1) {
      tl->stateIndex = 0;
    }
  }
    
  char state = tfMap[tl->stateIndex];
  if (digitalRead(PIN_SELECT_MODE) == LOW) {
    state = 'c';
  }
  
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

void update_traffic_lights() {
  if (blinkState == LOW) {
    blinkState = HIGH;
  } else {
    blinkState = LOW;
  }
  update_tl_state(&tl1);
  update_tl_state(&tl2);
}

void setup() {
  pinMode(PIN_SELECT_MODE, INPUT_PULLUP);

  pinMode(PIN_TF1_RED, OUTPUT);
  pinMode(PIN_TF1_YELLOW, OUTPUT);
  pinMode(PIN_TF1_GREEN, OUTPUT);
  pinMode(PIN_TF2_RED, OUTPUT);
  pinMode(PIN_TF2_YELLOW, OUTPUT);
  pinMode(PIN_TF2_GREEN, OUTPUT);
  
  tl1.stateIndex = 0;
  tl1.pinRed = PIN_TF1_RED;
  tl1.pinYellow = PIN_TF1_YELLOW;
  tl1.pinGreen = PIN_TF1_GREEN;
  tl2.stateIndex = 17;
  tl2.pinRed = PIN_TF2_RED;
  tl2.pinYellow = PIN_TF2_YELLOW;
  tl2.pinGreen = PIN_TF2_GREEN;

  MsTimer2::set(500, update_traffic_lights);
  MsTimer2::start();
}

void loop() {
}
