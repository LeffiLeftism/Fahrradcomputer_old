#define SIGNAL_PIN LED_BUILTIN
#define CALIBRATE_WEIGHT 1 // [kg]

class DMS
{
private:
    int ADC_F0_kg;
    int ADC_F1_kg;
    unsigned int DMS_Pin;
    double ADCtoForce; // kg pro ADC-Wert
    bool calibrated = true;

public:
    DMS(unsigned int zDMS_Pin) // Constructor
    {
        DMS_Pin = zDMS_Pin;
    }

    /****** Functions ******/
    void calibrate()
    {
        // LED blink 5 times for 500ms each
        ledBlink(SIGNAL_PIN, 500, 5);
        delay(2000);

        // LED HIGH while Sensor is read
        digitalWrite(SIGNAL_PIN, HIGH);
        ADC_F0_kg = readSensor(50, 200);
        digitalWrite(SIGNAL_PIN, LOW);
        delay(2000);

        // LED blink 5 times for 500ms each
        ledBlink(SIGNAL_PIN, 500, 5);
        delay(2000);

        // LED HIGH while Sensor is read
        digitalWrite(SIGNAL_PIN, HIGH);
        ADC_F1_kg = readSensor(50, 200);
        digitalWrite(SIGNAL_PIN, LOW);
        delay(2000);

        // LED blink 10 times for 200ms each
        ledBlink(SIGNAL_PIN, 200, 10);
        ADCtoForce = (CALIBRATE_WEIGHT) / (ADC_F1_kg - ADC_F0_kg);
        calibrated = true;
        delay(2000);
    }

    double calcWeight()
    {
        if (calibrated)
        {
            double weight;
            int ADC_val = readSensor(0, 200);
            weight = double(ADC_val - ADC_F0_kg) * ADCtoForce;
            return weight;
        }
        else
        {
            return NULL;
        }
    }

private:
    unsigned int readSensor(unsigned int tdelay, unsigned int count)
    {
        digitalWrite(SIGNAL_PIN, HIGH);
        delay(tdelay);
        unsigned int sum = 0, avg;

        for (size_t i = 0; i < count; i++)
        {
            sum += analogRead(DMS_Pin);
            delay(tdelay);
        }
        avg = sum / count;
        return avg;
    }

    void ledBlink(unsigned int LED_PIN, unsigned int ms, unsigned int count)
    {
        for (size_t i = 0; i < count; i++)
        {
            digitalWrite(LED_PIN, HIGH);
            delay(ms);
            digitalWrite(LED_PIN, LOW);
            delay(ms);
        }
    }
};