#include <Arduino.h>
#include "include/audio.h"
#include "include/rfid.h"

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