/****** Includes ******/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Zonen.h"

/****** Defines ******/
//#define ARDUINO_NANO // Care RAM Usage. Maybe Adafruit_SSD1306 cant be used, when too less RAM available.
#define RASPPI_PICO

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET 7        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define ButtonPin1 27
#define ButtonPin2 26
#define ButtonPin3 20

#ifdef ARDUINO_NANO
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif
#ifdef RASPPI_PICO
MbedI2C I2C_base(4, 5);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_base, OLED_RESET);
#endif

/****** Globale Variablen ******/
int selectedZone = 0;
unsigned int testno = 0;

// example Example Screenzones
// x, y, width, textSize, textColor, backgroundColor, content/value
#define ARRAYLENGTH 7
Zonen Screen_1[ARRAYLENGTH] = {
    {0, 0, 90, 3, 1, 0, "Zone1"},
    {0, 24, 64, 2, 1, 0, "Zone2"},
    {64, 24, 64, 2, 1, 0, 3},
    {0, 40, 64, 2, 1, 0, "Zone4"},
    {64, 40, 64, 2, 1, 0, 5},
    {0, 56, 64, 1, 1, 0, "Zone6"},
    {64, 56, 64, 1, 1, 0, 7}};

/************/

void setup()
{

#ifdef ARDUINO_NANO
    Serial.begin(115200);
#endif

    /****** PinModes ******/
    pinMode(ButtonPin1, INPUT);
    pinMode(ButtonPin2, INPUT);
    pinMode(ButtonPin3, INPUT);
    pinMode(6, INPUT);
    pinMode(13, OUTPUT);

    /************/

    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
#ifdef ARDUINO_NANO
        Serial.println(F("SSD1306 allocation failed"));
#endif
        /*digitalWrite(LED_BUILTIN, HIGH);
        for (;;)
            ; // Don't proceed, loop forever*/
    }
    else
    {
#ifdef ARDUINO_NANO
        Serial.println(F("SSD1306 allocation successful"));
#endif
    }

    // Screen Startup
    display.display();
    delay(2000); // Pause for 2 seconds
    display.clearDisplay();
    display.display();
}

void loop()
{
    showZones(Screen_1, ARRAYLENGTH);
    /*
    display.clearDisplay();
    display.setCursor(1, 1);
    display.setTextSize(1);
    display.setTextColor(1);
    display.print("IC2");
    display.display();
    IC2_1.requestFrom(0x20, 6);
    if (IC2_1.available())
    {
        display.setCursor(10, 10);
        display.print("IC2_1 is Available Nr:");
        display.println(testno);
        testno++;
        while (IC2_1.available())
        {
            char c = IC2_1.read(); // receive a byte as character
            display.print(c);
        }
    }

    display.display();
    delay(2000);
    */
}

void showZones(Zonen zzones[], int arrayLength)
{
    display.clearDisplay();
    for (size_t i = 0; i < arrayLength; i++)
    {
        zzones[i].printZone(display);
#ifdef ARDUINO_NANO
        Serial.println(zzones[i].content);
#endif
    }
    checkSelectedZone(arrayLength);
    zzones[selectedZone].invert(display);
    display.display();
}

void checkSelectedZone(unsigned int length)
{
    if (digitalRead(ButtonPin1))
    {
        selectedZone--;
        if (selectedZone < 0)
        {
            selectedZone = length - 1;
        }

        while (digitalRead(ButtonPin1))
        {
            delay(5);
        }
    }
    else if (digitalRead(ButtonPin2))
    {
        while (digitalRead(ButtonPin2))
        {
            delay(5);
        }
    }
    else if (digitalRead(ButtonPin3))
    {
        selectedZone++;
        if (selectedZone > length - 1)
        {
            selectedZone = 0;
        }
        while (digitalRead(ButtonPin3))
        {
            delay(5);
        }
    }
}
