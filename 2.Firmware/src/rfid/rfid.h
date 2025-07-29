#pragma once

#include <Arduino.h>

void rfidSetup();
void rfidLoop();
String tagToString(byte id[], uint8_t length);
void debugPrintTag();