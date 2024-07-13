# Meaw Pet Feeder: Firmware

## Introduction

This project implements the firmware for an ESP32-based pet feeding device, where the user can schedule the times at which food portions need to be served. <br>
The device works together with a Mobile App, through which it can also be controlled. <br>
**Make sure to to also visit the App respository:** [Meaw-PetFeeder-App](https://github.com/MatheusCavini/MeawApp)

The project was created as part of the discipline "Embedded Systems" (PMR3402) for my Mechatronics Engineering course at Escola Politécnica da Universidade de São Paulo (USP).

## Demonstration

[Link for YouTube video](https://youtu.be/Ps0keci-ifc)


## Features
• Connect to mobile app using a device ID code; <br>
• Program up to 30 daytimes at which food is to be served; <br>
• Erase existing times; <br>
• Serve food at anytime by selecting "serve now";
• Get an alert when the food level in the reservoir is low.

## Technical Aspects
The firmware of the device runs a state machine, processing external (user inputs, app connection) and internal (reaching a saved time, low level) events to switch between different states.<br>
FreeRTOS is used to run simultaneous tasks such as the state machine, checking food level and
