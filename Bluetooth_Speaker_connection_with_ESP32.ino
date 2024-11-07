#include "BluetoothA2DPSource.h"
#include <SPI.h>
#include <SD.h>

#define SD_CS_PIN 5  // Adjust this to match your CS pin


BluetoothA2DPSource a2dp_source;

void setup() {
  Serial.begin(115200);
  // Start Bluetooth and try connecting to the HW2020 speaker
  a2dp_source.start("realme Pocket Speaker");

  Serial.println("Attempting to connect to realme Pocket Speaker Bluetooth speaker...");

  // Check and print the connection status
  while (!a2dp_source.is_connected()) {
    Serial.println("Not connected. Waiting for realme Pocket Speaker...");
    delay(1000);
  }
  Serial.println("Connected to realme Pocket Speaker.");

  // Attempt to initialize the SD card
  Serial.println("Initializing SD card...");
  while (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card initialization failed. Retrying...");
    delay(1000);  // Retry every second
  }
  Serial.println("SD card initialized successfully.");
  delay(2000);
    // Continuously search for the 'piano.wav' file
  while(!SD.exists("/piano.wav")){
      Serial.println("File 'piano.wav' not found.");
      delay(1000);
  }
  Serial.println("File 'piano.wav' found!");

}

void loop() {

}