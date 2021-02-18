#ifndef POKITTOLIBEXTENSIONS_H
#define POKITTOLIBEXTENSIONS_H

#include <cstdint>

#ifndef POKITTO_SFML

namespace Pokitto {

class DisplayExtensions {
    static void shift(int16_t x, int16_t y);

public:

    static void drawClippedBitmap(int16_t x, int16_t y, int16_t w, int16_t h, int16_t clipw, int16_t cliph, const uint8_t* bitmap);

    static void drawTile(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t* tile);

    static void drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, uint8_t frame, bool flip = false);
};

}


namespace Audio {
    void getVolume(uint8_t* buf);
}

#endif

#endif // POKITTOLIBEXTENSIONS_H
