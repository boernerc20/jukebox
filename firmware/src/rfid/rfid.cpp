#include "rfid.h"
#include <HardwareSerial.h>
#include <PN532.h>
#include <PN532_HSU.h>
#include "include/tag_song_map.h"

#define PN532_RX_PIN 16
#define PN532_TX_PIN 17

HardwareSerial PN532Serial(1); // Use UART1
PN532_HSU pn532hsu(PN532Serial);
PN532 nfc(pn532hsu);
String tagId = "None", dispTag = "None";
byte nuidPICC[4];

void rfidSetup() {
    Serial.begin(115200);
    PN532Serial.begin(115200, SERIAL_8N1, PN532_RX_PIN, PN532_TX_PIN);
    Serial.println("Hello Maker!");
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
}

void rfidLoop() {
    boolean success;
    uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};
    uint8_t uidLength;
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
    if (success) {
        Serial.print("UID Length: ");
        Serial.print(uidLength, DEC);
        Serial.println(" bytes");
        Serial.print("UID Value: ");
        for (uint8_t i = 0; i < uidLength; i++) {
            nuidPICC[i] = uid[i];
            Serial.print(" "); Serial.print(uid[i], DEC);
        }
        Serial.println();
        tagId = tagToString(nuidPICC);
        dispTag = tagId;
        Serial.print(F("tagId is : "));
        Serial.println(tagId);
                // Map tag to CD number
        uint8_t cdNumber = getCdNumberForTag(tagId);
        if (cdNumber > 0) {
            Serial.print("Mapped to CD number: ");
            Serial.println(cdNumber);
            // TODO: Call your audio playback function here, e.g.:
            // playSong(cdNumber);
        } else {
            Serial.println("Tag not recognized!");
        }
        Serial.println("");
        delay(1000);
    } else {
        Serial.println("Timed out! Waiting for a card...");
    }
}

String tagToString(byte id[4]) {
    String tagId = "";
    for (byte i = 0; i < 4; i++) {
        if (i < 3) tagId += String(id[i]) + ".";
        else tagId += String(id[i]);
    }
    return tagId;
}