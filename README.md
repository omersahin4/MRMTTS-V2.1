# Rocket Motor Thrust Test Stand

> [!NOTE]  
> **Work in Progress:** Photos, detailed wiring schematics, and in-depth sensor specifications will be added soon.

An Arduino-based thrust measurement stand designed to capture, display, and record real-time thrust data for static motor burns.

---

## Features

- **Ready-to-Flash Firmware:** The included `.ino` file contains all necessary calibration, reading, and logging routines—no code modifications required to get started. Just connect your hardware and flash.
- **Live Monitoring:** Real-time thrust readings displayed via an attached LCD screen during the test.
- **Onboard Data Logging:** Automatically logs test data to a connected MicroSD card module for post-test analysis and thrust curve plotting (e.g., in Excel, MATLAB, or Python).
- **Flexible Mounting:** Designed to accommodate both vertical and horizontal motor test configurations.

---

## Mechanical Setup & 3D Printed Parts

The repository includes STL files designed for mounting the load cell:

- **Load Cell Mounts:** Print the provided STL files to securely fixture your load cell to the test base.
- **Orientation:** The mounting system supports testing motors in either a **vertical** or **horizontal** orientation depending on your test stand geometry and safety enclosure.

---

## Hardware Overview

To run the system, connect the following peripherals to your Arduino:

1. **Load Cell & Amplifier** (e.g., HX711)
2. **LCD Display** (e.g., 16x2 I2C) for live readout
3. **MicroSD Card Module** (SPI) for logging data to CSV

---

## Quick Start

1. **Print the Mounts:** 3D print the STL files in heat-resistant, rigid material (PETG, ABS, or sturdy PLA).
2. **Assemble the Hardware:** Fasten the load cell into the printed mounts and configure your motor mounting bracket (vertical or horizontal).
3. **Wire the Components:** Connect the load cell amplifier, LCD screen, and SD card module to your Arduino board.
4. **Flash the Firmware:** Open the `.ino` file in the Arduino IDE, ensure the required libraries are installed, and upload the sketch to the board.
5. **Run the Test:** Power on the system, confirm live values on the LCD, insert an SD card, and perform your burn.
6. **Plot Data:** Remove the SD card, open the generated log file on your computer, and plot your motor's thrust curve and total impulse.

Thanks to "RRacer" for STL files
