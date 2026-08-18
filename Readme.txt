System Overview:
This solution is built to assist Linda in entering her house safely at night. On reaching her home, pressing the push button turns on both the porch light and the hallway light. The porch light remains lit for 30 seconds before switching off, while the hallway light stays lit for 60 seconds.

Hardware Required
Arduino Nano 33 IoT
LEDs
Push Button
Resistors
Breadboard
Jumper Wires

Code Explanation

In this code, we first assign the pins for the porch light, hallway light and push button. Within the setup() function, the light pins are configured as outputs and the button pin is configured as an input. The LightsOn() function turns on both the lights.
The main timing sequence is handled by the LightsON() function: it activates both lights, pauses for 30 seconds, turns off the porch light, pauses for another 30 seconds, finally turns off the hallway light. The loop() function on detecting a press, starts the light sequence.