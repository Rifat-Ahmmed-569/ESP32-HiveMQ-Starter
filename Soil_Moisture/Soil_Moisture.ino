/*
  Example: Capacitive Soil Moisture Sensor → HiveMQ

  Only sections 3 and 4 differ from the main template.
  No extra library needed - this uses a plain analogRead().
*/

#include <WiFi.h>
#include <PubSubClient.h>

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
const char* MQTT_TOPIC     = "garden/moisture";
const char* MQTT_CLIENT_ID = "esp32-soil";

// ==============================
// 3. Data to Send
// ==============================
float sensorData = 0; // holds raw ADC reading (0-4095)

const int SOIL_PIN = 34;

// ==============================
// 4. Update Sensor
// ==============================
void updateSensor()
{
    sensorData = analogRead(SOIL_PIN);

    // Optional: convert to a 0-100% scale once you know your
    // sensor's dry/wet raw values, e.g.:
    // sensorData = map(analogRead(SOIL_PIN), 4095, 1200, 0, 100);
}

// ---- MQTT + Wi-Fi plumbing (same as main template) ----

WiFiClient   espClient;
PubSubClient client(espClient);

void connectWiFi()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }
}

void connectMQTT()
{
    while (!client.connected())
    {
        if (!client.connect(MQTT_CLIENT_ID))
        {
            delay(2000);
        }
    }
}

void setup()
{
    Serial.begin(115200);
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

    Serial.print("Soil reading: ");
    Serial.println(payload);

    delay(1000);
}
