#include <Adafruit_Fingerprint.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <TouchScreen.h>

#define TS_MINX 150
#define TS_MAXX 900
#define TS_MINY 120
#define TS_MAXY 940

#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8

#define YP A1
#define XM A2
#define YM 7
#define XP 6

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial1);

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

struct User {
  int id;
  char name[20];
  uint32_t fingerID;
};

User userList[50];
int numUsers = 0;
int userToDelete = -1;

// Función para mostrar la lista de usuarios
void showUserList() {
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(20, 60);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.println("Lista de usuarios");
  for (int i = 0; i < numUsers; i++) {
    tft.drawRect(0, 90 + (i * 30), 480, 30, TFT_WHITE);
    tft.setCursor(20, 100 + (i * 30));
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.print(userList[i].name);
  }
  tft.setCursor(0, 280);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.println("Agregar usuario");
  tft.drawRect(0, 280, 240, 40, TFT_WHITE);
  tft.setCursor(240, 280);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.println("Eliminar usuario");
  tft.drawRect(240, 280, 240, 40, TFT_WHITE);
}

// Función para agregar un usuario
void addUser() {
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(20, 60);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.println("Agregar usuario");
  tft.setCursor(20, 120);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.println("Ingrese el nombre");
  tft.drawRect(100, 50, 300, 40, TFT_WHITE);
  tft.setCursor(0, 280);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.println("Cancelar");
  tft.drawRect(0, 280, 240, 40, TFT_WHITE);
  tft.setCursor(240, 280);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.println("Guardar");
  tft.drawRect(240, 200, 240, 40, TFT_WHITE);
  uint8_t fingerID = 0;
  for (uint8_t i = 1; i <= 127; i++) {
    if (finger.storeModel(i, 1) == FINGERPRINT_OK) {
      fingerID = i;
      break;
    }
  }
  if (fingerID == 0) {
    tft.setCursor(20      , 200);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.println("No se pudo registrar");
    delay(2000);
    showUserList();
    return;
  }
  while (true) {
    TSPoint touch = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    if (touch.z > 0 && touch.z < 1000) {
      int x = map(touch.y, TS_MINY, TS_MAXY, tft.width(), 0);
      int y = map(touch.x, TS_MINX, TS_MAXX, tft.height(), 0);
      if (x >= 0 && x <= 240 && y >= 280 && y <= 320) {
        showUserList();
        return;
      }
      if (x >= 240 && x <= 480 && y >= 280 && y <= 320) {
        char name[20];
        tft.getRect(100, 50, 300, 40, (uint16_t*)tft.buffer);
        tft.setCursor(110, 60);
        tft.setTextColor(TFT_WHITE);
        tft.setTextSize(2);
        uint8_t i = 0;
        while (true) {
          TSPoint touch = ts.getPoint();
          pinMode(XM, OUTPUT);
          pinMode(YP, OUTPUT);
          if (touch.z > 0 && touch.z < 1000) {
            int x = map(touch.y, TS_MINY, TS_MAXY, tft.width(), 0);
            int y = map(touch.x, TS_MINX, TS_MAXX, tft.height(), 0);
            if (x >= 0 && x <= 480 && y >= 0 && y <= 30) {
              showUserList();
              return;
            }
            if (x >= 240 && x <= 480 && y >= 280 && y <= 320) {
              name[i] = '\0';
              addUserToList(fingerID, name);
              return;
            }
            if (x >= 100 && x <= 400 && y >= 50 && y <= 90) {
              while (true) {
                TSPoint touch = ts.getPoint();
                pinMode(XM, OUTPUT);
                pinMode(YP, OUTPUT);
                if (touch.z > 0 && touch.z < 1000) {
                  int x = map(touch.y, TS_MINY, TS_MAXY, tft.width(), 0);
                  int y = map(touch.x, TS_MINX, TS_MAXX, tft.height(), 0);
                  if (x >= 0 && x <= 480 && y >= 0 && y <= 30) {
                    showUserList();
                    return;
                  }
                  if (x >= 100 && x <= 400 && y >= 50 && y <= 90) {
                    tft.drawRect(100, 50, 300, 40, TFT_WHITE);
                    tft.setCursor(110, 60);
                    tft.setTextColor(TFT_WHITE);
                    tft.setTextSize(2);
                    break;
                  }
                  if (x >= 240 && x <= 480 && y >= 280 && y <= 320) {
                    name[i] = '\0';
                    addUserToList(fingerID, name);
                    return;
                  }
                  if (x >= 20 && x <= 60 && y >= 50 && y <= 90) {
                    if (i > 0) {
                      i--;
                      name[i] = '\0';
                      tft.fillRect(                      110 + (i * 20), 50, 20, 40, TFT_BLACK);
                      tft.drawRect(100, 50, 300, 40, TFT_WHITE);
                      tft.setCursor(110, 60);
                      tft.setTextColor(TFT_WHITE);
                      tft.setTextSize(2);
                      tft.print(name);
                    }
                  }
                  if (i < 19) {
                    int posX = 110 + (i * 20);
                    int posY = 50;
                    tft.fillRect(posX, posY, 20, 40, TFT_WHITE);
                    int16_t x, y;
                    uint16_t z;
                    TSPoint touch = ts.getPoint();
                    pinMode(XM, OUTPUT);
                    pinMode(YP, OUTPUT);
                    if (touch.z > 0 && touch.z < 1000) {
                      x = touch.y;
                      y = touch.x;
                      z = touch.z;
                    }
                    i++;
                    if (z < 1000 && x > 0 && x < 480 && y > 0 && y < 320) {
                      name[i - 1] = 'A' + (x / 30) + (y / 50) * 6;
                    }
                    tft.drawRect(100, 50, 300, 40, TFT_WHITE);
                    tft.setCursor(110, 60);
                    tft.setTextColor(TFT_BLACK);
                    tft.setTextSize(2);
                    tft.print(name);
                    delay(200);
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Función para eliminar un usuario
void deleteUser() {
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(20, 60);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.println("Eliminar usuario");
  tft.setCursor(20, 120);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.println("Seleccione el usuario");
  for (int i = 0; i < numUsers; i++) {
    tft.drawRect(0, 150 + (i * 30), 480, 30, TFT_WHITE);
    tft.setCursor(20, 160 + (i * 30));
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.print(userList[i].name);
  }
  tft.setCursor(0, 280);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.println("Cancelar");
  tft.drawRect(0, 280, 240, 40, TFT_WHITE);
  tft.setCursor(240, 280);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.println("Eliminar");
  tft.drawRect(240, 280, 240, 40, TFT_WHITE);
}

// Función para agregar un usuario a la lista
void addUserToList(uint32_t fingerID, char name[]) {
  User user;
  user.id = numUsers;
  strcpy(user.name, name);
  user.fingerID = fingerID;
  userList[numUsers] = user;
  numUsers++;
  showUserList();
}

void setup() {
  Serial.begin(9600);
  finger.begin(57600);
  tft.begin();
  showUserList();
}

void loop() {
  TSPoint touch = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if (touch.z > 0 && touch.z < 1000)
  {
    int x = map(touch.y, TS_MINY, TS_MAXY, tft.width(), 0);
    int y = map(touch.x, TS_MINX, TS_MAXX, tft.height(), 0);
    if (x >= 0 && x <= 240 && y >= 280 && y <= 320) {
      showUserList();
      return;
    }
    if (x >= 240 && x <= 480 && y >= 280 && y <= 320) {
      deleteUser();
      return;
    }
    for (int i = 0; i < numUsers; i++) {
      if (x >= 0 && x <= 480 && y >= 150 + (i * 30) && y <= 180 + (i * 30)) {
        deleteFingerprint(userList[i].fingerID);
        for (int j = i; j < numUsers - 1; j++) {
          userList[j] = userList[j + 1];
        }
        numUsers--;
        showUserList();
        return;
      }
    }
  }
}

