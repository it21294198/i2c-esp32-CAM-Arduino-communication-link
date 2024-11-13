// Arduino Slave Code (Save as separate file)
#include <Wire.h>

#define I2C_SLAVE_ADDR 0x08

volatile byte receivedValue = 0;
volatile bool newData = false;

void setup() {
  Wire.begin(I2C_SLAVE_ADDR);  // Initialize I2C as slave
  Wire.onReceive(receiveEvent);  // Register receive event
  Wire.onRequest(requestEvent);  // Register request event
  
  Serial.begin(9600);
  Serial.println("Arduino I2C Slave initialized");
}

void loop() {
  if (newData) {
    Serial.print("Received from master: ");
    Serial.println(receivedValue);
    newData = false;
  }
  delay(50);  // Small delay to prevent overwhelming the serial output
}

void receiveEvent(int numBytes) {
  if (Wire.available()) {
    receivedValue = Wire.read();
    newData = true;
  }
}

void requestEvent() {
  // Send back processed data (for example, multiply received value by 2)
  int processedValue = receivedValue * 2;
  Wire.write(highByte(processedValue));  // Send high byte
  Wire.write(lowByte(processedValue));   // Send low byte
}