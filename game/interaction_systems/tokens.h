#ifndef TOKENS_H
#define TOKENS_H

#include "core/babyecs.h"
#include "game/gamestate.h"
#include "game/components.h"
#include "core/collisions/rect.h"
#include "game/factory.h"
#include "core/audio/audiosystem.h"

void updateTokens(GameData &playerInfo) {

    static auto tokens = BabyECS::createGroup(GroupMask({Component::Position, Component::Size}, {Tag::Token}));

    auto count = tokens->members.size();
    if (count == 0) return;

    static uint8_t timer = 0;
    timer++;
    int timer_offset = 30 / count;
    int ct = 0;

    Eid player = BabyECS::getFirst(GroupMask({Component::Position, Component::Size}, {Tag::Paddle}));
    Rect player_rect;
    if (BabyECS::alive(player)) {
        Size &psz = BabyECS::get(player).size;
        Vector &ppos = BabyECS::get(player).position;
        player_rect = Rect(ppos.x,ppos.y,psz.x,psz.y);
    }

    for(auto token : tokens->members) {
        if (!BabyECS::alive(token)) continue;

        Size &sz = BabyECS::get(token).size;
        Vector &pos = BabyECS::get(token).position;
        Rect token_rect(pos.x,pos.y,sz.x,sz.y);

        if ((timer + ct * timer_offset) % 30*count == 0) {
            Factory::createGreenParticle(token_rect.centerX(), token_rect.centerY());
        }
        ct++;

        if (token_rect.overlaps(player_rect)) {
            BabyECS::destroy(token);
            playerInfo.token_counter++;
            AudioSystem::play(SFX::TechGet);
        }
    }
}

#endif // TOKENS_H
