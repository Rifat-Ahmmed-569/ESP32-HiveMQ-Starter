/*
  Example: DHT22 (Temperature) → HiveMQ

  This is the main template with ONLY sections 3 and 4 changed.
  Everything else (Wi-Fi, HiveMQ, MQTT plumbing) is identical
  to ESP32_HiveMQ_Template.ino.

  Library required:
    - DHT sensor library (by Adafruit)
    - Adafruit Unified Sensor (dependency)
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// ==============================
// 1. Wi-Fi
// ==============================
const char* WIFI_SSID     = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// ==============================
// 2. HiveMQ Broker
// ==============================
const char* MQTT_BROKER = "broker.hivemq.com";
const int   MQTT_PORT   = 1883;
const char* MQTT_TOPIC     = "home/temperature";
const char* MQTT_CLIENT_ID = "esp32-dht22";

// ==============================
// 3. Data to Send
// ==============================
float sensorData = 0; // holds temperature in Celsius

// DHT22 setup
#define DHTPIN  4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ==============================
// 4. Update Sensor
// ==============================
void updateSensor()
{
    float temperature = dht.readTemperature();

    if (!isnan(temperature))
    {
        sensorData = temperature;
    }
}

// ---- MQTT + Wi-Fi plumbing (same as main template) ----

WiFiClient   espClient;
PubSubClient client(espClient);

void connectWiFi()
{
    Serial.print("Connecting to Wi-Fi");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi connected!");
}

void connectMQTT()
{
    while (!client.connected())
    {
        if (client.connect(MQTT_CLIENT_ID))
        {
            Serial.println("Connected to HiveMQ!");
        }
        else
        {
            delay(2000);
        }
    }
}

void setup()
{
    Serial.begin(115200);
    dht.begin();
    connectWiFi();
    client.setServer(MQTT_BROKER, MQTT_PORT);
}

void loop()
{
    if (!client.connected())
    {
        connectMQTT();
    }
    client.loop();

    updateSensor();

    String payload = String(sensorData);
    client.publish(MQTT_TOPIC, payload.c_str());

    Serial.print("Sent: ");
    Serial.println(payload);

    delay(2000); // DHT22 needs at least ~2s between reads
}
