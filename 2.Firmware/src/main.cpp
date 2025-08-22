#include <Arduino.h>
#include "audio/audio.h"
#include "rfid/rfid.h"

void setup() {
    audioSetup();
    rfidSetup();
    // playSong("aria.wav"); // Start with a default song
}

void loop() {
    audioLoop();
    rfidLoop();
    // debugPrintTag();
    // delay(500); 

}