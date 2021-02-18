#include "tileset.h"

#include <stdlib.h>
#include <cmath>

Tileset::Tileset(const uint8_t * walls, const SpriteData &sprite) : m_sprite(sprite) {
  m_tilewidth = *(m_sprite.pixels);
  m_tileheight = *(m_sprite.pixels + 1);
  m_width = *walls;
  m_height = *(walls+1);
  const uint8_t * wallstart = walls+2;
  m_walls = new uint8_t[m_width * m_height]();
  for (uint16_t j = 0; j < m_height; j++) {
    for (uint16_t i = 0; i < m_width; i++) {
      uint16_t idx = i + j * m_width;
      uint8_t tile = wallstart[idx];
      if (tile == 2) {
        m_walls[idx] = 7;
      } else if (tile == 1) {
        m_walls[idx] = 4 + (rand()%2);
        if ((j+1) < m_height-1) {
          int offs = i + (j+1) * m_width;
          if (wallstart[offs] == 0) m_walls[idx] = 6;
        }
      } else {
        int left = 2;
        int top = 1;
        bool door = false;
        if ((j - 1) >= 0) {
          int offs = i + (j-1) * m_width;
          if (wallstart[offs] == 0) top = 0;
          door = wallstart[offs] == 2;
        }
        if ((i - 1) >= 0) {
          int offs = i - 1 + j * m_width;
          if (wallstart[offs] == 0) left = 0;
        }
        m_walls[idx] = door ? 8 : left + top;
      }
    }
  }
}

void Tileset::draw(RenderSystem * system, uint16_t x, uint16_t y, uint8_t w, uint8_t h, int8_t sx, int8_t sy) {
  uint16_t x_upper = x + w;
  if (x_upper >= m_width) x_upper = m_width-1;
  uint16_t y_upper = y + h;
  if (y_upper >= m_height) y_upper = m_height-1;
  for(uint16_t j = y; j < y_upper; j++) {
    for(uint16_t i = x; i < x_upper; i++) {
        uint16_t idx = i + j * m_width;
        uint8_t tile = *(m_walls + idx);
        system->sprite(sx + (i-x) * m_tilewidth, sy + (j-y) * m_tileheight, &m_sprite, tile);
    }
  }
}
