# ESP32 Timer Game

This project is a timer-based game created using the **ESP32 microcontroller** and **p5.js**. The objective is simple: try to set the timer as close to **10 seconds** as possible! The ESP32 handles the actual timing, sending updates via Bluetooth to a p5.js interface, which displays the timer and provides feedback on accuracy.

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Acknowledgments](#acknowledgments)

## Overview
The ESP32 Timer Game was developed as a homework assignment to demonstrate basic Bluetooth communication between an ESP32 microcontroller and a p5.js interface. Players attempt to stop the timer as close to 10 seconds as possible. Based on the accuracy, the game provides a **win/loss message**.

## Features
- **Real-time Bluetooth Updates**: The ESP32 communicates timing updates to the p5.js application.
- **Accuracy-Based Feedback**: A win/loss message is displayed based on how close the timer was to 10 seconds.
- **Simple p5.js Interface**: Visual feedback and countdown are provided in a clean, minimalist interface.

## Requirements
- **Hardware**: ESP32 microcontroller
- **Software**:
  - [p5.js](https://p5js.org/) (for the game interface)
  - Arduino IDE or another environment to program the ESP32
  - Bluetooth-enabled device for connection

## Installation
1. **Setup the ESP32**: Program the ESP32 to keep time and send Bluetooth updates.
   - Ensure the ESP32 is set to send timing information at regular intervals.
   - The Bluetooth communication should be configured to send updates to p5.js.
2. **Setup the p5.js Sketch**:
   - Clone this repository:
     ```bash
     git clone https://github.com/your-username/ESP32-Timer-Game.git
     ```
   - Open the `index.html` file in your p5.js sketch to run the game.
   - Connect to the ESP32 over Bluetooth when prompted.

## Usage
1. **Start the Game**: Begin timing by pressing the designated start button.
2. **Stop the Timer**: Attempt to stop the timer at exactly 10 seconds.
3. **Receive Feedback**: The p5.js interface will display a message based on your accuracy.

## Acknowledgments
This project was developed as part of a class assignment to practice **Bluetooth communication** and **timing accuracy** with the ESP32 and p5.js. Special thanks to instructor for his support and guidance.

---

Enjoy the game and good luck hitting exactly 10 seconds!

