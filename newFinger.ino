#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <TouchScreen.h>
#include <Adafruit_Fingerprint.h>
#include <SPI.h>
#include <SD.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define TFT_DC 9
#define TFT_CS 10
#define TFT_RST 8
#define TS_MINX 150
#define TS_MAXX 900
#define TS_MINY 120
#define TS_MAXY 940
#define YP A3
#define XM A2
#define YM 7
#define XP 6
#define TS_LEFT 150
#define TS_RT 920
#define TS_TOP 940
#define TS_BOT 120
#define MINPRESSURE 10
#define MAXPRESSURE 1000
#define SD_CS 4
#define MAX_USERS 50

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial1);
File dataFile;
char name[20];
uint8_t i = 0;
uint8_t numUsers = 0;

// Configura el sensor de pantalla táctil
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Configura la pantalla
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// Configura las credenciales de Wi-Fi y la dirección del servidor web
const char* ssid = "nombre_red_wifi";
const char* password = "contraseña_wifi";
const char* serverName = "https://sheets.googleapis.com";
const char* fingerprint = "XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX";

void setup() {
  Serial.begin(9600);
  finger.begin(FINGERPRINT_PASSWORD);
  if (finger.verifyPassword()) {
    Serial.println("Modulo de huella dactilar verificado");
  }
  else {
    Serial.println("Modulo de huella dactilar no se pudo verificar");
    while (1) {
      delay(100);
    }
  }

  // Configura la pantalla
  tft.begin();
  ts.begin();

  tft.fillScreen(ILI9341_WHITE);
  tft.setCursor(20, 50);
  tft.print("Sistema de Asistencia");
  tft.setCursor(20, 80);
  tft.print("Listo");
  delay(1000);
  tft.fillScreen(ILI9341_WHITE);

  // Configura la tarjeta SD
  if (!SD.begin(SD_CS)) {
    Serial.println("No se pudo inicializar la tarjeta SD");
    while (1) {
      delay(100);
    }
  }

  // Lee los usuarios de la tarjeta SD
  readUsers();
  printUsers();
  
  // Conecta a Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a Wi-Fi...");
  }
  Serial.println("Conectado a Wi-Fi");
}

void loop() {
  TSPoint touch = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if (touch.z > 0 && touch.z < 1000) {
    int16_t x = map(touch.y, TS_LEFT, TS_RT, 0, tft.width());
    int16_t y = map(touch.x, TS_TOP, TS_BOT, 0, tft.height());
    char buttonPressed = checkButtons(x, y);
   
// Si se presionó un botón de la pantalla táctil
if (buttonPressed != 0) {
switch (buttonPressed) {

  // Si se presionó el botón de añadir usuario
  case 'A': {
    // Mostrar mensaje en pantalla
    tft.fillScreen(ILI9341_WHITE);
    tft.setCursor(20, 50);
    tft.print("Ingrese nombre:");
    // Limpiar variable de nombre y mostrar teclado en pantalla
    clearName();
    drawKeyboard();
    break;
  }
  
  // Si se presionó el botón de eliminar usuario
  case 'D': {
    // Mostrar mensaje en pantalla
    tft.fillScreen(ILI9341_WHITE);
    tft.setCursor(20, 50);
    tft.print("Ingrese ID:");
    // Limpiar variable de ID y mostrar teclado numérico en pantalla
    clearID();
    drawNumericKeyboard();
    break;
  }
  
  // Si se presionó el botón de mostrar registros
  case 'L': {
    // Mostrar registros en la pantalla
    showRecords();
    break;
  }
  
  // Si se presionó el botón de hacer una asistencia
  case 'P': {
    // Hacer la asistencia con el sensor de huellas
    int fingerID = getFingerprintID();
    if (fingerID > 0) {
      // Mostrar el nombre del usuario y la hora en pantalla
      showUserAndTime(fingerID);
      // Agregar registro al archivo en la tarjeta SD
      addRecordToSD(fingerID);
      // Agregar registro a la hoja de cálculo de Google
      addRecordToGoogleSheet(fingerID);
    }
    else {
      // Si no se reconoce la huella, mostrar mensaje en pantalla
      tft.fillScreen(ILI9341_WHITE);
      tft.setCursor(20, 50);
      tft.print("Huella no reconocida");
      delay(2000);
      tft.fillScreen(ILI9341_WHITE);
    }
    break;
  }
  
  // Si se presionó el botón de salir del programa
  case 'Q': {
    // Salir del programa
    tft.fillScreen(ILI9341_WHITE);
    tft.setCursor(20, 50);
    tft.print("Saliendo del programa");
    delay(2000);
    return;
  }
  
  default:
    break;
}
}

delay(100);
}
  // Check if user is in the database
  if (fingerID != FINGERPRINT_INVALIDID) {
    Serial.println("User found, ID:" + String(fingerID));
    tft.fillRect(0, 150, 320, 40, ILI9341_WHITE);
    tft.setCursor(0, 150);
    tft.print("ID: " + String(fingerID) + " ");
    tft.print(userList[fingerID].name);
    tft.print(" checked in at " + String(now.year()) + "/" + String(now.month()) + "/" + String(now.day()) + " " + String(now.hour()) + ":" + String(now.minute()));
    tft.println();
    delay(1000);
    addLogToSD(String(fingerID) + "," + userList[fingerID].name + "," + String(now.year()) + "/" + String(now.month()) + "/" + String(now.day()) + "," + String(now.hour()) + ":" + String(now.minute()));
    addLogToGoogleSheets(String(fingerID), userList[fingerID].name, String(now.year()), String(now.month()), String(now.day()), String(now.hour()), String(now.minute()));
  }
  else {
    Serial.println("User not found");
    tft.fillRect(0, 150, 320, 40, ILI9341_WHITE);
    tft.setCursor(0, 150);
    tft.print("User not found");
    delay(1000);
  }
}
}
}

// Función para verificar si se ha presionado un botón en la pantalla táctil
char checkButtons(int16_t x, int16_t y) {
if (x >= 20 && x <= 60 && y >= 50 && y <= 90) {
return 'A';
}
else if (x >= 80 && x <= 120 && y >= 280 && y <= 320) {
return 'P';
}
else if (x >= 200 && x <= 240 && y >= 280 && y <= 320) {
return 'D';
}
else if (x >= 260 && x <= 300 && y >= 420 && y <= 460) {
return '3';
}
return 0;
}

// Función para añadir un usuario a la base de datos en la tarjeta SD
void addUserToSD(String name) {
dataFile = SD.open("users.txt", FILE_WRITE);
if (dataFile) {
dataFile.print(name + "\n");
dataFile.close();
readUsers();
}
}

// Función para leer los usuarios de la base de datos en la tarjeta SD
void readUsers() {
dataFile = SD.open("users.txt");
if (dataFile) {
numUsers = 0;
while (dataFile.available()) {
String name = dataFile.readStringUntil('\n');
name.trim();
if (name.length() > 0) {
userList[numUsers].name = name;
numUsers++;
}
}
dataFile.close();
Serial.println("Users loaded");
}
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
// Crea el objeto JSON con los datos del registro
String jsonPayload = "{"values": [["" + date + "", "" + time + "", "" + name + "", "" + status + ""]]}";

// Conecta con la API de Google Sheets
WiFiClientSecure client;
client.setCACert(root_ca);

HTTPClient http;
http.begin(client, "https://sheets.googleapis.com/v4/spreadsheets/" + spreadsheet_id + "/values/" + sheet_name + "!A1:D1:append?valueInputOption=RAW");
http.addHeader("Authorization", "Bearer " + access_token);
http.addHeader("Content-Type", "application/json");

// Envía los datos del registro a Google Sheets
int httpResponseCode = http.POST(jsonPayload);

if (httpResponseCode == 200) {
Serial.println("Log added to Google Sheets");
}
else {
Serial.print("Error adding log to Google Sheets: ");
Serial.println(httpResponseCode);
}

http.end();
}

// Función para obtener la fecha y hora actual en formato "YYYY-MM-DD" y "HH:MM:SS"
void getDateTime(String& date, String& time) {
time_t now = time(nullptr);
struct tm* timeinfo = localtime(&now);
char buffer[80];

strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);
date = buffer;

strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
time = buffer;
}

void loop() {
TSPoint touch = ts.getPoint();
pinMode(XM, OUTPUT);
pinMode(YP, OUTPUT);
if (touch.z > 0 && touch.z < 1000) {
int16_t x = map(touch.y, TS_LEFT, TS_RT, 0, tft.width());
int16_t y = map(touch.x, TS_TOP, TS_BOT, 0, tft.height());
char buttonPressed = checkButtons(x, y);
if (buttonPressed == 'A') { // Si se presiona el botón "Agregar usuario"
  addUser();
}
else if (buttonPressed == 'E') { // Si se presiona el botón "Eliminar usuario"
  deleteUser();
}
else if (buttonPressed == 'L') { // Si se presiona el botón "Ver registros"
  viewLogs();
}
else if (buttonPressed == 'P') { // Si se presiona el botón "Descargar registros"
  downloadLogs();
}
else if (buttonPressed == '1') { // Si se presiona el botón "Ingreso"
  String name = verifyFingerprint();
  if (name != "") {
    String date, time;
    getDateTime(date, time);
    addLogToSD(date + "," + time + "," + name + ",Ingreso");
    addLogToGoogleSheets(date, time, name, "Ingreso");
    tft.setCursor(20, 200);
    tft.println("Bienvenido(a), " + name);
    delay(2000);
  }
  else {
    tft.setCursor(20, 200);
    tft.println("Huella no reconocida");
    delay(2000);
  }
}
else if (buttonPressed == '2') { // Si se presiona el botón "Salida"
  String name = verifyFingerprint();
  if (name != "") {
    String date, time;
    getDateTime(date, time);
    addLogToSD(date"," + time + "," + name + ",S");
addLogToGoogleSheets(date, time, name, "S");
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
currentID = -1;
currentName = "";
}
else {
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
}
delay(100);
}
