/********************************************************************************************************** 
ThingworxWiFi.cpp - Library for uploading values to a ThingWorx "Thing" via WiFi
Created by Nick Milleson 3/9/2015 for
EAC Product Development Solutions
www.eacpds.com
nmilleson@eacpds.com

Create a service in your Thing or ThingShape and create inputs that will match the variable names you use
in your Arduino code.  In the JavaScript window type something similar to the following:

me.PropertyOne = parseFloat(InputOne);
me.PropertyTwo = parseFloat(InputTwo);

Where Property one is the name of your first Thing or ThingShape property and InputOne is the name of
your first input.  Everything is case sensitive.
************************************************************************************************************/

#include "Arduino.h"
#include "ThingWorxWiFi.h"
#include "WiFi.h"
#include "string.h"

ThingWorx::ThingWorx()
{
		
}

ThingWorx::ThingWorx(int sensorCount)
{
	pinMode(4,OUTPUT);
	digitalWrite(4,HIGH);		//Turn off SD card
	
	_sensorCount= sensorCount;
	 
}

void ThingWorx::UpdateValues(double values[] , WiFiClient &client, char server[], char appKey[], char thingName[], char serviceName[], char* sensorNames[])
{

 if (client.connect(server, 80)) {
 
    // send the HTTP POST request:
    client.print("POST /Thingworx/Things/");
    client.print(thingName);
    client.print("/Services/");
    client.print(serviceName);
    client.print("?appKey=");
    client.print(appKey);
    client.print("&method=post&x-thingworx-session=true");
    client.print("<");
    
    for(int idx = 0; idx < _sensorCount; idx++)
    {
    	client.print("&");
    	client.print(sensorNames[idx]);
    	client.print("=");
    	client.print(values[idx]);
    }
    client.print(">");
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("Content-Type: text/html");
    client.println();

	while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
	
	client.stop();
  }
  else {
    // if you couldn't make a connection:
  
    client.stop();
  }
}
