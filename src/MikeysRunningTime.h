/* This library is used for timekeeping of running times of actuators. It's usefull for positioning
without position indicator. Return values of actual position can be shown in percentage or absolut
e.g. cm 
*/

#ifndef MikeysRunningTime_h
#define MikeysRunningTime_h
#include "Arduino.h"

class MikeysRunningTime {
	public:
	MikeysRunningTime(unsigned int memoryAddress); // Instantiates class with address in EEPROM. Class uses 8 Byte. 
	// Make sure not to overlap with other functions or instances. To prevent wrong behaviour, use resetValues() at first use
	// of this address
	void minPosition(float minPosition); // Sets the physical minimal Position (e.g. 10cm)
	void maxPosition(float maxPosition); // Sets the physical maximal Postion (e.g. 100cm)
	void update(); // Updates position, should be called in loop
	void startMeasuring(); // Starts the runtime Measuring, should be called simultaneously as the actuator moves
	void stopMeasuring(); // Stops the runtime Measuring, should be called simultaneously ast the actuator stops
	unsigned long runningTime(); // Shows the total running Time in ms
	void positiveMovement(); // Should be called when actuator moves in positive direction
	void negativeMovement(); // Should be called when actuator moves in negative direction
	void stopMovement(); // Should be called when movement stops
	float actPositionPercentage(); // Gives back actual position in percent (gives back 0.0 when values aren't set or referencing is needed)
	float actPositionAbsolute(); // Gives back actual position in physical dimensions (e.g. cm, degrees,... gives back 0.0 when values aren't set or referencing is needed)
	void storeValues(); // Stores running time values in EEPROM (uses EEPROM.update) memoryAddress = startTime, memoryAddress + 4 = endTime
	void loadValues(); // Loads values from EEPROM, should be called in setup
	void resetValues(); // Resets values in EEPROM and loaded values in instance
	bool valuesSet(); // Gives back 1 when values are set
	bool referencingNeeded(); // Gives back 1 when home referencing is needed (e.g. after power loss and actual physical position is not known)
	void setHomePosition(); // When referencing is needed and actuator is in home position, call this to set new home position
		
	private:
	bool _positiveMovement, _negativeMovement, _consistent;
	unsigned int _memoryAddress;
	unsigned long _startTime, _endTime, _runningTime, _millis, _lastMillis, _actRunningTime;
	float _minPosition, _maxPosition;
	
	unsigned long __runnningTime();
};
#endif