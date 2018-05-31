#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "WiFiClient.h"

const char* ssid = "";
const char* password = "";

ESP8266WebServer server(80);
int timeout = 0;
bool motor = false;

//keep connection alive if client is connected, kill connection and turn off pins if timeout.
void keep_alive() {
  if (server.client() == 0) {
    timeout++;
    delay(5);
    if (timeout > 50) {
      timeout = 0;
      if (motor) {
        Serial.println("off");
        motor = false;
        digitalWrite(D0, LOW);
        digitalWrite(D1, LOW);
        digitalWrite(D2, LOW);
        digitalWrite(D3, LOW);
      }
    }
  }
}

void handleControls() {
  String message;
  timeout = 0;

  if (server.arg("left_bank") != "") {
    int left_bank = server.arg("left_bank").toInt();
    if (left_bank == 2) {
      digitalWrite(D0, LOW);
      digitalWrite(D1, LOW);
      message += "left_bank relays off\n";
    }
    if (left_bank == 1) {
      motor = true;
      digitalWrite(D0, LOW);
      digitalWrite(D1, HIGH);
      message += "left_bank_relay_0: on, left_bank_relay_1: off\n";
    }
    if (left_bank == 0) {
      motor = true;
      digitalWrite(D1, LOW);
      digitalWrite(D0, HIGH);
      message += "left_bank_relay_0: off, left_bank_relay_1: on\n";
    }
  }

  if (server.arg("right_bank") != "") {
    int right_bank = server.arg("right_bank").toInt();
    if (right_bank == 2) {
      digitalWrite(D2, LOW);
      digitalWrite(D3, LOW);
      message += "right_bank relays off\n";
    }
    if (right_bank == 1) {
      motor = true;
      digitalWrite(D2, LOW);
      digitalWrite(D3, HIGH);
      message += "right_bank_relay_0: on, right_bank_relay_1: off\n";
    }
    if (right_bank == 0) {
      motor = true;
      digitalWrite(D3, LOW);
      digitalWrite(D2, HIGH);
      message += "right_bank_relay_0: off, right_bank_relay_1: on\n";
    }
  }
  server.send(200, "text / plain", message);
}


void setup() {

  //Communication setup.
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Waiting to connect…");
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());
  server.on("/controls", handleControls);
  server.begin();
  Serial.println("Server listening:");

  //Motor Interface setup.
  pinMode(D0, OUTPUT); // left_bank_relay_0
  pinMode(D1, OUTPUT); // left_bank_relay_1
  pinMode(D2, OUTPUT); // right_bank_relay_0
  pinMode(D3, OUTPUT); // right_bank_relay_1

}

void loop() {
  server.handleClient();
  keep_alive();
}
