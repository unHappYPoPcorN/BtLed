#ifndef _ALARM_H_
#define _ALARM_H_

#include <RTClib.h>
#include <LinkedList.h>
#include <SPI.h>
#include <SD.h>
#include <TimeLib.h>

#define R_RST 12
#define R_CLK 4
#define R_DAT 15

typedef struct
{
    int hr;
    int mn;
    int on;
    int speed;
} ALM;

ALM alm;
LinkedList<ALM> almList;
File fl;
DS1302 rtc0(R_RST, R_CLK, R_DAT);

extern bool almOn;
extern bool hasSpeed;
extern int alTime;
extern int alSpeed;

String readFile();
void writeFile(String wStr);

void initAlarm();
void resetAlarm();
void addAlarm(String aStr);
void removeAlarm(String rStr);
void printAlarm();
void checkAlarm();

#endif
