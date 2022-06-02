/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>
#include <ArduinoJson.h>
ESP8266WiFiMulti WiFiMulti;
float ph, tds, ec;
void setup()
{

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--)
  {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("PARAMITA", "76602223");
}

void loop()
{
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED))
  {

    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://floating-falls-99674.herokuapp.com/api/planted/b75dd08d-212e-4ed2-a9c4-90d15aeb3feb"))
    { // HTTP

      http.setAuthorization("");
      http.addHeader("Authorization", "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6ImEwY2FiOTRiLTI0OWMtNGIwMS1iN2Q0LWVhZDY2ZjEzNzBhMiIsImlhdCI6MTY1NDE3NzQ4MCwiZXhwIjoxNjU0MjYzODgwfQ.XRwiZUgqItBBpmHJLVGf-zooLATPE8iijbG8Sx-CqJM");
      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0)
      {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
        {
          String payload = http.getString();
          Serial.println(payload);
          if (httpCode == 200)
          {
            // Allocate JsonBuffer
            // Use arduinojson.org/assistant to compute the capacity.
            const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(8);
            DynamicJsonBuffer jsonBuffer(capacity);
            // Parse JSON object
            JsonObject &root = jsonBuffer.parseObject(payload);
            if (!root.success())
            {
              Serial.println(F("Parsing failed!"));
              return;
            };

            ph = root["data"]["ph"];
            tds = root["data"]["tds"];
            ec = root["data"]["ec"];

            // Decode JSON/Extract values
            Serial.println("ph:" + String(ph));
            Serial.println("tds:" + String(tds));
            Serial.println("ec:" + String(ec));
            if (ph > 0)
            {
              Serial.print("Bisa konversi");
            }
          }
        }
      }
      else
      {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    }
    else
    {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }

  delay(5000);
}
