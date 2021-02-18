#ifndef GAMEOVER_H
#define GAMEOVER_H

#include "game/systems.h"

#include "game/rendering/drawgameview.h"

#include "game/states/scores.h"
#include "game/states/entername.h"

uint16_t counter = 0;

extern StoredData highscores;

void updateGameOver(FSM &fsm, GameData &state) {
    counter++;
    if (state.shake_frames > 0) {
        state.shake_frames = 0;
    }
    if (counter > 60 && state.controls.pa) {
        if (!highscores.isNewRecord(state.score)) {
            fsm.go(GameStateName::Title);
            BabyECS::destroyAll();
        } else {
            resetNameEntry();
            fsm.go(GameStateName::HighScoreEntry);
        }
    }
}

void drawGameOver(GameData &state) {
    drawGameView(state);

    SpriteData bg(ui_gameover_text);
    state.renderSystem->sprite(24, 60, &bg);

    bg = SpriteData(ui_gameover_panel);

    char buffer[40];
    int size = sprintf(buffer, "%d", state.score);

    if (highscores.isNewRecord(state.score)) {
        state.renderSystem->sprite(57, 81, &bg, 0);
        state.renderSystem->print(132 - size * 5, 86, buffer, 10, 1, 0);
    } else {
        state.renderSystem->sprite(53, 81, &bg, 1);
        state.renderSystem->print(124 - size * 5, 86, buffer, 10, 1, 0);
    }

    bg = SpriteData(ui_a_btn);
    if (counter > 60 && (counter % 60) < 36) {
        state.renderSystem->sprite(90, 105, &bg);
    }
}

#endif // GAMEOVER_H
