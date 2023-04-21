#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>
#include <Adafruit_Fingerprint.h>
#include <SD.h>
#include <SPI.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define SD_CS 5
#define TFT_CS 6
#define TFT_DC 7
#define TS_CS 8
#define FINGERPRINT_RX 9
#define FINGERPRINT_TX 10
#define FINGERPRINT_PASSWORD 0x0
#define MAX_USERS 50

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_FT6206 ts = Adafruit_FT6206();
SoftwareSerial mySerial(FINGERPRINT_RX, FINGERPRINT_TX);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

File dataFile;

char ssid[] = "nombre_de_la_red_wifi";     // Nombre de la red WiFi
char pass[] = "contraseña_de_la_red_wifi"; // Contraseña de la red WiFi
String access_token = "token_de_acceso_de_Google_Sheets";
String spreadsheet_id = "id_de_la_hoja_de_cálculo_de_Google_Sheets";
String range = "nombre_de_la_hoja_de_cálculo!A1:D1";

struct User {
  String name;
  uint32_t fingerprintID;
};

User userList[MAX_USERS];
uint8_t numUsers = 0;
uint8_t i = 0;

// Función para obtener la fecha y hora actual
void getDateTime(String& date, String& time) {
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);

  char buffer[80];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);
  date = buffer;

  strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
  time = buffer;
}

// Función para añadir un usuario a la base de datos en la tarjeta SD
void addUserToSD(String name) {
  if (numUsers < MAX_USERS) {
    userList[numUsers].name = name;
    userList[numUsers].fingerprintID = finger.storeTemplate();
    dataFile = SD.open("users.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.println(name);
      dataFile.println(userList[numUsers].fingerprintID);
      dataFile.close();
      numUsers++;
      tft.fillScreen(ILI9341_WHITE);
      tft.setCursor(20, 50);
      tft.print("Usuario agregado");
      delay(2000);
      tft.fillScreen(ILI9341_WHITE);
      tft.setCursor(20, 50);
      tft.print("Ingrese su huella");
      tft.setCursor(20, 80);
      tft.print("para registrar");
      tft.setCursor(20, 110);
      tft.print("entrada o salida");
    }
  } else {
    tft.fillScreen(ILI9341_WHITE);
    tft.setCursor(20, 50);
    tft.print("Número máximo");
    tft.setCursor(20, 80);
    tft.print("de usuarios alcanzado");
    delay(2000);
    tft.fillScreen(ILI9341_WHITE);
    tft.setCursor(20, 50);
    tft.print("Ingrese su huella");
    tft.setCursor(20, 80);
    tft.print("para registrar");
    tft.setCursor(20, 110);
    tft.print("entrada o salida");
  }
}
else if (buttonPressed == 'R') { // Si se presiona el botón "Registros"
printLog();
} else if (buttonPressed == 'P') { // Si se presiona el botón "Imprimir"
printLog();
}
}
}

// Función para verificar si la huella digital es válida y obtener el nombre del usuario
String verifyFingerprint() {
Serial.println("Coloque su dedo en el sensor de huellas digitales");
tft.fillScreen(ILI9341_WHITE);
tft.setCursor(20, 50);
tft.print("Coloque su dedo en");
tft.setCursor(20, 80);
tft.print("el sensor de huellas");
tft.setCursor(20, 110);
tft.print("digitales");
while (true) {
if (finger.getImage()) {
int fingerID = finger.fingerFastSearch();
if (fingerID == FINGERPRINT_NOTFOUND) {
tft.fillScreen(ILI9341_WHITE);
tft.setCursor(20, 50);
tft.print("Huella no encontrada");
delay(2000);
tft.fillScreen(ILI9341_WHITE);
tft.setCursor(20, 50);
tft.print("Coloque su dedo en");
tft.setCursor(20, 80);
tft.print("el sensor de huellas");
tft.setCursor(20, 110);
tft.print("digitales");
} else {
finger.loadTemplate(fingerID);
String name = userList[fingerID].name;
tft.fillScreen(ILI9341_WHITE);
tft.setCursor(20, 50);
tft.print("Huella reconocida");
tft.setCursor(20, 80);
tft.print("Bienvenido " + name);
delay(2000);
tft.fillScreen(ILI9341_WHITE);
tft.setCursor(20, 50);
tft.print("Ingrese su huella");
tft.setCursor(20, 80);
tft.print("para registrar");
tft.setCursor(20, 110);
tft.print("entrada o salida");
return name;
}
}
}
}

// Función para obtener la fecha y hora actual
void getDateTime(String& date, String& time) {
time_t now = time(nullptr);
struct tm* timeinfo = localtime(&now);
char buffer[80];
strftime(buffer, sizeof(buffer), "%Y-%m-%d,%H:%M:%S", timeinfo);
date = buffer[0] == '0' ? buffer + 1 : buffer;
strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
time = buffer;
}

// Función para añadir un registro a la base de datos en la tarjeta SD
void addLogToSD(String log) {
dataFile = SD.open("log.txt", FILE_WRITE);
if (dataFile) {
dataFile.print(log + "\n");
dataFile.close();
Serial.println("Log added to SD");
}
}

// Función para agregar un registro a Google Sheets usando la API de Google Sheets
void addLogToGoogleSheets(String date, String time, String name, String status) {
WiFiClientSecure client;
HTTPClient http;

// Obtener un token de acceso para la API de Google Sheets
client.setCACert(root_ca);
http.begin(client, "https://www.googleapis.com/oauth2/v4/token");
http.addHeader("Content-Type", "application/x-www-form-urlencoded");
String payload = "client_id=" + client_id + "&client_secret=" + client_secret + "&refresh_token=" + refresh_token + "&grant_type=refresh_token";
int httpCode = http.POST(payload);
if (httpCode == HTTP_CODE_OK) {
String response = http.getString();
String access_token = extractAccessToken(response);

// Agregar el registro a la hoja de cálculo de Google Sheets
String url = "https://sheets.googleapis.com/v4/spreadsheets/" + spreadsheet_id + "/values/" + range + "?valueInputOption=USER_ENTERED";
http.begin(client, url);
http.addHeader("Authorization", "Bearer " + access_token);
http.addHeader("Content-Type", "application/json");
String payload = "{"range":"' + range + '","majorDimension":"ROWS","values":[["' + date + '","' + time + '","' + name + '","' + status + '"]]}";
http.POST(payload);

Serial.println("Log added to Google Sheets");
} else {
Serial.println("Error getting access token");
}

http.end();
client.stop();
}

// Función para imprimir los registros de la base de datos
void printLog() {
  dataFile = SD.open("log.txt");
  if (dataFile) {
    tft.fillScreen(ILI9341_WHITE);
    tft.setCursor(20, 50);
    tft.print("Registros en la SD:");

    int count = 0;
    while (dataFile.available()) {
      String log = dataFile.readStringUntil('\n');
      tft.setCursor(20, 80 + count * 20);
      tft.print(log);
      count++;

      if (count >= 8) {
        delay(5000);
        tft.fillScreen(ILI9341_WHITE);
        tft.setCursor(20, 50);
        tft.print("Registros en la SD:");
        count = 0;
      }
    }

    dataFile.close();
    delay(5000);
    tft.fillScreen(ILI9341_WHITE);
    tft.setCursor(20, 50);
    tft.print("Ingrese su huella");
    tft.setCursor(20, 80);
    tft.print("para registrar");
    tft.setCursor(20, 110);
    tft.print("entrada o salida");
  } else {
    Serial.println("Error opening log file");
  }
}

void loop() {
  TSPoint touch = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if (touch.z > 0 && touch.z < 1000) {
    int16_t x = map(touch.y, TS_LEFT, TS_RT, 0, tft.width());
    int16_t y = map(touch.x, TS_TOP, TS_BOT, 0, tft.height());
    char buttonPressed = checkButtons(x, y);

    if (buttonPressed == 'E') { // Si se presiona el botón "Usuarios"
      i = 0;
      while (true) {
        tft.fillScreen(ILI9341_WHITE);
        tft.setCursor(20, 50);
        tft.print("Ingrese el nombre");
        tft.setCursor(20, 80);
        tft.print("del nuevo usuario");
        tft.setCursor(20, 110);
        tft.print(name);
        char c = getButtonInput();
        if (c == 'E') {
          addUserToSD(name);
          break;
        } else if (c == 'D') {
          if (i > 0) {
            i--;
            name[i] = '\0';
            tft.fillRect(20 + i * 20, 50, 20, 20, ILI9341_WHITE);
          }
        } else if (c >= 'A' && c <= 'Z' && i < MAX_NAME_LENGTH) {
          name[i] = c;
          i++;
          name[i] = '\0';
          tft.setCursor(20 + i * 20, 50);
          tft.print(c);
        }
      }
    } else if (buttonPressed == 'A') { // Si se presiona el botón "Asistencia"
      String name = verifyFingerprint();
      if (name != "") {
        String date, time;
        getDateTime(date, time);
        addLogToSD(date + "," + time + "," + name + ",Entrada");
        addLogToGoogleSheets(date, time, name, "Entrada");
        tft.fillScreen(ILI9341_WHITE);
        tft.setCursor(20, 50);
        tft.print("Entrada registrada");
        delay(2000);
        tft.fillScreen(ILI9341_WHITE);
        tft.setCursor(20, 50);
        tft.print("Ingrese su huella");
        tft.setCursor(20, 80);
        tft.print("para registrar");
        tft.setCursor(20, 110);
        tft.print("entrada o salida");
      } else {
        tft.fillScreen(ILI9341_WHITE);
        tft.setCursor(20, 50);
        tft.print("Huella no registrada");
        delay(2000);
        tft.fillScreen(ILI9341_WHITE);
        tft.setCursor(20, 50);
        tft.print("Ingrese su huella");
        tft.setCursor(20, 80);
        tft.print("para registrar");
        tft.setCursor(20, 110);
        tft.print("entrada o salida");
      }
    } else if (buttonPressed == 'S') { // Si se presiona el botón "Salida"
      String name = verifyFingerprint();
      if (name != "") {
        String date, time;
        getDateTime(date, time);
        addLogToSD(date + "," + time + "," + name + ",Salida");
        addLogToGoogleSheets(date, time, name, "Salida");
        tft.fillScreen(ILI9341_WHITE);
        tft.setCursor(20, 50);
        tft.print("Salida registrada");
        delay(2000);
        tft.fillScreen(ILI9341_WHITE);
        tft.setCursor(20, 50);
        tft.print("Ingrese su huella");
        tft.setCursor(20, 80);
        tft.print("para registrar");
        tft.setCursor(20, 110);
        tft.print("entrada o salida");
      } else {
        tft.fillScreen(ILI9341_WHITE);
        tft.setCursor(20, 50);
        tft.print("Huella no registrada");
        delay(2000);
        tft.fillScreen(ILI9341_WHITE);
        tft.setCursor(20, 50);
        tft.print("Ingrese su huella");
        tft.setCursor(20, 80);
        tft.print("para registrar");
        tft.setCursor(20, 110);
        tft.print("entrada o salida");
      }
    } else if (buttonPressed == 'R') { // Si se presiona el botón "Reiniciar"
      tft.fillScreen(ILI9341_WHITE);
      tft.setCursor(20, 50);
      tft.print("Reiniciando...");
      delay(2000);
      ESP.restart();
    } else if (buttonPressed == 'P') { // Si se presiona el botón "Imprimir"
      printLog();
    }
  }
  
  delay(100);
}
