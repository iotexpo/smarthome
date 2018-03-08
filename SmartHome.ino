#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define PIN 21
#define LED_COUNT 1

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "richacker"
#define AIO_KEY         "2067219354c949e399a89bc95df3b662"
#define AIO_FEED        "richacker/feeds/light"

const char* ssid     = "Rishi";
const char* password = "Rishi1234";

WiFiClient espClient;
PubSubClient client(espClient);

Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  Serial.begin(9600);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  client.setServer(AIO_SERVER, 1883);
  client.setCallback(callback);
  leds.begin();
  leds.show();
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    char receivedChar = (char)payload[i];
    Serial.print(receivedChar);
    if (receivedChar == '0') {
      leds.setPixelColor(0, 0x2e, 0x40, 0x53);
      leds.show();
    }
    if (receivedChar == '1') {
      leds.clear();
      leds.show();
    }

  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32", AIO_USERNAME, AIO_KEY)) {
      Serial.println("connected");
      client.subscribe(AIO_FEED);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}