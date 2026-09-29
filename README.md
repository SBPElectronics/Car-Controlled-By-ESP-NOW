# Car-Controlled-By-ESP-NOW

This is a 4 wheeled car which has 4 TT Motors and an ESP32-S3 as the microcontroller and L298N Motor Driver.

This 4 wheeled car is remotely controlled by an ESP32 which talks to the ESP32-S3 via the ESP-NOW protocol.

All the code is in the other files. The wiring is written (indirectly) in the code.

For the ESP-NOW protocol, you need mac address so the ESPs know where to send the info to. I added another file the code to collect the mac address of both your ESP32s. You don't have to use what I used.
