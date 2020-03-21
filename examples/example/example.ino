#include<MikeysRunningTime.h>
MikeysRunningTime runtime1(10); // Instantiate class with EEPROM start Adress 10

void setup() {
  pinMode(2, INPUT); // Used to Start Measuring
  pinMode(3, INPUT); // Used to stop measuring
  pinMode(4, INPUT); // used to simulate positive movement
  pinMode(5, INPUT); // used to simulate negatvie movement
  pinMode(8, INPUT); // used to store values
  pinMode(9, INPUT); // used to reset values
  pinMode(10, INPUT); // used to set home position

  runtime1.minPosition(10.0); // Set minimal physical position to 10cm
  runtime1.maxPosition(200.0); // Set maximal physical position to 200cm
  runtime1.loadValues(); // Load running time values from EEPROM if present

  Serial.begin(250000);
}

void loop() {
  runtime1.update(); // Updates position calculation, should be called in loop

  if (digitalRead(2)) { // Start running time measuring
    runtime1.startMeasuring();
  }
  if (digitalRead(3)) { // Stop running time measuring
    runtime1.stopMeasuring();
  }

  if (digitalRead(4)) { // Simulate positive movement
    runtime1.positiveMovement();
  } else if (digitalRead(5)) { // Simualte negative movement
    runtime1.negativeMovement();
  } else {
    runtime1.stopMovement(); // Stop position calculation
  }

  if (digitalRead(8)) { // Store values to EEPROM
    runtime1.storeValues();
  }

  if (digitalRead(9)) { // Resets values in EEPROM and loaded values in instance
    runtime1.resetValues();
  }

  if (digitalRead(10)) { // Sets home position manually when referencing is needed
    runtime1.setHomePosition();
  }

  Serial.print("runningTime "); Serial.println(runtime1.runningTime()); // Shows total running time in ms
  Serial.print("actPositionPercentage "); Serial.println(runtime1.actPositionPercentage()); // Shows actual position in percent
  Serial.print("actPositionAbsolut "); Serial.println(runtime1.actPositionAbsolute()); // Shows actual position in physical dimensions
  Serial.print("valuesSet "); Serial.println(runtime1.valuesSet()); // Is 1 when values are set
  Serial.print("referencingNeeded "); Serial.println(runtime1.referencingNeeded()); // Is 1 when home referencing is needed e.g. after power loss
  delay(500);
}
