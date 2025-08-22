#include "audio.h"
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
#define I2S_LRC   25
#define I2S_DOUT  22

// Potentiometer pin for volume control
#define POT_PIN 34

AudioGeneratorWAV *wav = nullptr;
AudioFileSourceSD *file = nullptr;
AudioOutputI2S *out = nullptr;

void audioSetup() {
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
    out->SetGain(0.4);

    pinMode(POT_PIN, INPUT);

    delay(1000); // Give time for serial monitor to connect
    // List files on SD card
    File root = SD.open("/");
    while (true) {
        File entry = root.openNextFile();
        if (!entry) break;
        Serial.println(entry.name());
        entry.close();
    }

    // Do NOT start any song here
}

void audioLoop() {
    int potValue = analogRead(POT_PIN);
    float gain = potValue / 4095.0;
    // float gain = 0.5; // Set gain to 50% for basic test
    if (out) out->SetGain(gain);

    if (wav && wav->isRunning()) {
        if (!wav->loop()) {
            wav->stop();
            delete wav;
            wav = nullptr;
            if (file) {
                delete file;
                file = nullptr;
            }
        }
    }
    // If nothing is playing, do nothing
}

void playSong(const char* filename) {
    // Stop any currently playing song
    if (wav && wav->isRunning()) {
        wav->stop();
        delete wav;
        wav = nullptr;
    }
    if (file) {
        delete file;
        file = nullptr;
    }

    // Open new file and start playback
    String path = "/";
    path += filename;
    file = new AudioFileSourceSD(path.c_str());
    wav = new AudioGeneratorWAV();
    wav->begin(file, out);
    Serial.print("Started playing: ");
    Serial.println(path);
}