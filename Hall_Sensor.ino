#include "hall-Sensor.h"
#include <Wire.h>

#define RASPPI_PICO

#ifdef RASPPI_PICO
MbedI2C IC2(4, 5);
#endif

#define SensorPin1 2 // Definiert den PIN, an welchem der Hallsensor angeschlossen wird. Dieser muss ein Interruptfähiger PIN sein!
HallSensor rpm_pedal;

void setup()
{
    pinMode(13, OUTPUT);
    IC2.begin(0x20);                      // join i2c bus with address 0x20 (32)
    IC2.onRequest(requestEvent);          // register event
    // Voreinstellungen für den RPM-Sensor festlegen
    /*pinMode(SensorPin1, INPUT_PULLUP);                                       // Legt den Modus des Pins fest, an welchem der Sensor angeschlossen ist
    attachInterrupt(digitalPinToInterrupt(SensorPin1), interrupt_func, LOW); // Fügt dem Pin des Sensors eine Interrupt-Funktion hinzu, welche beim festgelegten Status des Pins ausgeführt wird
    rpm_pedal.SensorPin = SensorPin1;
    rpm_pedal.calcDiffAverage = true;
    rpm_pedal.fractionType = 0;
    rpm_pedal.adjustmentMulti = 60000 / 1; // [ms/U] -> [U/min  / Anzahl der Magneten]*/
}

void loop()
{
    delay(100);
    //if (rpm_pedal.interrupt_active)
    /*
        Wenn der Interrupt_1 ausgeföst wurde, wird beim nächsten Durchlauf
        ein neuer Zeitwert dem RPM-Sensor hinzugefügt und die neue Drehzahl erneut berechnet.
    */
    /*{
        noInterrupts(); // Deaktiviert Interrupts während der Berechnung
        rpm_pedal.newValue(millis());
        rpm_pedal.calculateAverage();
        rpm_pedal.interrupt_active = false;
        interrupts(); // Aktiviert Interrupts wieder, nachdem die Berechnungen abgeschlossen sind
    }*/
}

//void interrupt_func() // Funktion, welche bei einem Interrupt ausgeführt wird.
/*
    Diese Funktion wird ausgeführt, wenn der Interrupt am SensorPin1 ausgelöst wird.
*/
/*{
    rpm_pedal.interrupt_active = true;
    while (digitalRead(SensorPin1) == 0)
    {
        delay(5);
    }
}*/

void requestEvent() {
    digitalWrite(13, HIGH);
    IC2.write("hello ");       // respond with message of 6 bytes
    delay(500);
    digitalWrite(13, LOW);
}