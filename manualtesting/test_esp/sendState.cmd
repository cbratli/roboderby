@echo off

echo Setting state to ACCEPT_COMMAND - Countdown 10000
"c:\program files\mosquitto\mosquitto_pub.exe" -h test.mosquitto.org -t "roboderby/robotIn/setState" -f roboderbySetStateCountdown.txt
pause

echo Sending commands
"c:\program files\mosquitto\mosquitto_pub.exe" -h test.mosquitto.org -t "roboderby/robotIn/command" -f roboderbyCmd1.txt
pause