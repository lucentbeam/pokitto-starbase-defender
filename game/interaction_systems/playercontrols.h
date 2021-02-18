#ifndef PLAYERCONTROLS_H
#define PLAYERCONTROLS_H

#include "core/controls/controls.h"
#include "game/components.h"
#include "core/babyecs.h"
#include "game/factory.h"
#include "core/collisions/rect.h"
#include "core/audio/audiosystem.h"

void handlePaddleState(GameData &info) {
    Eid playerId = BabyECS::getFirst(GroupMask({Component::Position},{Tag::Paddle}));

    if (!BabyECS::alive(playerId)) {
        info.respawn_timer--;

        if (info.respawn_timer < 0) {
            Factory::createPlayer(info);
        }
        return;
    }

    Vector &pos = BabyECS::get(playerId).position;
    Size &sz = BabyECS::get(playerId).size;
    Rect rect(pos.x, pos.y, sz.x, sz.y);

    if (info.life <= 0) {
        Factory::createExplosion(rect);
        BabyECS::destroy(playerId);

        info.death_counter++;
        info.respawn_timer = 180;
        info.barrier_time = 0;
        
        AudioSystem::play(SFX::PlayerDestroyed);
        return;
    }

    float speed = float(info.speed_rank) * 20.0f + 100.0f;
    pos.x += info.controls.x * speed * 0.016f;

    if (rect.left() < 0) {
        pos.x -= rect.left();
    } else if (rect.right() > 220) {
        pos.x -= (rect.right()-220);
    } else {
        const int spawnFrequency = 5;
        static int trailspawn = spawnFrequency;
        if (info.controls.x != 0) {
            trailspawn--;
            if (trailspawn < 0) {
                trailspawn = spawnFrequency;
                if (info.controls.x < 0) {
                    Factory::createPlayerThrust(rect.right(), rect.centerY()-1, -12);
                } else {
                    Factory::createPlayerThrust(rect.left(), rect.centerY()-1, 12);
                }
            }
        } else {
            trailspawn = spawnFrequency;
        }
    }
    if (info.barrier_time > 0) {
        info.barrier_time--;
        Factory::createPlayerShield(pos.x-6, pos.y-3);
        if (info.barrier_time % 5 == 0) {
            Factory::createBlueParticle(pos.x - 8 + (rand() % 46), pos.y - 2, 0, -30 - (rand() % 10));
        }
    }
    if (info.blast_time > 0) {
        info.blast_time--;
    }

    if (info.shot_flash > 0) {
        info.shot_flash--;
    }

    if (info.controls.pa && info.heat >= 25.0f) {
        Factory::createPlayerShot(pos.x+2, pos.y-5);
        Factory::createPlayerShot(pos.x+31, pos.y-5);
        info.heat -= 25.0f;
        AudioSystem::play(SFX::PlayerAttack);
    } else if (info.controls.pa) {
        info.shot_flash = 10;
    }

    const float bigshot_cost = 60.0f;
    const float barrier_cost = 50.0f;
    const float barrier_lower_cost = 20.0f;
    const float blast_cost = 30.0f;

    if (info.controls.pb) {
        switch (info.alt_ability) {
        case GameData::AbilityB::Blast:
            if (info.heat >= blast_cost) {
                AudioSystem::play(SFX::PlayerSpecial);
                Factory::createPlayerBlast(pos.x, pos.y);
                info.heat -= blast_cost;
                info.blast_time = info.max_blast_time;
            } else {
                info.shot_flash = 10;
            }
            break;
        case GameData::AbilityB::Barrier:
            if (info.heat >= barrier_cost) {
                AudioSystem::play(SFX::PlayerSpecial);
                info.barrier_time = info.max_barrier_time;
                info.heat -= barrier_cost;
            } else if (info.heat >= barrier_lower_cost) { // add leeway to preserve barrier as panic button
                AudioSystem::play(SFX::PlayerSpecial);
                info.barrier_time = info.max_barrier_time;
                info.heat = 0;
            } else {
                info.shot_flash = 10;
            }
            break;
        case GameData::AbilityB::BigShot:
            if (info.heat > bigshot_cost) {
                AudioSystem::play(SFX::PlayerSpecial);
                Factory::createPlayerShot(pos.x+2, pos.y-5, true);
                Factory::createPlayerShot(pos.x+31, pos.y-5, true);
                info.heat -= bigshot_cost;
            } else {
                info.shot_flash = 10;
            }
            break;
        }
    }

    float charge_rate = float(info.charge_rank) * 5.0f + 15.0f;

    info.heat = std::fmax(info.heat, 0.0f);
    info.heat += charge_rate * 0.016f;
    info.heat = std::fmin(info.heat, 100.0f);
}
#endif // PLAYERCONTROLS_H
