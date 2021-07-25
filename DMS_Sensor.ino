#include "DMS-Sensor.h"
#define DMS_1_PIN 26
DMS dms_1(DMS_1_PIN);

void setup()
{
    /****** PinModes ******/
    pinMode(DMS_1_PIN, INPUT);
    dms_1.calibrate();
}

void loop()
{
    double weight = dms_1.calcWeight();
}