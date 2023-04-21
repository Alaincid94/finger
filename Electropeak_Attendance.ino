#include <SPI.h>
#include <EEPROM.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <FreeDefaultFonts.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include "RTClib.h"
#include "icons.h"


#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

#define ORANGE      0xFD20
#define BLACK       0x0000
#define BLUE        0x001F
#define RED         0xF800
#define YELLOW      0xFFE0
#define WHITE       0xFFFF
#define ELECTRO     0x8865
#define ENTER       0x6FA7
#define GREEN       0x07E0
#define EXIT        0xD186

SoftwareSerial mySerial(A12, A13);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);



File myFile;

MCUFRIEND_kbv tft;

RTC_DS1307 rtc;
DateTime now;
DateTime Time_Enter1;
DateTime Time_Exit1;
DateTime Time_Enter2;
DateTime Time_Exit2;
DateTime Time_Enter3;
DateTime Time_Exit3;
DateTime Time_Enter4;
DateTime Time_Exit4;
DateTime Time_Enter5;
DateTime Time_Exit5;
DateTime Time_Enter6;
DateTime Time_Exit6;
DateTime Time_Enter7;
DateTime Time_Exit7;
DateTime Time_Enter8;
DateTime Time_Exit8;
DateTime Time_Enter9;
DateTime Time_Exit9;
DateTime Time_Enter10;
DateTime Time_Exit10;
DateTime Time_Enter11;
DateTime Time_Exit11;
DateTime Time_Enter12;
DateTime Time_Exit12;
DateTime Time_Enter13;
DateTime Time_Exit13;
DateTime Time_Enter14;
DateTime Time_Exit14;
DateTime Time_Enter15;
DateTime Time_Exit15;

const int RED_LED = 23;
const int GREEN_LED = 25;
const int BUZZER = 29;
const int BUT = 31;

const int ID1 = 1, ID2 = 2, ID3 = 3, ID4 = 4, ID5 = 5;
const int ID6 = 6, ID7 = 7, ID8 = 8, ID9 = 9, ID10 = 10;
const int ID11 = 11, ID12 = 12, ID13 = 13, ID14 = 14, ID15 = 15;

int up_data = 1;
int upload_data = 1;
int query = 0;

int presence[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int min_old = 0;
int Finger_ID = 0;
int id = 0;

String Final = "";

String Name_ID1 = "User1";
String Name_ID2 = "User2";
String Name_ID3 = "User3";
String Name_ID4 = "User4";
String Name_ID5 = "User5";
String Name_ID6 = "User6";
String Name_ID7 = "User7";
String Name_ID8 = "User8";
String Name_ID9 = "User9";
String Name_ID10 = "User10";
String Name_ID11 = "User11";
String Name_ID12 = "";
String Name_ID13 = "";
String Name_ID14 = "";
String Name_ID15 = "";

String WT = "";

int ID1_Enter_Hour = 0;
int ID1_Enter_Min = 0;
int ID2_Enter_Hour = 0;
int ID2_Enter_Min = 0;
int ID3_Enter_Hour = 0;
int ID3_Enter_Min = 0;
int ID4_Enter_Hour = 0;
int ID4_Enter_Min = 0;
int ID5_Enter_Hour = 0;
int ID5_Enter_Min = 0;
int ID6_Enter_Hour = 0;
int ID6_Enter_Min = 0;
int ID7_Enter_Hour = 0;
int ID7_Enter_Min = 0;
int ID8_Enter_Hour = 0;
int ID8_Enter_Min = 0;
int ID9_Enter_Hour = 0;
int ID9_Enter_Min = 0;
int ID10_Enter_Hour = 0;
int ID10_Enter_Min = 0;
int ID11_Enter_Hour = 0;
int ID11_Enter_Min = 0;
int ID12_Enter_Hour = 0;
int ID12_Enter_Min = 0;
int ID13_Enter_Hour = 0;
int ID13_Enter_Min = 0;
int ID14_Enter_Hour = 0;
int ID14_Enter_Min = 0;
int ID15_Enter_Hour = 0;
int ID15_Enter_Min = 0;

int Exit_Hour = 0;
int Exit_Min = 0;

void Get_from_EEPROM()
{
  EEPROM.get(100, ID1_Enter_Hour);
  EEPROM.get(105, ID1_Enter_Min);
  EEPROM.get(110, ID2_Enter_Hour);
  EEPROM.get(115, ID2_Enter_Min);
  EEPROM.get(120, ID3_Enter_Hour);
  EEPROM.get(125, ID3_Enter_Min);
  EEPROM.get(130, ID4_Enter_Hour);
  EEPROM.get(135, ID4_Enter_Min);
  EEPROM.get(140, ID5_Enter_Hour);
  EEPROM.get(145, ID5_Enter_Min);
  EEPROM.get(150, ID6_Enter_Hour);
  EEPROM.get(155, ID6_Enter_Min);
  EEPROM.get(160, ID7_Enter_Hour);
  EEPROM.get(165, ID7_Enter_Min);
  EEPROM.get(170, ID8_Enter_Hour);
  EEPROM.get(175, ID8_Enter_Min);
  EEPROM.get(180, ID9_Enter_Hour);
  EEPROM.get(185, ID9_Enter_Min);
  EEPROM.get(190, ID10_Enter_Hour);
  EEPROM.get(195, ID10_Enter_Min);
  EEPROM.get(200, ID11_Enter_Hour);
  EEPROM.get(205, ID11_Enter_Min);
  EEPROM.get(210, ID12_Enter_Hour);
  EEPROM.get(215, ID12_Enter_Min);
  EEPROM.get(220, ID13_Enter_Hour);
  EEPROM.get(225, ID13_Enter_Min);
  EEPROM.get(230, ID14_Enter_Hour);
  EEPROM.get(235, ID14_Enter_Min);
  EEPROM.get(240, ID15_Enter_Hour);
  EEPROM.get(245, ID15_Enter_Min);
  Serial.println("EEPROM Update");
}

void Check_Presence()
{
  if (ID1_Enter_Hour > 0) presence[1] = 1;
  if (ID2_Enter_Hour > 0) presence[2] = 1;
  if (ID3_Enter_Hour > 0) presence[3] = 1;
  if (ID4_Enter_Hour > 0) presence[4] = 1;
  if (ID5_Enter_Hour > 0) presence[5] = 1;
  if (ID6_Enter_Hour > 0) presence[6] = 1;
  if (ID7_Enter_Hour > 0) presence[7] = 1;
  if (ID8_Enter_Hour > 0) presence[8] = 1;
  if (ID9_Enter_Hour > 0) presence[9] = 1;
  if (ID10_Enter_Hour > 0) presence[10] = 1;
  if (ID11_Enter_Hour > 0) presence[11] = 1;
  if (ID12_Enter_Hour > 0) presence[12] = 1;
  if (ID13_Enter_Hour > 0) presence[13] = 1;
  if (ID14_Enter_Hour > 0) presence[14] = 1;
  if (ID15_Enter_Hour > 0) presence[15] = 1;
  Serial.println("Presence Update");
}


void LCD_Print(int x, int y, int col, int sz, String msg)
{
  int16_t x1, y1;
  uint16_t wid, ht;
  tft.setFont(&FreeSans9pt7b);
  tft.setCursor(x, y);
  tft.setTextColor(col);
  tft.setTextSize(sz);
  tft.print(msg);
}

void LCD_Setup_Menu()
{
  int r = 255, g = 255, b = 255;
  for (int j = 0; j < 20; j++) {
    tft.setFont(&FreeSans12pt7b);
    tft.setCursor(125, 125);
    tft.setTextSize(2);
    int color = tft.color565(r -= 12, g -= 12, b -= 12);
    tft.setTextColor(color);
    tft.print("Electropeak");
    delay(250);
  }
}

void LCD_Wich_User(int wid)
{
  switch (wid)
  {
    case 1:
      LCD_Print(150, 50, BLACK, 2,  Name_ID1);
      break;
    case 2:
      LCD_Print(150, 50, BLACK, 2,  Name_ID2);
      break;
    case 3:
      LCD_Print(155, 50, BLACK, 2,  Name_ID3);
      break;
    case 4:
      LCD_Print(150, 50, BLACK, 2,  Name_ID4);
      break;
    case 5:
      LCD_Print(150, 50, BLACK, 2,  Name_ID5);
      break;
    case 6:
      LCD_Print(150, 50, BLACK, 2,  Name_ID6);
      break;
    case 7:
      LCD_Print(150, 50, BLACK, 2,  Name_ID7);
      break;
    case 8:
      LCD_Print(150, 50, BLACK, 2,  Name_ID8);
      break;
    case 9:
      LCD_Print(145, 50, BLACK, 2,  Name_ID9);
      break;
    case 10:
      LCD_Print(145, 50, BLACK, 2,  Name_ID10);
      break;
    case 11:
      LCD_Print(150, 50, BLACK, 2,  Name_ID10);
      break;
    case 12:
      LCD_Print(150, 50, BLACK, 2,  Name_ID12);
      break;
    case 13:
      LCD_Print(150, 50, BLACK, 2,  Name_ID13);
      break;
    case 14:
      LCD_Print(150, 50, BLACK, 2,  Name_ID14);
      break;
    case 15:
      LCD_Print(150, 50, BLACK, 2,  Name_ID15);
      break;
  }
}

void LCD_Enter_User(int f_ID)
{
  now = rtc.now();
  String En = String(now.hour()) + ":" + String(now.minute());
  tft.fillScreen(GREEN);
  LCD_Wich_User(f_ID);
  tft.drawBitmap(165, 85, wel, 150, 150, 0x0000);
  tft.drawBitmap(130, 250, login, 50, 50, 0x0000);
  LCD_Print(200, 285, BLACK, 2, En);

}

void LCD_Exit_User(int f_ID)
{
  now = rtc.now();
  String En = String(now.hour()) + ":" + String(now.minute());
  tft.fillScreen(EXIT);
  LCD_Wich_User(f_ID);
  LCD_Print(150, 115, BLACK, 2,  "GOODBYE");
  tft.drawBitmap(130, 148, logout, 50, 50, 0x0000);
  LCD_Print(200, 180, BLACK, 2, En);
  tft.drawBitmap(130, 200, work2, 50, 50, 0x0000);
  LCD_Print(200, 240, BLACK, 2, WT);
}

void showmsgXY(int x, int y, int sz, const GFXfont *f, const char *msg)
{
  int16_t x1, y1;
  uint16_t wid, ht;
  tft.setFont(f);
  tft.setCursor(x, y);
  tft.setTextColor(WHITE);
  tft.setTextSize(sz);
  tft.print(msg);
  delay(1000);
}

void LCD_Main_Menu()
{
  int p = 0;
  String watch = "";
  String today = "";

  now = rtc.now();
  for (int i = 0; i < 16; i++)
  {
    if (presence[i] == 1) p++;
  }

  watch = String(now.hour()) + ":" + String(now.minute());
  today = String(now.year()) + "/" + String(now.month()) + "/" + String(now.day());
  tft.fillScreen(ELECTRO);
  //tft.drawBitmap(420, 10, wifi, 50, 50, 0x0000);
  // tft.drawBitmap(420, 10, nowifi, 50, 50, 0x0000);
  showmsgXY(0, 50, 2, &FreeSansBold12pt7b, "Electropeak");
  tft.drawBitmap(100, 80, wt, 50, 50, 0x0000);
  LCD_Print(165, 112, WHITE, 2,  watch);
  tft.drawBitmap(100, 130, dat, 45, 48, 0x0000);
  LCD_Print(165, 162, WHITE, 2,  today);
  tft.drawBitmap(100, 180, user, 50, 42, 0x0000);
  LCD_Print(165, 212, WHITE, 2,  String(p));
}

void LCD_Finger_Error()
{
  tft.fillScreen(RED);
  LCD_Print(110, 100, BLUE, 2,  "User not found :(");
  LCD_Print(220, 135, BLUE, 1,  "OR");
  LCD_Print(90, 170, BLUE, 2,  "Image is too messy");
  LCD_Print(160, 220, BLUE, 2,  "Try again");
  delay(1500);
  LCD_Main_Menu();
}

void LCD_Upload_Menue()
{
  tft.fillScreen(YELLOW);
  LCD_Print(110, 100, BLACK, 2,  "Uploading...");
  LCD_Print(90, 170, BLACK, 2,  "Please waite :)");
}


int getFingerprintIDez()
{
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return 255;
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}

void Enroll()
{
  Serial.println("Ready to enroll a fingerprint!");
  Serial.println("Please type in the ID # (from 1 to 15) you want to save this finger as...");
  id = readnumber();
  if (id == 0)
  {
    return;
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);
  while (!  getFingerprintEnroll() );
}

int getFingerprintEnroll()
{
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID ");
  Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");
  Serial.println(id);
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID ");
  Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    Serial.println("Type your user name: ");
    Get_User_Name(id);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }
}

int readnumber(void)
{
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void Get_User_Name(int finger_id)
{
  while (! Serial.available());
  switch (finger_id)
  {
    case 1:
      Name_ID1 = Serial.readString();
      break;
    case 2:
      Name_ID2 = Serial.readString();
      break;
    case 3:
      Name_ID3 = Serial.readString();
      break;
    case 4:
      Name_ID4 = Serial.readString();
      break;
    case 5:
      Name_ID5 = Serial.readString();
      break;
    case 6:
      Name_ID6 = Serial.readString();
      break;
    case 7:
      Name_ID7 = Serial.readString();
      break;
    case 8:
      Name_ID8 = Serial.readString();
      break;
    case 9:
      Name_ID9 = Serial.readString();
      break;
    case 10:
      Name_ID10 = Serial.readString();
      break;
    case 11:
      Name_ID11 = Serial.readString();
      break;
    case 12:
      Name_ID12 = Serial.readString();
      break;
    case 13:
      Name_ID13 = Serial.readString();
      break;
    case 14:
      Name_ID14 = Serial.readString();
      break;
    case 15:
      Name_ID15 = Serial.readString();
      break;
    default:
      Serial.println("Unknown Error");
      break;
  }
}

void Enter_User(int uid)
{
  presence[uid] = 1;
  switch (uid)
  {
    case ID1:
      LCD_Enter_User(ID1);
      Time_Enter1 = rtc.now();
      ID1_Enter_Hour = Time_Enter1.hour();
      ID1_Enter_Min = Time_Enter1.minute();
      EEPROM.put(100, ID1_Enter_Hour);
      EEPROM.put(105, ID1_Enter_Min);
      delay(4000);
      break;
    case ID2:
      LCD_Enter_User(ID2);
      Time_Enter2 = rtc.now();
      ID2_Enter_Hour = Time_Enter2.hour();
      ID2_Enter_Min = Time_Enter2.minute();
      EEPROM.put(110, ID2_Enter_Hour);
      EEPROM.put(115, ID2_Enter_Min);
      delay(4000);
      break;
    case ID3:
      LCD_Enter_User(ID3);
      Time_Enter3 = rtc.now();
      ID3_Enter_Hour = Time_Enter3.hour();
      ID3_Enter_Min = Time_Enter3.minute();
      EEPROM.put(120, ID3_Enter_Hour);
      EEPROM.put(125, ID3_Enter_Min);
      delay(4000);
      break;
    case ID4:
      LCD_Enter_User(ID4);
      Time_Enter4 = rtc.now();
      ID4_Enter_Hour = Time_Enter4.hour();
      ID4_Enter_Min = Time_Enter4.minute();
      EEPROM.put(130, ID4_Enter_Hour);
      EEPROM.put(135, ID4_Enter_Min);
      delay(4000);
      break;
    case ID5:
      LCD_Enter_User(ID5);
      Time_Enter5 = rtc.now();
      ID5_Enter_Hour = Time_Enter5.hour();
      ID5_Enter_Min = Time_Enter5.minute();
      EEPROM.put(140, ID5_Enter_Hour);
      EEPROM.put(145, ID5_Enter_Min);
      delay(4000);
      break;
    case ID6:
      LCD_Enter_User(ID6);
      Time_Enter6 = rtc.now();
      ID6_Enter_Hour = Time_Enter6.hour();
      ID6_Enter_Min = Time_Enter6.minute();
      EEPROM.put(150, ID6_Enter_Hour);
      EEPROM.put(155, ID6_Enter_Min);
      delay(4000);
      break;
    case ID7:
      LCD_Enter_User(ID7);
      Time_Enter7 = rtc.now();
      ID7_Enter_Hour = Time_Enter7.hour();
      ID7_Enter_Min = Time_Enter7.minute();
      EEPROM.put(160, ID7_Enter_Hour);
      EEPROM.put(165, ID7_Enter_Min);
      delay(4000);
      break;
    case ID8:
      LCD_Enter_User(ID8);
      Time_Enter8 = rtc.now();
      ID8_Enter_Hour = Time_Enter8.hour();
      ID8_Enter_Min = Time_Enter8.minute();
      EEPROM.put(170, ID8_Enter_Hour);
      EEPROM.put(175, ID8_Enter_Min);
      delay(4000);
      break;
    case ID9:
      LCD_Enter_User(ID9);
      Time_Enter9 = rtc.now();
      ID9_Enter_Hour = Time_Enter9.hour();
      ID9_Enter_Min = Time_Enter9.minute();
      EEPROM.put(180, ID9_Enter_Hour);
      EEPROM.put(185, ID9_Enter_Min);
      delay(4000);
      break;
    case ID10:
      LCD_Enter_User(ID10);
      Time_Enter10 = rtc.now();
      ID10_Enter_Hour = Time_Enter10.hour();
      ID10_Enter_Min = Time_Enter10.minute();
      EEPROM.put(190, ID10_Enter_Hour);
      EEPROM.put(195, ID10_Enter_Min);
      delay(4000);
      break;
    case ID11:
      LCD_Enter_User(ID11);
      Time_Enter11 = rtc.now();
      ID11_Enter_Hour = Time_Enter11.hour();
      ID11_Enter_Min = Time_Enter11.minute();
      EEPROM.put(200, ID11_Enter_Hour);
      EEPROM.put(205, ID11_Enter_Min);
      delay(4000);
      break;
    case ID12:
      LCD_Enter_User(ID12);
      Time_Enter12 = rtc.now();
      ID12_Enter_Hour = Time_Enter12.hour();
      ID12_Enter_Min = Time_Enter12.minute();
      EEPROM.put(210, ID12_Enter_Hour);
      EEPROM.put(215, ID12_Enter_Min);
      delay(4000);
      break;
    case ID13:
      LCD_Enter_User(ID1);
      Time_Enter13 = rtc.now();
      ID13_Enter_Hour = Time_Enter13.hour();
      ID13_Enter_Min = Time_Enter13.minute();
      EEPROM.put(220, ID13_Enter_Hour);
      EEPROM.put(225, ID13_Enter_Min);
      delay(4000);
      break;
    case ID14:
      LCD_Enter_User(ID1);
      Time_Enter14 = rtc.now();
      ID14_Enter_Hour = Time_Enter14.hour();
      ID14_Enter_Min = Time_Enter14.minute();
      EEPROM.put(230, ID14_Enter_Hour);
      EEPROM.put(235, ID14_Enter_Min);
      delay(4000);
      break;
    case ID15:
      LCD_Enter_User(ID15);
      Time_Enter15 = rtc.now();
      ID15_Enter_Hour = Time_Enter15.hour();
      ID15_Enter_Min = Time_Enter15.minute();
      EEPROM.put(240, ID15_Enter_Hour);
      EEPROM.put(245, ID15_Enter_Min);
      delay(4000);
      break;

  }
  LCD_Main_Menu();
}


void Exit_User(int uid)
{
  int work_time = 0, wh = 0, wm = 0;
  presence[uid] = 0;
  switch (uid)
  {
    case ID1:
      Time_Exit1 = rtc.now();
      work_time = (Time_Exit1.hour() * 60 + Time_Exit1.minute()) - ((ID1_Enter_Hour * 60) + ID1_Enter_Min);
      wh = work_time / 60;
      wm = work_time % 60;
      WT = String(wh) + ":" + String(wm);
      Serial.print(Time_Exit1.hour());
      Serial.print(":");
      Serial.println(Time_Exit1.minute());
      Make_String(ID1, Time_Exit1.day(), Time_Exit1.month(), Time_Exit1.year(), ID1_Enter_Hour, ID1_Enter_Min, Time_Exit1.hour(), Time_Exit1.minute(), WT);
      Serial.println(Final);
      Writ_to_Main_File(Final, ID1);
      query++;
      if (query > 255) query = 1;
      Write_to_Upload_File(query, Final);
      LCD_Exit_User(ID1);
      //Uploading();
      EEPROM.put(100, 0);
      EEPROM.put(105, 0);
      delay(3000);
      break;
    case ID2:
      Time_Exit2 = rtc.now();
      work_time = (Time_Exit2.hour() * 60 + Time_Exit2.minute()) - ((ID2_Enter_Hour * 60) + ID2_Enter_Min);
      wh = work_time / 60;
      wm = work_time % 60;
      WT = String(wh) + ":" + String(wm);
      Make_String(ID2, Time_Exit1.day(), Time_Exit1.month(), Time_Exit1.year(), ID1_Enter_Hour, ID1_Enter_Min, Time_Exit1.hour(), Time_Exit1.minute(), WT);
      Serial.println(Final);
      Writ_to_Main_File(Final, ID2);
      query++;
      if (query > 255) query = 1;
      Write_to_Upload_File(query, Final);
      EEPROM.put(110, 0);
      EEPROM.put(115, 0);
      LCD_Exit_User(ID2);
      //Uploading();
      delay(3000);
      break;
    case ID3:
      Time_Exit3 = rtc.now();
      work_time = (Time_Exit3.hour() * 60 + Time_Exit3.minute()) - ((ID3_Enter_Hour * 60) + ID3_Enter_Min);
      wh = work_time / 60;
      wm = work_time % 60;
      WT = String(wh) + ":" + String(wm);
      Make_String(ID3, Time_Exit3.day(), Time_Exit3.month(), Time_Exit1.year(), ID3_Enter_Hour, ID3_Enter_Min, Time_Exit3.hour(), Time_Exit3.minute(), WT);
      Serial.println(Final);
      Writ_to_Main_File(Final, ID3);
      query++;
      if (query > 255) query = 1;
      Write_to_Upload_File(query, Final);
      EEPROM.put(120, 0);
      EEPROM.put(125, 0);
      LCD_Exit_User(ID3);
      //Uploading();
      delay(3000);
      break;
    case ID4:
      Time_Exit4 = rtc.now();
      work_time = (Time_Exit4.hour() * 60 + Time_Exit4.minute()) - ((ID4_Enter_Hour * 60) + ID4_Enter_Min);
      wh = work_time / 60;
      wm = work_time % 60;
      WT = String(wh) + ":" + String(wm);
      Make_String(ID4, Time_Exit4.day(), Time_Exit4.month(), Time_Exit4.year(), ID1_Enter_Hour, ID4_Enter_Min, Time_Exit4.hour(), Time_Exit4.minute(), WT);
      Serial.println(Final);
      Writ_to_Main_File(Final, ID4);
      query++;
      if (query > 255) query = 1;
      Write_to_Upload_File(query, Final);
      EEPROM.put(130, 0);
      EEPROM.put(135, 0);
      LCD_Exit_User(ID4);
      //Uploading();
      delay(3000);
      break;
    case ID5:
      Time_Exit5 = rtc.now();
      work_time = (Time_Exit5.hour() * 60 + Time_Exit5.minute()) - ((ID5_Enter_Hour * 60) + ID5_Enter_Min);
      wh = work_time / 60;
      wm = work_time % 60;
      WT = String(wh) + ":" + String(wm);
      Make_String(ID5, Time_Exit5.day(), Time_Exit5.month(), Time_Exit5.year(), ID5_Enter_Hour, ID5_Enter_Min, Time_Exit5.hour(), Time_Exit5.minute(), WT);
      Serial.println(Final);
      Writ_to_Main_File(Final, ID5);
      query++;
      if (query > 255) query = 1;
      Write_to_Upload_File(query, Final);
      EEPROM.put(140, 0);
      EEPROM.put(145, 0);
      LCD_Exit_User(ID5);
      //Uploading();
      delay(3000);
      break;
    case ID6:
      Time_Exit6 = rtc.now();
      work_time = (Time_Exit6.hour() * 60 + Time_Exit6.minute()) - ((ID6_Enter_Hour * 60) + ID6_Enter_Min);
      wh = work_time / 60;
      wm = work_time % 60;
      WT = String(wh) + ":" + String(wm);
      Make_String(ID6, Time_Exit6.day(), Time_Exit6.month(), Time_Exit6.year(), ID1_Enter_Hour, ID6_Enter_Min, Time_Exit6.hour(), Time_Exit6.minute(), WT);
      Serial.println(Final);
      Writ_to_Main_File(Final, ID6);
      query++;
      if (query > 255) query = 1;
      Write_to_Upload_File(query, Final);
      EEPROM.put(150, 0);
      EEPROM.put(155, 0);
      LCD_Exit_User(ID6);
      //Uploading();
      delay(3000);
      break;
    case ID7:
      Time_Exit7 = rtc.now();
      work_time = (Time_Exit7.hour() * 60 + Time_Exit7.minute()) - ((ID7_Enter_Hour * 60) + ID7_Enter_Min);
      wh = work_time / 60;
      wm = work_time % 60;
      WT = String(wh) + ":" + String(wm);
      Make_String(ID7, Time_Exit7.day(), Time_Exit7.month(), Time_Exit7.year(), ID7_Enter_Hour, ID7_Enter_Min, Time_Exit7.hour(), Time_Exit7.minute(), WT);
      Serial.println(Final);
      Writ_to_Main_File(Final, ID7);
      query++;
      if (query > 255) query = 1;
      Write_to_Upload_File(query, Final);
      EEPROM.put(160, 0);
      EEPROM.put(165, 0);
      LCD_Exit_User(ID7);
      //Uploading();
      delay(3000);
      break;
    case ID8:
      Time_Exit8 = rtc.now();
      work_time = (Time_Exit8.hour() * 60 + Time_Exit8.minute()) - ((ID8_Enter_Hour * 60) + ID8_Enter_Min);
      wh = work_time / 60;
      wm = work_time % 60;
      WT = String(wh) + ":" + String(wm);
      Make_String(ID8, Time_Exit8.day(), Time_Exit8.month(), Time_Exit8.year(), ID8_Enter_Hour, ID8_Enter_Min, Time_Exit8.hour(), Time_Exit8.minute(), WT);
      Serial.println(Final);
      Writ_to_Main_File(Final, ID8);
      query++;
      if (query > 255) query = 1;
      Write_to_Upload_File(query, Final);
      EEPROM.put(170, 0);
      EEPROM.put(175, 0);
      LCD_Exit_User(ID8);
      //Uploading();
      delay(3000);
      break;
    case ID9:
      Time_Exit9 = rtc.now();
      work_time = (Time_Exit9.hour() * 60 + Time_Exit9.minute()) - ((ID9_Enter_Hour * 60) + ID9_Enter_Min);
      wh = work_time / 60;
      wm = work_time % 60;
      WT = String(wh) + ":" + String(wm);
      Make_String(ID9, Time_Exit9.day(), Time_Exit9.month(), Time_Exit9.year(), ID9_Enter_Hour, ID9_Enter_Min, Time_Exit9.hour(), Time_Exit9.minute(), WT);
      Serial.println(Final);
      Writ_to_Main_File(Final, ID9);
      query++;
      if (query > 255) query = 1;
      Write_to_Upload_File(query, Final);
      EEPROM.put(180, 0);
      EEPROM.put(185, 0);
      LCD_Exit_User(ID9);
      //Uploading();
      delay(3000);
      break;
    case ID10:
      Time_Exit10 = rtc.now();
      work_time = (Time_Exit10.hour() * 60 + Time_Exit10.minute()) - ((ID10_Enter_Hour * 60) + ID10_Enter_Min);
      wh = work_time / 60;
      wm = work_time % 60;
      WT = String(wh) + ":" + String(wm);
      Make_String(ID10, Time_Exit10.day(), Time_Exit10.month(), Time_Exit10.year(), ID1_Enter_Hour, ID10_Enter_Min, Time_Exit10.hour(), Time_Exit10.minute(), WT);
      Serial.println(Final);
      Writ_to_Main_File(Final, ID10);
      query++;
      if (query > 255) query = 1;
      Write_to_Upload_File(query, Final);
      EEPROM.put(190, 0);
      EEPROM.put(195, 0);
      LCD_Exit_User(ID10);
      //Uploading();
      delay(3000);
      break;
    case ID11:
      Time_Exit11 = rtc.now();
      work_time = (Time_Exit11.hour() * 60 + Time_Exit11.minute()) - ((ID11_Enter_Hour * 60) + ID11_Enter_Min);
      wh = work_time / 60;
      wm = work_time % 60;
      WT = String(wh) + ":" + String(wm);
      Make_String(ID11, Time_Exit11.day(), Time_Exit11.month(), Time_Exit11.year(), ID11_Enter_Hour, ID11_Enter_Min, Time_Exit11.hour(), Time_Exit11.minute(), WT);
      Serial.println(Final);
      Writ_to_Main_File(Final, ID11);
      query++;
      if (query > 255) query = 1;
      Write_to_Upload_File(query, Final);
      EEPROM.put(200, 0);
      EEPROM.put(205, 0);
      LCD_Exit_User(ID11);
      //Uploading();
      delay(3000);
      break;
    case ID12:
      Time_Exit12 = rtc.now();
      work_time = (Time_Exit12.hour() * 60 + Time_Exit12.minute()) - ((ID12_Enter_Hour * 60) + ID12_Enter_Min);
      wh = work_time / 60;
      wm = work_time % 60;
      WT = String(wh) + ":" + String(wm);
      Serial.print(Time_Exit12.hour());
      Serial.print(":");
      Serial.println(Time_Exit12.minute());
      Make_String(ID12, Time_Exit12.day(), Time_Exit12.month(), Time_Exit12.year(), ID12_Enter_Hour, ID12_Enter_Min, Time_Exit12.hour(), Time_Exit12.minute(), WT);
      Serial.println(Final);
      Writ_to_Main_File(Final, ID12);
      query++;
      if (query > 255) query = 1;
      Write_to_Upload_File(query, Final);
      LCD_Exit_User(ID12);
      //Uploading();
      EEPROM.put(210, 0);
      EEPROM.put(215, 0);
      delay(3000);
      break;
    case ID13:
      Time_Exit13 = rtc.now();
      work_time = (Time_Exit13.hour() * 60 + Time_Exit13.minute()) - ((ID13_Enter_Hour * 60) + ID13_Enter_Min);
      wh = work_time / 60;
      wm = work_time % 60;
      WT = String(wh) + ":" + String(wm);
      Serial.print(Time_Exit13.hour());
      Serial.print(":");
      Serial.println(Time_Exit13.minute());
      Make_String(ID13, Time_Exit13.day(), Time_Exit13.month(), Time_Exit13.year(), ID13_Enter_Hour, ID13_Enter_Min, Time_Exit13.hour(), Time_Exit13.minute(), WT);
      Serial.println(Final);
      Writ_to_Main_File(Final, ID1);
      query++;
      if (query > 255) query = 1;
      Write_to_Upload_File(query, Final);
      LCD_Exit_User(ID13);
      //Uploading();
      EEPROM.put(220, 0);
      EEPROM.put(225, 0);
      delay(3000);
      break;
    case ID14:
      Time_Exit14 = rtc.now();
      work_time = (Time_Exit14.hour() * 60 + Time_Exit14.minute()) - ((ID14_Enter_Hour * 60) + ID14_Enter_Min);
      wh = work_time / 60;
      wm = work_time % 60;
      WT = String(wh) + ":" + String(wm);
      Serial.print(Time_Exit14.hour());
      Serial.print(":");
      Serial.println(Time_Exit14.minute());
      Make_String(ID14, Time_Exit14.day(), Time_Exit14.month(), Time_Exit14.year(), ID14_Enter_Hour, ID14_Enter_Min, Time_Exit14.hour(), Time_Exit14.minute(), WT);
      Serial.println(Final);
      Writ_to_Main_File(Final, ID14);
      query++;
      if (query > 255) query = 1;
      Write_to_Upload_File(query, Final);
      LCD_Exit_User(ID14);
      //Uploading();
      EEPROM.put(230, 0);
      EEPROM.put(235, 0);
      delay(3000);
      break;
    case ID15:
      Time_Exit15 = rtc.now();
      work_time = (Time_Exit15.hour() * 60 + Time_Exit15.minute()) - ((ID15_Enter_Hour * 60) + ID15_Enter_Min);
      wh = work_time / 60;
      wm = work_time % 60;
      WT = String(wh) + ":" + String(wm);
      Serial.print(Time_Exit15.hour());
      Serial.print(":");
      Serial.println(Time_Exit15.minute());
      Make_String(ID15, Time_Exit15.day(), Time_Exit15.month(), Time_Exit15.year(), ID15_Enter_Hour, ID15_Enter_Min, Time_Exit15.hour(), Time_Exit15.minute(), WT);
      Serial.println(Final);
      Writ_to_Main_File(Final, ID15);
      query++;
      if (query > 255) query = 1;
      Write_to_Upload_File(query, Final);
      LCD_Exit_User(ID15);
      //Uploading();
      EEPROM.put(240, 0);
      EEPROM.put(245, 0);
      delay(3000);
      break;

  }
  LCD_Main_Menu();
}


void Uploading()
{
  int up_status = 1;
  String str = "";
  char c;
  String fname = "";

  for (up_data = 1; up_data < 256; up_data++)
  {
    if (up_status) {
      fname = String(up_data) + ".txt";
      if (SD.exists(fname))
      {
        myFile = SD.open(fname);
        if (myFile) {
          Serial.print(fname);
          Serial.println(" is availabel to read");

          while (myFile.available()) {
            c = myFile.read();
            if (c == '\n') {
              delay(20);
              Serial.println("Send for uplaod");
              Send_for_Upload(str);
              up_status = Upload_Check(up_data);
              if (up_status)
              {
                Serial.print("waiting");
                delay(17000);
              }
              str.remove(0);
            }
            else {
              str += c;
            }
          }
          myFile.close();
        }
        else
          Serial.println("error opening Upload.txt");
      }
      else
      {

        Serial.print(fname);
        Serial.println(" Dosent Exist.... go to next one");
      }
    }
  }
}

void Send_for_Upload(String string)
{
  Serial.println(string);
  Serial1.println(string);
  Serial.println("Send to ESP");
  Final.remove(0);
  string.remove(0);
}

int Upload_Check(int upload_file)
{
  int chk = 1;
  Serial.println(upload_file);
  String Upload_Status = "";
  String fname = "";
  fname = String(upload_file) + ".txt";

  while (!Serial1.available())
  {
    Serial.println("Im here");
  }
  Upload_Status = Serial1.readString();
  if (Upload_Status == "1")
  {
    tft.drawBitmap(420, 10, wifi, 50, 50, 0x0000);
    Serial.println("Uploaded");
    SD.remove(fname);
    Serial.println("removed");
    chk = 1;
  }
  else if (Upload_Status == "0")
  {
    tft.drawBitmap(420, 10, nowifi, 50, 50, 0x0000);
    Serial.println("Faild");
    chk = 0;
  }
  else
  {
    tft.drawBitmap(420, 10, nowifi, 50, 50, 0x0000);
    Serial.println("No answer from ESP");
  }
  return chk;
}

void Make_String(int ID, int da, int mon, int ye, int hin, int minut, int hot, int mot, String wt)
{
  Final =  String(da) + "/" + String(mon) + "/" + String(ye);
  Final += "*";
  switch (ID) {
    case ID1:
      Final += Name_ID1;
      break;
    case ID2:
      Final += Name_ID2;
      break;
    case ID3:
      Final += Name_ID3;
      break;
    case ID4:
      Final += Name_ID4;
      break;
    case ID5:
      Final += Name_ID5;
      break;
    case ID6:
      Final += Name_ID6;
      break;
    case ID7:
      Final += Name_ID7;
      break;
    case ID8:
      Final += Name_ID8;
      break;
    case ID9:
      Final += Name_ID9;
      break;
    case ID10:
      Final += Name_ID10;
      break;
    case ID11:
      Final += Name_ID11;
      break;
    default:
      Serial.println("Error");
      break;
  }


  Final += "*";
  Final += String(hin) + ":" + String(minut);
  Final += "*";
  Final += String(hot) + ":" + String(mot);
  Final += "*";
  Final += wt;
}


void Writ_to_Main_File(String str, int fid)
{
  String file_name = "";
  switch (fid)
  {
    case ID1:
      file_name = Name_ID1 + ".txt";
      myFile = SD.open(file_name, FILE_WRITE);
      if (myFile)
      {
        myFile.println(Final);
        myFile.close();
      }
      else
      {
        Serial.println("error opening test.txt");
        // beep(1500);
      }
      break;
    case ID2:
      file_name = Name_ID2 + ".txt";
      myFile = SD.open(file_name, FILE_WRITE);
      if (myFile)
      {
        myFile.println(Final);
        myFile.close();
      }
      else
      {
        Serial.println("error opening test.txt");
        // beep(1500);
      }
      break;
    case ID3:
      file_name = Name_ID3 + ".txt";
      myFile = SD.open(file_name, FILE_WRITE);
      if (myFile)
      {
        myFile.println(Final);
        myFile.close();
      }
      else
      {
        Serial.println("error opening test.txt");
        // beep(1500);
      }
      break;
    case ID4:
      file_name = Name_ID4 + ".txt";
      myFile = SD.open(file_name, FILE_WRITE);
      if (myFile)
      {
        myFile.println(Final);
        myFile.close();
      }
      else
      {
        Serial.println("error opening test.txt");
        // beep(1500);
      }
      break;
    case ID5:
      file_name = Name_ID5 + ".txt";
      myFile = SD.open(file_name, FILE_WRITE);
      if (myFile)
      {
        myFile.println(Final);
        myFile.close();
      }
      else
      {
        Serial.println("error opening test.txt");
        //  beep(1500);
      }
      break;
    case ID6:
      file_name = Name_ID6 + ".txt";
      myFile = SD.open(file_name, FILE_WRITE);
      if (myFile)
      {
        myFile.println(Final);
        myFile.close();
      }
      else
      {
        Serial.println("error opening test.txt");
        // beep(1500);
      }
      break;
    case ID7:
      file_name = Name_ID7 + ".txt";
      myFile = SD.open(file_name, FILE_WRITE);
      if (myFile)
      {
        myFile.println(Final);
        myFile.close();
      }
      else
      {
        Serial.println("error opening test.txt");
        //  beep(1500);
      }
      break;
    case ID8:
      file_name = Name_ID8 + ".txt";
      myFile = SD.open(file_name, FILE_WRITE);
      if (myFile)
      {
        myFile.println(Final);
        myFile.close();
      }
      else
      {
        Serial.println("error opening test.txt");
        // beep(1500);
      }
      break;
    case ID9:
      file_name = Name_ID9 + ".txt";
      myFile = SD.open(file_name, FILE_WRITE);
      if (myFile)
      {
        myFile.println(Final);
        myFile.close();
      }
      else
      {
        Serial.println("error opening test.txt");
        // beep(1500);
      }
      break;
    case ID10:
      file_name = Name_ID10 + ".txt";
      myFile = SD.open(file_name, FILE_WRITE);
      if (myFile)
      {
        myFile.println(Final);
        myFile.close();
      }
      else
      {
        Serial.println("error opening test.txt");
        //  beep(1500);
      }
      break;
    case ID11:
      file_name = Name_ID11 + ".txt";
      myFile = SD.open(file_name, FILE_WRITE);
      if (myFile)
      {
        myFile.println(Final);
        myFile.close();
      }
      else
      {
        Serial.println("error opening test.txt");
        //        beep(1500);
      }
      break;
    case ID12:
      file_name = Name_ID12 + ".txt";
      myFile = SD.open(file_name, FILE_WRITE);
      if (myFile)
      {
        myFile.println(Final);
        myFile.close();
      }
      else
      {
        Serial.println("error opening test.txt");
        // beep(1500);
      }
      break;
    case ID13:
      file_name = Name_ID13 + ".txt";
      myFile = SD.open(file_name, FILE_WRITE);
      if (myFile)
      {
        myFile.println(Final);
        myFile.close();
      }
      else
      {
        Serial.println("error opening test.txt");
        // beep(1500);
      }
      break;
    case ID14:
      file_name = Name_ID14 + ".txt";
      myFile = SD.open(file_name, FILE_WRITE);
      if (myFile)
      {
        myFile.println(Final);
        myFile.close();
      }
      else
      {
        Serial.println("error opening test.txt");
        // beep(1500);
      }
      break;
    case ID15:
      file_name = Name_ID15 + ".txt";
      myFile = SD.open(file_name, FILE_WRITE);
      if (myFile)
      {
        myFile.println(Final);
        myFile.close();
      }
      else
      {
        Serial.println("error opening test.txt");
        // beep(1500);
      }
      break;
  }

}

void Write_to_Upload_File(int fdata, String str)
{
  String fname = "";
  fname = String(fdata) + ".txt";
  myFile = SD.open(fname, FILE_WRITE);
  if (myFile) {
    myFile.println(str);
    myFile.close();
  } else {
    Serial.println("error opening test.txt");
  }
}

void Beep(int beep)
{
  digitalWrite(BUZZER, HIGH);
  delay(beep);
  digitalWrite(BUZZER, LOW);
}

void setup() {

  Serial.begin(9600);
  finger.begin(57600);
  Serial1.begin(9600);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUT,INPUT_PULLUP);
  rtc.begin();
  Get_from_EEPROM();
  Check_Presence();
  if (!SD.begin(53))
  {
    Serial.println("initialization failed!");

    while (1) {
      Beep(300);
      digitalWrite(RED_LED, HIGH);
      delay(200);
      digitalWrite(RED_LED, LOW);
      delay(1500);
    }
  }
  else
    Serial.println("SD OK");
  static uint16_t g_identifier;
  g_identifier = tft.readID();
  Serial.print("ID = 0x");
  Serial.println(g_identifier, HEX);
  tft.begin(g_identifier);
  tft.setRotation(1);
  tft.invertDisplay(1);
  tft.fillScreen(ORANGE);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  }
  else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }

  finger.getTemplateCount();
  LCD_Setup_Menu();
  LCD_Main_Menu();
  now = rtc.now();
  min_old = now.minute();

}


void loop() {
  int flag = 0;
  int upload_queue = 1;
  String upload_str = "";
  String n_watch = "";

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);

  now = rtc.now();
  if (now.minute() != min_old)
  {
    tft.fillRect(165, 95, 90, 30, ELECTRO);

    n_watch = String(now.hour()) + ":" + String(now.minute());
    LCD_Print(165, 112, WHITE, 2,  n_watch);
    min_old = now.minute();
  }

  Finger_ID = getFingerprintIDez();

  if (Finger_ID < 127  && Finger_ID > 0)
  {
    if (presence[Finger_ID] == 0)
    {
      Beep(200);
      digitalWrite(GREEN_LED, HIGH);
      Enter_User(Finger_ID);
    }
    else if (presence[Finger_ID] == 1)
    {
      Beep(150);
      delay(150);
      Beep(150);
      digitalWrite(GREEN_LED, HIGH);
      Exit_User(Finger_ID);
    }
  }
  else if (Finger_ID == 255)
  {
    Beep(1000);
    digitalWrite(RED_LED, HIGH);
    LCD_Finger_Error();
  }
  if (now.hour() > 20 || now.hour() < 6)
  {
    for (upload_queue = 1; upload_queue < 256; upload_queue++)
    {
      upload_str = String(upload_queue) + ".txt";
      if (SD.exists(upload_str))
      {
        LCD_Upload_Menue();
        Uploading();
        LCD_Main_Menu();
      }
    }
  }
  if (digitalRead(BUT) == 0)
  {
    flag = 1;
    if (flag == 1)
    {
      Enroll();
      flag = 0;
    }
  }
}
