# ESP32-HiveMQ-Starter
A minimal ESP32 template for sending any sensor data to HiveMQ over MQTT — edit one variable, keep the networking code untouched. Includes DHT22, ultrasonic, GPS, and soil moisture examples.



# ESP32 → HiveMQ Starter

A minimal, beginner-friendly template for sending **any** data from an ESP32
to [HiveMQ](https://www.hivemq.com/) over MQTT.

It's not built around one specific sensor. The "sensor" is just a single
variable (`sensorData`) that you fill in however you like — temperature,
distance, GPS coordinates, a button press, a battery voltage, anything.

```
ESP32-HiveMQ-Starter/
│
├── README.md
├── ESP32_HiveMQ_Template.ino
├── images/
│   ├── architecture.png
│   └── flowchart.png
└── examples/
    ├── DHT22/
    ├── Ultrasonic/
    ├── GPS/
    └── Soil_Moisture/
```

The `examples/` folder shows the same template adapted to a few common
sensors, so you can see exactly what changes and what doesn't.

---

## The Big Picture

Imagine your ESP32 is sending a text message every second:

```
ESP32
  ↓
Reads Sensor
  ↓
Makes a Message
  ↓
Sends to HiveMQ
  ↓
Website Receives It
  ↓
Shows Live Data
```

That's it. Everything in this template exists to make that loop happen.

---

## The Template Has Only 4 Editable Sections

Open `ESP32_HiveMQ_Template.ino`. You'll see it's split into 4 sections at
the top, followed by networking code you shouldn't need to touch.

```cpp
// 1. Wi-Fi
const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// 2. HiveMQ Broker
const char* MQTT_BROKER = "broker.hivemq.com";
const int MQTT_PORT = 1883;
const char* MQTT_TOPIC = "your/topic";

// 3. Data to Send
float sensorData = 0;

// 4. Update Sensor
void updateSensor() {
    sensorData++;
    // Example:
    // sensorData = analogRead(34);
    // sensorData = temperature;
    // sensorData = humidity;
}
```

Everything below that — connecting to Wi-Fi, connecting to HiveMQ,
reconnecting if the connection drops, and publishing the message every
second — is already written for you.

---

## Let Users Replace Only One Line

Whatever your sensor gives you, put it on the right side of `sensorData =`.

If your sensor gives you...

```cpp
temperature
```

...then write:

```cpp
sensorData = temperature;
```

If it gives you `distance`:

```cpp
sensorData = distance;
```

If it's a raw analog pin:

```cpp
sensorData = analogRead(34);
```

Nothing else in the file needs to change.

---

## What Can I Send?

Anything that fits in a number (or a short string) can go into `sensorData`:

```cpp
sensorData = temperature;
sensorData = humidity;
sensorData = pressure;
sensorData = distance;
sensorData = lightLevel;
sensorData = batteryVoltage;
sensorData = gasValue;
sensorData = analogRead(34);
```

---

## What's a "Topic"?

A topic is just the address your data is sent to. HiveMQ routes your
message to anyone who is listening on that same address.

```
Topic: home/temperature

ESP32
  temperature
    │
    ▼
HiveMQ Broker
    │
    ▼
Website subscribes to:
home/temperature
```

Topics are just strings, so you can organize them however makes sense
for your project:

```
robot/battery
factory/ph
weather/rain
garden/moisture
school/classroom/temp
```

---

## Replacing `updateSensor()` you can also use AI for this. Claude Code is good at these

The template includes a clearly marked block for this:

```cpp
// =======================================================
// Replace this function with your own sensor code.
// AI Prompt:
// "Replace updateSensor() with code for a DHT22 sensor."
// =======================================================
```

You can hand that comment to an AI assistant and ask it to generate just
the sensor-reading code. It only needs to fill in `updateSensor()` (and
possibly `setup()` for sensor initialization) — the Wi-Fi and MQTT code
stays untouched.

---


## Verifying Your Setup

Now that you have your code with your unique topic, let's confirm everything is working end to end.

### 1. Upload and Check the Wi-Fi Connection

Upload the sketch to your ESP32 and open the Serial Monitor. Confirm that the device connects to Wi-Fi successfully — once connected, it will automatically start publishing data to the HiveMQ broker.

<img width="1282" height="698" alt="Serial monitor showing Wi-Fi connected" src="https://github.com/user-attachments/assets/221384cb-4842-471c-b98d-dec168dd9d4a" />

Code uploaded and running.

### 2. Open the HiveMQ WebSocket Client

Go to HiveMQ's free browser-based MQTT client:
👉 **[https://www.hivemq.com/demos/websocket-client/](https://www.hivemq.com/demos/websocket-client/)**

**Connect to the broker:**

<img width="1920" height="1080" alt="Connecting to the broker in HiveMQ WebSocket client" src="https://github.com/user-attachments/assets/03038033-30d4-45da-87b3-19e5d8a23ca7" />

**Open the Subscriptions panel:**

<img width="1920" height="1080" alt="Opening the subscriptions panel" src="https://github.com/user-attachments/assets/a5041309-d5b9-4ba3-a3af-ce8fec94245e" />

**Subscribe to your topic:**

<img width="1920" height="1080" alt="Subscribing to your topic" src="https://github.com/user-attachments/assets/e243108b-bd96-4516-a3e2-acaf21013a22" />

**You should now see your live data streaming in:**

<img width="1920" height="1080" alt="Live data appearing in the client" src="https://github.com/user-attachments/assets/f6fdb8cb-9ce1-4b0a-892a-621ca3e75a5d" />

---

### 🎉 Congratulations!

Your ESP32 is now publishing live data to HiveMQ, and you've confirmed it's arriving on your topic.

---

## What's Next: Build Your Own Dashboard

With your broker and topic details in hand, you can build any custom website or data collection system on top of this. Here are two live examples built using this exact setup:

<img width="1920" height="895" alt="Karigar IoT dashboard example" src="https://github.com/user-attachments/assets/f1dcc3a5-b311-4b20-981a-c2860c641e00" />

**Live site:** [karigar-iot-dashboard](https://v0-karigar-io-t-dashboard.vercel.app/)

<img width="1919" height="970" alt="DrainSense dashboard example" src="https://github.com/user-attachments/assets/b3d2e355-d2a4-4d86-b39d-cc3f8ae45b22" />

**Live site:** [drain-sense-website](https://drain-sense-website.vercel.app/)



Here's simple prompt for you to command prompt for Google AI Studio or Claude to make them buikd custom website for you:

# AI Prompt: Build a Live Dashboard for This Data

Copy the prompt below, fill in the 3 blanks with your own broker info
(from the "2. HiveMQ Broker" section of your `.ino` file), and give it to
any AI coding assistant.

---

```
Build a single-page dashboard that connects to an MQTT broker over
WebSockets and shows live data as it arrives. Include a "Broker Info"
section at the top displaying the broker host, port, and topic being
used.

Broker details:
- Broker host: broker.hivemq.com
- WebSocket port: 8884 (TLS) 
- Topic: your/topic

Requirements:
- Use the MQTT.js library (from a CDN) to subscribe to the topic above.
- Show a "Broker Info" panel with the host, port, and topic clearly visible.
- Show the live value big and clear, updating in real time as messages arrive.
- Add a simple line chart of the last 50 values (use Chart.js from a CDN).
- Show a connection status indicator (Connected / Disconnected).
- Keep it a single self-contained HTML file, no build step, no backend.
```

---

### Notes for viewers

- Replace `broker.hivemq.com`, `8884`, and `your/topic` with the exact
  values from your own `.ino` file's "2. HiveMQ Broker" section.
- Public HiveMQ (`broker.hivemq.com`) exposes MQTT over WebSockets on
  port `8884` (TLS) or `8000` (non-TLS) — not the `1883` port your ESP32
  uses, since that's plain MQTT, not WebSockets. Browsers can only speak
  MQTT over WebSockets.
- If you're using your own private HiveMQ Cloud cluster instead of the
  public broker, use its WebSocket port and add your cluster's username/
  password to the prompt.





























## Learning Path

If you're new to this, work through the steps in order:

1. **What is MQTT?** A lightweight messaging protocol devices use to send
   small messages to each other.
2. **What is a broker?** A server that receives messages from senders and
   forwards them to anyone subscribed to the same topic.
3. **What is HiveMQ?** A public/hosted MQTT broker you can use for free
   to get started (`broker.hivemq.com`).
4. **Install Arduino IDE libraries** — you'll need `PubSubClient` (and
   any sensor-specific library, e.g. `DHT sensor library`).
5. **Connect ESP32 to Wi-Fi** — fill in section 1 of the template.
6. **Configure the broker and topic** — fill in section 2.
7. **Replace `updateSensor()`** with your sensor code — sections 3 and 4.
8. **Upload to the ESP32.**
9. **Verify messages** using a free MQTT client like
   [MQTT Explorer](http://mqtt-explorer.com/) or HiveMQ's own
   [WebSocket client](https://www.hivemq.com/demos/websocket-client/).
10. **Build a website or database** that subscribes to the same topic
    to display or store your live data.

---

<img width="1920" height="1080" alt="IoT" src="https://github.com/user-attachments/assets/f8fdd261-49ee-4aa6-84b6-da6387fa8d58" />

---

## Examples

Each folder in `examples/` is a complete sketch — the main template with
only sections 3 and 4 changed:

| Example | Sensor | What it sends |
|---|---|---|
| `DHT22/` | DHT22 temperature/humidity sensor | Temperature (°C) |
| `Ultrasonic/` | HC-SR04 ultrasonic sensor | Distance (cm) |
| `GPS/` | GPS module (TinyGPS++) | Latitude/longitude as JSON |
| `Soil_Moisture/` | Capacitive soil moisture sensor | Raw analog reading |

Use these as reference for how little needs to change to support a new
sensor.

---




`architecture.png` and `flowchart.png` are placeholders for your own
diagrams (e.g. exported from draw.io or Excalidraw) if you'd like visuals
beyond the ASCII diagrams above. They aren't required for the template
to work.
