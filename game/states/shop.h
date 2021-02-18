#ifndef SHOP_H
#define SHOP_H
#include "game/systems.h"

#include "game/interaction_systems/levelsequencer.h"

#include "core/audio/audiosystem.h"

uint8_t store_region = 0;
uint8_t store_row = 0;

const Vector button_positions[6] = {
    Vector(18,46),  // speed level
    Vector(126,46), // charge level
    Vector(18,94),  // barrier
    Vector(92,94),  // blast
    Vector(166,94), // bigshot
    Vector(83,130)  // continue
};

const uint8_t* ui_buttons[6] = {
    ui_shop_speed_placard,
    ui_shop_charge_placard,
    ui_shop_barrier_placard,
    ui_shop_blast_placard,
    ui_shop_bigshot_placard,
    ui_shop_confirm
};

const uint8_t* ui_highlights[5] = {
    ui_shop_highlight_speed,
    ui_shop_highlight_charge,
    ui_shop_highlight_barrier,
    ui_shop_highlight_blast,
    ui_shop_highlight_bigshot
};

const uint8_t costs_upgrade[3] = { 5, 12, 17 };
const uint8_t cost_barrier = 13;
const uint8_t cost_blast = 15;
const uint8_t cost_bigshot = 14;

void goShop(FSM &fsm) {
    store_region = 0;
}

void updateShop(FSM &fsm, GameData &state) {
    if (state.controls.pu && store_region > 0) {
        AudioSystem::play(SFX::MenuChange);
        store_region--;
    }
    if (state.controls.pd && store_region < 2) {
        AudioSystem::play(SFX::MenuChange);
        store_region++;
    }
    if (state.controls.pl && store_row > 0) {
        AudioSystem::play(SFX::MenuChange);
        store_row--;
    }
    if (state.controls.pr) {
        AudioSystem::play(SFX::MenuChange);
        store_row++;
    }
    int max = store_region == 1 ? 2 : store_region == 2 ? 0 : 1;
    store_row = std::fmin(store_row, max);

    if (state.controls.pa) {
        int activeIndex = store_row;
        activeIndex += store_region == 1 ? 2 : store_region == 2 ? 5 : 0;

        if (activeIndex < 2) {
            int cost = activeIndex == 0 ? state.speed_rank : state.charge_rank;
            if (cost < 3) {
                cost = costs_upgrade[cost];
                if (state.total_tech >= cost) {
                    state.total_tech -= cost;
                    AudioSystem::play(SFX::MenuSelect);
                    if (activeIndex == 0) {
                        state.speed_rank++;
                    } else {
                        state.charge_rank++;
                    }
                }
            }
        } else if (activeIndex < 5 && state.alt_ability == GameData::AbilityB::None) {
            int cost = activeIndex == 2 ? cost_barrier : activeIndex == 3 ? cost_blast : cost_bigshot;
            if (state.total_tech >= cost) {
                AudioSystem::play(SFX::MenuSelect);
                state.total_tech -= cost;
                state.alt_ability = activeIndex == 2 ? GameData::AbilityB::Barrier : activeIndex == 3 ? GameData::AbilityB::Blast :GameData::AbilityB::BigShot;
            }
        } else if (activeIndex == 5) {
            advanceLevelSequence();
            state.level_complete = false;
            fsm.go(GameStateName::Game);
            AudioSystem::play(SFX::MenuSelect);
        }
    }
}

void drawShop(GameData &state) {
    drawGameView(state, true, true, false);

    state.renderSystem->sprite(53, 0, ui_shop_header);
    state.renderSystem->sprite(47, 34, ui_shop_passive_title);
    state.renderSystem->sprite(47, 82, ui_shop_btitle);

    for (int i = 0; i < 6; i++) {
        state.renderSystem->sprite(button_positions[i].x, button_positions[i].y, ui_buttons[i]);
    }

    int activeIndex = store_row;
    activeIndex += store_region == 1 ? 2 : store_region == 2 ? 5 : 0;
    state.renderSystem->sprite(button_positions[activeIndex].x-15, button_positions[activeIndex].y, ui_shop_active_button);

    if (activeIndex < 5) {
        state.renderSystem->sprite(button_positions[activeIndex].x+4, button_positions[activeIndex].y+3, ui_highlights[activeIndex]);
    } else {
        state.renderSystem->sprite(button_positions[5].x+25, button_positions[5].y+3, ui_shop_highlight_continue);
    }
    
    state.renderSystem->sprite(0, 164, ui_shop_option_text, activeIndex);

    for (int i = 0; i < state.speed_rank; i++) {
        state.renderSystem->sprite(button_positions[0].x+22+21*i, button_positions[0].y+4, ui_shop_purchased);
    }
    for (int i = 0; i < state.charge_rank; i++) {
        state.renderSystem->sprite(button_positions[1].x+22+21*i, button_positions[1].y+4, ui_shop_purchased);
    }
    if (state.alt_ability != GameData::AbilityB::None) {
        int idx = state.alt_ability == GameData::AbilityB::Barrier ? 2 : state.alt_ability == GameData::AbilityB::Blast ? 3 : 4;
        state.renderSystem->sprite(button_positions[idx].x+22, button_positions[idx].y+4, ui_shop_purchased);
    }

    char point_values[6];
    int sz = sprintf(point_values, "%d", state.total_tech);
    state.renderSystem->print(139 - sz * 2.5, 6, point_values, 10);

    const int cost_color = 15;

    Vector location = button_positions[activeIndex];
    if (activeIndex < 2) {
        int rank = activeIndex == 0 ? state.speed_rank : state.charge_rank;
        if (rank < 3) {
            sz = sprintf(point_values, "%d", costs_upgrade[rank]);
            int shift = sz == 1 ? 5 : 0;
            state.renderSystem->print(location.x+22+21*rank + shift, location.y+6, point_values, cost_color);
        }
    } else if (activeIndex < 5 && state.alt_ability == GameData::AbilityB::None) {
        int cost = activeIndex == 2 ? cost_barrier : activeIndex == 3 ? cost_blast : cost_bigshot;
        sz = sprintf(point_values, "%d", cost);
        state.renderSystem->print(location.x+22, location.y + 6, point_values, cost_color);
    }
}

#endif // SHOP_H
