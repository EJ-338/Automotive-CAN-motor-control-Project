# Automotive-CAN-motor-control-Project
Two-Arduino CAN project for distance-based motor speed control and collision avoidance, with encoder RPM measurement.

# CAN-Based Motor Speed Control

Automotive Mechatronics I (ECE 4520), Summer 2025  
Team: Eric Janowiecki and Dale Szydlowski

## Project

We built a bench-top demonstration of adaptive cruise control and
forward collision avoidance concepts. An Arduino reads an IR distance
sensor and sends distance data over CAN to a second Arduino. The
receiving Arduino sets motor PWM through an L298N driver and measures
motor RPM with an encoder.

## Test results

| Reported distance | PWM command | Measured motor speed |
| ---: | ---: | ---: |
| 13 cm | 19 | 0 RPM |
| 15 cm | 58 | 31.11 RPM |
| 19 cm | 137 | 82.44 RPM |
| 23 cm | 215 | 101.67 RPM |

At 13 cm, the motor did not turn at the low PWM command. Motor speed
increased as the reported distance and PWM command increased. RPM was
measured for observation; the PWM command was set from distance.

## Project notes

This was a team project based on course-provided schematics and starter
code. We assembled, completed, and tested the two-node prototype.

**Encoder clarification:** One encoder channel was sufficient to
measure RPM in this test. The phase relationship between channels A
and B can also be used to determine rotation direction. This corrects
the explanation of the two channels in our original report.

## Arduino code

- [CAN transmitter — distance sensor](CAN_Transmit_Side.ino)
- [CAN receiver — motor and encoder](CAN_Receive_Side.ino)

The receiver sketch includes a later correction to its timing and RPM
calculation. The measurements above are from the original course project;
the updated sketch has not been retested on hardware.
