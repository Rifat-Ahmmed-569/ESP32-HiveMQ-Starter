/*
  ============================================================
   ESP32 → HiveMQ Starter Template
  ============================================================
  This template sends ANY piece of data from your ESP32 to
  HiveMQ over MQTT. It doesn't care what the data is -
  temperature, distance, GPS coordinates, a button press,
  whatever. You only edit the 4 sections below.

  Everything below "MQTT + Wi-Fi plumbing" is networking code
  that stays the same no matter what sensor you use.

  Library required:
    - PubSubClient (install via Arduino IDE Library Manager)
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

const char* MQTT_TOPIC     = "your/topic";
const char* MQTT_CLIENT_ID = "esp32-client"; // change this if you run more than one ESP32


// ==============================
// 3. Data to Send
// Replace this variable with your own sensor value.
// ==============================

float sensorData = 0;


// ==============================
// 4. Update Sensor
// =======================================================
// Replace this function with your own sensor code.
//
// AI Prompt you can use:
// "Replace updateSensor() with code for a DHT22 sensor."
// =======================================================

void updateSensor()
{
    sensorData++;

    // Example replacements:
    // sensorData = analogRead(34);
    // sensorData = temperature;
    // sensorData = humidity;
    // sensorData = distance;
}


// ============================================================
// MQTT + Wi-Fi plumbing
// You shouldn't need to touch anything below this line.
// ============================================================

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

    Serial.println();
    Serial.print("Wi-Fi connected! IP address: ");
    Serial.println(WiFi.localIP());
}

void connectMQTT()
{
    while (!client.connected())
    {
        Serial.print("Connecting to HiveMQ...");

        if (client.connect(MQTT_CLIENT_ID))
        {
            Serial.println("connected!");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" -> retrying in 2 seconds");
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

    Serial.print("Sent to ");
    Serial.print(MQTT_TOPIC);
    Serial.print(": ");
    Serial.println(payload);

    delay(1000); // send once per second
}
