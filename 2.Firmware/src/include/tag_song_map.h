#include <Arduino.h>

// Tag IDs as strings, mapped to CD numbers
struct TagSongMapEntry {
    String tagId;
    uint8_t cdNumber;
};

// Add your tag IDs here 
const TagSongMapEntry tagSongMap[] = {
    {"4.54.229.34.54.30.145", 1},
    {"4.54.228.34.54.30.145", 2},
    {"4.54.227.34.54.30.145", 3},
    {"4.54.230.34.54.30.145", 4},
    {"4.54.231.34.54.30.145", 5},
    {"4.54.226.34.54.30.145", 6},
    {"4.54.202.34.54.30.145", 7},
    {"4.54.204.34.54.30.145", 8},
    {"4.54.203.34.54.30.145", 9},
    {"4.54.205.34.54.30.145", 10}
    // Add more tags here
};

const size_t tagSongMapSize = sizeof(tagSongMap) / sizeof(TagSongMapEntry);

// Returns CD number for a given tagId, or 0 if not found
uint8_t getCdNumberForTag(const String& tagId) {
    for (size_t i = 0; i < tagSongMapSize; ++i) {
        if (tagSongMap[i].tagId == tagId) {
            return tagSongMap[i].cdNumber;
        }
    }
    return 0; // Not found
}

const char* getSongFilenameForCd(uint8_t cdNumber) {
    switch (cdNumber) {
        case 1: return "13.wav";
        case 2: return "cat.wav"; // will replace
        case 3: return "blocks.wav";
        case 4: return "chirp.wav";
        case 5: return "far.wav";
        case 6: return "mall.wav";
        case 7: return "mellohi.wav";
        case 8: return "stal.wav";
        case 9: return "strad.wav";
        case 10: return "ward.wav";
        default: return nullptr;
    }
}