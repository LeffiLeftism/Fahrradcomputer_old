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
    double value = 0;
    uint8_t decimals = 0;
    bool selectable;

public:
    Zonen(int zx, int zy, int zwidth, int ztextSize, int ztextColor, int zbackgroundColor, String zcontent, bool zselectable) // Constructor
    {
        x = zx;
        y = zy;
        width = zwidth;
        height = ztextSize * 7 + 1;
        textSize = ztextSize;
        textColor = ztextColor;
        backgroundColor = zbackgroundColor;
        content = zcontent;
        selectable = zselectable;
    }
    void setContent(String zcontent, bool convertToValue)
    {
        if (convertToValue)
        {
            String valueStr;
            for (uint8_t i = 0; i < zcontent.length(); i++)
            {
                if (isDigit(zcontent[i]))
                {
                    for (uint8_t n = i; n < zcontent.length(); n++)
                    {
                        valueStr += zcontent[n];
                    }
                    i = zcontent.length();
                }
            }
            value = valueStr.toDouble();
        }
        content = zcontent;
    }
    void printZone(Adafruit_SSD1306 &zdisplay)
    {
        zdisplay.fillRect(x, y, width, height, backgroundColor);
        zdisplay.setCursor(x, y);
        zdisplay.setTextSize(textSize);
        zdisplay.setTextColor(textColor);
        zdisplay.print(content);
    }
    void invert(Adafruit_SSD1306 &zdisplay)
    {
        zdisplay.fillRect(x, y, width, height, 2);
    }
};
