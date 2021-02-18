#ifndef SCORES_H
#define SCORES_H

#include <array>
#include <algorithm>

#include "game/systems.h"
#include "core/serialization/serialization.h"

struct Score {
    char name[4] = "BEN";
    uint16_t score = 100;
};

struct StoredData {
    std::array<Score, 6> scores;

    void add(const char * n, uint16_t score) {
        scores[5].name[0] = n[0];
        scores[5].name[1] = n[1];
        scores[5].name[2] = n[2];
        scores[5].score = score;

        std::sort(scores.begin(), scores.end(), [](Score s1, Score s2) {
            return s1.score > s2.score;
        });
    }

    bool isNewRecord(uint16_t score) {
        return score > scores[4].score;
    }
};

StoredData highscores;

void readScores() {
    Serialization::tryGet<StoredData>("stardef.dat", &highscores);
}

void addAndWrite(const char * n, uint16_t score) {
    highscores.add(n, score);
    Serialization::tryStore<StoredData>("stardef.dat", &highscores);
}

void updateScores(FSM &fsm, GameData &state) {
    if (state.controls.pb) {
        fsm.go(GameStateName::Title);
    }
}

void drawScores(GameData &state) {
    drawGameView(state, true, false, false);

    SpriteData bg(ui_highscore_bg);
    state.renderSystem->sprite(28, 22, &bg);

    char buffer[20];
    for (int i = 0; i < 5; i++) {
        int sz = sprintf(buffer, "%s - %d", highscores.scores[i].name, highscores.scores[i].score);
        state.renderSystem->print(120-sz*5, 49 + 24 * i, buffer, 10);
    }
}


#endif // SCORES_H
