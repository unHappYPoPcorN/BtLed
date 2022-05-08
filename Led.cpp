#include "Led.h"

RGB rgb = {0, 0, 0};
float vo = 100;

void changeColor()
{
    setColor(int(rgb.r * vo / 100), int(rgb.g * vo / 100), int(rgb.b * vo / 100));
    Serial.println((String)rgb.r + ' ' + rgb.g + ' ' + rgb.b);
}

void changeStrColor(String cStr)
{
    rgb = changeRGB(cStr.substring(2, cStr.length()));
    changeColor();
}

RGB changeRGB(String hx)
{
    long numb = (long)strtol(&hx[0], NULL, 16);
    RGB rgbh = {0, 0, 0};
    rgbh.r = numb >> 16;
    rgbh.g = numb >> 8 & 0xFF;
    rgbh.b = numb & 0xFF;

    return rgbh;
}

void setColor(int red, int green, int blue)
{
    ledcWrite(0, red); // analogWrite(Pin, value) -> ledcWrite(ledChannel, value)
    ledcWrite(1, green);
    ledcWrite(2, blue);
}