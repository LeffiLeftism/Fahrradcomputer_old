#define COUNT_VALUES 20

class HallSensor
/*
    Diese Klasse beinhaltet das Grundgerüst für einen Sensor, der regelmäßig
    ausgelesen wird und ein Mittelwert berechnet wird.
    Es werden grundlegende Werte festgelegt und Funktionen bereitgestellt.
*/
{
private:
    String name;                        // Beinhaltet den Namen / die Bezeichnung des Sensors
    volatile long values[COUNT_VALUES]; // Beinhaltet alle Werte
    byte ind_values = 0;                // Hält die Position (Index) für den nächsten Wert im values-Array fest
    unsigned char SensorPin;            // Legt den Ausgelesenen Pin fest
    float adjustmentMultiplikator = 1;  // Multiplikator zur Umrechnung des Sensorwertes in die richtige Einheit
    unsigned long lastNewValue;

public:
    float average_value;           // Hält den letzten berechneten gewichteten Mittelwert fest
    bool interrupt_active = false; // Hält fest, ob ein Interrupt am SensorPin1 ausgelöst wurde
    bool oneTimeFull = false;      // Sagt aus, ob das values Array schon einmal gefüllt wurde

    HallSensor(String zname, uint8_t zSensorPin, float zadjustmentMultiplikator)
    {
        name = zname;
        SensorPin = zSensorPin;
        adjustmentMultiplikator = zadjustmentMultiplikator;
    }

    void newValue(unsigned long ExtraValue)
    /*
        Fügt dem values-Array einen neuen Wert hinzu
        und erhöht den Index. Ist das Ende des Arrays erreicht,
        wird dieses von Position "0" an überschrieben.
    */
    {
        values[ind_values] = ExtraValue;
        lastNewValue = ExtraValue;
        ind_values++;

        if (ind_values >= COUNT_VALUES)
        {
            ind_values = 0;
            oneTimeFull = true;
        }
    }

    void calculateAverage()
    {
        if (oneTimeFull)
        {
            uint32_t dif;        // Hält die Differenz zwischen zwei Werten fest
            uint32_t sumDif = 0; // Hält die Summe der Differenzen fest
            uint8_t n;           // Hält die Position im Array fest, um ind_values nicht überschreiben zu müssen
            uint8_t weight = 0;
            for (size_t i = ind_values; i < (sizeof(values) / 4) + ind_values - 1; i++)
            // Durchläuft das values-Array. Startet im Array eine Position nach dem zuletzt hinzugefügten Wert.
            // Die Schleife läuft über die Länge des Arrays und startet bei ind_values.
            // sizeof(values) / 4 --> durch 4, weil eine long-Variable 4 Bytes groß ist
            // i < [...] - 1 --> minus 1, damit die exakte Länge des Arrays durchlaufen wird
            {
                weight++;
                if (i >= (sizeof(values) / 4))
                /*
                Überprüft, ob i größer ist als die Länge des Arrays und setzt n 
                an die richtige Position der nächsten Position im Array.
                */
                {
                    n = i - (sizeof(values) / 4);
                }
                else
                {
                    n = i;
                }

                if (n == COUNT_VALUES - 1)
                /*
                Löst den Problemfall, wenn n auf der letzten Position des Arrays sitzt und 
                array[n + 1] - array [n] gerechnet werden müsste. Da dies nicht möglich ist,
                wird hier array[0] - array[n] gerechnet.
                */
                {
                    dif = values[0] - values[n];
                }
                else
                {
                    dif = values[n + 1] - values[n];
                }
                if (dif < 0)
                {
                    dif = 0;
                }
                sumDif += dif * weight;
            }
            average_value = float(sumDif) / float((weight + 1) * (weight / 2));
            average_value = 1 / average_value * adjustmentMultiplikator;
        }
    }

    void checkTimeout()
    {
        if (millis() - lastNewValue > 10000)
        {
            for (uint8_t i = 0; i < COUNT_VALUES; i++)
            {
                values[i] = 0;
            }
            average_value = 0.0;
            ind_values = 0;
            oneTimeFull = false;
        }
    }
};
