The following file details the structure, build and run instructions for ECE-4380-001 Capstone Project Group 4. 

## Dependencies
The following software must be installed in the host computer before any attempts to run this project can be made:
STM32 Cube IDE - STM32 and RTOS control
Arduino IDE - with ESP Dev Module Library and PGSQL Link library. If the host machine experiences issues locating the port that the ESP32 is connected to the, the driver obtained from the following link can be used to aid in device detection and connection
Visual Studio Code- with node.js, express.js, and API key library
PostgreSQL 18 - used as the backend of this application. Create a table labeled to store the sensor readings. Recommended column structure is: id SERIAL (the primary key), timestamp (DEFAULT CURRENT_TIMESTAMP so add a timestamp to every sensor reading), fanvoltage (double precision), LEDvoltage (double precission), and temp (double precision)

An account was created in Twilio Sendgrid for email notifications from the website. Due to the sensitive nature of API keys and passwords
, the ones used during the development of this project have been omitted from this public repository. To enable user email communications from the website. Please create an account in Twilio Sendgrid using an Email account. From there, verify the sender email from the “Sender Authenticcacction” tab on the left hand side of the screen and create a new API key. Within the Embedded Github files that have been downloaded onto the new host computer, create a .env file containing both the API Key that will be used to send emails from Sendgrid, and the sender email associated with the Twilio count, in the same general location, create a file called .sendgrid and also add the API key.

Once all required software and libraries have been enabled in the host device, the Embedded System can be initialized by followed the steps detailed below:
Download INSERT OUR FINAL RTOS PROJECT FOLDER and import the folder as a project within the STM32 CubeIDE, once the STM32 has been connected to the desired port, set GPIOPins PA2 and PA3 as RX and TX for USART1 and GPIOPins PA9 and PA10 as RX and TX for UART2, flash the code onto the device
Open the file labeled ESPInteraction.ino in the ArduinoIDE and add your local Wi-Fi’s 
Connect the ESP32 to the STM32 and to the host device and, use ArduinoIDE to upload the file in the Master branch labeled ESPInteraction.ino onto the ESP32
In VS Code, open a new terminal in the project’s home folder and run execute server.js using node. The link to. New instance of the user interface will be printed in the terminal.

In that front end the user will be able to locate 3 real-time graphs, one displaying each sensor readings and the time the sample was taken. On the left hand side of the graph is a color picker for the LEDs, once the user has selected the RGB value, this data is sent back to the RTOS and the circuit’s LEDs colors are changed. Toggling the slider to the right, fill turn the integrated fan on or off. The last set of toggles control user notifications. Once the user has set a warning toggle switch on, they will receive an email from the program’s email letting them know when their condition has been triggered. 
