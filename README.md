# Ball-Balancing Project Using Image Processing and PID Controller

This project demonstrates a ball-balancing system utilizing image processing and a PID controller, implemented in C++ with OpenCV. The system employs two MG 995 servo motors, a RAPOO C200 webcam, and an Arduino Nano. The entire development was done in Visual Studio Code (VS Code), using CMake to manage the OpenCV library dependencies.

## Project Overview

The goal of this project is to balance a ball on a flat surface using two servo motors. The webcam captures the position of the ball, and a PID controller adjusts the servos to keep the ball centered.

## Components

- **MG 995 Servo Motors**: Two servo motors are used to control the tilt of the balancing surface.
- **RAPOO C200 Webcam**: This webcam provides real-time video feed for ball position detection.
- **Arduino Nano**: The microcontroller used to interface between the computer and the servo motors.

## Software and Tools

- **OpenCV**: An open-source computer vision library used for image processing.
- **C++**: The programming language used for implementing the project.
- **CMake**: A cross-platform tool used to manage the build process and link OpenCV libraries.

## Key Features

- **Image Processing**: The webcam captures real-time images of the ball, and OpenCV processes these images to determine the ball's position.
- **PID Controller**: A proportional-integral-derivative (PID) controller adjusts the servo motors to keep the ball balanced on the surface.
- **Real-Time Control**: The system continuously adjusts the servos based on the ball's position, achieving stable balance.

## Installation and Setup

1. **Install OpenCV**:
Download and extract the precompiled OpenCV for MinGW from this GitHub repository (https://github.com/huihut/OpenCV-MinGW-Build).

2. **Install CMake**:
Download and install CMake from the official CMake website (https://cmake.org/).

3. **Install MinGW-w64**:
Download and install MinGW-w64 (POSIX) from this SourceForge link. (https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/6.3.0/threads-posix/)

## Project structure
![ps](https://github.com/user-attachments/assets/16645eb5-9b92-4063-bb74-7c3b07371299)![ps2](https://github.com/user-attachments/assets/fbf7a0c8-26b1-427a-b2ab-46cb95c791c2)


## Ball detection
![ball](https://github.com/user-attachments/assets/3d657f55-0166-4e96-9cff-9180044594ac)![ball_filter](https://github.com/user-attachments/assets/ee0f5538-a02a-493e-bf05-520c17d33831)





