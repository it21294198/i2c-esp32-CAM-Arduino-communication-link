// ESP32 Master Code
#include <Wire.h>
#include <ArduinoJson.h>

#define I2C_SLAVE_ADDR 0x08
#define SDA_PIN 15
#define SCL_PIN 14
#define JSON_CAPACITY 256

StaticJsonDocument<JSON_CAPACITY> doc;

void setup() {
  Serial.begin(115200);
  
  if (!Wire.begin(SDA_PIN, SCL_PIN, 100000)) {
    Serial.println("I2C initialization failed!");
    while(1);
  }
  
  Serial.println("ESP32 I2C Master initialized");
}

void loop() {
  // Create JSON data
  doc.clear();
  JsonArray array = doc.createNestedArray("points");
  
  JsonObject point1 = array.createNestedObject();
  point1["x"] = 2200;
  point1["y"] = 2500;
  point1["z"] = 2000;

  JsonObject point2 = array.createNestedObject();
  point2["x"] = 200;
  point2["y"] = 500;
  point2["z"] = 1000;
  
  // Serialize JSON to buffer
  char jsonBuffer[JSON_CAPACITY];
  size_t len = serializeJson(doc, jsonBuffer);
  
  // Send length first
  Wire.beginTransmission(I2C_SLAVE_ADDR);
  Wire.write((byte)(len & 0xFF));  // Lower byte of length
  Wire.write((byte)(len >> 8));    // Upper byte of length
  byte error = Wire.endTransmission();
  
  if (error == 0) {
    // Send JSON data in chunks
    for(size_t i = 0; i < len; i++) {
      Wire.beginTransmission(I2C_SLAVE_ADDR);
      Wire.write(jsonBuffer[i]);
      error = Wire.endTransmission();
      
      if (error != 0) {
        Serial.print("Error sending chunk. Error code: ");
        Serial.println(error);
        break;
      }
      delay(5);  // Small delay between chunks
    }
    
    // Request processed data back
    Wire.requestFrom(I2C_SLAVE_ADDR, 2);
    if (Wire.available() >= 2) {
      byte highByte = Wire.read();
      byte lowByte = Wire.read();
      int receivedValue = (highByte << 8) | lowByte;
      
      Serial.print("Received from slave: ");
      Serial.println(receivedValue);
    }
  } else {
    Serial.print("Error sending length. Error code: ");
    Serial.println(error);
  }
  
  delay(1000);
}