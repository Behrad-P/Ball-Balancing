# Ball-Balancing Project Using Image Processing and PID Controller

This project demonstrates a ball-balancing system utilizing image processing and a PID controller, implemented in C++ with OpenCV. The system employs two MG 995 servo motors, a RAPOO C200 webcam, and an Arduino Nano. The entire development was done in Visual Studio Code (VS Code), using CMake to manage the OpenCV library dependencies.

## Project Overview

The goal of this project is to balance a ball on a flat surface using two servo motors. The webcam captures the position of the ball, and a PID controller adjusts the servos to keep the ball centered.

## Components

- **MG 995 Servo Motors**: Two servo motors are used to control the tilt of the balancing surface.
- **RAPOO C200 Webcam**: This webcam provides real-time video feed for ball position detection.
- **Arduino Nano**: The microcontroller used to interface between the computer and the servo motors.
- **OpenCV**: An open-source computer vision library used for image processing.
- **C++**: The programming language used for implementing the project.
- **CMake**: A cross-platform tool used to manage the build process and link OpenCV libraries.

## Key Features

- **Image Processing**: The webcam captures real-time images of the ball, and OpenCV processes these images to determine the ball's position.
- **PID Controller**: A proportional-integral-derivative (PID) controller adjusts the servo motors to keep the ball balanced on the surface.
- **Real-Time Control**: The system continuously adjusts the servos based on the ball's position, achieving stable balance.
