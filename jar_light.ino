// ATtiny85 mini jar light
// components: ATtiny85, tilt switch, LED, Arduino (for programming ATtiny85)

#define TILT_PIN 4
#define LED_PIN 3

enum class State {IDLE, WAIT_RISE, WAIT_FALL, TOGGLE};
State system_state = State::IDLE;

bool    tilt_state          = false;
bool    tilt_state_default  = false;  // default tilt position (at start-up)
bool    light_switch        = false;
uint8_t threshold           = 10;     // tap sensitivity, adjustable
uint8_t count               = 0;

void setup() {
  Serial.begin(9600);
  pinMode(TILT_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  tilt_state_default = digitalRead(TILT_PIN);
}

void loop() {
  if (system_state == State::WAIT_RISE) {
    tilt_state = digitalRead(TILT_PIN);
    count = (count < threshold)?(count + 1):threshold;
    if (tilt_state ^ tilt_state_default) {
      if (count >= threshold) {
        system_state = State::WAIT_FALL;
      }
      count = 0;
    }
    delay(1);
  } else if (system_state == State::WAIT_FALL) {
    tilt_state = digitalRead(TILT_PIN);
    count = (count < threshold)?(count + 1):threshold;
    if (tilt_state ^ !tilt_state_default) {
      if (count >= threshold) {
        system_state = State::TOGGLE;
      }
      count = 0;
    }
    delay(1);
  } else if (system_state == State::TOGGLE) {
    light_switch = !light_switch;
    digitalWrite(LED_PIN, light_switch);
    system_state = State::WAIT_RISE;
  } else {
    system_state = State::WAIT_RISE;
  }
}
