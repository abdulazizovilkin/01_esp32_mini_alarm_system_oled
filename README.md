# VoltShell Lab 01 — ESP32 Mini Alarm System with OLED Display

This is my first hands-on ESP32 embedded systems project.

The system reads an analog value from a potentiometer, shows the level on an OLED display, uses LEDs for visual feedback, triggers a buzzer when the danger threshold is crossed, and allows the system to be armed or disarmed using a push button.

## Features

- ESP32-based mini alarm system
- Potentiometer as analog input
- 2 LED indicators
- Buzzer alarm
- Push button for ARM / DISARM control
- OLED display for live system status
- Basic state logic using `millis()`

## Hardware Used

- ESP32 development board
- Breadboard
- 2 LEDs
- 2 × 220Ω resistors
- Potentiometer
- Push button
- Buzzer
- 0.96 OLED I2C display
- Jumper wires

## Pin Mapping

| Component | ESP32 Pin |
|---|---|
| Armed LED | GPIO23 |
| Danger LED | GPIO19 |
| Buzzer | GPIO25 |
| Button | GPIO27 |
| Potentiometer | GPIO34 |
| OLED SDA | GPIO21 |
| OLED SCL | GPIO22 |

## What I Learned

- GPIO output control
- Digital input with `INPUT_PULLUP`
- Analog input using ADC
- I2C OLED display communication
- Button debounce logic
- Buzzer control
- Basic alarm state management
- Breadboard wiring and debugging

## Current Limitations

- Potentiometer is used as a test input instead of a real sensor
- Wiring is still on breadboard
- Alarm threshold is hardcoded
- No Wi-Fi or logging yet

## Next Steps

- Improve wiring
- Add a real sensor
- Add Wi-Fi dashboard
- Add event logging
- Write a basic security analysis
