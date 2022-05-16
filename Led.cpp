#include "Led.h"

RGB curRgb = {0, 0, 0};

void changeColor(RGB cRgb, float cVo)
{
    curRgb = cRgb;
    // Serial.println((String)(cRgb.r));
    setColor(int(cRgb.r * cVo / 100), int(cRgb.g * cVo / 100), int(cRgb.b * cVo / 100));
}

void changeStrColor(String cStr)
{
    RGB sRgb = changeRGB(cStr.substring(2, cStr.length()));
    changeColor(sRgb, 100);
}

RGB changeRGB(String hx)
{
    long numb = (long)strtol(&hx[0], NULL, 16);
    RGB rgbh = {0, 0, 0};
    rgbh.r = numb >> 16;
    rgbh.g = numb >> 8 & 0xFF;
    rgbh.b = numb & 0xFF;
    curRgb = rgbh;

    return rgbh;
}

void changeVolume(float cVol)
{
    Serial.println(cVol);
    changeColor(curRgb, cVol);
}

void setColor(int red, int green, int blue)
{
    ledcWrite(0, red); // analogWrite(Pin, value) -> ledcWrite(ledChannel, value)
    ledcWrite(1, green);
    ledcWrite(2, blue);
}