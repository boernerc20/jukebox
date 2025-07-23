// /*
//   ESP32 SD I2S Music Player
//   esp32-i2s-sd-player.ino
//   Plays MP3 file from microSD card
//   Uses MAX98357 I2S Amplifier Module
//   Uses ESP32-audioI2S Library - https://github.com/schreibfaul1/ESP32-audioI2S
//   * 
//   DroneBot Workshop 2022
//   https://dronebotworkshop.com
// */

// // Include required libraries
// #include "Arduino.h"
// #include "Audio.h"
// #include "SD.h"
// #include "FS.h"
 
// // microSD Card Reader connections
// #define SD_CS          5
// #define SPI_MOSI      23 
// #define SPI_MISO      19
// #define SPI_SCK       18
 
// // I2S Connections
// #define I2S_DOUT      22
// #define I2S_BCLK      26
// #define I2S_LRC       25
 
//  // Create Audio object
// Audio audio;
 
// void setup() {
    
//     // Set microSD Card CS as OUTPUT and set HIGH
//     pinMode(SD_CS, OUTPUT);      
//     digitalWrite(SD_CS, HIGH); 
    
//     // Initialize SPI bus for microSD Card
//     SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    
//     // Start Serial Port
//     Serial.begin(9600);
    
//     // Start microSD Card
//     if(!SD.begin(SD_CS))
//     {
//       Serial.println("Error accessing microSD card!");
//       while(true); 
//     }
    
//     // Setup I2S 
//     audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    
//     // Set Volume
//     audio.setVolume(5);
    
//     // Open music file
//     audio.connecttoFS(SD,"/aria.wav");
    
// }
 
// void loop()
// {
//     audio.loop();    
// }

#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>

#include <AudioFileSourceSD.h>
#include <AudioGeneratorWAV.h>
#include <AudioOutputI2S.h>

// SPI Pins for SD card
#define SD_CS     5
#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_SCK  18

// I2S Pins to MAX98357
#define I2S_BCLK  26
#define I2S_LRC   25  // also called WS or LRCK
#define I2S_DOUT  22  // also called DIN or DATA

AudioGeneratorWAV *wav;
AudioFileSourceSD *file;
AudioOutputI2S *out;

void setup() {
  Serial.begin(115200);

  // Init SD
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  if (!SD.begin(SD_CS)) {
    Serial.println("SD Card mount failed!");
    while (true);
  }

  // Init I2S output
  out = new AudioOutputI2S();
  out->SetPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  out->SetGain(0.6);  // Set volume (0.0 to 1.0)

  // Load WAV file
  file = new AudioFileSourceSD("/aria.wav");
  wav = new AudioGeneratorWAV();
  wav->begin(file, out);
}

void loop() {
  if (wav->isRunning()) {
    if (!wav->loop()) wav->stop();
  } else {
    delay(1000);  // Finished playing
  }
}
