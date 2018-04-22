void mortorRoll() {
    customDelayMapped = speedUp(); // Gets custom delay values from the custom speedUp function
    // Makes pules with custom delay, depending on the Potentiometer, from which the speed of the motor depends
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(customDelayMapped);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(customDelayMapped);
}
// Function for reading the Potentiometer
int speedUp() {
    int customDelay = analogRead(poten); // Reads the potentiometer
    int newCustom = map(customDelay, 0, 1023, 300,4000); // Convrests the read values of the potentiometer from 0 to 1023 into desireded delay values (300 to 4000)
    return newCustom;
}
