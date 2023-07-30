# Real-Time-Buggy-Tracking-System

## Abstract

## Setup

1. Hardware components needed includes:
- NodeMCU 
- NEO-6M GPS Module 
- LCD Display 16x2
- LCD 12C Interfacing module

2. Make the following circuit connections: 
### LCD I2C to NodeMCU
GND - GND
VCC - Vin
SDA - D2
SCL - D1

### Neo-6M to NodeMCU
VCC - 3V3
RX  - D6
TX  - D7
GND - GND

3. Download Arduino IDE from https://www.arduino.cc/en/software 
4. Open the given the .ino file in Arduino and setup the IDE for nodemcu using the board manager and download mentioned libraries using the library manager
- ArduinoJSON version = 6.21.2
- LiquidCrsytal by Adafruit
- LiquidCrystal I2C by frank de brabander 
- TinyGPS by Mikel Hart
5. After successfully doing the above, change the myssid and mypass to your Network SSID and password respectively.
6. Connect the nodemcu microcontroller to the system 
7. Verify and upload the code
8. Open the serial monitor at baud rate 115200 to see the results
