#ifndef TITLE_H
#define TITLE_H

#include "game/systems.h"

#include "game/states/info.h"

#include "game/interaction_systems/levelsequencer.h"

#include "game/volume.h"

#include "core/audio/audiosystem.h"

static int choice = 0;

static int title_delay = 0;

void enterTitle() {
    AudioSystem::playSong(Song::TitleTheme);
}

void updateTitle(FSM &fsm, GameData &state) {
    static bool initialized = false;
    if (!initialized) {
        enterTitle();
        initialized = true;
    }
    title_delay++;
    if (title_delay < 10) {
        return;
    }
    if (state.controls.pu) {
        AudioSystem::play(SFX::MenuChange);
        choice--;
        if (choice < 0) { choice += 4; }
    } else if (state.controls.pd) {
        AudioSystem::play(SFX::MenuChange);
        choice = (choice + 1) % 4;
    }
    if (state.controls.pa) {
        AudioSystem::play(SFX::MenuSelect);
        if (choice == 0) {
            state.reset();
            BabyECS::destroyAll();
            fsm.go(GameStateName::Game);
            initialized = false;
            AudioSystem::playSong(Song::MainTheme);
            title_delay = 0;
            resetLevelSequence();
        } else if (choice == 1){
            setupInfo(state);
            fsm.go(GameStateName::Info);
            AudioSystem::playSong(Song::None);
            initialized = false;
            title_delay = 0;
        } else if (choice == 2) {
            fsm.go(GameStateName::Scores);
            initialized = false;
            title_delay = 0;
        } else if (choice == 3) {
            toggleMute();
        }
    }
    if (choice == 3 && state.controls.x != 0) {
        incrementVolume(state.controls.x * 30.0f * 0.016f);
    }
}

void drawTitle(GameData &state) {
    SpriteData bg(title_image);
    state.renderSystem->sprite(0, 0, &bg);

    bg = SpriteData(sprite_ball);
    state.renderSystem->sprite(14, 69 + choice * 19, &bg);

    drawVolume(state.renderSystem, 89, 127, choice != 3);
}

#endif // TITLE_H
