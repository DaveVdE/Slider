# Slider

This is my ongoing project to create a three axis camera slider remotely controlled by a PS4 controller.

## Hardware

So far:
- Wemos D1 R32 "Arduino" form factor ESP32 board.
- Knockoff Arduino CNC Shield v3, modded so the VIN pin supplies motor voltage
- 3x TMC2208 stepper drivers
- 1x NEMA 17 45Ncm stepper motor
- 2x 28BYJ-28 5V geared stepper motor hacked to work in bipolar mode
- 1 PS4 Controller V2

## Software

Using PlatformIO, I was able to leverage the fine work done by the [PS4-esp32](https://github.com/aed3/PS4-esp32) project. I'm also using the well known AccelStepper library to help with the acceleration/deceleration stuff. 

So far I've all three axis working on my desk, still working on the design of the actual slider, though.
