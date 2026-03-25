#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#define UART_BAUD 115200  // use stable default; increase if hardware supports
#define PACK_TIMEOUT_MS 2 // reduced packet assembly delay
#define BUFFER_SIZE 4096

#define UDP_PORT 14550

const char *AP_SSID = "AA_Link_00001";
const char *AP_PASS = "TroLoLo_AA";
const IPAddress AP_IP(192, 168, 4, 1);
const IPAddress AP_NETMASK(255, 255, 255, 0);

const int LED_PIN = LED_BUILTIN;
const int LED_ON = LOW;
const int LED_OFF = HIGH;

WiFiUDP udp;
IPAddress remoteIp;
uint16_t remotePort = 0;
unsigned long lastUdpTime = 0;
const unsigned long UDP_TIMEOUT_MS = 30000; // 30 seconds

uint8_t uartToUdpBuf[BUFFER_SIZE];
uint16_t uartToUdpLen = 0;

uint8_t udpToUartBuf[BUFFER_SIZE];

void setup() {
  Serial.begin(UART_BAUD);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LED_OFF);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(AP_IP, AP_IP, AP_NETMASK);
  WiFi.softAP(AP_SSID, AP_PASS);

  udp.begin(UDP_PORT);

  // Serial.println("UDP MAVLink bridge started");
  // Serial.print("AP "); Serial.println(AP_SSID);
  // Serial.print("UDP port "); Serial.println(UDP_PORT);
}

void loop() {
  int stationCount = WiFi.softAPgetStationNum();
  digitalWrite(LED_PIN, stationCount ? LED_ON : LED_OFF);

  int packetSize = udp.parsePacket();
  if (packetSize > 0) {
    if (packetSize > BUFFER_SIZE) packetSize = BUFFER_SIZE;
    remoteIp = udp.remoteIP();
    remotePort = udp.remotePort();
    lastUdpTime = millis();
    udp.read(udpToUartBuf, packetSize);
    Serial.write(udpToUartBuf, packetSize);
  }

  if (remotePort > 0 && millis() - lastUdpTime > UDP_TIMEOUT_MS) {
    remoteIp = IPAddress(0, 0, 0, 0);
    remotePort = 0;
  }

  while (Serial.available() && uartToUdpLen < BUFFER_SIZE) {
    uartToUdpBuf[uartToUdpLen++] = Serial.read();
  }

  if (uartToUdpLen > 0 && remotePort > 0) {
    udp.beginPacket(remoteIp, remotePort);
    udp.write(uartToUdpBuf, uartToUdpLen);
    udp.endPacket();
    uartToUdpLen = 0;
  }
}
