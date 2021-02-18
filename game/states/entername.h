#ifndef ENTERNAME_H
#define ENTERNAME_H

#include "game/systems.h"
#include "game/spritesheets.h"
#include "game/states/scores.h"
#include "core/audio/audiosystem.h"

char nameEntry[4] = "BEN";
int atIndex = 0;
int blinkCounter = 0;

void resetNameEntry() {
    atIndex = 0;
    std::strcpy(nameEntry, "BEN");
}

void updateNameEntry(FSM &fsm, GameData &state) {
    if (state.controls.pa) {
        if (atIndex < 3) {
            AudioSystem::play(SFX::MenuChange);
            atIndex++;
        } else {
            AudioSystem::play(SFX::MenuSelect);
            addAndWrite(nameEntry, state.score);
            BabyECS::destroyAll();
            AudioSystem::playSong(TitleTheme);
            fsm.go(GameStateName::Scores);
        }
    }
    if (state.controls.pb && atIndex > 0) {
        AudioSystem::play(SFX::Alert);
        atIndex--;
    }
    if (state.controls.pu) {
        nameEntry[atIndex] = ((nameEntry[atIndex] + 1 - 65) % 26) + 65;
        AudioSystem::play(SFX::MenuChange);
    } else if (state.controls.pd) {
        AudioSystem::play(SFX::MenuChange);
        nameEntry[atIndex]--;
        if (nameEntry[atIndex] < 65) {
            nameEntry[atIndex] += 26;
        }
    }
    blinkCounter++;
}

void drawNameEntry(GameData &state) {
    drawGameView(state);

    state.renderSystem->sprite(34, 60, ui_enter_name);

    char letter[2] = "B";
    for (int i = 0; i < 3; i++) {
        if (i == atIndex && blinkCounter % 60 > 40) {
            continue;
        }
        if (i <= atIndex) {
            letter[0] = nameEntry[i];
            state.renderSystem->print(76 + 24 * i, 89, letter, 10, 2);
        } else {
            state.renderSystem->print(76 + 24 * i, 89, "_", 10, 2);
        }
    }

    if (atIndex == 3) {
        state.renderSystem->sprite(85, 116, ui_a_btn);
    }
}

#endif // ENTERNAME_H
