#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "CARLALAPTOP 7629";
const char* password = "42R83=j8";
const char* serverUrl = "http://10.175.20.195:3000/"; 

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  float temperature = 25.5; 
  float humidity = 60.0;    
  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");

  String httpRequestData = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + "}";
  int httpResponseCode = http.POST(httpRequestData);

  if (httpResponseCode > 0) {
    Serial.printf("HTTP Response code: %d\n", httpResponseCode);
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.printf("HTTP Error: %s\n", http.errorToString(httpResponseCode).c_str());
  }
  http.end();
  delay(5000); 
}