#pragma once

#include <Arduino.h>

//
// WIFI & MQTT
//
#ifdef ESP32
#include <WiFi.h>
#include <Ticker.h>
#else
#include <ESP8266WiFi.h>
#endif

#include <AsyncMqttClient.h>

extern AsyncMqttClient mqttClient;

#ifdef ESP32
extern TimerHandle_t mqttReconnectTimer;
#endif

extern void setup();
extern void loop();

extern void connectToWifi();
extern void connectToMqtt();
extern void onMqttConnect(bool sessionPresent);
extern void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
extern void onMqttMessage(char* topic, char* payload, const AsyncMqttClientMessageProperties& properties, const size_t& len, const size_t& index, const size_t& total);
extern void onMqttPublish(uint16_t packetId);