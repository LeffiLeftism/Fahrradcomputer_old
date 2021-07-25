/****** Includes ******/
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/****** Structure ******/
struct Zonen
{
public:
    int x;               // x-Position
    int y;               // y-Position
    int width;           // Zone width
    int height;          // Zone height
    int textSize;        // 1-3
    int textColor;       // 0 = Black, 1 = White, 2 = Inverse
    int backgroundColor; // 0 = Black, 1 = White, 2 = Inverse
    String content;
    double value;
    bool b_value; // defines, if content or value will be print

public:
    Zonen(int zx, int zy, int zwidth, int ztextSize, int ztextColor, int zbackgroundColor, String zcontent) // Constructor
    {
        x = zx;
        y = zy;
        width = zwidth;
        height = ztextSize * 7 + 1;
        textSize = ztextSize;
        textColor = ztextColor;
        backgroundColor = zbackgroundColor;
        content = zcontent;
        b_value = false;
    }
    Zonen(int zx, int zy, int zwidth, int ztextSize, int ztextColor, int zbackgroundColor, double zvalue) // Constructor
    {
        x = zx;
        y = zy;
        width = zwidth;
        height = ztextSize * 7 + 1;
        textSize = ztextSize;
        textColor = ztextColor;
        backgroundColor = zbackgroundColor;
        value = zvalue;
        b_value = true;
    }
    void setContent(String zcontent)
    {
        content = zcontent;
        b_value = false;
    }
    void setValue(double zvalue)
    {
        value = zvalue;
        b_value = true;
    }
    void printZone(Adafruit_SSD1306 &zdisplay)
    {
        zdisplay.fillRect(x, y, width, height, backgroundColor);
        zdisplay.setCursor(x, y);
        zdisplay.setTextSize(textSize);
        zdisplay.setTextColor(textColor);
        if (b_value)
        {
            zdisplay.print(value);
        }
        else
        {
            zdisplay.print(content);
        }
    }
    void invert(Adafruit_SSD1306 &zdisplay)
    {
        zdisplay.fillRect(x, y, width, height, 2);
    }
};
