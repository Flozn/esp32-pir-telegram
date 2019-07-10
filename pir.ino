#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

//Telegram credentials
#define BOT_TOKEN "my_telegram_token"
#define chatID "my_telegram_chatid"

//WIFI credentials
#define SSID "my_ssid"
#define PASSWORD "my_password"

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

//listen to PIR at Pin 27
const byte interruptPin = 27;
SemaphoreHandle_t syncSemaphore;

void IRAM_ATTR detectMovement() {
  xSemaphoreGiveFromISR(syncSemaphore, NULL);
}

void setup()
{
  Serial.begin(115200);
  setupWiFi();

  syncSemaphore = xSemaphoreCreateBinary();
  
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), detectMovement, RISING);
}

void loop() {
  xSemaphoreTake(syncSemaphore, portMAX_DELAY);
  bot.sendMessage(chatID, "Movement DETECTED!"); 
}

void setupWiFi() {
  Serial.print("Connecting to SSID: ");
  Serial.println(SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected");
}
