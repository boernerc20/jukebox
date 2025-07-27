#include <Arduino.h>

// Tag IDs as strings, mapped to CD numbers
struct TagSongMapEntry {
    String tagId;
    uint8_t cdNumber;
};

// Add your tag IDs here 
const TagSongMapEntry tagSongMap[] = {
    {"41.59.13.6", 1},
    {"57.135.30.6", 2},
    {"4.79.86.146.204.119.128", 3},
    {"8.8.220.76", 4},
    {"8.44.218.166", 5},
    {"8.139.101.123", 6}
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