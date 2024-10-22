# Autonomous Hovercraft

This repository contains the code for an Arduino-controlled hovercraft that uses Infrared (IR) and Ultrasonic (US) sensors for navigation and obstacle avoidance.

## About The Project

The Autonomous Hovercraft project is designed to give hands-on experience with the Arduino IDE and microcontroller features. It utilizes the ATmega328 microcontroller, commonly found in Arduino boards, along with Infrared and Ultrasonic rangefinders to measure distance and control an LED’s brightness. The project outputs distance and Analog-to-Digital Conversion (ADC) readings to a screen via UART. Additionally, it demonstrates real-time sensor feedback, helping users understand how to interpret sensor data for obstacle detection.

### Key Features:

- **Sensor Integration**: Uses IR and US sensors to detect obstacles and control hovercraft behavior.
- **Distance Measurement**: Outputs the distance between sensors and obstacles, and adjusts LED brightness based on proximity.
- **Serial Communication**: Transmits real-time sensor data and ADC values through UART for monitoring.

## Getting Started

To get a local copy up and running, follow these simple steps.

### Prerequisites

Before you begin, ensure you have the following:

- Arduino IDE installed on your system
- Hardware:
  - Ultrasonic rangefinder (HC-SR04)
  - IR analog rangefinder (GP2Y0A21)
  - Arduino microcontroller
  - Voltmeter (Digital Multimeter - DMM)

### Installation

1. **Clone the repository**:

   ```bash
   git clone https://github.com/hlbels/AutonomousHover.git
   ```

2. **Open the project**:
   - Launch the Arduino IDE, and open the project from the cloned directory.
3. **Upload the code**:
   - Connect your Arduino board, verify the code, and upload it to the board.

### Wiring Instructions

- Connect the **Ultrasonic sensor** to the following pins:
  - Trigger pin to digital pin X
  - Echo pin to digital pin Y
- Connect the **IR analog sensor** to any available ADC channel on the Arduino.

Refer to the wiring diagram in the documentation folder for detailed connections.

## Usage

1. **Sensor Setup**: Connect the IR and Ultrasonic sensors to the board as described above.
2. **Run Trials**: Observe the hovercraft's behavior and fill out the performance table for your report based on the trial outcomes.
3. **Monitor Outputs**: View the distance readings and ADC output on the serial monitor.
4. **Observe LED Behavior**: Check how the LED’s brightness adjusts in real time based on the distance between the sensors and obstacles.
