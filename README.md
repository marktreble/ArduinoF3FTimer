# ArduinoF3FTimer
Timer Application for Arduino for use with f3f-timer Android App

## Prerequisites
Arduino Board (Any should work!?)

Real Time Clock (DS3231)

BlueTooth Module (HC-05) (Optional)

## Getting Started
Connect Buzzer buttons or relays to Pins 3 (A) & 4 (B) & to ground.

You can also connect a launch button (perhaps foot operated?) to Pin 5 & ground.

## OTG
The timer will work over a USB connection taking it's power from the Android device. 

This requires an OTG Cable and set PIN_RX = 0 and PIN_TX = 1

## Bluetooth
A better experience can be obtained using Bluetooth as you can keep the android battery topped up from a power bank and have greater freedom of movement.



