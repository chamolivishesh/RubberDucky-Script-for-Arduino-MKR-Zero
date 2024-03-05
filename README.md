# RubberDucky-Script-for-Arduino-MKR-Zero

This is a script for the Arduino MKR Zero board designed to emulate a RubberDucky device.

<h2>Pre-requisites:</h2>

1. Suitable Arduino Board (Arduino MKR Zero in my case)
2. A USB cable to connect to PC / Laptop / Mac
3. An SD card with code.txt file specified with DuckyScript code

<h2>Here's how it operates:</h2>

1. It reads the code.txt file from the SD card, containing DuckyScript commands, and executes them on the connected PC or laptop.
2. Upon successful execution of the DuckyScript, the LED on the Arduino board remains static, indicating completion.
3. In the event of a fatal error during execution, the program halts, and the LED on the Arduino board blinks rapidly.
4. Ensure the code.txt file is properly formatted with valid DuckyScript commands.
