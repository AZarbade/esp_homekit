#include <Arduino.h>
#include <WiFiMulti.h>

class WiFiSetup {
public:
  WiFiSetup(const char* ssid, const char* password) : ssid(ssid), password(password) {}

  void begin() {
    wifiMulti.addAP(ssid, password);
    pinMode(LED_BUILTIN, OUTPUT);

    int connectionAttempts = 0;
    while (wifiMulti.run() != WL_CONNECTED) {
      Serial.print(".");
      delay(100);
      connectionAttempts++;

      if (connectionAttempts >= MAX_CONNECTION_ATTEMPTS) {
        Serial.println("\nFailed to connect to WiFi");
        return; // Exit the function if connection fails
      }
    }

    Serial.println("Connected!");
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println(WiFi.localIP());
  }

private:
  const char* ssid;
  const char* password;
  WiFiMulti wifiMulti;
  static const int MAX_CONNECTION_ATTEMPTS = 10;
};
