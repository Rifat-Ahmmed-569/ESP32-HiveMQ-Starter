/*
  Example: GPS Module → HiveMQ

  This one sends TWO values (latitude and longitude), so it
  publishes a small JSON string instead of a single number.
  Everything else about the pattern is the same.

  Library required:
    - TinyGPS++ (by Mikal Hart)
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

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
const char* MQTT_TOPIC     = "vehicle/location";
const char* MQTT_CLIENT_ID = "esp32-gps";

// ==============================
// 3. Data to Send
// ==============================
String sensorData = "{}"; // holds a small JSON payload: {"lat":..,"lng":..}

TinyGPSPlus gps;
HardwareSerial GPSSerial(1); // UART1: RX=16, TX=17

// ==============================
// 4. Update Sensor
// ==============================
void updateSensor()
{
    while (GPSSerial.available() > 0)
    {
        gps.encode(GPSSerial.read());
    }

    if (gps.location.isValid())
    {
        sensorData = "{\"lat\":" + String(gps.location.lat(), 6) +
                     ",\"lng\":" + String(gps.location.lng(), 6) + "}";
    }
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
    GPSSerial.begin(9600, SERIAL_8N1, 16, 17);
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

    // Note: sensorData is already a String here, so publish it directly
    client.publish(MQTT_TOPIC, sensorData.c_str());

    Serial.print("Sent: ");
    Serial.println(sensorData);

    delay(2000);
}
