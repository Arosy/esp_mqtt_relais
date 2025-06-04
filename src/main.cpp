#include "config.h"
#include "main.h"

AsyncMqttClient mqttClient;
#ifdef ESP32
TimerHandle_t mqttReconnectTimer;
#endif

unsigned long previous = 0;

void setup()
{
    Serial.begin(BAUD_RATE);

    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);

#ifdef ESP32
    mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
#endif

    mqttClient.onConnect(onMqttConnect);
    mqttClient.onDisconnect(onMqttDisconnect);
    mqttClient.onPublish(onMqttPublish);
    mqttClient.onMessage(onMqttMessage);
    mqttClient.setServer(MQTT_HOST, MQTT_PORT);

    connectToWifi();
    connectToMqtt();

    Serial.println("booted");
}

void loop()
{
    unsigned long ticks = millis();

    // if wifi is down, try reconnecting every WIFI_RECON_INTERVAL seconds
    if ((WiFi.status() != WL_CONNECTED) && (ticks - previous >= WIFI_RECON_INTERVAL))
    {
        Serial.println("restarting esp, because wifi connection is lost");
        ESP.restart();
    }
}

void connectToWifi()
{
#ifdef ESP32
    WiFi.mode(WIFI_STA);
#endif

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
}

void connectToMqtt()
{
    Serial.println("Connecting to MQTT...");
    mqttClient.connect();
}

void onMqttPublish(uint16_t packetId)
{
    Serial.printf("Publish acknowledged with packetId: %d\n", packetId);
}

void onMqttMessage(char *topic, char *payload, const AsyncMqttClientMessageProperties &properties, const size_t &len, const size_t &index, const size_t &total)
{
    (void)payload;

    if (payload[0] == '1' || (payload[0] == 'O' && payload[1] == 'N') || (payload[0] == 'o' && payload[1] == 'n'))
    {
        digitalWrite(RELAY_PIN, HIGH);
    }
    else
    {
        digitalWrite(RELAY_PIN, LOW);
    }
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
    Serial.println("restarting esp, because mqtt connection is lost");
    ESP.restart();
}

void onMqttConnect(bool sessionPresent)
{
    Serial.println("Connected to MQTT.");
    Serial.print("Session present: ");
    Serial.println(sessionPresent);
    mqttClient.subscribe(MQTT_TOPIC, 2);
}