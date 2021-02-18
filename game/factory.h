#ifndef FACTORY_H
#define FACTORY_H

#include "game/components.h"
#include "core/rendering/sprites.h"
#include "game/spritesheets.h"
#include "core/collisions/rect.h"
#include "game/gamestate.h"

#include "core/babyecs.h"

#include <cstdlib>
#include <cmath>

struct Factory {
private:
    static bool stopParticles() {
        return BabyECS::aliveCount() > 50;
    }

public:
    static void createPlayer(GameData &playerInfo) {
        playerInfo.life = 5;
        playerInfo.heat = 100;
        Entity &p = BabyECS::create(
            {Component::SpriteComponent, Component::Position, Component::Size},
            {Tag::Paddle, Tag::BounceTarget}
        );
        p.sprite = Sprites::Player;
        p.position = Vector(90, 154);
        p.size = Size(36, 8);
    }

    static Entity &createPlayerShot(float x, float y, bool big_shot = false) {
        Entity &shot = BabyECS::create(
            {Component::SpriteComponent, Component::Position, Component::Velocity, Component::Size},
            {Tag::Shot, Tag::DestroyOnBounce, Tag::BounceTarget}
        );
        shot.sprite = Sprites::PlayerShot;
        shot.position = Vector(x,y);
        shot.velocity = Vector(0, -200);
        shot.size = Size(3, 14);

        if (big_shot) {
            shot.sprite = Sprites::Bigshot;
            shot.velocity.y *= 0.5f;
            shot.size = Size(7, 14);
            Eid id = BabyECS::lastCreatedId();
            BabyECS::add(id, Component::BigShot);
            BabyECS::setTag(id, Tag::DestroyOnBounce, false);
        }

        return shot;
    }

    static Entity &createEnemyShot(float x, float y, float vx = 0) {
        Entity &s = BabyECS::create(
            {Component::SpriteComponent, Component::Position, Component::Velocity, Component::Size},
            {Tag::EnemyShot, Tag::DestroyOnBounce, Tag::BounceTarget}
        );
        s.sprite = Sprites::EnemyShot;
        s.position = Vector(x,y);
        s.velocity = Vector(vx,80);
        s.size = Size(3,14);
        return s;
    }

    static Entity &createToken(float x, float y) {
        Entity &t = BabyECS::create(
            {Component::SpriteComponent, Component::Position, Component::Velocity, Component::Size},
            {Tag::Token}
        );
        t.sprite = Sprites::Token;
        t.position = Vector(x,y);
        t.velocity = Vector(0,40);
        t.size = Size(6, 6);
        return t;
    }

    static Entity &createBall(float x, float y, float vx, float vy) {
        Entity &b = BabyECS::create(
            {Component::SpriteComponent, Component::Position, Component::Velocity, Component::Size},
            {Tag::Ball, Tag::BounceTarget}
        );
        b.sprite = Sprites::Ball;
        b.position = Vector(x,y);
        b.velocity = Vector(vx, vy);
        b.size = Size(11, 11);
        return b;
    }

    static Entity &createEnemy(float x, float y = -34) {
        x = (x / 100.0f) * 178;
        Entity &e = BabyECS::create(
            {Component::SpriteComponent, Component::Position, Component::Velocity, Component::Size, Component::Enemy},
            {Tag::Enemy, Tag::BounceTarget}
        );
        e.sprite = Sprites::EnemySprite;
        e.position = Vector(x,y);
        e.velocity = Vector(0, 0);
        e.size = Size(32, 19);
        e.enemy = Enemy(rand() % 3);
        return e;
    }

    static void createEnemyBarrier(float x, float y, float x_offset, bool left) {
        x += left ? -x_offset : x_offset;
        x += 15;
        Entity &b = BabyECS::create(
            {Component::SpriteComponent, Component::Position, Component::Size},
            {Tag::Barrier, Tag::BounceTarget}
        );
        b.sprite = left ? Sprites::BarrierLeft : Sprites::BarrierRight;
        b.position = Vector(x, y);
        b.size = Size(2, 19);
    }

    static Entity& createGenericParticle(Sprites sprite, float x, float y, float vx, float vy) {
        Entity &t = BabyECS::create(
            {Component::SpriteComponent, Component::Position, Component::Velocity, Component::Particle},
            {Tag::Background}
        );
        t.sprite = sprite;
        t.position = Vector(x,y);
        t.velocity = Vector(vx, vy);
        return t;
    }

    static void createPlayerBlast(float x, float y) {
        for (int i = 0; i <= 10; i++) {
            float vx = 50 - i * 10 + ((rand() % 10) - 5);
            float vy = 50 - std::fabs(vx);
            float l = std::sqrt(vx*vx + vy*vy);

            Entity &t = createGenericParticle(Sprites::SparkParticle, x+14, y, vx * 800 / l, -vy * 800 / l);
            t.particle = Particle(5, 2);
            Eid id = BabyECS::lastCreatedId();
            BabyECS::add(id, Component::Blast);
        }
    }

    static void createPlayerShield(float x, float y) {
        Entity &t = createGenericParticle(Sprites::PlayerShield, x, y, 0, 0);
        t.particle = Particle(1, 1);
        Eid id = BabyECS::lastCreatedId();
        BabyECS::add(id, Component::PlayerBarrier);
    }

    static void createPlayerThrust(float x, float y, float vx) {
        if (stopParticles()) return;
        Entity &t = createGenericParticle(Sprites::ShiptrailParticle, x, y, vx, 0);
        t.particle = Particle(3,8);
    }

    static void createBlueParticle(float x, float y, float vx, float vy) {
        if (stopParticles()) return;
        Entity &t = createGenericParticle(Sprites::NebulaParticle, x, y, vx, vy);
        t.particle = Particle(5, 8);
    }

    static void createNebulaParticle(float x, float y) {
        float vx = (rand() % 50) - 25;
        float vy = (rand() % 50) - 25;
        createBlueParticle(x + vx/10.0f, y + vy /10.0f, vx, vy);
    }

    static void createGreenParticle(float x, float y) {
        if (stopParticles()) return;

        float vx = (rand() % 50) - 25;
        float vy = (rand() % 50) - 25;
        Entity &t = createGenericParticle(Sprites::TokenParticle, x + vx/10.0f, y + vy /10.0f, vx, vy);
        t.particle = Particle(5, 5);
    }

    static void createExplosion(const Rect &rect, float ox = 0, float oy = 0, float density = 40.0f) {

        if (stopParticles()) return;

        uint8_t count = uint8_t(rect.width() * rect.height() / density);
        for ( ; count > 0; count--) {
            float px = rect.left() + (rand() % int(rect.width()));
            float py = rect.top() + (rand() % int(rect.height()));
            float dx = px - rect.centerX();
            float dy = py - rect.centerY();
            float l = std::sqrt(dx*dx + dy*dy);
            float vx = 0, vy = 0;
            if (l > 0) {
                int v = 30 + (rand() % 10);
                vx = dx / l * v;
                vy = dy / l * v;
            }
            Entity &t = createGenericParticle(Sprites::SparkParticle, px, py, vx + ox, vy + oy);
            t.particle = Particle(5, 4);
        }
    }
};

#endif // FACTORY_H
