#include "rfid.h"
#include "audio/audio.h"
#include <HardwareSerial.h>
#include <PN532.h>
#include <PN532_HSU.h>
#include "include/tag_song_map.h"
#include <AudioGeneratorWAV.h> 

#define PN532_RX_PIN 16
#define PN532_TX_PIN 17
#define CD_TOGGLE_PIN 32

HardwareSerial PN532Serial(1); // Use UART1
PN532_HSU pn532hsu(PN532Serial);
PN532 nfc(pn532hsu);
String tagId = "None", dispTag = "None";
byte nuidPICC[4];
String lastTagId = "";
extern AudioGeneratorWAV *wav;

enum JukeboxState {
    WAITING_FOR_CD,
    POLLING_FOR_TAG,
    PLAYING_AUDIO
};

JukeboxState currentState = WAITING_FOR_CD;
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 200; // ms

void rfidSetup() {
    Serial.begin(115200);
    PN532Serial.begin(115200, SERIAL_8N1, PN532_RX_PIN, PN532_TX_PIN);
    nfc.begin();
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata) {
        Serial.print("Didn't Find PN53x Module");
        while (1); // Halt
    }
    Serial.print("Found chip PN5");
    Serial.println((versiondata >> 24) & 0xFF, HEX);
    Serial.print("Firmware ver. ");
    Serial.print((versiondata >> 16) & 0xFF, DEC);
    Serial.print('.');
    Serial.println((versiondata >> 8) & 0xFF, DEC);
    nfc.SAMConfig();
    
    // Setup CD toggle pin
    pinMode(CD_TOGGLE_PIN, INPUT_PULLUP);
    delay(100); // Let pin stabilize
    lastButtonState = digitalRead(CD_TOGGLE_PIN);
    
    Serial.print("Initial button state: ");
    Serial.println(lastButtonState == HIGH ? "HIGH (unpressed)" : "LOW (pressed)");
    Serial.println("Jukebox ready - press button to insert CD...");
}

unsigned long lastRfidPoll = 0;
const unsigned long rfidPollInterval = 300; // ms

void rfidLoop() {
    // Handle button press (CD insert/eject toggle)
    handleCdToggle();
    
    // Only poll RFID when in POLLING_FOR_TAG state
    if (currentState == POLLING_FOR_TAG) {
        if (millis() - lastRfidPoll < rfidPollInterval) return;
        lastRfidPoll = millis();
        
        boolean success;
        uint8_t uid[8] = {0};
        uint8_t uidLength;
        success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
        
        if (success) {
            tagId = tagToString(uid, uidLength);
            Serial.print("Tag detected: ");
            Serial.println(tagId);
            
            uint8_t cdNumber = getCdNumberForTag(tagId);
            if (cdNumber > 0) {
                const char* filename = getSongFilenameForCd(cdNumber);
                if (filename) {
                    Serial.print("Playing: ");
                    Serial.println(filename);
                    playSong(filename);
                    currentState = PLAYING_AUDIO;
                    Serial.println("State: PLAYING_AUDIO - RFID polling stopped");
                }
            } else {
                Serial.println("Unknown tag!");
            }
        }
    }
}

void handleCdToggle() {
    bool currentButtonState = digitalRead(CD_TOGGLE_PIN);
    
    // Simple state change detection without complex debouncing
    if (currentButtonState != lastButtonState) {
        delay(50); // Simple debounce
        
        // Re-read to confirm state
        currentButtonState = digitalRead(CD_TOGGLE_PIN);
        
        if (currentButtonState != lastButtonState) {
            Serial.print("Button state changed: ");
            Serial.print(lastButtonState == HIGH ? "HIGH" : "LOW");
            Serial.print(" -> ");
            Serial.println(currentButtonState == HIGH ? "HIGH" : "LOW");
            
            if (currentButtonState == LOW) {
                // Switch LOW = CD Inserted: Start polling
                if (currentState == WAITING_FOR_CD) {
                    currentState = POLLING_FOR_TAG;
                    Serial.println("CD inserted - State: POLLING_FOR_TAG");
                    lastTagId = ""; // Reset last tag
                }
                
            } else {
                // Switch HIGH = CD Ejected: Stop everything
                if (currentState == POLLING_FOR_TAG || currentState == PLAYING_AUDIO) {
                    if (wav && wav->isRunning()) {
                        wav->stop();
                        delete wav;
                        wav = nullptr;
                        Serial.println("Audio stopped");
                    }
                    
                    currentState = WAITING_FOR_CD;
                    lastTagId = "";
                    Serial.println("CD ejected - State: WAITING_FOR_CD");
                    Serial.println("Insert CD to continue...");
                }
            }
            
            lastButtonState = currentButtonState;
        }
    }
}

String tagToString(byte id[], uint8_t length) {
    String tagId = "";
    for (uint8_t i = 0; i < length; i++) {
        tagId += String(id[i]);
        if (i < length - 1) tagId += "."; // Add a dot between bytes
    }
    return tagId;
}

void debugPrintTag() {
    uint8_t uid[8] = {0}; // Support up to 8 bytes
    uint8_t uidLength;
    if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
        String tagString = tagToString(uid, uidLength); // Use updated tagToString
        Serial.print("Detected tag UID: ");
        Serial.println(tagString);
    }
}