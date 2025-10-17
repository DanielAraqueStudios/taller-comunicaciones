/*
 * ============================================
 * ESP32 - SENSORES MQTT
 * Taller Comunicaciones - Universidad Militar Nueva Granada
 * ============================================
 * 
 * Este código implementa un sistema de sensores en ESP32 que publica
 * datos a un broker MQTT en diferentes tópicos.
 * 
 * SENSORES IMPLEMENTADOS (mínimo 7):
 * 1. Sensor de humo (MQ-2) -> incendio/sensor_humo
 * 2. Botón alarma manual -> incendio/alarma
 * 3. Sensor de puerta (Reed Switch) -> seguridad/puerta
 * 4. DHT11 (Temperatura) -> clima/temperatura
 * 5. DHT11 (Humedad) -> clima/humedad
 * 6. LDR (Luz solar) -> iluminacion/luz
 * 7. Sensor de movimiento (PIR) -> seguridad/movimiento
 * 
 * ============================================
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h>

// ============================================
// CONFIGURACIÓN WIFI
// ============================================
const char* ssid = "TuRedWiFi";           // Cambiar por tu SSID
const char* password = "TuPassword";       // Cambiar por tu contraseña

// ============================================
// CONFIGURACIÓN MQTT
// ============================================
const char* mqtt_server = "192.168.1.100"; // IP del servidor Kali Linux
const int mqtt_port = 1883;
const char* mqtt_user = "";                // Dejar vacío si allow_anonymous = true
const char* mqtt_password = "";
const char* device_id = "ESP32_01";

// ============================================
// PINES DE SENSORES
// ============================================
#define DHT_PIN 11              // DHT11 (Temperatura y Humedad)
#define DHT_TYPE DHT11
#define MQ2_PIN 34              // Sensor de humo MQ-2 (Analógico)
#define LDR_PIN 35              // LDR - Sensor de luz (Analógico)
#define PIR_PIN 13              // Sensor de movimiento PIR
#define REED_PIN 14             // Sensor de puerta (Reed Switch)
#define ALARM_BUTTON_PIN 12     // Botón de alarma manual
#define LED_STATUS 2            // LED integrado para estado

// ============================================
// TÓPICOS MQTT
// ============================================
const char* TOPIC_HUMO = "incendio/sensor_humo";
const char* TOPIC_ALARMA = "incendio/alarma";
const char* TOPIC_PUERTA = "seguridad/puerta";
const char* TOPIC_TEMPERATURA = "clima/temperatura";
const char* TOPIC_HUMEDAD = "clima/humedad";
const char* TOPIC_LUZ = "iluminacion/luz";
const char* TOPIC_MOVIMIENTO = "seguridad/movimiento";
const char* TOPIC_STATUS = "sistema/estado";

// ============================================
// OBJETOS GLOBALES
// ============================================
WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHT_PIN, DHT_TYPE);

// ============================================
// VARIABLES GLOBALES
// ============================================
unsigned long lastMsg = 0;
unsigned long lastStatusMsg = 0;
const long interval = 5000;        // Intervalo de publicación (5 segundos)
const long statusInterval = 30000; // Intervalo de estado (30 segundos)

bool lastPirState = false;
bool lastReedState = false;
bool lastAlarmState = false;

// ============================================
// CONFIGURACIÓN INICIAL
// ============================================
void setup() {
  Serial.begin(115200);
  Serial.println("\n============================================");
  Serial.println("ESP32 - SENSORES MQTT");
  Serial.println("Universidad Militar Nueva Granada");
  Serial.println("============================================\n");

  // Configurar pines
  pinMode(PIR_PIN, INPUT);
  pinMode(REED_PIN, INPUT_PULLUP);
  pinMode(ALARM_BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_STATUS, OUTPUT);
  pinMode(MQ2_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);

  // Inicializar DHT
  dht.begin();
  Serial.println("✅ DHT11 inicializado");

  // Conectar WiFi
  setup_wifi();

  // Configurar MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  Serial.println("\n✅ Sistema listo");
  digitalWrite(LED_STATUS, HIGH);
}

// ============================================
// CONEXIÓN WIFI
// ============================================
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_STATUS, !digitalRead(LED_STATUS));
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi conectado");
    Serial.print("📡 Dirección IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("📶 Intensidad señal: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("\n❌ Error: No se pudo conectar a WiFi");
  }
}

// ============================================
// CALLBACK MQTT (RECEPCIÓN)
// ============================================
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("📩 Mensaje recibido [");
  Serial.print(topic);
  Serial.print("]: ");
  
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);
}

// ============================================
// RECONEXIÓN MQTT
// ============================================
void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando a MQTT...");
    
    String clientId = String(device_id) + "-" + String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println(" ✅ Conectado");
      
      // Publicar mensaje de conexión
      publishStatus("online");
      
      // Suscribirse a tópicos si es necesario
      // client.subscribe("incendio/activar_sirena");
      
    } else {
      Serial.print(" ❌ Error, rc=");
      Serial.print(client.state());
      Serial.println(" Reintentando en 5s...");
      digitalWrite(LED_STATUS, LOW);
      delay(5000);
    }
  }
  digitalWrite(LED_STATUS, HIGH);
}

// ============================================
// PUBLICAR MENSAJE JSON
// ============================================
void publishSensor(const char* topic, const char* tipo, float valor, const char* unidad, const char* estado = "") {
  StaticJsonDocument<256> doc;
  
  doc["sensor_id"] = device_id;
  doc["tipo"] = tipo;
  doc["valor"] = valor;
  doc["unidad"] = unidad;
  if (strlen(estado) > 0) {
    doc["estado"] = estado;
  }
  doc["timestamp"] = millis();
  
  char buffer[256];
  serializeJson(doc, buffer);
  
  if (client.publish(topic, buffer)) {
    Serial.print("📤 [");
    Serial.print(topic);
    Serial.print("] ");
    Serial.println(buffer);
  } else {
    Serial.print("❌ Error publicando en: ");
    Serial.println(topic);
  }
}

// ============================================
// PUBLICAR ESTADO DEL SISTEMA
// ============================================
void publishStatus(const char* estado) {
  StaticJsonDocument<256> doc;
  
  doc["device_id"] = device_id;
  doc["estado"] = estado;
  doc["ip"] = WiFi.localIP().toString();
  doc["rssi"] = WiFi.RSSI();
  doc["uptime"] = millis() / 1000;
  doc["free_heap"] = ESP.getFreeHeap();
  
  char buffer[256];
  serializeJson(doc, buffer);
  
  client.publish(TOPIC_STATUS, buffer);
}

// ============================================
// LEER SENSORES
// ============================================
void readSensors() {
  // 1. Sensor DHT11 - Temperatura
  float temp = dht.readTemperature();
  if (!isnan(temp)) {
    publishSensor(TOPIC_TEMPERATURA, "temperatura", temp, "°C");
  }

  delay(100);

  // 2. Sensor DHT11 - Humedad
  float hum = dht.readHumidity();
  if (!isnan(hum)) {
    publishSensor(TOPIC_HUMEDAD, "humedad", hum, "%");
  }

  delay(100);

  // 3. Sensor MQ-2 - Humo
  int smokeValue = analogRead(MQ2_PIN);
  float smokePercent = map(smokeValue, 0, 4095, 0, 100);
  publishSensor(TOPIC_HUMO, "humo", smokePercent, "%", smokePercent > 50 ? "alerta" : "normal");

  delay(100);

  // 4. Sensor LDR - Luz
  int ldrValue = analogRead(LDR_PIN);
  float luzPercent = map(ldrValue, 0, 4095, 0, 100);
  publishSensor(TOPIC_LUZ, "luz", luzPercent, "%");

  delay(100);

  // 5. Sensor PIR - Movimiento (solo si hay cambio)
  bool pirState = digitalRead(PIR_PIN);
  if (pirState != lastPirState) {
    publishSensor(TOPIC_MOVIMIENTO, "movimiento", pirState ? 1 : 0, "", pirState ? "detectado" : "sin_movimiento");
    lastPirState = pirState;
  }

  // 6. Sensor Reed - Puerta (solo si hay cambio)
  bool reedState = digitalRead(REED_PIN);
  if (reedState != lastReedState) {
    publishSensor(TOPIC_PUERTA, "puerta", reedState ? 1 : 0, "", reedState ? "abierta" : "cerrada");
    lastReedState = reedState;
  }

  // 7. Botón Alarma (solo si se presiona)
  bool alarmState = !digitalRead(ALARM_BUTTON_PIN); // Lógica invertida (pull-up)
  if (alarmState != lastAlarmState && alarmState) {
    publishSensor(TOPIC_ALARMA, "alarma_manual", 1, "", "activada");
  }
  lastAlarmState = alarmState;
}

// ============================================
// LOOP PRINCIPAL
// ============================================
void loop() {
  // Mantener conexión MQTT
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();

  // Publicar lecturas de sensores periódicamente
  if (now - lastMsg > interval) {
    lastMsg = now;
    readSensors();
  }

  // Publicar estado del sistema periódicamente
  if (now - lastStatusMsg > statusInterval) {
    lastStatusMsg = now;
    publishStatus("online");
  }
}

// ============================================
// NOTAS DE IMPLEMENTACIÓN
// ============================================
/*
 * CONEXIONES FÍSICAS:
 * 
 * DHT11:
 *   - VCC -> 3.3V
 *   - DATA -> GPIO 11
 *   - GND -> GND
 * 
 * MQ-2 (Sensor de Humo):
 *   - VCC -> 5V
 *   - AO -> GPIO 34
 *   - GND -> GND
 * 
 * LDR (Fotoresistor):
 *   - Un terminal -> 3.3V
 *   - Otro terminal -> GPIO 35 + Resistencia 10kΩ a GND
 * 
 * PIR (HC-SR501):
 *   - VCC -> 5V
 *   - OUT -> GPIO 13
 *   - GND -> GND
 * 
 * Reed Switch:
 *   - Un terminal -> GPIO 14
 *   - Otro terminal -> GND
 *   - (Usar INPUT_PULLUP interno)
 * 
 * Botón Alarma:
 *   - Un terminal -> GPIO 12
 *   - Otro terminal -> GND
 *   - (Usar INPUT_PULLUP interno)
 * 
 * LIBRERÍAS NECESARIAS:
 *   - PubSubClient by Nick O'Leary
 *   - DHT sensor library by Adafruit
 *   - ArduinoJson by Benoit Blanchon
 * 
 * CONFIGURACIÓN ANTES DE CARGAR:
 *   1. Modificar ssid y password
 *   2. Modificar mqtt_server con IP del servidor Kali
 *   3. Verificar pines según tu conexión física
 */
