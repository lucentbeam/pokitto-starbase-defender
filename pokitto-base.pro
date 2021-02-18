TEMPLATE = app
CONFIG += console c++17
CONFIG += app_bundle
CONFIG -= qt

SOURCES += \
    core/audio/audiosystem.cpp \
    core/collisions/rect.cpp \
    core/controls/controls.cpp \
    core/pokittolibextensions.cpp \
    core/rendering/rendersystem.cpp \
    main.cpp \
    core/rendering/sprites.cpp \
    core/rendering/tileset.cpp

INCLUDEPATH += \
    game \
    core \
    core/rendering \
    core/controls \
    core/collisions \
    core/serialization \
    game/interaction_systems \
    game/rendering \
    game/states

HEADERS += \
    core/audio/audiosystem.h \
    core/babyecs.h \
    core/babyfsm.h \
    core/pokittolibextensions.h \
    core/rendering/rendersystem.h \
    core/serialization/serialization.h \
    game/gamestate.h \
    game/interaction_systems/ball.h \
    game/interaction_systems/enemies.h \
    game/interaction_systems/enemyshots.h \
    game/components.h \
    game/factory.h \
    game/interaction_systems/levelsequencer.h \
    game/interaction_systems/physics.h \
    game/interaction_systems/playercontrols.h \
    game/interaction_systems/playershots.h \
    game/interaction_systems/tokens.h \
    game/palettes.h \
    game/rendering/drawgameview.h \
    game/interaction_systems/particles.h \
    game/sfx/sfx.h \
    game/spritesheets.h \
    game/states/entername.h \
    game/states/game.h \
    game/states/gameover.h \
    game/states/info.h \
    game/states/levelcomplete.h \
    game/states/pause.h \
    game/states/scores.h \
    game/states/shop.h \
    game/states/title.h \
    game/systems.h \
    core/collisions/rect.h \
    core/controls/controls.h \
    core/settings_pokitto.h \
    core/rendering/sprites.h \
    core/rendering/tileset.h \
    game/volume.h

win32:INCLUDEPATH += C:/SFML\SFML-2.5.1_msvc2017_64/include
win32:DEPENDPATH += C:/SFML\SFML-2.5.1_msvc2017_64/include

win32:DEFINES += SFML_STATIC

win32:CONFIG(release, debug|release): LIBS += -LC:/SFML/SFML-2.5.1_msvc2017_64/lib/ -lsfml-main -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lfreetype #-ljpeg
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/SFML/SFML-2.5.1_msvc2017_64/lib/ -lsfml-main-d -lsfml-graphics-s-d -lsfml-window-s-d -lsfml-system-s-d -lfreetype #-ljpeg

win32: LIBS += -L"$$PWD/../../../../../Program Files (x86)/Windows Kits/10/Lib/10.0.16299.0/um/x64/" -lwinmm -lgdi32 -lopengl32 -luser32 -ladvapi32

mac: QMAKE_LFLAGS += -F$$PWD/../../../SFML-2.5.1/Frameworks/
mac: LIBS += -framework SFML -framework sfml-system -framework sfml-window -framework sfml-graphics

mac: INCLUDEPATH += $$PWD/../../../SFML-2.5.1/include
mac: DEPENDPATH += $$PWD/../../../SFML-2.5.1/include

DEFINES += POKITTO_SFML
