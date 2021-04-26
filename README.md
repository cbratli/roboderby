GET STARTED


a) Open the Arduino IDE. Make sure that you are at version 1.8 or higher, if not then update your IDE with the latest version. https://www.arduino.cc/en/software
b)  Add the ESP32 board
Click on the File->Preferences menu item. This will open a Preferences dialog box.
You should be on the Settings tab in the Preferences dialog box by default.
Look for the textbox labeled “Additional Boards Manager URLs”.
If there is already text in this box add a coma at the end of it, then follow the next step.
Paste the following link into the text box – https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
Click the OK button to save the setting.
c) Add board by going to Tools->Boards->Boards Manager.... Search for ESP32, and select the one from Espressif Systems, it has to be 1.04 and not 1.0.5(as 1.0.5 and 1.0.6 will give error messages for PS4-esp32 library used for PS4 Bluetooth controller)

d) Tools -> Library manager , search for analogwrite. Add ESP32 Analogwrite by ERROPix
e) Tools -> Manage Libraries - Download ArduinoJson
f) Download mosquitto executables (for sending MQTT messages) to test the robot https://mosquitto.org/download/
g) Test that the unit is connected roboderby\manualtesting\test_esp>startListening.cmd
You should receive incomming messages from the robot
h) Sketch -> Incude Library -> Add .zip library... and select roboderby_esp8266\libraries\PS4-esp32-master.zip
i) If you want to connect a PS4 contoller, you will need to find the bluetooth id of the last device the controller was connected to and insert it into the bluetoothIdPS4ControllerWillConnectTo variable.
The way we did it was to connect the controller to the PC, and then find the bluetoothid of the PC.
If on linux, you can most likely use 'hcitool dev' and look for 'hci0', the bluetooth id will be on the
format: xx:xx:xx:xx:xx:xx



