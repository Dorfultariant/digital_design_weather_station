// Pin constants:
const int LIGHT_LED_LOW = 2;
const int LIGHT_LED_MED = 3;
const int LIGHT_LED_HIGH = 4;
const int LIGHT_SENSOR = A0;

const int GAS_LED_LOW = 5;
const int GAS_LED_MED = 6;
const int GAS_LED_HIGH = 7;
const int GAS_SENSOR = A2;

const int TEMP_LED_LOW = 8;
const int TEMP_LED_MED = 9;
const int TEMP_LED_HIGH = 10;
const int TEMP_SENSOR = A1;

void setup() {
  // Set pin modes:
  pinMode(LIGHT_LED_LOW, OUTPUT);
  pinMode(LIGHT_LED_MED, OUTPUT);
  pinMode(LIGHT_LED_HIGH, OUTPUT);
  pinMode(TEMP_LED_LOW, OUTPUT);
  pinMode(TEMP_LED_MED, OUTPUT);
  pinMode(TEMP_LED_HIGH, OUTPUT);
  pinMode(GAS_LED_LOW, OUTPUT);
  pinMode(GAS_LED_MED, OUTPUT);
  pinMode(GAS_LED_HIGH, OUTPUT);
}

void set_led_state(int state, int p_low, int p_med, int p_high) {
  digitalWrite(p_low, state >= 1 ? 1 : 0);
  digitalWrite(p_med, state >= 2 ? 1 : 0);
  digitalWrite(p_high, state >= 3 ? 1 : 0);
}

/*
 * Logic,(which can arguably be seen easily from the if else)
 * if val <= low_thresh   == state = 0 = All led off
 * if low < val < med     == state = 1 = LOW led ON
 * if med < val < high    == state = 2 = LOW; MED leds ON
 * if val > high          == state = 3 = All leds ON
 */
int normalize_range_to_state(int val, int low_thresh, int med_thresh,
                             int high_thresh) {
  if (val < low_thresh)
    return 0;
  else if (low_thresh <= val && val < med_thresh)
    return 1;
  else if (med_thresh <= val && val < high_thresh)
    return 2;
  else if (high_thresh <= val)
    return 3;
}

// Ranges are based on empirical testing
void check_light_sensor() {
  int light_val = analogRead(LIGHT_SENSOR);
  int state = normalize_range_to_state(light_val, 900, 940, 960);
  set_led_state(state, LIGHT_LED_LOW, LIGHT_LED_MED, LIGHT_LED_HIGH);
}

// Ranges are based on empirical testing
void check_gas_sensor() {
  int gas_val = analogRead(GAS_SENSOR);
  int state = normalize_range_to_state(gas_val, 500, 550, 700);
  set_led_state(state, GAS_LED_LOW, GAS_LED_MED, GAS_LED_HIGH);
}

void check_temp_sensor() {
  int tmp_sensorvalue = analogRead(TEMP_SENSOR); // Range 0 - 1023
  // to get scaled value for voltage, Analog Input MAX (5V)
  // Alternative way with straight 3.3V # 3.3 / 5 / 1023 = 675.18
  float voltage = tmp_sensorvalue * 5 / 1023.0;
  // approximate temperature value, -40 temp low bound
  // -100 is voltage low bound and 0.1 is to get integer Celsius
  int cel = -40 + (voltage * 1000 - 100) * 0.1;
  int state = normalize_range_to_state(cel, -10, 10, 30);
  set_led_state(state, TEMP_LED_LOW, TEMP_LED_MED, TEMP_LED_HIGH);
}

void loop() {
  // Update Sensor values
  check_temp_sensor();
  check_light_sensor();
  check_gas_sensor();
  delay(200); // Wait for 200 millisecond(s)
}
