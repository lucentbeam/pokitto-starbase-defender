#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <stdint.h>
#include <cstdlib>

#include "game/spritesheets.h"
#include "core/rendering/sprites.h"
#include "core/collisions/rect.h"

namespace Component {
    enum Type {
        Position,
        Velocity,
        Size,
        Particle,
        Enemy,
        SpriteComponent, // uses SpriteData
        BigShot,
        Blast,
        PlayerBarrier
    };
}

namespace Tag {
    enum Type {
        Background,
        Paddle,
        Shot,
        Ball,
        Enemy,
        EnemyShot,
        Stunned,
        BounceTarget,
        DestroyOnBounce,
        Barrier,
        Token
    };
}

struct Vector {
    float x, y;
    Vector(float x = 0, float y = 0): x(x), y(y) {}
};

struct Size {
    uint8_t x,y;
    Size(uint8_t x = 0, uint8_t y = 0) : x(x), y(y) {}
};

struct Particle {
    uint8_t frame = 0, last_frame, counter = 0, counts_per_frame;
    Particle( uint8_t endframe = 0, uint8_t countsperframe = 0) : last_frame(endframe), counts_per_frame(countsperframe) {}
};

struct Enemy {
    uint8_t sprite_index;
    uint8_t shot_counter = 90;
    uint8_t target_y;
    int8_t life;
    uint8_t iframes = 0;
    uint8_t difficulty = 0;
    Enemy(uint8_t sprite = 0, uint8_t life = 1) : sprite_index(sprite), life(life) {
        target_y = 5 + (rand() % 40);
    }
};

struct Entity {
    Vector position;
    Vector velocity;
    enum Sprites sprite;
    struct Enemy enemy;
    Size size;
    Particle particle;
    Entity() {}
};

#endif // COMPONENTS_H
