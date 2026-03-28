#include <Arduino.h>
#include <WiFi.h>
#include <WiFiServer.h>
#include <WiFiClient.h>
#include "config.h"

const uint32_t UART_BAUD = 115200;  // stable baud rate
const int PACK_TIMEOUT_MS = 2;      // reduced packet assembly delay
const int BUFFER_SIZE = 4096;

const uint16_t TCP_PORT = 14550;

const int LED_PIN = LED_BUILTIN;
const int LED_ON = LOW;
const int LED_OFF = HIGH;

WiFiServer server(TCP_PORT);
WiFiClient client;

uint8_t uartToTcpBuf[BUFFER_SIZE];
uint16_t uartToTcpLen = 0;

uint8_t tcpToUartBuf[BUFFER_SIZE];

void setup() {
  Serial.begin(UART_BAUD);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LED_OFF);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(AP_IP, AP_IP, AP_NETMASK);
  WiFi.softAP(AP_SSID, AP_PASS);

  server.begin(TCP_PORT);
}

void loop() {
  int stationCount = WiFi.softAPgetStationNum();
  digitalWrite(LED_PIN, stationCount ? LED_ON : LED_OFF);

  if (server.hasClient()) {
    if (client && client.connected()) {
      WiFiClient newClient = server.available();
      newClient.stop();
    } else {
      client = server.available();
    }
  }

  if (client && client.connected()) {
    int avail = client.available();
    if (avail > 0) {
      int toRead = (avail > BUFFER_SIZE) ? BUFFER_SIZE : avail;
      client.read(tcpToUartBuf, toRead);
      Serial.write(tcpToUartBuf, toRead);
    }
  } else if (client) {
    client.stop();
  }

  int readCount = 0;
  while (Serial.available() && uartToTcpLen < BUFFER_SIZE && readCount < 256) {
    uartToTcpBuf[uartToTcpLen++] = Serial.read();
    readCount++;
  }

  if (uartToTcpLen > 0) {
    if (client && client.connected()) {
      client.write(uartToTcpBuf, uartToTcpLen);
      uartToTcpLen = 0;
    }
  } else if (uartToTcpLen >= BUFFER_SIZE) {
    uartToTcpLen = 0;
  }
}
