/*





TODO:
Add statemachine
Add cmd files(in source control) that will start the robot.
Add robotname, and subscribe to robot's topic
Add possibility to connect to a different wifi?
Add possibility to change maxtime.


Add announcement from robot.... [robotname, maxtime, connection riddle]

Recalculate jsonbuffersize, and increse number of messages to 50.
Test with 100 messages in.
Test to send a really long json file that will overflow the buffers.

Add password/connection riddle


Clean up code......




 
 ESP8266 ROBO DERBY
 
If you get compile error for the line
Serial.setRxBufferSize(bufferSize);
then follow the installation guide here:
https://github.com/esp8266/Arduino#using-git-version

HARDWARE:
This is supposed to  be run on an ESP8266, but it may be that it can run on other hardware also.

Electrical schematic:

Motordriver that takes two inputs 
Dxxx PWM / enable.
Dx HIGH - LEFT FORWARD
Dxx HIGH - RIGHT FORWARD


The MQTT topics we subscribe to:

roboderby/command/

roboderby/getSavedCommands
replies on the topic roboderby/replyGetSavedCommands


roboderby/startCountdownMs - Here we receive the number of ms until start.
replies on the topic roboderby/replyStartCountdownMs

In addition, all robots publish their names on:
topic roboderby/botnames



We can send approximately 19 MQTT messages from the with ESP8266 and the mqtt library per second.

Currently:
 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
This must be corrected so its non-blocking.

To generate a new Json specification, use : https://arduinojson.org/v6/assistant/

{
 "player":"Playername",
 "name":"Carname",
 "msgid":303,
 "pwd": "pwdString",
  "cmd": [  
{ "cmdId": 1, "leftMotor": 0.1,"rightMotor": 0.5, "duration_ms":  2000},
{ "cmdId": 2, "leftMotor": 0.2,"rightMotor": 0.6, "duration_ms":  3000},
{ "cmdId": 3, "leftMotor": 0.3,"rightMotor": 0.7, "duration_ms":  3000}
]
 }




Kan prøve med flere kommandoer:




*/

/*  TODO: 
 *   Receive ssid, wlanPassword, mqttServerIp, mqttUser, mqttPassword, mqttPort, , publishKey, subscribeKey.
 *   Save settings to eeprom????
 *   
 *   
 *   
 *   REQUIREMENTS: 
 *   100 messages per second. Each message is separated by \n if concatenated.
 *   100 bytes every 10 ms -> 10000 bytes/second
 *   10000 bytes per second is approximately 100000 baud/s.
 *
 */


#include <WiFi.h>
#include <analogWrite.h>
#include "PubSubClient.h"
#include "HardwareSerial.h"
#include <ArduinoJson.h>
#include "StateMachine.h"
#include "config.h"
#include <math.h>
#include <PS4Controller.h>

// Update these with values suitable for your network.

const int MAX_LOG_MESSAGE_SIZE=200;

//const char* ssid = "LaplaceTelenor";
//const char* password = "InnbyggertallRapportere3";
//const char* mqtt_server = "10.0.0.15";

//const char* ssid = "norbot_pc";
//const char* password = "Pass4norbot"; 
//const char* mqtt_server = "192.168.137.1";

//const char* ssid = "Tieto Guest";
//const char* password = "hemmelig";
//const char* mqtt_server = "broker.mqtt-dashboard.com";

//const char* ssid = "bratlchrs iPhone 6SP";
const char* ssid = "Bra Phone X";
const char* password = "hackie887";
//const char* mqtt_server = "broker.mqttdashboard.com";

const char* mqtt_server = "test.mosquitto.org";
//const char* mqtt_server = "iot.eclipse.org";



//const char* mqtt_server = "broker.mqtt-dashboard.com";
const char* mqttChannelOut = "norbot/folkrace/cyborgproto/out";
const char* mqttChannelIn = "norbot/folkrace/cyborgproto/in";
const char* mqttChannelIn2 = "norbot/folkrace/cyborgproto/in2";
const char* mqttChannelIn3 = "norbot/folkrace/cyborgproto/in3";


const char* mqttTopicsetState =  "roboderby/robotIn/setState";
const char* mqttTopicCommandIn = "roboderby/robotIn/command";

TaskHandle_t Task1;
TaskHandle_t Task2;


int numberOfCommands = 20;
const size_t capacity = JSON_ARRAY_SIZE(numberOfCommands) + JSON_OBJECT_SIZE(4) + numberOfCommands*JSON_OBJECT_SIZE(6) + 50*numberOfCommands+100;
DynamicJsonDocument doc(capacity);


const size_t capacityMessage = JSON_ARRAY_SIZE(3) + JSON_OBJECT_SIZE(4) + 3*JSON_OBJECT_SIZE(6) + 50*3+100;
DynamicJsonDocument messageToProcess(capacityMessage);


bool hasCommands = false;
bool runningCommands = false;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msgToSend[MAX_LOG_MESSAGE_SIZE]; //  Buffer that contains messages to send.

int value = 0;
int msgIndex = 0; // Index into array; where to store the character
String messageToSend = "";

void Task1code( void * pvParameters ){
  for(;;){
    delay(1000);
    Serial.print("Task1 running on core ");
    Serial.println(xPortGetCoreID());
    client.publish(mqttChannelOut, "INFO:FROM Task1");
  }
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println("");

  if (runningCommands)
  {
    Serial.println("Running commands - Skipping data");
    return;
  } 
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }



if (strcasecmp(topic, mqttChannelIn) == 0) {
  Serial.print("Match on topic");
  Serial.println(mqttChannelIn);
  } 
  else if((strcasecmp(topic, mqttChannelIn2) == 0)) {

  DeserializationError error = deserializeJson(messageToProcess, payload);

  if (error) {
    Serial.println("Deserialisation failed");
    return;
  }

    if(strcasecmp(messageToProcess["state"],"ACCEPT_COMMANDS") == 0)
    {
      Serial.println("Going to state accept commands");
        long countdown = messageToProcess["countdown"];
        
    } else {
      Serial.println("Unknown state");
      }

  }
else if((strcasecmp(topic, mqttChannelIn2) == 0)) {
  
  Serial.print("Match on topic");
  Serial.println(mqttChannelIn2);
  }
else if((strcasecmp(topic, mqttTopicCommandIn) == 0)) {
  
  Serial.print("Match on topic");
  Serial.println(mqttChannelIn2);
  Serial.println("I will do the following");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println("\nAs you requested!");
//const size_t capacity = JSON_OBJECT_SIZE(6) + 70;



DeserializationError error = deserializeJson(doc, payload);

if (error) {
  Serial.println("Deserialisation failed");
  return;
}

int password = doc["pwd"]; // 0
int msgId = doc["msgId"]; // 0



JsonArray cmd = doc["cmd"];

if (cmd.isNull()) {
  Serial.println("cmd is null");
  return;
}


long totalDuration_ms = 0;
int cmdId = 0;
while ((cmd[cmdId]["cmdId"] | -1) != -1 && cmdId < 30000)
{
  Serial.print("cmd ");
  Serial.print(cmdId);
  Serial.print(" - duration: ");
  int duration_ms = cmd[cmdId]["duration_ms"];
  Serial.println(duration_ms);
  totalDuration_ms += duration_ms;
 // long time = cmd["time"]; // 1351824120
  float leftMotor = cmd[cmdId]["leftMotor"]; // 0.1
  float rightMotor = cmd[cmdId]["rightMotor"]; // 0.1
  cmdId++;
}

/*
const size_t capacity = JSON_ARRAY_SIZE(20) + 21*JSON_OBJECT_SIZE(3);
DynamicJsonDocument sendDoc(capacity);


Serial.print("Total Duration_ms:");
Serial.println(totalDuration_ms);
snprintf (msgToSend, 1500, "Total duration_ms = (%ld)  ", totalDuration_ms);

sendDoc["t"] = millis();
sendDoc["msgId"] = msgId;
sendDoc["total_ms"] = totalDuration_ms;
serializeJson(sendDoc,msgToSend);
client.publish(mqttChannelOut, msgToSend);
*/

 client.publish(mqttChannelOut, "INFO:GOT COMMANDS");
hasCommands = true;
}


  
  Serial.println("");
  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    Serial.print("LED ON");
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else { 
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
    Serial.print("LED OFF");
  }

}

void reconnect() {
  // Loop until we're reconnected
  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
//    if (client.connect("rabbit@asusG","norbot","Pass4norbot")) {
    //if (client.connect("roboderby","norbot","Pass4norbot")) {
    if (client.connect("roboderby")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(mqttChannelOut, "INFO:Reconnected");
      // ... and resubscribe
      client.subscribe(mqttChannelIn);
      client.subscribe(mqttChannelIn2);
      client.subscribe(mqttChannelIn3);
      client.subscribe(mqttTopicCommandIn);
 
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  disableMotors();
  //Serial.begin(9600);
  Serial.begin(115200);
  //Serial.begin(1152000);
  //Serial.begin(1152000*2);
  //Serial.begin(4608000);
  //Serial.setRxBufferSize(10240);
  setup_wifi();
  //client.setServer(mqtt_server, 1883);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  lastMsg = millis();

  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(MOTOR2_PIN1, OUTPUT);
  pinMode(MOTOR2_PIN2, OUTPUT);

  //digitalWrite(MOTOR1_PIN2, HIGH);
  //digitalWrite(MOTOR1_PIN1, 1);

xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);


PS4.begin("94:e6:f7:20:bb:69");
Serial.println("Initialization PS4 controller ready!");

if(PS4.isConnected()) {
    Serial.println("Controller connected");
}




}

void enableMotors()
{
  Serial.println("Enable motors:");
}

void setLeftMotor(float percentage) {
 //int output =  255.0*percentage;
 //Serial.print("LeftMotor power:");
 //Serial.println(output);
 //client.publish(mqttChannelOut, "DEBUG:SET LEFT MOTOR");
  if (percentage > 0.5) {
    digitalWrite(MOTOR1_PIN1, 0);
    digitalWrite(MOTOR1_PIN2, 1);
  } else if (percentage < -0.5) {
    digitalWrite(MOTOR1_PIN1, 1);
    digitalWrite(MOTOR1_PIN2, 0);
  } else {
    digitalWrite(MOTOR1_PIN1, 0);
    digitalWrite(MOTOR1_PIN2, 0);
  }
}

#define __min(a,b) ((a)<(b)?(a):(b))
#define __max(a,b) ((a)<(b)?(b):(a))


void setLeftMotorPwm(float percentage) {
 int output =  MOTOR_PWM_RANGE*percentage;
 output = __max(output,-MOTOR_PWM_RANGE);
 output = __min(output, MOTOR_PWM_RANGE);
 //client.publish(mqttChannelOut, "DEBUG:SET LEFT MOTOR PWM");
 //Serial.print("LeftMotor power:");
 //Serial.println(output);
 if (abs(percentage) < 0.001) {
  disableLeftMotor();
 }else if (percentage > 0) {
    digitalWrite(MOTOR1_PIN1, 0);
    analogWrite(MOTOR1_PIN2, output);
  } else {
    analogWrite(MOTOR1_PIN1, abs(output));
    digitalWrite(MOTOR1_PIN2, 0);
  }
}


void setRightMotorPwm(float percentage) {
 int output =  MOTOR_PWM_RANGE*percentage;
 output = __max(output,-MOTOR_PWM_RANGE);
 output = __min(output, MOTOR_PWM_RANGE);
 
 //client.publish(mqttChannelOut, "DEBUG:SET RIGHT MOTOR");
 //Serial.print("LeftMotor power:");
 //Serial.println(output);
 if (abs(percentage) < 0.001) {
  disableRightMotor();
 }else if (percentage > 0) {
    digitalWrite(MOTOR2_PIN1, 0);
    analogWrite(MOTOR2_PIN2, output);
  } else {
    analogWrite(MOTOR2_PIN1, abs(output));
    digitalWrite(MOTOR2_PIN2, 0);
  }
}



void setRightMotor(float percentage) {
  //client.publish(mqttChannelOut, "DEBUG:SET RIGHT MOTOR");
  if (percentage > 0.5) {
    digitalWrite(MOTOR2_PIN1, 0);
    digitalWrite(MOTOR2_PIN2, 1);
  } else if (percentage < -0.5) {
    digitalWrite(MOTOR2_PIN1, 1);
    digitalWrite(MOTOR2_PIN2, 0);
  } else {
    digitalWrite(MOTOR2_PIN1, 0);
    digitalWrite(MOTOR2_PIN2, 0);
  }
}

void disableMotors()
{
  disableRightMotor();
  disableLeftMotor();
  
  client.publish(mqttChannelOut, "DEBUG:DISABLE_MOTORS");
 Serial.println("Disable motors:"); 

}

void disableLeftMotor() {
  analogWrite(MOTOR1_PIN1, 0);
  analogWrite(MOTOR1_PIN2, 0);
  digitalWrite(MOTOR1_PIN1, 0);
  digitalWrite(MOTOR1_PIN2, 0);
}

void disableRightMotor() {
  analogWrite(MOTOR2_PIN1, 0);
  analogWrite(MOTOR2_PIN2, 0);
  digitalWrite(MOTOR2_PIN1, 0);
  digitalWrite(MOTOR2_PIN2, 0); 
}

void PS4Controller()
{
  if (!PS4.isConnected()) {
    Serial.println("PS4 controller reconnect");
    //PS4.begin("94:e6:f7:20:bb:69");
    delay(200);
    Serial.println("Initialization PS4 controller ready!");
   }
  
  if (PS4.isConnected()) {
    if (PS4.Right()) Serial.println("Right Button");
    if (PS4.Down()) Serial.println("Down Button");
    if (PS4.Up()) Serial.println("Up Button");
    if (PS4.Left()) Serial.println("Left Button");

    if (PS4.Square()) Serial.println("Square Button");
    if (PS4.Cross()) Serial.println("Cross Button");
    if (PS4.Circle()) Serial.println("Circle Button");
    if (PS4.Triangle()) Serial.println("Triangle Button");

    if (PS4.UpRight()) Serial.println("Up Right");
    if (PS4.DownRight()) Serial.println("Down Right");
    if (PS4.UpLeft()) Serial.println("Up Left");
    if (PS4.DownLeft()) Serial.println("Down Left");

    if (PS4.L1()) Serial.println("L1 Button");
    if (PS4.R1()) Serial.println("R1 Button");

    if (PS4.Share()) Serial.println("Share Button");
    if (PS4.Options()) Serial.println("Options Button");
    if (PS4.L3()) Serial.println("L3 Button");
    if (PS4.R3()) Serial.println("R3 Button");

    if (PS4.PSButton()) Serial.println("PS Button");
    if (PS4.Touchpad()) Serial.println("Touch Pad Button");

    if (PS4.L2()) {
      Serial.printf("L2 button at %d\n", PS4.L2Value());
      //setLeftMotorPwm(((float)PS4.L2Value())/255.0);
    }
    
    if (PS4.R2()) {
      Serial.printf("R2 button at %d\n", PS4.R2Value());
      //setRightMotorPwm(((float)PS4.R2Value())/255.0);
    }
    Serial.printf("L2 button at %d\n", PS4.L2Value());
    Serial.printf("R2 button at %d\n", PS4.R2Value());
    
    if (PS4.LStickX()) {
      Serial.printf("Left Stick x at %d\n", PS4.LStickX());
    }
    if (PS4.LStickY()) {
      Serial.printf("Left Stick y at %d\n", PS4.LStickY());
    }
    if (PS4.RStickX()) {
      Serial.printf("Right Stick x at %d\n", PS4.RStickX());
    }
    if (PS4.RStickY()) {
      Serial.printf("Right Stick y at %d\n", PS4.RStickY());
    }

    if (PS4.Charging()) Serial.println("The controller is charging");
    if (PS4.Audio()) Serial.println("The controller has headphones attached");
    if (PS4.Mic()) Serial.println("The controller has a mic attached");

    Serial.printf("Battery Level : %d\n", PS4.Battery());

    Serial.println();
    
  }
  
} 

void loop() {
 
  if (!client.connected()) {
    reconnect();
  } 
  client.loop();

  PS4Controller();
  runGameEngine();
  runPS4ControllerEngine();
  // ****STATEMACHONE GOES HERE****

  // START_MODE 
    
  
  // AWAITING COMMANDS COUNTDOWN

  // HAS COMMANDS

  // COUNTDOWN TO RUN COMMANDS

  // RUNNING COMMANDS
  

  
  if (hasCommands)
  {
    enableMotors();
    runningCommands = true;
    long totalDuration_ms = 0;
    long startTime = millis(); // note that it may overflow....
    long runUntil = startTime; 
    JsonArray cmd = doc["cmd"];
    int cmdId = 0;
    Serial.println("*****START RUNNING cmd ");
    while ((cmd[cmdId]["cmdId"] | -1) != -1 && cmdId < 30000)
    {

      Serial.print("*****RUNNING cmd ");
      Serial.print(cmdId);
      Serial.print(" - duration: ");
      int duration_ms = cmd[cmdId]["duration_ms"];
  
      // Prevent cheating by giving negative duration.
      if (duration_ms < 0)
      {
         duration_ms = 0;  
      }
  
      Serial.println(duration_ms);
      long MAX_DURATION = 10000;
      if (totalDuration_ms + duration_ms > MAX_DURATION) {
        // to long duration, change duration to be rest of time available
        duration_ms = MAX_DURATION - totalDuration_ms;
      }
  
      if (duration_ms < 0) {
         // WE ARE DONE!
         break;  
      }
  
    
      totalDuration_ms += duration_ms;

      // long time = cmd["time"]; // 1351824120
      setLeftMotor(cmd[cmdId]["leftMotor"]); 
      setRightMotor(cmd[cmdId]["rightMotor"]); 
      
      cmdId++;
  
      delay(duration_ms);
  }

  Serial.println("*****DONE RUNNING cmds ****** ");
  
  disableMotors();
  EscStateMachine.transitionToState(ESC_STATE_INIT);
  
  hasCommands = false;
  runningCommands = false;
  // Reset the command object.
  
  }


long start_millis = millis();
while (millis() - start_millis < dT_ms) {}
    //  client.publish(mqttChannelOut, msgToSend);
 }
