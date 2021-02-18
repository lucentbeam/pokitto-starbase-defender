#ifndef INFO_H
#define INFO_H

#include "game/systems.h"

void setupInfo(GameData &state) {
    freezeEnemies(true);
    setBallTop(78);
    Factory::createPlayer(state);
    Factory::createBall(54, 86, 0, 50);
    Factory::createEnemy(82, 80);
}

void updateInfo(FSM &fsm, GameData &state) {
    static int token_dropper = 240;
    if (state.controls.pb) {
        BabyECS::destroyAll();
        freezeEnemies(false);
        setBallTop(0);
        fsm.go(GameStateName::Title);
    } else {
        handlePaddleState(state);
        updateParticles();
        updatePhysics();
        updatePlayerShots(state);
        updateBalls(state);
        updateEnemies();
        updateEnemyShots(state);
        updateTokens(state);
        if (state.shake_frames > 0) {
            state.shake_frames--;
        }
        if (state.baseLife > 6 || state.baseLife <= 0) {
            state.baseLife = 6;
        }
        token_dropper--;
        if (token_dropper <= 0) {
            Factory::createToken(107, 70);
            token_dropper = 240;
        }
    }
}

void drawInfo(GameData &state) {
    state.renderSystem->clear(0);

    state.renderSystem->sprite(0, 168, ui_tutorial_background);

    drawGameView(state, false, true, true);

    state.renderSystem->sprite(55, 0, ui_tutorial_controls);

    if (state.controls.a) {
        state.renderSystem->sprite(135, 12, ui_tutorial_highlight_button);
    }

    if (state.controls.x < 0) {
        state.renderSystem->sprite(72, 23, ui_tutorial_highlight_dpad);
    } else if (state.controls.x > 0) {
        state.renderSystem->sprite(94, 23, ui_tutorial_highlight_dpad);
    }

    state.renderSystem->sprite(24, 60, ui_tutorial_objectives);

    auto playerId = BabyECS::getFirst(GroupMask({Component::Position, Component::Size},{Tag::Paddle}));
    if (BabyECS::alive(playerId)) {
        Vector &pos = BabyECS::get(playerId).position;
        Size &sz = BabyECS::get(playerId).size;
        Rect r(pos.x, pos.y, sz.x, sz.y);
        if (r.centerX() < 89) {
            state.renderSystem->sprite(35, 66, ui_tutorial_objective_block);
        } else if (r.centerX() < 133) {
            state.renderSystem->sprite(95, 66, ui_tutorial_objective_get);
        } else {
            state.renderSystem->sprite(140, 66, ui_tutorial_objective_dodge);
        }
    }
}

#endif // INFO_H
