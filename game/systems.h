#ifndef ECS_H
#define ECS_H

#include "core/babyecs.h"
#include "core/babyfsm.h"
#include "game/components.h"

#include "game/interaction_systems/ball.h"
#include "game/interaction_systems/enemyshots.h"
#include "game/interaction_systems/enemies.h"
#include "game/interaction_systems/playershots.h"
#include "game/interaction_systems/physics.h"
#include "game/interaction_systems/playercontrols.h"
#include "game/interaction_systems/levelsequencer.h"
#include "game/interaction_systems/particles.h"
#include "game/rendering/drawgameview.h"
#include "game/interaction_systems/tokens.h"

enum GameStateName {
    Title,
    Game,
    GameOver,
    LevelComplete,
    Info,
    Scores,
    Paused,
    HighScoreEntry,
    Shop
};

#endif // ECS_H
