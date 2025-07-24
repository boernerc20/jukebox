#include <Arduino.h>

// Example: Tag IDs as strings, mapped to CD numbers (song indices)
struct TagSongMapEntry {
    String tagId;
    uint8_t cdNumber;
};

// Add your tag IDs here (replace with your actual tag IDs)
const TagSongMapEntry tagSongMap[] = {
    {"41.59.13.6", 1}, // Example tag, maps to CD 1
    {"57.135.30.6", 2}
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