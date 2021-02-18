#ifndef __SYSTEM
#define __SYSTEM

#include "sprites.h"

struct RenderSystem
{
private:
    uint8_t m_clip_width, m_clip_height;
    bool m_clipping = false;

public:
    uint32_t getTimeMs();

    void initialize();

    bool running();

    bool update();

    void print(int x, int y, const char * line, int color, int size = 1, int bgcol = 0);

    void pixel(int x, int y, uint8_t pixel_idx);

    void sprite(int x, int y, SpriteData * spr, int frame = 0, bool flip = false);

    void sprite(int x, int y, const uint8_t * sprite, int frame = 0, bool flip = false);

    void setClip(bool clip, uint8_t clip_width = 0, uint8_t clip_height = 0);

    void drawLine(int x0, int y0, int x1, int y1, int color);

    void drawRect(int x0, int y0, int w, int h, int color);

    void clear(int idx);

    void debug();
};

#endif
