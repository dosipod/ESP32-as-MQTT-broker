#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "SSID";          // Replace with your network SSID
const char* password = "Password";  // Replace with your network password
const char* mqtt_server = "192.168.178.32";  // Replace with your MQTT broker
const char* mqtt_username = "alice"; // Replace with your MQTT username
const char* mqtt_password = "secret"; // Replace with your MQTT password

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
     if (client.connect("ESP32Client", mqtt_username, mqtt_password)) { 
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 1000) {  // Send a message every second
    lastMsg = now;

   int randomNumber = random(0, 100);  // Generate random number between 0 and 100
    snprintf(msg, 50, "Random Number: %d", randomNumber);
    Serial.print("Publishing message: ");
    Serial.println(msg);
    client.publish("esp32/random", msg);  // Publish message to topic "esp32/random"
  }
}
