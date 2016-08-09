/********************************************************************************************************** 
ThingworxWiFi.h - Library for uploading values to a ThingWorx "Thing" via WiFi
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

#ifndef ThingWorxWiFi_h
#define ThingWorxWiFi_h

#include "Arduino.h"
#include "WiFi.h"
#include "string.h"

class ThingWorx
{
	public:
		ThingWorx();
		ThingWorx(int sensorCount);
		void UpdateValues(double values[] , WiFiClient &client, char server[], char appKey[], char thingName[], char serviceName[], char* sensorNames[]);

	private:
		int _sensorCount;

		
};

#endif
