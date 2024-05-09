#include <Wire.h>
#include "Adafruit_S35710.h"

// Optional reset pin connected to the S35710
const uint8_t RESET_PIN = -1;

// Create the S35710 with the reset pin
Adafruit_S35710 s35710 = Adafruit_S35710(RESET_PIN);


void setup() {
  // Start the serial communication
  Serial.begin(115200);
  while (!Serial); // Wait for serial port to connect

  Serial.println("Adafruit S-35710 Test!");

  if (!s35710.begin(&Wire, S35710_I2C_ADDRESS)) {
    Serial.println("Failed to initialize S-35710!");
    while (1);
  }
  Serial.println("S35710 initialized!");
  
  uint32_t wakeupTimeValue = 3;  // 3 seconds

  Serial.print("Setting wake-up time to ");
  Serial.print(wakeupTimeValue);
  Serial.println(" seconds");
  if (! s35710.setWakeUpTimeRegister(wakeupTimeValue)) {
    Serial.println("Failed to set wake-up time register!");
    while (1);
  }
  
  // Test getting the wake-up time register
  int32_t readWakeupTimeValue = s35710.getWakeUpTimeRegister();
  if (readWakeupTimeValue == -1) {
    Serial.println("Failed to read wake-up time register!");
    while (1);
  }
  Serial.print("Wake-up time set to ");
  Serial.print(readWakeupTimeValue);
  Serial.println(" seconds");

  Serial.println();
}

void loop() {
  int32_t readTimeValue = s35710.getTimeRegister();
  int32_t readWakeupTimeValue = s35710.getWakeUpTimeRegister();
  if (readTimeValue == -1 || readWakeupTimeValue == -1) {
    // Hmm failed to read? wait & retry
    delay(1000);
    return;
  }

  Serial.print("Internal timer: ");
  Serial.print(readTimeValue);
  Serial.print(" s, alarm set: ");
  Serial.print(readWakeupTimeValue);
  Serial.println(" s");
  
  delay(1000);
}
