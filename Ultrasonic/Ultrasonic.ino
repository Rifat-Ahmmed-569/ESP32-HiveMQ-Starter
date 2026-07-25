/*
  Example: HC-SR04 Ultrasonic Distance Sensor → HiveMQ

  Only sections 3 and 4 differ from the main template.
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
const char* MQTT_TOPIC     = "garage/distance";
const char* MQTT_CLIENT_ID = "esp32-ultrasonic";

// ==============================
// 3. Data to Send
// ==============================
float sensorData = 0; // holds distance in centimeters

// HC-SR04 pins
const int TRIG_PIN = 5;
const int ECHO_PIN = 18;

// ==============================
// 4. Update Sensor
// ==============================
void updateSensor()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    float distance = duration * 0.0343 / 2.0; // cm

    sensorData = distance;
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
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
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

    Serial.print("Distance: ");
    Serial.println(payload);

    delay(1000);
}
