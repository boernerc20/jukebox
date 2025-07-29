#pragma once

#include <Arduino.h>

void audioSetup();
void audioLoop();
void playSong(const char* filename);
void handleCdToggle();
