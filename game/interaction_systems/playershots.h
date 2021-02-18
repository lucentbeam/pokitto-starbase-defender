#ifndef PLAYERSHOTS_H
#define PLAYERSHOTS_H

#include "game/components.h"
#include "core/babyecs.h"
#include "core/collisions/rect.h"
#include "game/factory.h"

#include "core/audio/audiosystem.h"

#include <cmath>

void updatePlayerShots(GameData &playerInfo) {
    static auto playershots = BabyECS::createGroup(GroupMask({Component::Position},{Tag::Shot}));

    static auto enemies = BabyECS::createGroup(GroupMask({Component::Position,Component::Size,Component::Velocity,Component::Enemy},{Tag::Enemy}));

    for(auto shot : playershots->members) {
        if (!BabyECS::alive(shot)) continue;

        Vector &pos = BabyECS::get(shot).position;
        Size &sz = BabyECS::get(shot).size;
        Rect shot_rect(pos.x, pos.y, sz.x, sz.y);
        if (shot_rect.bottom() < 0) {
            BabyECS::destroy(shot);
        }

        for (auto enemy : enemies->members) {
            if (!BabyECS::alive(enemy)) continue;
            if (BabyECS::hasTag(enemy, Tag::Stunned)) {
                continue;
            }
            Vector &epos = BabyECS::get(enemy).position;
            Size &esz = BabyECS::get(enemy).size;
            Rect enemy_rect(epos.x, epos.y, esz.x, esz.y);
            if (shot_rect.overlaps(enemy_rect)) {
                if (BabyECS::has(shot, Component::BigShot)) {
                    BabyECS::get(enemy).enemy.life--;
                    Factory::createExplosion(enemy_rect,0,0,80.0f);
                } else {
                    AudioSystem::play(SFX::EnemyStruck);
                }
                BabyECS::destroy(shot);
                BabyECS::setTag(enemy, Tag::Stunned, true);
                BabyECS::get(enemy).enemy.shot_counter = 60; // stun duration
                break;
            }
        }
    }
}

#endif // PLAYERSHOTS_H
