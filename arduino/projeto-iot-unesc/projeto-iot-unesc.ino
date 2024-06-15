/* INCLUDES */
#include "WiFi.h"
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <DHT.h>

#define INTERVAL 1000
#define DHTPIN 12
#define DHTTYPE DHT11 
#define LED 19

#define API_KEY "AIzaSyCJzobgUQPZ-nRpICq-V2o_1U6JDL6_kNc"
#define DATABASE_URL "https://projeto-iot-unesc-default-rtdb.firebaseio.com/"
#define SENDER_ID_COUNT 2


FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
WiFiClientSecure client;
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "projetoiot";
const char* password = "unesc123.";

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

uint32_t lastCheckTime = 0;
uint32_t lastTimeTemperatura = 0;
uint32_t lastTimeUmidade = 0;


String readTemp() {
  return String(dht.readTemperature());
}

String readHumi() {
  return String(dht.readHumidity());
}


void setup() {
  dht.begin();
  Serial.begin(115200);
  Serial.println();

  Serial.print("Conectando na rede ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nWiFi conectado. Endereço de IP: ");
  Serial.println(WiFi.localIP());

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("Firebase conectado");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void loop() {
  uint32_t now = millis();

  Serial.println(dht.readTemperature());
  Serial.println(dht.readHumidity());

  digitalWrite(LED, LOW);

  if ((dht.readTemperature() >= 22 && dht.readTemperature() <= 26) && (dht.readHumidity() >= 50 && dht.readHumidity() <= 80)) {
    digitalWrite(LED, LOW);
    enviaFirebase(0);
  }

  if (dht.readTemperature() < 22 || dht.readTemperature() > 26) {
    piscaLed();
    enviaFirebase(1);
  }

  if (dht.readHumidity() < 50 || dht.readHumidity() > 80) {
    piscaLed();
    enviaFirebase(1);
  }

  digitalWrite(LED, LOW);

}

void piscaLed() {
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
}


void enviaFirebase(int cor_led) {
  Firebase.RTDB.setInt(&fbdo, "esp/led", cor_led);
  Firebase.RTDB.setString(&fbdo, "esp/temperatura", readTemp());
  Firebase.RTDB.setString(&fbdo, "esp/umidade", readHumi());
}
/* 
void handleNewMessages(int numNewMessages) {
  for (int i=0; i<numNewMessages; i++)
  {
    String chatId = String(bot.messages[i].chat_id);    
    String senderId = String(bot.messages[i].from_id); 
    
    Serial.println("senderId: " + senderId);

    boolean validSender = validateSender(senderId);

    if(!validSender) {
      bot.sendMessage(chatId, "Desculpe, você não possui permissão", "HTML");
      continue;
    }
    
    String text = bot.messages[i].text;

    if (text.equalsIgnoreCase(START)) {
      ComandoStart(chatId, bot.messages[i].from_name); 
    }
    else if (text.equalsIgnoreCase(STATS_LED)) {
      ComandoLed(chatId); 
    }
    else if(text.equalsIgnoreCase(TEMPERATURA)) {
      ComandoTemperatura(chatId); 
    }
    else if(text.equalsIgnoreCase(UMIDADE)) {
      ComandoUmidade(chatId); 
    }
  }
}

boolean validateSender(String senderId) {
  for(int i=0; i<SENDER_ID_COUNT; i++) {
    if(senderId == validSenderIds[i]) {
      return true;
    }
  }

  return false;
}

void ComandoStart(String chatId, String fromName) {
  //Mostra Olá e o nome do contato seguido dos comandos válidos
  String message = "<b>Olá " + fromName + ".</b>\n";
  message += comandosAceitos();
  bot.sendMessage(chatId, message, "HTML");
}

String comandosAceitos() {
  String message = "Os comandos disponíveis são:\n\n";
  message += "<b>" + STATS_LED + "</b>: Para verificar o status do led\n";
  message += "<b>" + TEMPERATURA + "</b>: Para verificar apenas a temperatura\n";
  message += "<b>" + UMIDADE + "</b>: Para verificar apenas a umidade\n";
  return message;
}

void ComandoLed(String chatId) {
  bot.sendMessage(chatId, "O led está <b>"+ statusLed() + "</b>", "HTML");
}

void ComandoTemperatura(String chatId) {
  String message = "A temperatura é de " + readTemp() + " °C";
  bot.sendMessage(chatId, message, "HTML");
}

void ComandoUmidade(String chatId) {
  String message = "A umidade é de " + readHumi() + "%";
  bot.sendMessage(chatId, message, "HTML");
}

void alertaTemperatura(String chatId){
  String message = "Alerta de Temperatura!!";
  message += "A temperatura está " + readTemp() + " °C";
  bot.sendMessage(chatId, message, "");
}

void alertaUmidade(String chatId){
  String message = "Alerta de Umidade!!";
  message += "A umidade está " + readHumi() + "%";
  bot.sendMessage(chatId, message, "");
} */