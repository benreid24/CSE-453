//Power input
#define MAX_TURBINE_VOLTAGE 1024 //Change to actual max
#define N_TURBINES 3
#define FIRST_TURBINE_SWITCH_PIN 10

//LEDs
#define N_POWER_LEDS 3
#define FIRST_POWER_LED_PIN 22

#define HOUSE_LED_PIN 2
#define MAX_PWM_VALUE 255

#define FIRST_TURBINE_LED_PIN 50
//Each turbine uses 2 pins (1 for green, 1 for red)
//green pin = FIRST_TURBINE_LED_PIN + (turbine#)*2
//red pin = FIRST_TURBINE_LED_PIN + (turbine#)*2 + 1

int isTurbineActive(int turbine) {
  int pin = FIRST_TURBINE_SWITCH_PIN+turbine;
  return (analogRead(pin) >= 512)?(1):(0);
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
  int ledsToLight = powerLevel * N_POWER_LEDS + 0.5;
  int pwmLevel = (powerLevel*powerLevel) * MAX_PWM_VALUE;

  analogWrite(HOUSE_LED_PIN, pwmLevel);
  for (int pin = FIRST_POWER_LED_PIN; pin < FIRST_POWER_LED_PIN+N_POWER_LEDS; ++pin) {
    digitalWrite(pin, (pin-FIRST_POWER_LED_PIN<ledsToLight)?(HIGH):(LOW));
  }
}

void setTurbineLEDs() {
  for (int i = 0; i<N_TURBINES; ++i) {
    int greenPin = FIRST_TURBINE_LED_PIN + i*2;
    int redPin = greenPin+1;
    if (isTurbineActive(i)) {
      digitalWrite(greenPin, HIGH);
      digitalWrite(redPin, LOW);
    }
    else {
      digitalWrite(greenPin, LOW);
      digitalWrite(redPin, HIGH);
    }
  }
}

double pl = 0, diff = 0.001;

void setup() {
  Serial.begin(115200);
  
  pinMode(HOUSE_LED_PIN, OUTPUT);
  for (int pin = FIRST_POWER_LED_PIN; pin < FIRST_POWER_LED_PIN+N_POWER_LEDS; ++pin) {
    pinMode(pin, OUTPUT);
  }
  for (int pin = FIRST_TURBINE_SWITCH_PIN; pin < FIRST_TURBINE_SWITCH_PIN + N_TURBINES; ++pin) {
    pinMode(pin, INPUT);
  }
  for (int pin = FIRST_TURBINE_LED_PIN; pin<FIRST_TURBINE_LED_PIN + N_TURBINES*2; ++pin) {
    pinMode(pin, OUTPUT);
  }

  double ip = 0;
  while (ip<=1) {
    setPowerLEDs(ip);
    ip += 0.01;
    delay(30);
  }
  delay(1000);
}

void loop() {
  pl += diff;
  if (pl>=1)
    diff = diff * -1;
  if (pl < 0)
    diff = diff * -1;
    
  setPowerLEDs(pl);
  setTurbineLEDs();
  delay(20);
}
