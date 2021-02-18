#include "core/rendering/rendersystem.h"
#include "core/controls/controls.h"

#include "game/systems.h"
#include "game/factory.h"
#include "core/babyfsm.h"

#include "game/states/game.h"
#include "game/states/title.h"
#include "game/states/gameover.h"
#include "game/states/info.h"
#include "game/states/levelcomplete.h"
#include "game/states/scores.h"
#include "game/states/pause.h"
#include "game/states/entername.h"
#include "game/states/shop.h"

#include "game/volume.h"

int main ()
{
    BabyECS::initialize();

    RenderSystem renderSystem;
    renderSystem.initialize();

    GameData state;
    state.renderSystem = &renderSystem;

    Controls controls;

    FSM fsm;
    fsm
            .add(GameStateName::Title,          updateTitle,         drawTitle)
            .add(GameStateName::Scores,         updateScores,        drawScores)
            .add(GameStateName::Game,           updateGame,          drawGame)
            .add(GameStateName::GameOver,       updateGameOver,      drawGameOver)
            .add(GameStateName::LevelComplete,  updateLevelComplete, drawLevelComplete)
            .add(GameStateName::Info,           updateInfo,          drawInfo)
            .add(GameStateName::HighScoreEntry, updateNameEntry,     drawNameEntry)
            .add(GameStateName::Shop,           updateShop,          drawShop)
            .add(GameStateName::Paused,         updatePause,         drawPause);

    readScores();
    initVolume();

    int32_t gameTime = 0;
    uint32_t lastGameTime = renderSystem.getTimeMs();
    
    while (renderSystem.running())
    {
        bool draws = renderSystem.update();

        gameTime += renderSystem.getTimeMs() - lastGameTime;
        lastGameTime = renderSystem.getTimeMs();
        uint8_t step_counter = 0; // prevent death spiral
        while (gameTime > 0 && step_counter < 5) {
            state.controls = controls.getStatus();
            BabyECS::compress();
            fsm.update(state);
            gameTime -= physicsTimestepMs;
            step_counter++;
        }

        if (draws) {
            fsm.draw(state); // TODO: add frame interpolation?
            //renderSystem.debug();
        }
    }

    BabyECS::destroyAll();

    return 0;
}
