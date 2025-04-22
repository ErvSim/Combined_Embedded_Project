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

## How It Works
This project brings together four hardware components — an ultrasonic distance sensor, an I2C LCD, an I2C OLED, and Wi-Fi networking — to create a real-time, web-controlled measurement display system.

1. Ultrasonic Sensor (USS) – GPIO with Interrupts  
The ultrasonic sensor measures distance by sending a pulse and waiting for its echo. It’s connected via GPIO and uses interrupts on both rising and falling edges of the echo pin to calculate the time-of-flight. The distance is then computed using the speed of sound and converted into inches. All logic for this was implemented from scratch using GPIO interrupt handling on the Raspberry Pi Pico W.

2. I2C LCD – Real-Time Distance Display
The LCD uses a PCF8574 I/O expander for I2C communication and operates in 4-bit mode. The initialization sequence and data transfer were manually implemented, including command splitting and backlight control. It continuously displays the distance measured by the USS on two lines: a static label on the first and a dynamic numeric value on the second.

3. I2C OLED – Status Display
The OLED (SSD1306, 128x64) is used to display system status messages like “Online and Active” or “Going to Sleep, Goodnight”. It communicates over I2C on the same bus as the LCD. The initialization sequence, character rendering, and screen handling are based on an open-source SSD1306 driver. 

4. Wi-Fi Web Server – Remote Control
The Raspberry Pi Pico W connects to a local Wi-Fi network using cyw43_arch and the lwIP TCP/IP stack. A minimal TCP server is launched that hosts a webpage with two buttons: Start and Stop. When a user clicks either button, the server sets internal flags to trigger device behavior. For example:

Start: OLED says “Online and Active”, USS begins measuring, and LCD displays values.

Stop: OLED displays “Going to Sleep, Goodnight” for 5 seconds before clearing, and LCD is turned off.

----

## With_Wifi_Output
![image](https://github.com/user-attachments/assets/98c9bdc5-e10c-40f4-b4cc-88af184837c3)  

![image7](https://github.com/user-attachments/assets/03c61af0-c14f-4ebf-9cec-814d8fd1fe67)  
*GPIO On*
  
![image](https://github.com/user-attachments/assets/42a153a4-dae8-4e0d-bb8e-512d2fad1164)  

![image8](https://github.com/user-attachments/assets/297842d3-07cc-48fb-927a-0116fd06637c)
*GPIO Off*  
*Tried to take a picture of system status before it fully shutoff* 


----


## Folder Structure

/src        --> Source files for each module (LCD, OLED, USS, Wi-Fi)  
/include    --> Header files for each module  
Rest of files (Combined_project.c, lwipopts.h (automated code when enabling wifi), and CMakeList) are in main files

----

## Known Bugs
Double "Off" Click Required:  
When using the web interface, pressing the "Off" button once sometimes fails to stop the system. A second click is required for the shutdown sequence to trigger fully. Not sure where the error lies yet.

----

## Alternate Offline Version

An alternate offline version can also be created. To do this, all we have to do is change the main and add a component. First what was used was a Single Pole Double Throw switch. We connected middle pin to GPIO7, add another pin to ground, and we also activated internal pull up resistor. Then in main, we jsut have to change it in a way that checks to see which logic is active for GPIO7 and continue from there.
![image](https://github.com/user-attachments/assets/b4d09462-ab93-43eb-924f-a9aeb3ee061e)

![image9](https://github.com/user-attachments/assets/ab79a492-8440-4fa9-8c0f-b48929242293)
*System is on. Notice switch is on the right which actiavtes pullup resistor for GPIO7 and knows to be online*


![image10](https://github.com/user-attachments/assets/638c72d0-203d-4dd3-93e4-cca7ce7100a6)
*System is off. Notice switch is on the left now which grounds pullup resistor for GPIO7 and knows to go offline*

---

## Credits

- OLED SSD1306 driver base: Initialization and addressing adapted from the Raspberry Pi Pico SDK ssd1306_i2c example.

- Wi-Fi HTTP server: TCP and lwIP usage guided by Pico W wifi/tcp_server example.
