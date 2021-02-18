#ifndef _SPRITES
#define _SPRITES

#include <stdint.h>

struct SpriteData {
  SpriteData(const uint8_t * pix = nullptr);
  const uint8_t * pixels;
};

enum Sprites : uint8_t {
    Player = 0,
    Ball = 1,
    PlayerShot = 2,
    EnemyShot = 3,
    EnemySprite = 4,
    Token = 5,
    BarrierLeft = 6,
    BarrierRight = 7,
    Blast = 8,
    Bigshot = 9,
    PlayerShield = 10,
    NebulaParticle = 11,
    ShiptrailParticle = 12,
    TokenParticle = 13,
    SparkParticle = 14
};

#endif
