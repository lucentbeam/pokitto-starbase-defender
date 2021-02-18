#ifndef _TILESET
#define _TILESET

#include <stdint.h>

#include "rendersystem.h"

class Tileset {
  uint8_t m_tilewidth, m_tileheight;
  uint8_t m_width, m_height;
  uint8_t * m_walls;
  SpriteData m_sprite;
public:
  Tileset(const uint8_t * walls, const SpriteData &sprite);
  void draw(RenderSystem * system, uint16_t x, uint16_t y, uint8_t w, uint8_t h, int8_t sx, int8_t sy);
};

#endif
