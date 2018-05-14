//Power input
#define MAX_TURBINE_VOLTAGE 106

//LEDs
#define N_POWER_LEDS 9
#define FIRST_POWER_LED_PIN 22

#define HOUSE_LED_PIN 2
#define MAX_PWM_VALUE 255

double getPowerLevel() {
  return ((double)analogRead(0))/MAX_TURBINE_VOLTAGE;
}

void setPowerLEDs(double powerLevel) {
  int ledsToLight = powerLevel * N_POWER_LEDS + 0.5;
  int pwmLevel = powerLevel * MAX_PWM_VALUE;

  analogWrite(HOUSE_LED_PIN, pwmLevel);
  for (int pin = FIRST_POWER_LED_PIN; pin < FIRST_POWER_LED_PIN+N_POWER_LEDS; ++pin) {
    digitalWrite(pin, (pin-FIRST_POWER_LED_PIN<ledsToLight)?(HIGH):(LOW));
  }
}

void setup() {
  pinMode(HOUSE_LED_PIN, OUTPUT);
  for (int pin = FIRST_POWER_LED_PIN; pin < FIRST_POWER_LED_PIN+N_POWER_LEDS; ++pin) {
    pinMode(pin, OUTPUT);
  }
  pinMode(0, INPUT); //Turbine analog input

  double ip = 0;
  while (ip<=1) {
    setPowerLEDs(ip);
    ip += 0.01;
    delay(30);
  }
  delay(1000);
}

void loop() {
  setPowerLEDs(getPowerLevel());
  delay(20);
}
