#define COUNT_VALUES 20

class HallSensor
/*
    Diese Klasse beinhaltet das Grundgerüst für einen Sensor, der regelmäßig
    ausgelesen wird und ein Mittelwert berechnet wird.
    Es werden grundlegende Werte festgelegt und Funktionen bereitgestellt.
*/
{
private:
    volatile long values[COUNT_VALUES]; // Beinhaltet alle Werte
    byte ind_values = 0;                // Hält die Position (Index) für den nächsten Wert im values-Array fest

public:
    String name;                   // Beinhaltet den Namen / die Bezeichnung des Sensors
    bool calcDiffAverage = false;  // Legt fest, ob der Mittelwert der Summe oder der Mittelwert der Differenz zwischen den Werten berechnet wird (false -> Summenmittelwert, true -> Differenzmittelwert)
    unsigned char SensorPin;       // Legt den Ausgelesenen Pin fest
    bool fractionType = true;      // Legt fest, ob der average_value bei der Umrechnung im Nenner (0) oder Zähler(1) steht.
    float adjustmentMulti = 1;    // Multiplikator zur Umrechnung des Sensorwertes in die richtige Einheit
    float average_value; // Hält den letzten berechneten Mittelwert fest
    bool interrupt_active = false; // Hält fest, ob ein Interrupt am SensorPin1 ausgelöst wurde

    void newValue(long ExtraValue)
    /*
        Fügt dem values-Array einen neuen Wert hinzu
        und erhöht den Index. Ist das Ende des Arrays erreicht,
        wird dieses von Position "0" an überschrieben.
    */
    {
        values[ind_values] = ExtraValue;
        ind_values++;

        if (ind_values >= COUNT_VALUES)
        {
            ind_values = 0;
        }
    }

    void calculateAverage()
    {

        if (calcDiffAverage)
        /*
            Berechet die Mittelwert der Differenzen zwischen den Werten im values-Array
        */
        {
            unsigned long dif;        // Hält die Differenz zwischen zwei Werten fest
            unsigned long sumDif = 0; // Hält die Summe der Differenzen fest
            unsigned int n;           // Hält die Position im Array fest, um ind_values nicht überschreiben zu müssen
            for (size_t i = ind_values; i < (sizeof(values) / 4) + ind_values - 1; i++)
            /*
            Durchläuft das values-Array. Startet im Array eine
            Position nach dem zuletzt hinzugefügten Wert.
            Die Schleife läuft über die Länge des Arrays und startet bei ind_values.

             -- sizeof(values) / 4 --> durch 4, weil eine long-Variable 4 Bytes groß ist --
             -- i < [...] - 1 --> minus 1, damit die exakte Länge des Arrays durchlaufen wird
            */
            {
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
                sumDif += dif;
            }
            average_value = float(sumDif) / float(COUNT_VALUES - 1);
            if (fractionType)
            {
                average_value *= adjustmentMulti;
            }
            else
            {
                average_value = 1 / average_value * adjustmentMulti;
            }
        }
        else
        /*
            Berechet die Mittelwert der Summe der Werte im values-Array
        */
        {
            long sum = 0;
            for (size_t i = 0; i < (sizeof(values) / 4); i++)
            {
                sum += values[i];
            }
            average_value = float(sum) / float(COUNT_VALUES);
        }
    }
};