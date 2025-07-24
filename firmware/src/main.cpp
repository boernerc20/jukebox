#include <Arduino.h>
#include "audio/audio.h"
#include "rfid/rfid.h"

void setup() {
    // Uncomment one of these to test the module
    // audioSetup();
    rfidSetup();
}

void loop() {
    // Uncomment one of these to test the module
    // audioLoop();
    rfidLoop();
}