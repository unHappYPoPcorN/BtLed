#ifndef _LED_H_
#define _LED_H_

#include <SPI.h>
#include <SD.h>

#define led_r 27
#define led_g 26
#define led_b 25

typedef struct
{
    int r;
    int g;
    int b;
} RGB;

extern RGB rgb;
extern float vo;

void changeColor(RGB cRgb, float cVo);
void changeStrColor(String cStr);
RGB changeRGB(String hx);
void changeVolume(float cVol);
void setColor(int red, int green, int blue);

#endif
