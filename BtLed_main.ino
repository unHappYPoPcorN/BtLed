#include <RTClib.h>
#include "BluetoothSerial.h"
#include <LinkedList.h>
#include <SPI.h>
#include <SD.h>
#include <TimeLib.h>
#include <vector>

#include "Alarm.h"
#include "Led.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define inLed 2 // 내장 LED 핀

String str = "";
String tStr;
String alStr = "";
int almCount = 0;
int timeVal = 0;
char i = 0;
int nhr, nmn;
char buf[20];

BluetoothSerial SerialBT;

void setup()
{
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); // Bluetooth device name
  Serial.println("Initializing SD card...");

  if (!SD.begin(5))
  {                                           // SD카드 모듈을 초기화합니다.
    Serial.println("initialization failed!"); // SD카드 모듈 초기화에 실패하면 에러를 출력합니다.
  }
  else
    Serial.println("Sd card initialization done.");

  if (!SD.exists("/alarm.txt"))
  {
    Serial.println("Opening file failed");
    fl.close();
    fl = SD.open("/alarm.txt", FILE_WRITE);
    if (fl.print(""))
    {
      Serial.println("File write success");
    }
    else
    {
      Serial.println("File write failed");
    }
  }
  else
  {
    initAlarm();
  }

  rtc0.begin();
  rtc0.adjust(DateTime(__DATE__, __TIME__));

  if (!rtc0.isrunning())
    Serial.println("RTC is NOT running!");

  Serial.println("The device started, now you can pair it with bluetooth!");

  ledcAttachPin(led_r, 0);
  ledcAttachPin(led_g, 1);
  ledcAttachPin(led_b, 2);

  ledcSetup(0, 5000, 8); // R 채널 channel: 0, 5000Hz, 8bits = 256(0 ~ 255)
  ledcSetup(1, 5000, 8); // G 채널
  ledcSetup(2, 5000, 8); // B 채널
}

void loop()
{
  if (Serial.available())
    SerialBT.write(Serial.read());

  if (SerialBT.available())
  {
    i = SerialBT.read();
    if (i == 10)
    {
      getStr(str);
      SerialBT.println(str);
      str = "";
    }
    else
    {
      str += String(i);
    }
  }
  if (millis() - timeVal >= 1000) // 1초단위로 세기
  {
    timeVal = millis();
    checkAlarm();
    turnAlarm();
    // 카운트 동작;
  }
}

void getStr(String strn)
{
  Serial.println(strn);
  String str1 = str.substring(0, str.indexOf("_"));
  String str2 = str.substring(str.indexOf("_") + 1, str.length());

  if (str1.equals("C"))
  {
    changeStrColor(str2);
  }
  else if (str1.equals("L"))
  {
    String strH = str2.substring(7, str2.indexOf("H"));
    String strM = str2.substring(str2.indexOf("H") + 1, str2.indexOf("M"));
    String strO = str2.substring(str2.indexOf("M") + 1, str2.indexOf("O"));
    String strS = str2.substring(str2.indexOf("O") + 1, str2.indexOf("S"));
    if (str2.substring(0, 6).equals("insert"))
    {
      addAlarm(str2.substring(7, str2.length()));
    }
    else if (str2.substring(0, 6).equals("remove"))
    {
      removeAlarm(str2.substring(7, str2.length()));
    }
    Serial.println(alm2Str());
  }
  else if (str1.equals("I"))
  {
    resetAlarm();
  }
  else if (str1.equals("T"))
  {
    tStr = getTime();
    SerialBT.println(tStr);
    Serial.println(tStr);
    setTime(str2);
  }
  else
  {
    float cVol = str2.toFloat();
    changeVolume(cVol);
  }
}

int getListIdx(int ghr, int gmn, int gon, int gspeed)
{
  for (int i = 0; i < almList.size(); i++)
  {
    if (almList.get(i).hr == ghr && almList.get(i).mn == gmn && almList.get(i).on == gon && almList.get(i).speed == gspeed)
      return i;
  }
  return -1;
}

String alm2Str()
{
  String aStr = "";
  for (int i = 0; i < almList.size(); i++)
  {
    aStr += String(almList.get(i).hr);
    aStr += "H";
    aStr += String(almList.get(i).mn);
    aStr += "M";
    aStr += String(almList.get(i).on);
    aStr += "O";
    aStr += String(almList.get(i).speed);
    aStr += "S";
    if ((i + 1) != almList.size())
      aStr += "\n";
  }
  return aStr;
}

String getTime()
{
  String tStr = "";
  tStr += rtc0.now().tostr(buf);
  return tStr;
}

void setTime(String st)
{
  int syear = st.substring(0, 4).toInt();
  int smonth = st.substring(5, 7).toInt();
  int sday = st.substring(8, 10).toInt();
  int shour = st.substring(11, 13).toInt();
  int smin = st.substring(14, 16).toInt();
  int ssec = st.substring(17, 19).toInt();

  DateTime newDt(syear, smonth, sday, shour, smin, ssec);

  rtc0.adjust(newDt);

  Serial.print(rtc0.now().hour() + " ");
  Serial.print(rtc0.now().minute() + "\n");
}

void wakeUpAlarm()
{
}