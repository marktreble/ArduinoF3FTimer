# ArduinoF3FTimer
Timer Application for Arduino for use with f3f-timer Android App

## Prerequisites
Arduino Board (Any should work!?)

Real Time Clock (DS1307 or similar)

BlueTooth Module (HC-05) (Optional)

## Getting Started
Connect Buzzer buttons or relays to Pins 2 (A) & 3 (B)

You can also connect a launch button (perhaps foot operated?) to Pin 4

The code as is will work as an OTG device - plug in to the USB port with an OTG cable and go!

## Bluetooth
A better experience can be obtained using Bluetooth as you can keep the android battery topped up from a power bank and have greater freedom of movement.

Just Change constants PIN_RX and PIN_TX to wherever you HC-05 RX/TX is connected.

## Pending
Real Time Clock (ETA mid Jan 2019)
