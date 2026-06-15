#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA 21
#define OLED_SCL 22

#define LED_ARMED_PIN 23
#define LED_DANGER_PIN 19
#define BUZZER_PIN 25
#define BUTTON_PIN 27
#define POT_PIN 34

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

bool systemArmed = false;
bool armingDelayActive = false;

unsigned long armingStartTime = 0;
unsigned long armingDelay = 3000;

bool lastButtonState = HIGH;
bool currentButtonState = HIGH;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

unsigned long lastBlinkTime = 0;
bool armedLedState = LOW;

unsigned long lastBuzzerTime = 0;
bool buzzerState = LOW;

unsigned long lastDisplayUpdate = 0;

int dangerThreshold = 2500;

void setup() {
  pinMode(LED_ARMED_PIN, OUTPUT);
  pinMode(LED_DANGER_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  digitalWrite(LED_ARMED_PIN, LOW);
  digitalWrite(LED_DANGER_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true) {
      // OLED tapilmasa burada qalacaq
    }
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("VoltShell Lab");
  display.setCursor(0, 15);
  display.println("Mini Alarm System");
  display.setCursor(0, 30);
  display.println("Starting...");
  display.display();

  delay(1500);
}

void loop() {
  int potValue = analogRead(POT_PIN);

  handleButton();
  handleArmingDelay();
  handleLeds(potValue);
  handleAlarm(potValue);
  updateDisplay(potValue);
}

void handleButton() {
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != currentButtonState) {
      currentButtonState = reading;

      if (currentButtonState == LOW) {
        if (systemArmed == false && armingDelayActive == false) {
          armingDelayActive = true;
          armingStartTime = millis();
        } else {
          systemArmed = false;
          armingDelayActive = false;
          digitalWrite(BUZZER_PIN, LOW);
          digitalWrite(LED_DANGER_PIN, LOW);
        }
      }
    }
  }

  lastButtonState = reading;
}

void handleArmingDelay() {
  if (armingDelayActive == true) {
    if (millis() - armingStartTime >= armingDelay) {
      systemArmed = true;
      armingDelayActive = false;
    }
  }
}

void handleLeds(int potValue) {
  if (armingDelayActive == true) {
    if (millis() - lastBlinkTime > 200) {
      lastBlinkTime = millis();
      armedLedState = !armedLedState;
      digitalWrite(LED_ARMED_PIN, armedLedState);
    }
  } else if (systemArmed == true) {
    digitalWrite(LED_ARMED_PIN, HIGH);
  } else {
    digitalWrite(LED_ARMED_PIN, LOW);
  }

  if (systemArmed == true && potValue > dangerThreshold) {
    digitalWrite(LED_DANGER_PIN, HIGH);
  } else {
    digitalWrite(LED_DANGER_PIN, LOW);
  }
}

void handleAlarm(int potValue) {
  if (systemArmed == true && potValue > dangerThreshold) {
    if (millis() - lastBuzzerTime > 150) {
      lastBuzzerTime = millis();
      buzzerState = !buzzerState;
      digitalWrite(BUZZER_PIN, buzzerState);
    }
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    buzzerState = LOW;
  }
}

void updateDisplay(int potValue) {
  if (millis() - lastDisplayUpdate < 300) {
    return;
  }

  lastDisplayUpdate = millis();

  int percent = map(potValue, 0, 4095, 0, 100);

  display.clearDisplay();
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("VoltShell Alarm");

  display.setCursor(0, 15);
  display.print("Level: ");
  display.print(percent);
  display.println("%");

  display.setCursor(0, 30);
  display.print("System: ");

  if (armingDelayActive == true) {
    display.println("ARMING...");
  } else if (systemArmed == true) {
    display.println("ARMED");
  } else {
    display.println("DISARMED");
  }

  display.setCursor(0, 45);
  display.print("Status: ");

  if (systemArmed == true && potValue > dangerThreshold) {
    display.println("DANGER");
  } else {
    display.println("NORMAL");
  }

  display.display();
}
