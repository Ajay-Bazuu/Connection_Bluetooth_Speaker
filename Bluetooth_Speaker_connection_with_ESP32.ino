#include "BluetoothA2DPSource.h"

BluetoothA2DPSource a2dp_source;

void setup() {
  Serial.begin(115200);

  // Set the name of the ESP32 for Bluetooth pairing
  //a2dp_source.set_name("ESP32_Foot_Piano");
  
  // Start Bluetooth and try connecting to the HW2020 speaker
  a2dp_source.start("HW2020");

  Serial.println("Attempting to connect to HW2020 Bluetooth speaker...");
}

void loop() {
  // Keep the loop running to maintain the Bluetooth connection
  delay(1000);

  // Check and print the connection status
  if (a2dp_source.is_connected()) {
    Serial.println("Connected to HW2020 speaker.");
  } else {
    Serial.println("Not connected. Waiting for HW2020 speaker...");
  }
}