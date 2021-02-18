#ifndef GAME_H
#define GAME_H

#include "game/systems.h"
#include "game/states/pause.h"
#include "game/states/levelcomplete.h"

void updateGame(FSM &fsm, GameData &state) {

    updateLevelSequenceFrame(state, state.controls);
    handlePaddleState(state);
    updateEnemies();
    updateBalls(state);
    updateTokens(state);
    updatePhysics();
    updateEnemyShots(state);
    updatePlayerShots(state);
    updateParticles();

    if (state.alert_timer > 0) {
        state.alert_timer--;
    }
    if (state.shake_frames > 0) {
        state.shake_frames--;
    }

    if (state.controls.pc) {
        goPause();
        fsm.go(GameStateName::Paused);
        return;
    }

    if (state.level_complete) {
        levelcomplete_counter = 0;
        fsm.go(GameStateName::LevelComplete);
        return;
    }

    if (state.gameOver()) {
        fsm.go(GameStateName::GameOver);
        return;
    }
}

void drawGame(GameData &state) {
    drawGameView(state);
}

#endif // GAME_H
