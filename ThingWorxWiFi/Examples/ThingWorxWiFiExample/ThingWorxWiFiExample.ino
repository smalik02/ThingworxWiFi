/****************************************************************************
Arduino to Thingworx Ethernet Web Client using WiFi Shield

This sketch reads sensor values and sends the values to a Thing on ThingWorx.
The sketch uses the WiFi Repeating Web client (info below) and a custom ThingWorx library
as a starting point. On the Thingworx side make sure your 
nameArray[] values, your thingName[], your serviceName[] and your server[] matches exactly.
Use your provided appKey that you recieve when you set up your Thing.

Create a service in your Thing or ThingShape and create inputs that will match the variable names you use
in your Arduino code.  In the JavaScript window type something similar to the following:

me.PropertyOne = parseFloat(InputOne);
me.PropertyTwo = parseFloat(InputTwo);

Where Property one is the name of your first Thing or ThingShape property and InputOne is the name of
your first input.  Everything is case sensitive.

Update sensorCount to reflect how many sensors (or whatever data variables) you want to
send.

created 3/5/2015
by Nick Milleson
Design Engineer 
EAC Product Development Solutions
www.eacpds.com
nmilleson@eacpds.com

***********************************************************************************************/

/***********************************************************************************************
  Repeating Wifi Web client

 This sketch connects to a a web server and makes a request
 using an Arduino Wifi shield.

 Circuit:
 * Wifi shield attached to pins 10, 11, 12, 13

 created 23 April 2012
 modifide 31 May 2012
 by Tom Igoe

 http://arduino.cc/en/Tutorial/WifiWebClientRepeating
 This code is in the public domain.
 ************************************************************************/

#include <SPI.h>
#include <WiFi.h>
#include <ThingWorxWiFi.h>

#define sensorCount 4  //How many values you will be pushing to ThingWorx

// initialize the library instance:
char ssid[] = "tufts-guest";      //  your network SSID (name)
//char pass[] = "secretPassword";   // your network password
//int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

// Initialize the Wifi client library
WiFiClient client;

//Initialize ThingWorx server and app key
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
char server[] = "52.87.145.72";                  //ThingWorx server (do not include http://)
char appKey[] = "45a3ae1a-269a-4a7b-ab3c-05e693e6b135";
char thingName[] = "Arduino";                     //Name of your Thing in ThingWorx
char serviceName[] = "ArduinoThingService";                          //Name of your Service (see above)

//Initialize Sensor Name Array
char* nameArray[] = {"sensor one"};
double sensorValues[sensorCount];
int sensorPins[sensorCount] = {A2};  //Match up your pins with your nameArray


unsigned long lastConnectionTime = 0;          // last time you connected to the server, in milliseconds
boolean lastConnected = false;                 // state of the connection last time through the main loop
const unsigned long postingInterval = 10 * 1000; // delay between updates, in milliseconds

ThingWorx myThing(sensorCount); // Initialize your ThingWorx Object

void setup() {
  // start serial port:
  Serial.begin(9600);
  // give the ethernet module time to boot up:
  delay(1000);

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if ( fv != "1.1.0" )
    Serial.println("Please upgrade the firmware");

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid);

    // wait 10 seconds for connection:
    delay(10000);
  }
  // you're connected now, so print out the status:
  printWifiStatus();

  //Initialize pins
  for (int idx = 0; idx < sensorCount; idx++)
  {
    pinMode(sensorPins[idx], INPUT);
  }

}


void loop() {
 
  // Aquire sensor values
 for(int ids = 0; ids < sensorCount; ids++)
 {
  sensorValues[ids] = analogRead(sensorPins[ids]);  //This gets the raw AD converted (0-1028) values.  Convert them to usable values.  Modify if using digital io.
 }
 
  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:
  if (!client.connected() && ((millis() - lastConnectionTime) > postingInterval))
  {
 
    myThing.UpdateValues(sensorValues,client,server,appKey,thingName,serviceName,nameArray);  //This is the library function that constructs the REST call and sends it to your ThingWorx service
    lastConnectionTime = millis();  //Note the last connection time

  }

  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}



