#ifndef ENEMYSHOTS_H
#define ENEMYSHOTS_H

#include "game/components.h"
#include "core/babyecs.h"
#include "core/collisions/rect.h"
#include "game/factory.h"
#include "core/audio/audiosystem.h"

void updateEnemyShots(GameData &playerInfo) {
    Eid playerId = BabyECS::getFirst(GroupMask({Component::Position},{Tag::Paddle}));
    Rect rect;
    if (BabyECS::alive(playerId)) {
        Entity &p = BabyECS::get(playerId);;
        rect = Rect(p.position.x, p.position.y, p.size.x, p.size.y);
        if (playerInfo.barrier_time > 0) {
            rect.grow(6, 4);
        }
    }

    static auto shots = BabyECS::createGroup(GroupMask({Component::Position},{Tag::EnemyShot}));

    for(auto shot : shots->members) {
        if (!BabyECS::alive(shot)) continue;
        Entity &sht = BabyECS::get(shot);
        Vector &pos = sht.position;
        Size &sz = sht.size;
        Rect srect(pos.x, pos.y, sz.x, sz.y);
        if (srect.top() > 176) {
            BabyECS::destroy(shot);
        } else if (BabyECS::alive(playerId) && srect.overlaps(rect)) {
            BabyECS::destroy(shot);
            if (playerInfo.barrier_time == 0 && playerInfo.shake_frames <= 0) {
                AudioSystem::play(SFX::PlayerStruck);
                playerInfo.life--;
                playerInfo.shake_frames = 1;
            }
        }
    }
}


#endif // ENEMYSHOTS_H
