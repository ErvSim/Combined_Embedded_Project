# Combined Embedded Project – In Progress
This project will integrate multiple embedded systems peripherals using the Raspberry Pi Pico W. The final build will feature:

- An ultrasonic distance sensor for real-time measurement

- An I2C 16x2 LCD to display the current distance

- An I2C OLED to show system status messages

- A Wi-Fi interface to control the system remotely via a browser (future scope)

- A physical slide switch to allow full offline operation

All components are modularized using clean .c and .h structures. The goal is to demonstrate multi-peripheral communication, modular design, and offline/online flexibility using embedded C and the I2C protocol.
