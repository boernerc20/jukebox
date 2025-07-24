#pragma once

#include <Arduino.h>

void rfidSetup();
void rfidLoop();
String tagToString(byte id[4]);