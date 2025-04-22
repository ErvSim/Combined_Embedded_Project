# Combined Embedded Project
## Project Overview
This project showcases a Wi-Fi enabled system using the Raspberry Pi Pico W that integrates three modules: an OLED display, an ultrasonic distance sensor (USS), and a 16x2 I2C LCD. A web server hosted on the Pico W allows the user to remotely start or stop the system by pressing “On” or “Off” from any browser connected to the same network. When activated, the OLED displays a status message, the ultrasonic sensor measures distance (in inches) in real time, and the LCD displays those distance readings. When the system is turned off, the OLED displays a sleep message, and the LCD backlight is disabled.

This project is built with modular code architecture and demonstrates clean peripheral integration using I2C and GPIO interrupts, along with non-blocking TCP server logic.

---

## Features
Wi-Fi Web Server Control
Host a lightweight web server directly on the Pico W to toggle system behavior via two buttons: "Turn On" and "Turn Off".

Real-Time Distance Measurement
Continuously reads distance from an ultrasonic sensor using GPIO interrupts for precise timing.

OLED Status Display
Displays current system status messages such as “Online and Active” or “Going to Sleep, Goodnight”

LCD Distance Display
A 16x2 I2C LCD module displays the current distance in inches on the second line while preserving a static label on the first line.

Modular Code Design
All major components (Wi-Fi, OLED, LCD, USS) are organized into separate .c/.h files for clarity, reusability, and scalability.

Offline-Compatible Variant
A fully functional version of the project is available without Wi-Fi support, allowing demonstrations in environments with no internet access.

----

## Hardware Used

Raspberry Pi Pico W
The main microcontroller responsible for running the code, managing I2C peripherals, GPIO interrupts, and hosting the web server.

Ultrasonic Sensor (HC-SR04)
Measures distance using echo timing on two GPIOs (Trigger and Echo). Connected to GPIO 0 (Trigger) and GPIO 1 (Echo).

16x2 I2C LCD Module (PCF8574 backpack)
Displays static and dynamic messages. Uses I2C0 on GPIO 16 (SDA) and GPIO 17 (SCL).

0.96" I2C OLED Display (SSD1306, 128x64)
Displays status messages using a full 8x8 ASCII font table. Also connected via I2C0 using GPIO 16 (SDA) and GPIO 17 (SCL).

USB Cable and Serial Terminal
Used for debugging and serial output via CoolTerm or the VSCode serial monitor.

-----

## How it Works


----


## Folder Structure

/src        → Source files for each module (LCD, OLED, USS, Wi-Fi)
/include    → Header files for each module
Rest of files (Combined_project.c, lwipopts.h (automated code when enabling wifi), and CMakeList) are in main files
