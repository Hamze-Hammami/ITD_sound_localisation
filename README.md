# Sound Localization Robot

## Overview

This repository contains the code and documentation for a **sound localization robot** that estimates the azimuth (bearing) of a sound source and navigates toward it. The system is based on **Interaural Time Difference (ITD)** calculations and is designed to work with minimal hardware requirements.

## Project Goal

The goal of this project is to develop an efficient and cost-effective **sound localization algorithm** that can be implemented on different robotic platforms using only two microphones.

## How It Works

1. **Listening Mode**: The robot waits for a sound signal and determines its general direction based on which microphone receives the sound first.
2. **Locating Mode**: Uses the ITD formula to estimate the precise angle of the sound source.
3. **Moving Mode**: The robot moves towards the estimated location, stopping when the ultrasonic sensor detects an obstacle.

## Hardware

- **EMoRo 2560 Board**
- **KY-038 Sound Sensors (x2)**
- **Ultrasonic Sensor (HC-SR04)**
- **LCD Display**
- **Servo Motors for movement**



## Software

- **Arduino IDE**
- **C/C++**


## Mathematical Model

The robot determines the **azimuth angle (θ)** of a sound source using **Interaural Time Difference (ITD)**. Woodworth’s ITD formula is given as:

\[
ITD = \frac{r (\theta + \sin \theta)}{c}
\]

where:
- \( ITD \) = Interaural Time Difference (time difference between sound reaching the left and right microphones)
- \( r \) = Radius between microphones (in meters)
- \( c \) = Speed of sound (343 m/s)
- \( \theta \) = Bearing (azimuth angle)

To calculate \( \theta \):

\[
\theta + \sin \theta = \frac{ITD \times c}{r}
\]

By solving this equation, we estimate the direction of the sound source. The robot then turns toward this direction and refines its movement using additional ITD calculations.

### Example:

If:
- \( r = 0.057 \) m (average human head radius)
- \( c = 343 \) m/s
- \( ITD = 0.1485 \) s

Then,

\[
\theta + \sin \theta = \frac{0.1485 \times 343}{0.057}
\]

\[
\theta + \sin \theta \approx 89
\]

This suggests that the source is located at approximately **89°** relative to the robot.