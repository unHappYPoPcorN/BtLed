#include "Alarm.h"
#include "Led.h"

bool almOn = false;
bool hasSpeed = false;
int alTime;
int alSpeed;

String readFile()
{
    String rStr = "";
    char ai = 0;
    File rfl = SD.open("/alarm.txt");
    while (true)
    {
        ai = rfl.read();
        if (ai == 13 || ai == 255)
            break;
        else
            rStr = rStr + ai;
    }
    rfl.close();
    return rStr;
}

void writeFile(String wStr)
{
    File wfl = SD.open("/alarm.txt", FILE_WRITE);
    if (wfl.print(wStr))
    {
        Serial.println("File write success");
    }
    else
    {
        Serial.println("File write failed");
    }

    Serial.println("alm : ");
    String inStr = readFile();
    Serial.println(inStr);

    wfl.close();
}

void initAlarm()
{
    String iStr = "";
    char ai = 0;
    fl = SD.open("/alarm.txt");
    while (true)
    {
        {
            ai = fl.read();
            if (ai == 13 || ai == 255)
            {
                alm.hr = iStr.substring(0, iStr.indexOf("H")).toInt();
                alm.mn = iStr.substring(iStr.indexOf("H") + 1, iStr.indexOf("M")).toInt();
                alm.on = iStr.substring(iStr.indexOf("M") + 1, iStr.indexOf("O")).toInt();
                alm.speed = iStr.substring(iStr.indexOf("O") + 1, iStr.indexOf("S")).toInt();
                almList.add(alm);
                break;
            }
            else if (ai == 10)
            {
                alm.hr = iStr.substring(0, iStr.indexOf("H")).toInt();
                alm.mn = iStr.substring(iStr.indexOf("H") + 1, iStr.indexOf("M")).toInt();
                alm.on = iStr.substring(iStr.indexOf("M") + 1, iStr.indexOf("O")).toInt();
                alm.speed = iStr.substring(iStr.indexOf("O") + 1, iStr.indexOf("S")).toInt();
                almList.add(alm);
                iStr = "";
            }
            else
            {
                iStr = iStr + ai;
            }
        }
        fl.close();
        Serial.println(alm2Str());
    }
}

void resetAlarm()
{
    Serial.println("reset alarm");
    SD.remove("/alarm.txt");
    fl = SD.open("/alarm.txt", FILE_WRITE);
    if (fl.print(""))
    {
        Serial.println("File write success");
    }
    else
    {
        Serial.println("File write failed");
    }
    fl.close();
    Serial.println(readFile());
}

void addAlarm(String aStr)
{
    alm.hr = aStr.substring(0, aStr.indexOf("H")).toInt();
    alm.mn = aStr.substring(aStr.indexOf("H") + 1, aStr.indexOf("M")).toInt();
    alm.on = aStr.substring(aStr.indexOf("M") + 1, aStr.indexOf("O")).toInt();
    alm.speed = aStr.substring(aStr.indexOf("O") + 1, aStr.indexOf("S")).toInt();
    almList.add(alm);

    writeFile(alm2Str());
}

void removeAlarm(String rStr)
{
    int rhr = rStr.substring(0, rStr.indexOf("H")).toInt();
    int rmn = rStr.substring(rStr.indexOf("H") + 1, rStr.indexOf("M")).toInt();
    int ron = rStr.substring(rStr.indexOf("M") + 1, rStr.indexOf("O")).toInt();
    int rspeed = rStr.substring(rStr.indexOf("O") + 1, rStr.indexOf("S")).toInt();
    int idx = getListIdx(rhr, rmn, ron, rspeed);
    if (idx != -1)
    {
        almList.remove(idx);
    }

    writeFile(alm2Str());
}

void printAlarm()
{
    for (int i = 0; i < almList.size(); i++)
    {
        Serial.println(almList.get(i).hr);
        Serial.println(almList.get(i).mn);
        Serial.println(almList.get(i).on);
        Serial.println(almList.get(i).speed);
    }
}

void checkAlarm()
{
    int wMn;  // 알람 등록 시간
    int waMn; // 알람 시작 시간
    int nown = rtc0.now().hour() * 60 + rtc0.now().minute();
    if (almList.size() > 0)
    {
        // Serial.println(almList.get(0).hr * 60 + almList.get(0).mn);
    }
    Serial.println(nown);

    for (int i = 0; i < almList.size(); i++)
    {
        wMn = almList.get(i).hr * 60 + almList.get(i).mn;
        if (almList.get(i).on == 1)
        {
            if (wMn - almList.get(i).speed < 0)
                waMn = wMn + 1440 - almList.get(i).speed;
            else
                waMn = wMn - almList.get(i).speed;
        }
        if (waMn == nown)
        {
            Serial.println("wake up!");
            almOn = true;
            if (almList.get(i).on == 1)
            {
                hasSpeed = true;
                alSpeed = almList.get(i).speed;
            }
            else
                hasSpeed = false;
        }
    }
}