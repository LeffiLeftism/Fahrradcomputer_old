/****** Includes ******/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Zonen.h"

/****** Defines ******/
#define RASPPI_PICO

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET 7        // Reset pin #
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define ButtonPin1 27
#define ButtonPin2 26
#define ButtonPin3 20

#ifdef RASPPI_PICO
MbedI2C I2C_base(4, 5);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_base, OLED_RESET);
#endif

/****** Globale Variablen ******/

int activeZone = 0;
int activeScreen = 0;
unsigned int tirediameter = 30;

// example Example Screenzones
// x, y, width, textSize, textColor, backgroundColor, content/value, decimals, selectable
Zonen mainscreen[] = {
    {0, 0, 69, 3, 1, 0, "88.8", true},    // Zone 1     val km/h
    {75, 2, 11, 1, 1, 0, "km", false},    // Zone 2     txt km
    {75, 11, 11, 1, 1, 0, "/h", false},   // Zone 3     txt /h
    {50, 24, 58, 2, 1, 0, "888.8", true}, // Zone 4     val rpm
    {110, 23, 17, 1, 1, 0, "RPM", false}, // Zone 5     txt RPM
    {110, 32, 17, 1, 1, 0, "Ped", true},  // Zone 6     txt Ped
    {0, 41, 46, 2, 1, 0, "8888", true},   // Zone 7     val power
    {48, 40, 11, 1, 1, 0, "PW", false},   // Zone 8     txt power
    {48, 49, 11, 1, 1, 0, "tt", true},    // Zone 9     txt tt
    {62, 41, 46, 2, 1, 0, "8888", true}}; // Zone 10    val üNN

Zonen tireDia[] = {
    {0, 3, 127, 8, 1, 0, String(tirediameter), true}, // Zone 1
};

uint8_t countZones[] = {
    sizeof(mainscreen) / sizeof(Zonen),
    sizeof(tireDia) / sizeof(Zonen)};

/****** Bitmaps ******/
/**
 * Made with Marlin Bitmap Converter
 * https://marlinfw.org/tools/u8glib/converter.html
 *
 *
 */

#define BIKE_BMPWIDTH 30
#define BIKE_BMPHEIGHT 22
// name, x-position, y-position, width, height, color
const uint8_t bitmapBikeSettings[] = {SCREEN_WIDTH - BIKE_BMPWIDTH, 0, BIKE_BMPWIDTH, BIKE_BMPHEIGHT, 1};

const unsigned char bitmap_bike[][88] = {
    // Bike 1
    {0x01, 0xC0, 0x00, 0x00,
     0x03, 0xE0, 0x00, 0x00,
     0x03, 0x60, 0x00, 0x00,
     0x03, 0xE4, 0x00, 0x00,
     0x01, 0xCF, 0x00, 0x00,
     0x00, 0x1B, 0x80, 0x00,
     0x01, 0xF0, 0xE0, 0x00,
     0x03, 0xE0, 0x70, 0x00,
     0x00, 0x00, 0x30, 0x00,
     0x1F, 0xC1, 0xF7, 0xC0,
     0x3F, 0xE3, 0xCF, 0xE0,
     0x70, 0x73, 0x1C, 0x70,
     0x60, 0x33, 0x38, 0x38,
     0xE0, 0x3B, 0x70, 0x1C,
     0xC0, 0x1B, 0x60, 0x0C,
     0xC0, 0x1B, 0x60, 0x0C,
     0xC0, 0x1B, 0x60, 0x0C,
     0xC0, 0x38, 0x60, 0x0C,
     0xE0, 0x70, 0x70, 0x1C,
     0x70, 0xE0, 0x38, 0x38,
     0x3F, 0xE0, 0x1F, 0xF0,
     0x1F, 0x80, 0x0F, 0xE0},
    // Bike 2
    {0x01, 0xC0, 0x00, 0x00,
     0x03, 0xE0, 0x00, 0x00,
     0x03, 0x60, 0x00, 0x00,
     0x03, 0xE4, 0x00, 0x00,
     0x01, 0xCF, 0x00, 0x00,
     0x00, 0x1B, 0x80, 0x00,
     0x01, 0xF0, 0xE0, 0x00,
     0x03, 0xE0, 0x70, 0x00,
     0x00, 0x03, 0xF0, 0x00,
     0x1F, 0xC7, 0xE7, 0xC0,
     0x3F, 0xE6, 0x0F, 0xE0,
     0x70, 0x76, 0x1C, 0x70,
     0x60, 0x33, 0x38, 0x38,
     0xE0, 0x3B, 0x70, 0x1C,
     0xC0, 0x1B, 0x60, 0x0C,
     0xC0, 0x18, 0x60, 0x0C,
     0xC0, 0x18, 0x60, 0x0C,
     0xC0, 0x38, 0x60, 0x0C,
     0xE0, 0x70, 0x70, 0x1C,
     0x70, 0xE0, 0x38, 0x38,
     0x3F, 0xE0, 0x1F, 0xF0,
     0x1F, 0x80, 0x0F, 0xE0},
    // Bike 1
    {0x01, 0xC0, 0x00, 0x00,
     0x03, 0xE0, 0x00, 0x00,
     0x03, 0x60, 0x00, 0x00,
     0x03, 0xE4, 0x00, 0x00,
     0x01, 0xCF, 0x00, 0x00,
     0x00, 0x1B, 0x80, 0x00,
     0x01, 0xF0, 0xE0, 0x00,
     0x03, 0xE0, 0x70, 0x00,
     0x00, 0x00, 0x30, 0x00,
     0x1F, 0xC1, 0xF7, 0xC0,
     0x3F, 0xE3, 0xCF, 0xE0,
     0x70, 0x73, 0x1C, 0x70,
     0x60, 0x33, 0x38, 0x38,
     0xE0, 0x3B, 0x70, 0x1C,
     0xC0, 0x1B, 0x60, 0x0C,
     0xC0, 0x1B, 0x60, 0x0C,
     0xC0, 0x1B, 0x60, 0x0C,
     0xC0, 0x38, 0x60, 0x0C,
     0xE0, 0x70, 0x70, 0x1C,
     0x70, 0xE0, 0x38, 0x38,
     0x3F, 0xE0, 0x1F, 0xF0,
     0x1F, 0x80, 0x0F, 0xE0},
    // Bike 3
    {0x01, 0xC0, 0x00, 0x00,
     0x03, 0xE0, 0x00, 0x00,
     0x03, 0x60, 0x00, 0x00,
     0x03, 0xE4, 0x00, 0x00,
     0x01, 0xCF, 0x00, 0x00,
     0x00, 0x1B, 0x80, 0x00,
     0x01, 0xF0, 0xE0, 0x00,
     0x03, 0xE0, 0x70, 0x00,
     0x00, 0x00, 0x30, 0x00,
     0x1F, 0xC0, 0x77, 0xC0,
     0x3F, 0xE0, 0xEF, 0xE0,
     0x70, 0x71, 0xDC, 0x70,
     0x60, 0x31, 0xB8, 0x38,
     0xE0, 0x3B, 0x70, 0x1C,
     0xC0, 0x1B, 0x60, 0x0C,
     0xC0, 0x1B, 0x60, 0x0C,
     0xC0, 0x1B, 0x60, 0x0C,
     0xC0, 0x3B, 0x60, 0x0C,
     0xE0, 0x70, 0x70, 0x1C,
     0x70, 0xE0, 0x38, 0x38,
     0x3F, 0xE0, 0x1F, 0xF0,
     0x1F, 0x80, 0x0F, 0xE0}};

#define HOEHENMETER_BMPWIDTH 17
#define HOEHENMETER_BMPHEIGHT 15
// name, x-position, y-position, width, height, color
const uint8_t bitmapHoeheSettings[] = {SCREEN_WIDTH - HOEHENMETER_BMPWIDTH, 41, HOEHENMETER_BMPWIDTH, HOEHENMETER_BMPHEIGHT, 1};

const unsigned char bitmap_hoehenmeter[] = {
    0x00, 0x00, 0x00, // ........................
    0x00, 0x04, 0x00, // .............#..........
    0x71, 0xCE, 0x00, // .###...###..###.........
    0x71, 0xD5, 0x00, // .###...###.#.#.#........
    0x71, 0xC4, 0x00, // .###...###...#..........
    0x71, 0xC4, 0x00, // .###...###...#..........
    0x7F, 0xC4, 0x00, // .#########...#..........
    0x7F, 0xC0, 0x00, // .#########..............
    0x7F, 0xC4, 0x00, // .#########...#..........
    0x71, 0xC4, 0x00, // .###...###...#..........
    0x71, 0xC4, 0x00, // .###...###...#..........
    0x71, 0xD5, 0x00, // .###...###.#.#.#........
    0x71, 0xCE, 0x00, // .###...###..###.........
    0x00, 0x04, 0x00, // .............#..........
    0x00, 0x00, 0x00  // ........................
};

/**** Globale Variablen End ****/

void setup()
{

    /****** PinModes ******/
    pinMode(ButtonPin1, INPUT);
    pinMode(ButtonPin2, INPUT);
    pinMode(ButtonPin3, INPUT);
    pinMode(6, INPUT);
    pinMode(13, OUTPUT);

    /************/

    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        digitalWrite(LED_BUILTIN, HIGH);
        for (;;)
            ; // Don't proceed, loop forever
    }

    // Screen Startup
    display.display();
    delay(2000); // Pause for 2 seconds
    display.clearDisplay();
    display.display();
}

uint32_t bikeAnimationTimer = millis();
uint8_t bikeAnimationBitmap = 0;

void loop()
{
    display.clearDisplay();
    collectSensorData();
    switch (activeScreen)
    {
    case 0:
        updateBitmapCycle(bikeAnimationTimer, bikeAnimationBitmap, 750, mainscreen[3].value, 3);
        printBitmap(bitmap_bike[bikeAnimationBitmap], bitmapBikeSettings);
        printBitmap(bitmap_hoehenmeter, bitmapHoeheSettings);
        printZones(mainscreen, sizeof(mainscreen) / sizeof(Zonen));
        break;
    case 1:
        printZones(tireDia, sizeof(tireDia) / sizeof(Zonen));
        break;
    default:
        break;
    }
}

void updateBitmapCycle(uint32_t &ptimer, uint8_t &variable, uint16_t delay, double value, uint8_t uBound)
{
    if ((millis() - ptimer) > delay && value > 0.0)
    {
        variable++;
        if (variable > uBound)
        {
            variable = 0;
        }
        ptimer = millis();
    }
    else if (value == 0.0)
    {
        variable = 0;
    }
}

void printBitmap(const unsigned char bitmap[], const uint8_t setting[])
{
    display.drawBitmap(setting[0], setting[1], bitmap, setting[2], setting[3], setting[4]);
}

void printZones(Zonen zzones[], int arrayLength)
{
    /*
        Draw all zones
    */
    for (size_t i = 0; i < arrayLength; i++)
    {
        zzones[i].printZone(display);
    }
    /*
        Check which zone is selected and invert it
    */
    buttons(zzones);
    zzones[activeZone].invert(display);
    display.display();
}

/*
    Datenverwaltung, Sammlung, (Speicherung)
*/
void collectSensorData()
{
    /************************************************/
    // Velocity-Sensor ADDR: 0x21 XXX Byte Request
    /*I2C_base.requestFrom(0x21, 5); // Request 5 Byte from I2C-ADDR 0x20
    if (I2C_base.available())
    {
        String i2c_answer;
        while (I2C_base.available())
        {
            char c = I2C_base.read(); // receive a byte as character
            i2c_answer += c;
        }
        mainscreen[0].setContent(i2c_answer);
    }*/
    /************************************************/
    // RPM-Sensor ADDR: 0x20 5 Byte Request
    I2C_base.requestFrom(0x20, 5); // Request 5 Byte from I2C-ADDR 0x20
    if (I2C_base.available())
    {
        String i2c_answer;
        while (I2C_base.available())
        {
            char c = I2C_base.read(); // receive a byte as character
            i2c_answer += c;
        }
        mainscreen[3].setContent(i2c_answer, true); // write sensorvalue in Zonenarray
    }
    /************************************************/
}

void buttons(Zonen zzones[])
{
    // Button 1
    if (digitalRead(ButtonPin1))
    {
        switch (activeScreen)
        {
        case 0:
            changeSelectedZone(0, zzones);
            break;
        case 1:
            tirediameter--;
            tireDia[0].setContent(String(tirediameter), false);
            break;
        default:
            break;
        }
        while (digitalRead(ButtonPin1))
        {
            delay(5);
        }
    }
    // Button 2
    else if (digitalRead(ButtonPin2))
    {
        activeZone = 0;
        switch (activeScreen)
        {
        case 0:
            activeScreen = 1;
            break;

        default:
            activeScreen = 0;
            break;
        }
        while (digitalRead(ButtonPin2))
        {
            delay(5);
        }
    }
    // Button 3
    else if (digitalRead(ButtonPin3))
    {
        switch (activeScreen)
        {
        case 0:
            changeSelectedZone(1, zzones);
            break;
        case 1:
            tirediameter++;
            tireDia[0].setContent(String(tirediameter), false);
            break;
        default:
            break;
        }
        while (digitalRead(ButtonPin3))
        {
            delay(5);
        }
    }
}

void changeSelectedZone(uint8_t direction, Zonen zzones[])
{
    switch (direction)
    {
    case 0:
    flagMinus:
        activeZone--;
        if (activeZone < 0)
        {
            activeZone = countZones[activeScreen] - 1;
        }
        if (!zzones[activeZone].selectable)
        {
            goto flagMinus;
        }

        break;
    case 1:
    flagPlus:
        activeZone++;
        if (activeZone > countZones[activeScreen] - 1)
        {
            activeZone = 0;
        }
        if (!zzones[activeZone].selectable)
        {
            goto flagPlus;
        }
        break;
    default:
        break;
    }
}
