#ifndef PAUSE_H
#define PAUSE_H

#include "game/systems.h"
#include "game/volume.h"
#include "core/audio/audiosystem.h"

int pause_selection = 0;

void goPause() {
    pause_selection = 0;
    AudioSystem::play(SFX::MenuSelect);
}

void updatePause(FSM &state, GameData &game) {
    if (game.controls.pc) {
        state.go(GameStateName::Game);
        return;
    }
    if (game.controls.pa) {
        AudioSystem::play(SFX::MenuSelect);
        switch (pause_selection) {
        case 0:
            state.go(GameStateName::Game);
            return;
        case 1:
            toggleMute();
            break;
        case 2:
            BabyECS::destroyAll();
            state.go(GameStateName::Title);
            return;
        }
    }
    if (pause_selection == 1 && game.controls.x != 0) {
        incrementVolume(game.controls.x * 30.0f * 0.016f);
    }
    if (game.controls.pu || game.controls.pd) {
        AudioSystem::play(SFX::MenuChange);
        int y = game.controls.y < 0 ? -1 : 1;
        pause_selection += y;
        if (pause_selection < 0) {
            pause_selection += 3;
        } else {
            pause_selection %= 3;
        }
    }
}

void drawPause(GameData &data) {
    drawGameView(data);

    SpriteData fg(ui_pause_prompt);
    data.renderSystem->sprite(65, 56, &fg);

    drawVolume(data.renderSystem, 148, 98, pause_selection != 1);

    fg = SpriteData(sprite_ball);
    data.renderSystem->sprite(74, 80 + 16 * pause_selection, &fg);
}


#endif // PAUSE_H
