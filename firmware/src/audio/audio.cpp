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

AudioGeneratorWAV *wav;
AudioFileSourceSD *file;
AudioOutputI2S *out;

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
    out->SetGain(0.2);

    // Load WAV file
    file = new AudioFileSourceSD("/aria-mono.wav");
    wav = new AudioGeneratorWAV();
    wav->begin(file, out);

    pinMode(POT_PIN, INPUT);
}

void audioLoop() {
    int potValue = analogRead(POT_PIN);
    float gain = potValue / 4095.0;
    out->SetGain(gain);

    if (wav->isRunning()) {
        if (!wav->loop()) wav->stop();
    } else {
        delay(1000);
    }
}