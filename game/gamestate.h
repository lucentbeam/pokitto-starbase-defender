#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <cstdint>
#include <cmath>

#include "core/rendering/rendersystem.h"
#include "core/controls/controls.h"

struct GameData {
    ControlStatus controls;
    RenderSystem * renderSystem;

    float heat = 0;
    float respawn_timer = 0;

    uint16_t score = 0;
    uint16_t total_tech = 0;

    uint16_t alert_timer = 0;
    uint16_t token_counter = 0;
    uint16_t death_counter = 0;

    uint8_t alert_color = 13;
    char alert_text[22];
    uint8_t alert_offset = 0;

    uint8_t current_level = 0;

    int8_t life = 5;

    int8_t baseLife = 6;

    uint8_t shake_frames = 0;

    uint8_t shot_flash = 0;

    uint8_t barrier_time = 0;
    const uint8_t max_barrier_time = 120;

    uint8_t blast_time = 0;
    const uint8_t max_blast_time = 8;

    uint8_t speed_rank = 0;
    uint8_t charge_rank = 0;

    bool level_complete = false;

    enum AbilityB : uint8_t {
        None,
        BigShot,
        Barrier,
        Blast
    } alt_ability = AbilityB::None;

    void levelReset() {
        death_counter = 0;
        heat = 0;
        life = 5;
        baseLife = 6;
        shake_frames = 0;
        shot_flash = 0;
        token_counter = 0;
        barrier_time = 0;
        blast_time = 0;
    }

    void reset() {
        levelReset();
        score = 0;
        alert_timer = 0;
        token_counter = 0;
        death_counter = 0;
        current_level = 0;
        speed_rank = 0;
        charge_rank = 0;
    }

    uint16_t currentScore() const {
        return uint16_t((float(tokenScore()) + float(baseScore())) * deathModifier());
    }

    int tokenScore() const {
        return token_counter * 25;
    }

    int baseScore() const {
        return int(std::fmax(baseLife, 0) * 45);
    }

    float deathModifier() const {
        return 1.0f - float(std::fmin(death_counter, 6)) / 10.0f;
    }

    bool gameOver() const {
        return baseLife <= 0;
    }
};

#endif // GAMESTATE_H
