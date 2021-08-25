#include "Arduino.h"
#include "hall-Sensor.h"
#include "I2C_functions.h"
#include <Wire.h>

#define SENSOR_ADDR 0x20
MbedI2C IC2(4, 5);

#define SensorPin1 2 // Definiert den PIN, an welchem der Hallsensor angeschlossen wird. Dieser muss ein Interruptfähiger PIN sein!
#define MAGNETS 1    // Definiert die Anzahl der der verwendeten Magneten

HallSensor rpm_pedal("Pedal-RPM", SensorPin1, 60000 / MAGNETS);

void setup()
{
    pinMode(13, OUTPUT);
    IC2.begin(SENSOR_ADDR);      // join i2c bus with SENSOR_ADDR
    IC2.onRequest(requestEvent); // register event

    pinMode(SensorPin1, INPUT);                                              // Legt den Modus des Pins fest, an welchem der Sensor angeschlossen ist
    attachInterrupt(digitalPinToInterrupt(SensorPin1), interrupt_func, LOW); // Fügt dem Pin des Sensors eine Interrupt-Funktion hinzu, welche beim festgelegten Status des Pins ausgeführt wird
}

void loop()
{
    delay(100);
    /*
        Wenn der Interrupt_1 ausgelöst wurde, wird beim nächsten Durchlauf
        ein neuer Zeitwert dem RPM-Sensor hinzugefügt und die neue Drehzahl erneut berechnet.
    */
    if (rpm_pedal.interrupt_active)
    {
        rpm_pedal.newValue(millis());
        rpm_pedal.calculateAverage();
        rpm_pedal.interrupt_active = false;
    }
    rpm_pedal.checkTimeout();
}

/*
    Diese Funktion wird ausgeführt, wenn der Interrupt am SensorPin1 ausgelöst wird.
*/
void interrupt_func()
{
    rpm_pedal.interrupt_active = true;
    while (digitalRead(SensorPin1) == 0)
    {
        delay(5);
    }
}

void requestEvent()
{
    IC2.print(floatToString(rpm_pedal.average_value, 1, 5)); // respond with message of 5 bytes
}