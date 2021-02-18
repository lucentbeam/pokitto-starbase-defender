#ifndef LEVELSEQUENCER_H
#define LEVELSEQUENCER_H

#include <cstdint>
#include <cstring>

#include "game/factory.h"
#include "core/controls/controls.h"
#include "core/babyecs.h"
#include "core/audio/audiosystem.h"

/* Level sequences are 3*N arrays, where each group N lists:
 *
 *   [delta_time, directive, data]
 *
 *   delta_time:   number of frames to wait for occuring since last directive completion
 *   directive:    an enum identifying the directive
 *   data:         a value used by the enum (usage varies)
 *
 */

enum SequencedEvent : uint8_t {
    RunSetup,              // data = unused
    SpawnEnemy,            // data = x location
    BarrageAlert,          // data = duration (80 frames per flash)
    ShotBarrage,           // data = duration
    BossAlert,             // data = duration (80 frames per flash)
    SpawnBoss,             // data = difficulty level, must be 1, 2, or 3
    WaitForClear,          // data = unused
    LevelOver,             // data = index of next level
    SpawnBall,             // data = x location
    SpawnToken,            // data = percengate (0-100) x location
    SpawnFastBall          // data = velocity bonus in percentage
};

const uint16_t level_one[] = {
    0, RunSetup, 0,
    180, SpawnEnemy, 20,
    180, SpawnEnemy, 80,
    180, SpawnEnemy, 40,
    10, WaitForClear, 0,
    10, BarrageAlert, 240,
    260, ShotBarrage, 420,
    180, SpawnEnemy, 20,
    180, SpawnEnemy, 80,
    180, SpawnEnemy, 40,
    10, WaitForClear, 0,
    240, LevelOver, 1
};

const uint16_t level_two[] = {
    0, RunSetup, 0,
    180, SpawnEnemy, 20,
    180, SpawnEnemy, 80,
    180, SpawnEnemy, 40,
    180, SpawnEnemy, 60,
    300, BarrageAlert, 60,
    220, ShotBarrage, 200,
    180, SpawnEnemy, 20,
    180, SpawnEnemy, 80,
    180, SpawnEnemy, 40,
    180, SpawnEnemy, 20,
    180, SpawnEnemy, 80,
    10, WaitForClear, 0,
    10, BossAlert, 180,
    200, SpawnBoss, 1,
    10, WaitForClear, 0,
    240, LevelOver, 2
};

const uint16_t level_three[] = {
    0, RunSetup, 0,
    180, SpawnEnemy, 20,
    180, SpawnEnemy, 80,
    180, SpawnEnemy, 40,
    180, SpawnEnemy, 60,
    10, WaitForClear, 0,
    300, BarrageAlert, 240,
    220, ShotBarrage, 200,
    100, SpawnBall, 110,
    180, SpawnEnemy, 20,
    180, SpawnEnemy, 80,
    180, SpawnEnemy, 40,
    180, SpawnEnemy, 40,
    10, WaitForClear, 0,
    10, BossAlert, 180,
    200, SpawnBoss, 2,
    10, WaitForClear, 0,
    240, LevelOver, 3
};

const uint16_t level_four[] = {
    0, RunSetup, 0,
    100, SpawnBall, 20,
    180, SpawnEnemy, 20,
    180, SpawnEnemy, 80,
    180, SpawnEnemy, 40,
    180, SpawnEnemy, 60,
    180, SpawnEnemy, 80,
    180, SpawnEnemy, 20,
    10, WaitForClear, 0,
    300, BarrageAlert, 240,
    220, ShotBarrage, 200,
    100, SpawnBall, 180,
    180, SpawnEnemy, 20,
    180, SpawnEnemy, 80,
    180, SpawnEnemy, 40,
    180, SpawnEnemy, 40,
    10, WaitForClear, 0,
    180, SpawnEnemy, 80,
    180, SpawnEnemy, 40,
    180, SpawnEnemy, 40,
    180, SpawnEnemy, 20,
    180, SpawnEnemy, 80,
    180, SpawnEnemy, 40,
    180, SpawnEnemy, 60,
    10, WaitForClear, 0,
    10, BossAlert, 180,
    200, SpawnBoss, 3,
    10, WaitForClear, 0,
    240, LevelOver, 4
};

const uint16_t level_five[] = {
    0, RunSetup, 0,
    100, SpawnBall, 20,
    180, SpawnEnemy, 20,
    180, SpawnEnemy, 80,
    180, SpawnEnemy, 40,
    180, SpawnEnemy, 60,
    180, SpawnEnemy, 80,
    180, SpawnEnemy, 20,
    10, WaitForClear, 0,
    300, BarrageAlert, 60,
    220, ShotBarrage, 200,
    100, SpawnFastBall, 20,
    180, SpawnEnemy, 20,
    180, SpawnEnemy, 80,
    180, SpawnEnemy, 40,
    180, SpawnEnemy, 40,
    10, WaitForClear, 0,
    10, BossAlert, 180,
    200, SpawnBoss, 3,
    500, BarrageAlert, 60,
    220, ShotBarrage, 200,
    10, WaitForClear, 0,
    240, LevelOver, 5
};

const uint16_t level_six[] = {
    0, RunSetup, 0,
    100, SpawnFastBall, 20,
    180, SpawnEnemy, 20,
    180, SpawnEnemy, 80,
    10, WaitForClear, 0,
    300, BarrageAlert, 240,
    140, ShotBarrage, 200,
    100, SpawnFastBall, 20,
    180, SpawnEnemy, 20,
    180, SpawnEnemy, 80,
    10, WaitForClear, 0,
    10, BossAlert, 180,
    200, SpawnBoss, 3,
    500, BarrageAlert, 60,
    220, ShotBarrage, 200,
    10, WaitForClear, 0,
    240, LevelOver, 6 // go to kill screen
};

// aspirational kill screen, cycle back to previous on survival
const uint16_t level_seven[] = {
    0, RunSetup, 0,
    100, SpawnFastBall, 10,
    100, SpawnFastBall, 20,
    100, SpawnFastBall, 30,
    100, SpawnFastBall, 40,
    140, ShotBarrage, 240,
    340, ShotBarrage, 240,
    240, LevelOver, 5 // go to previous level
};

const uint16_t * levels[] = {
    level_one,
    level_two,
    level_three,
    level_four,
    level_five,
    level_six,
    level_seven
};

struct LevelProgress {
    const uint16_t * data;
    uint16_t counter = 0;
    uint8_t position = 0;

    LevelProgress(const uint16_t * dat) : data(dat) {}

    void tick() { counter++; }
    void advance() { position += 3; counter = 0; }

    bool ready() const { return counter >= data[position]; }
    SequencedEvent event() const { return SequencedEvent(data[position+1]); }
    uint16_t info() const { return data[position+2]; }
};

bool first = true;
uint16_t levelcounter = 0;
LevelProgress level(levels[0]);

void resetLevelSequence() {
    first = true;
    levelcounter = 0;
    level = LevelProgress(levels[0]);
}

void advanceLevelSequence() {
    int l = level.info();
    level = LevelProgress(levels[l]);
    levelcounter = 0;
}

void updateLevelSequenceFrame(GameData &playerInfo, ControlStatus controls) {
    level.tick();

    static auto enemies = BabyECS::createGroup(GroupMask({Component::Position,Component::Size,Component::Velocity,Component::Enemy},{Tag::Enemy}));

    Entity * entity;
    Enemy * enemy;
    
    float bonus;
    if (level.ready()) {
        switch (level.event()) {
        case RunSetup:
            AudioSystem::playSong(Song::MainTheme);
            BabyECS::destroyAll();
\
            if (!first) {
                playerInfo.score += playerInfo.currentScore();
                playerInfo.levelReset();
            }
            first = false;
            playerInfo.current_level++;

            //TODO: replace with level specific stuff via level.info()?
            Factory::createPlayer(playerInfo);
            Factory::createBall(110, 130, -70, -50);
            level.advance();
            break;
        case SpawnBall:
            Factory::createBall(level.info(), -34, -70, 50);
            level.advance();
            break;
        case SpawnFastBall:
            bonus = 1.0f + (float(level.info()))/100.0f;
            Factory::createBall(110, -34, -70.0f * bonus, 50.0f * bonus);
            level.advance();
            break;
        case WaitForClear:
            if (enemies->members.size() == 0) {
                level.advance();
            }
            break;
        case SpawnEnemy:
            Factory::createEnemy(level.info());
            level.advance();
            break;
        case SpawnBoss:
            entity = &Factory::createEnemy(0);
            enemy = &(entity->enemy);
            enemy->life = 2 + level.info();
            enemy->difficulty = level.info();
            enemy->target_y = 40;
            level.advance();
            break;
        case BarrageAlert:
            playerInfo.alert_timer = level.info();
            playerInfo.alert_color = 13;
            std::strcpy(playerInfo.alert_text, "Incoming Barrage");
            playerInfo.alert_offset = 72;
            level.advance();
            break;
        case BossAlert:
            AudioSystem::playSong(Song::BossTheme);
            playerInfo.alert_timer = level.info();
            playerInfo.alert_color = 11;
            std::strcpy(playerInfo.alert_text, "Incoming Boss");
            playerInfo.alert_offset = 58;
            level.advance();
            break;
        case ShotBarrage:
            if (levelcounter == 0) {
                levelcounter = level.info();
            } else if (levelcounter == 1) {
                level.advance();
                levelcounter = 0;
            } else {
                levelcounter--;
                if (levelcounter % 15 == 0) {
                    AudioSystem::play(SFX::EnemyAttack);
                    Factory::createEnemyShot(rand() % 220, -34);
                }
            }
            break;
        case SpawnToken:
            Factory::createToken(float(level.info())/100.0f * 200 + 10, -30);
            level.advance();
            break;
        case LevelOver:
            AudioSystem::playSong(Song::None);
            playerInfo.level_complete = true;
            break;
        }
    }
}

#endif // LEVELSEQUENCER_H
