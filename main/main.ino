//Power input
#define MAX_TURBINE_VOLTAGE 1024 //Change to actual max
#define N_TURBINES 3
#define FIRST_TURBINE_SWITCH_PIN 52 //?

//LED towers
#define N_POWER_LEDS 3
#define FIRST_POWER_LED_PIN 22

//Town display brightness
#define HOUSE_LED_PIN 2
#define MAX_PWM_VALUE 255

int isTurbineActive(int turbine) {
  int pin = FIRST_TURBINE_SWITCH_PIN+turbine;
  return (digitalRead(pin) == HIGH)?(1):(0);
}

double getPowerLevel() {
  double powerLevel = 0;

  for (int pin = 0; pin<N_TURBINES; ++pin) {
    double val = (double)analogRead(pin);
    powerLevel += (val/MAX_TURBINE_VOLTAGE)/N_TURBINES * isTurbineActive(pin);
  }

  return powerLevel;
}

void setPowerLEDs(double powerLevel) {
  int ledsToLight = powerLevel * N_POWER_LEDS;
  int pwmLevel = powerLevel * MAX_PWM_VALUE;

  analogWrite(HOUSE_LED_PIN, pwmLevel);
  for (int pin = FIRST_POWER_LED_PIN; pin < FIRST_POWER_LED_PIN+N_POWER_LEDS; ++pin) {
    digitalWrite(pin, (pin<=ledsToLight)?(HIGH):(LOW));
  }
}

void setup() {
  pinMode(HOUSE_LED_PIN, OUTPUT);
  for (int pin = FIRST_POWER_LED_PIN; pin < FIRST_POWER_LED_PIN+N_POWER_LEDS; ++pin) {
    pinMode(pin, OUTPUT);
  }
  for (int pin = FIRST_TURBINE_SWITCH_PIN; pin < FIRST_TURBINE_SWITCH_PIN + N_TURBINES; ++pin) {
    pinMode(pin, INPUT);
  }
}

void loop() {
  setPowerLEDs(getPowerLevel());
}
