#include "Arduino.h"
#include "MikeysRunningTime.h"
#include <EEPROM.h>

// PUBLIC
MikeysRunningTime::MikeysRunningTime(unsigned int memoryAddress){
	_memoryAddress = memoryAddress;
}

void MikeysRunningTime::minPosition(float minPosition){
	_minPosition = minPosition;
}

void MikeysRunningTime::maxPosition(float maxPosition){
	_maxPosition = maxPosition;
}

void MikeysRunningTime::update(){
	_millis = millis() - _lastMillis;
	if (_positiveMovement){
		_actRunningTime += _millis;
		_lastMillis = millis();
	} else if (_negativeMovement){
		_actRunningTime -= _millis;
		_lastMillis=millis();
	} else {
		_lastMillis=millis();
	}
}

void MikeysRunningTime::startMeasuring(){
	_startTime = millis();
}

void MikeysRunningTime::stopMeasuring(){
	_endTime = millis();
	_actRunningTime = __runnningTime();
	_consistent = true;
}

unsigned long MikeysRunningTime::runningTime(){
	return __runnningTime();
}

void MikeysRunningTime::positiveMovement(){
	_positiveMovement=true;
	_negativeMovement=false;
}

void MikeysRunningTime::negativeMovement(){
	_negativeMovement=true;
	_positiveMovement=false;
}

void MikeysRunningTime::stopMovement(){
	_positiveMovement=false;
	_negativeMovement=false;
}

float MikeysRunningTime::actPositionPercentage(){
	if (valuesSet() && !referencingNeeded()){
		float x = (100.0/__runnningTime()) * _actRunningTime;
		return x;
	}else{
		return 0.0;
	}
}

float MikeysRunningTime::actPositionAbsolute(){
	if (valuesSet() && !referencingNeeded()){
		float x = (actPositionPercentage() / 100.0) * (_maxPosition - _minPosition) + _minPosition;
		return x;
	}else{
		return 0.0;
	}
}

void MikeysRunningTime::storeValues(){
	EEPROM.update(_memoryAddress, _startTime);
	EEPROM.update(_memoryAddress + 4, _endTime);
}

void MikeysRunningTime::loadValues(){
	EEPROM.get(_memoryAddress, _startTime);
	EEPROM.get(_memoryAddress + 4, _endTime);
}

void MikeysRunningTime::resetValues(){
	for (unsigned int i = _memoryAddress; i < _memoryAddress + 8; i++){
		EEPROM.write(i,0);
	}
	_endTime = 0;
	_startTime = 0;
}

bool MikeysRunningTime::valuesSet(){
	if (__runnningTime() != 0.0){
		return true;
	}else{
		return false;
	}
}

bool MikeysRunningTime::referencingNeeded(){
	if (_consistent){
		return false;
	}else{
		return true;
	}
}

void MikeysRunningTime::setHomePosition(){
	if(!_consistent){
		_actRunningTime = 0;
		_consistent = true;
	}
}

// PRIVATE
unsigned long MikeysRunningTime::__runnningTime(){
	if(_endTime != 0 && _startTime != 0){
		return _endTime - _startTime;
	}else{
		return 0;
	}
}