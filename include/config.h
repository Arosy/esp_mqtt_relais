#pragma once
#ifndef __CONFIG_H
#define __CONFIG_H
#define WIFI_SSID "YOUR_WIFI_NAME"
#define WIFI_PASS "YOUR_WIFI_PASS"

#define MQTT_HOST IPAddress(192, 168, 2, 2)
#define MQTT_PORT 1883
#define MQTT_TOPIC "esp32/mqtt_relais"

#define RELAY_PIN 16
#define BAUD_RATE 115200
#define WIFI_RECON_INTERVAL 30000
#endif
