#ifndef LEVELCOMPLETE_H
#define LEVELCOMPLETE_H

#include "game/systems.h"

#include "game/interaction_systems/levelsequencer.h"

#include "game/states/shop.h"

#include "core/audio/audiosystem.h"

uint16_t levelcomplete_counter = 0;

const int advance_timer = 120;

const int base_score_time = 10;
const int tech_score_time = 30;
const int penalty_score_time = 50;
const int round_score_time = 70;
const int total_score_time = 110;

void updateLevelComplete(FSM &fsm, GameData &state) {
    levelcomplete_counter++;
    if (levelcomplete_counter > advance_timer && state.controls.pa) {
        state.total_tech += state.token_counter;
        goShop(fsm);
        fsm.go(GameStateName::Shop);
        AudioSystem::play(SFX::MenuSelect);
    }
    if (levelcomplete_counter == base_score_time || levelcomplete_counter == tech_score_time || levelcomplete_counter == penalty_score_time || levelcomplete_counter == round_score_time) {
        AudioSystem::play(SFX::TechGet);
    }
    if (levelcomplete_counter == total_score_time) {
        AudioSystem::play(SFX::PlayerSpecial);
    }
}

void drawLevelComplete(GameData &state) {
    
        // PLAYER UI
    Eid playerId = BabyECS::getFirst(GroupMask({Component::Position},{Tag::Paddle}));
    if (playerId != nullEntity) {
        Vector &pos = BabyECS::get(playerId).position;
        Sprites &spr = BabyECS::get(playerId).sprite;
        state.renderSystem->sprite(pos.x, pos.y, sprite_list[spr], 0);
    
        SpriteData heat(playerheat);
        state.renderSystem->setClip(true, uint8_t(state.heat/100.0f * 20), 14);
        state.renderSystem->sprite(int(pos.x + 9.5f), int(pos.y + 5.5f), &heat);
    
        SpriteData life(playerhealth);
        state.renderSystem->setClip(true, uint8_t(state.life/5.0f * 18), 14);
        state.renderSystem->sprite(int(pos.x + 9.5f), int(pos.y + 3.5f), &life);
        state.renderSystem->setClip(false);
    }
    
    SpriteData bg(background);

    state.renderSystem->clear(0);
    state.renderSystem->sprite(0, 115, &bg, 0);

    bg = SpriteData(ui_lvl_win);
    state.renderSystem->sprite(20, 26, &bg, 0);

    char buffer[10];
    SpriteData pill(ui_levelover_bars);
    SpriteData token(ui_levelover_bars_token);
    
    int size = sprintf(buffer, "%d", state.current_level);
    state.renderSystem->print(59-size*5, 31, buffer, 10, 1, 0);

    
    if (levelcomplete_counter > base_score_time) {
        size = sprintf(buffer, "%d", state.baseScore());
        state.renderSystem->print(78-size*10, 55, buffer, 10, 1, 0);
        for (int i = 0; i < state.baseLife; i++) {
            state.renderSystem->sprite(101 + 16 * i, 57, &pill, 0);
        }
    }
    
    if (levelcomplete_counter > tech_score_time) {
        size = sprintf(buffer, "%d", state.tokenScore());
        state.renderSystem->print(78-size*10, 77, buffer, 10, 1, 0);
        for (int i = 0; i < state.token_counter && i < 12; i++) {
            state.renderSystem->sprite(101 + 8 * i, 79, &token, 0);
        }
    }
    
    if (levelcomplete_counter > penalty_score_time) {
        int mod1 = state.deathModifier();
        int mod2 = state.deathModifier() * 10;
        if (mod2 == 10) {
            mod2 = 0;
        }
        size = sprintf(buffer, "x%d.%d", mod1, mod2);
        state.renderSystem->print(78-size*10, 100, buffer, 10, 1, 0);
        for (int i = 0; i < state.death_counter && i < 6; i++) {
            state.renderSystem->sprite(101 + 16 * i, 101, &pill, 1);
        }
    }

    if (levelcomplete_counter > round_score_time) {
        size = sprintf(buffer, "%d", state.currentScore());
        state.renderSystem->print(78-size*10, 125, buffer, 10, 1, 0);
    }

    if (levelcomplete_counter > total_score_time) {
        size = sprintf(buffer, "%d", state.currentScore() + state.score);
        state.renderSystem->print(178-size*10, 125, buffer, 10, 1, 0);
    }

    if (levelcomplete_counter > advance_timer && levelcomplete_counter % 60 > 36) {
        SpriteData btn(ui_a_btn);
        state.renderSystem->sprite(87, 136, &btn);
    }
}

#endif // LEVELCOMPLETE_H
