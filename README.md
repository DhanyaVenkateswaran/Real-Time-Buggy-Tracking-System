# Real-Time-Buggy-Tracking-System

## Abstract

The GPS-based real-time buggy tracking system aims to provide an efficient and reliable solution for tracking buggies or other small vehicles in real-time. The system utilizes the NodeMCU microcontroller and GPS technology to track the position of the buggy and transmit the location data to a central server for processing. To ensure real-time tracking, the microcontroller connects to the internet via Wi-Fi or a cellular network, enabling continuous data transmission to the server. The server, equipped with appropriate software, receives the GPS data and processes it to visualize the buggy's location on a digital map or display. Users can access the tracking system through a web or mobile application, allowing them to monitor the buggy's movement in real-time. The application provides features such as tracking history, geofencing, and alerts for unauthorized or abnormal movements.

## Setup

1. Hardware components needed include:
- NodeMCU 
- NEO-6M GPS Module 
- LCD Display 16x2
- LCD 12C Interfacing module

2. Make the following circuit connections:
  
LCD I2C to NodeMCU
GND - GND
VCC - Vin
SDA - D2
SCL - D1

Neo-6M to NodeMCU
VCC - 3V3
RX  - D6
TX  - D7
GND - GND

3. Download Arduino IDE from https://www.arduino.cc/en/software 
4. Open the given the .ino file in Arduino and set up the IDE for nodemcu using the board manager and download mentioned libraries using the library manager
- ArduinoJSON version = 6.21.2
- LiquidCrsytal by Adafruit
- LiquidCrystal I2C by frank de brabander 
- TinyGPS by Mikel Hart
5. After successfully doing the above, change the myssid and mypass to your Network SSID and password respectively.
6. Connect the nodemcu microcontroller to the system 
7. Verify and upload the code
8. Open the serial monitor at baud rate 115200 to see the results
