#include "BluetoothA2DPSource.h"
#include <SPI.h>
#include <SD.h>

#define SD_CS_PIN 5  // Adjust this to match your CS pin

BluetoothA2DPSource a2dp_source;
File audioFile;

// Derived class that implements the abstract SoundData class
class MySoundData : public SoundData {
public:
  byte* buffer;  // To hold the audio data
  int size;      // Size of the data

  MySoundData(byte* buffer, int size) {
    this->buffer = buffer;
    this->size = size;
  }

  // Implementing pure virtual functions from SoundData
  int32_t get2ChannelData(int32_t pos, int32_t len, uint8_t* data) override {
    if (pos + len <= size) {
      memcpy(data, buffer + pos, len);
      return len;
    }
    return 0;
  }

  int32_t getData(int32_t pos, Frame& channels) override {
    return 0;  // You can implement this if you need specific channel data.
  }

  void setDataRaw(uint8_t* data, int32_t len) override {
    memcpy(buffer, data, len);  // Just copies data into buffer
  }
};

void setup() {
  Serial.begin(115200);

  // Start Bluetooth and try connecting to the HW2020 speaker
  a2dp_source.start("realme Pocket Speaker");

  Serial.println("Attempting to connect to HW2020");
  while (!a2dp_source.is_connected()) {
    Serial.println("Not connected. Waiting for HW2020");
    delay(1000);
  }
  Serial.println("Connected to HW2020 Speaker.");
  delay(1000);

  // Initialize SD card
  Serial.println("Initializing SD card...");
  while (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card initialization failed. Retrying...");
    delay(1000);  // Retry every second
  }
  Serial.println("SD card initialized successfully.");

  delay(2000);  // Wait a little before checking the file

  // Search for the piano1.pcm file on the SD card
  while (!SD.exists("/piano4.pcm")) {
    Serial.println("Fil piano not found.");
    delay(1000);
  }
  Serial.println("File 'piano' found!");

  // Open the .pcm file for reading
  audioFile = SD.open("/piano4.pcm", FILE_READ);
  if (!audioFile) {
    Serial.println("Failed to open the file.");
    return;
  }

  // Now, stream the audio data from the PCM file
  playAudioFile(audioFile);
}

void loop() {
  // Nothing in loop, as everything is handled in setup for this task
}

void playAudioFile(File file) {
  Serial.println("Starting to play the audio data...");

  byte buffer[6144];  // Buffer to hold audio data
  size_t bytesRead;
  
  // Read and play the data from the PCM file
  while (file.available()) {
    bytesRead = file.read(buffer, sizeof(buffer));  // Read raw PCM audio data from the file

    if (bytesRead > 0) {
      // Create an instance of MySoundData
      MySoundData soundData(buffer, bytesRead);

      // Send the audio data to the Bluetooth speaker
      a2dp_source.write_data(&soundData);
    }

    delay(20);  // Small delay to control playback rate
  }

  Serial.println("Finished playing the audio data.");
  file.close();  // Close the file when done
}
