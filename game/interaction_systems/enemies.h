#ifndef ENEMIES_H
#define ENEMIES_H

#include "game/components.h"
#include "core/babyecs.h"
#include "core/collisions/rect.h"
#include "game/factory.h"
#include "core/audio/audiosystem.h"

bool enemies_frozen = false;

void freezeEnemies(bool val) {
    enemies_frozen = val;
}

void resetCounter(Enemy &enemy) {
    enemy.shot_counter = 90 + (rand() % 30);
}

void updateEnemies() {

    static auto enemies = BabyECS::createGroup(GroupMask({Component::Position,Component::Size,Component::Velocity,Component::Enemy},{Tag::Enemy}));

    static auto barriers = BabyECS::createGroup(GroupMask({Component::Position,Component::Size},{Tag::Barrier}));

    for (auto b : barriers->members) {
        BabyECS::destroy(b);
    }

    const float yspeed = 40.0f;
    const float xspeed = 60.0f;

    for(auto en : enemies->members) {
        if (!BabyECS::alive(en)) continue;
        Entity &e = BabyECS::get(en);
        Enemy &enemy = e.enemy;
        if (enemy.iframes > 0) {
            enemy.iframes--;
        }
        Vector &pos = e.position;
        if (enemy.difficulty > 0) {
            Factory::createEnemyBarrier(pos.x, pos.y, 30, true);
            Factory::createEnemyBarrier(pos.x, pos.y, 30, false);
        }

        Vector &vel = e.velocity;

        Size &sz = e.size;
        Rect rect(pos.x,pos.y,sz.x,sz.y);

        enemy.shot_counter--;
        if (BabyECS::hasTag(en, Tag::Stunned)) {
            vel.x = 0;
            vel.y = 0;
            if (enemy.shot_counter <= 0) {
                resetCounter(enemy);
                BabyECS::setTag(en, Tag::Stunned, false);
            }
        } else {
            if (enemy.shot_counter <= 0) {
                resetCounter(enemy);
                float cx = rect.centerX();
                float cy = rect.bottom();
                const float vx = 5;
                switch (enemy.difficulty) {
                    case 1:
                        Factory::createEnemyShot(cx + 8, cy, vx);
                        Factory::createEnemyShot(cx - 8, cy, -vx);
                        break;
                    case 2:
                        Factory::createEnemyShot(cx + 16, cy, vx * 2);
                        Factory::createEnemyShot(cx, cy);
                        Factory::createEnemyShot(cx - 16, cy, -2 * vx);
                        break;
                    case 3:
                        Factory::createEnemyShot(cx + 8, cy, vx);
                        Factory::createEnemyShot(cx - 8, cy,-vx);
                        Factory::createEnemyShot(cx + 24, cy,vx*2);
                        Factory::createEnemyShot(cx - 24, cy,-2*vx);
                        break;
                    default:
                        Factory::createEnemyShot(cx, cy);
                        break;
                }
                AudioSystem::play(SFX::EnemyAttack);
            }
            if (pos.y < enemy.target_y) {
                vel.y = yspeed;
                vel.x = 0.0f;
            } else {
                vel.y = 0.0f;
                if (std::fabs(vel.x) < std::numeric_limits<float>::epsilon()) {
                    vel.x = xspeed;
                    if (rand() % 2) {
                        vel.x *= -1.0f;
                    }
                }
                if (rect.right() > 220 && vel.x > 0) {
                    pos.x -= (rect.right()-220);
                    vel.x = -xspeed;
                } else if (rect.left() < 0 && vel.x < 0) {
                    pos.x = -rect.left();
                    vel.x = xspeed;
                }
            }
        }

        if (enemies_frozen) {
            vel.x = 0;
            vel.y = 0;
        }

        if (enemy.life == 0) {
            AudioSystem::play(SFX::EnemyDestroyed);
            BabyECS::destroy(en);
            Factory::createToken(rect.centerX(), rect.centerY());
        }
    }
}
#endif // ENEMIES_H
