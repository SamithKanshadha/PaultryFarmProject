#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define DHTPIN 5       
#define DHTTYPE DHT22   

const char* ssid = "Redmi Note 12";
const char* password = "skb123456";

const char* postmanApiUrl = "https://testingmyid.antlerfoundry.app/api/application/QualityControlData/SubmitTest";

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");

  dht.begin();
}

 void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  DynamicJsonDocument jsonDoc(1024);
  jsonDoc["temperature"] = temperature;
  jsonDoc["humidity"] = humidity;
  jsonDoc["QcParameter"] = "12"; 

  String jsonData;
  serializeJson(jsonDoc, jsonData);

  postDataToPostmanAPI(jsonData);

  delay(5000);
}


void postDataToPostmanAPI(String data) {
  HTTPClient http;

  http.begin(postmanApiUrl);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(data);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.print("HTTP POST request failed. Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();  
}
