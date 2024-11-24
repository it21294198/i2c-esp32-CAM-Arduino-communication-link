// ESP32 Master Code
#include <Wire.h>

#define I2C_SLAVE_ADDR 0x08  // Arduino slave address
#define SDA_PIN 15           // SDA pin on ESP32
#define SCL_PIN 14           // SCL pin on ESP32

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN, 100000);  // Initialize I2C with specified pins and frequency
  
  if (!Wire.begin(SDA_PIN, SCL_PIN, 100000)) {
    Serial.println("I2C initialization failed!");
    while(1);
  }
  
  Serial.println("ESP32 I2C Master initialized");
}

void loop() {
  static int counter = 0;
  
  // Send data to slave
  Wire.beginTransmission(I2C_SLAVE_ADDR);
  Wire.write(counter);
  byte error = Wire.endTransmission();
  
  if (error == 0) {
    Serial.print("Sent value to slave: ");
    Serial.println(counter);
    
    // Request 2 bytes from slave
    Wire.requestFrom(I2C_SLAVE_ADDR, 2);
    
    if (Wire.available() >= 2) {
      byte highByte = Wire.read();
      byte lowByte = Wire.read();
      int receivedValue = (highByte << 8) | lowByte;
      
      Serial.print("Received from slave: ");
      Serial.println(receivedValue);
    }
  } else {
    Serial.print("Error sending data. Error code: ");
    Serial.println(error);
  }
  
  counter++;
  if (counter > 255) counter = 0;
  
  delay(1000);  // Wait for a second before next transmission
}