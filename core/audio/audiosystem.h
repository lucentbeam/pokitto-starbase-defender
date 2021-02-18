#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

// enum declarations
#include "game/sfx/sfx.h"

class AudioSystem
{
public:

    static void setVolume(float value);

    static float getVolume();

    static void play(SFX);

    static void playSong(Song);
};

#endif // AUDIOSYSTEM_H
