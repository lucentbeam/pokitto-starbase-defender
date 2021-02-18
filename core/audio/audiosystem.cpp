#include "audiosystem.h"

#ifdef POKITTO_SFML

float vol = 0;

void AudioSystem::setVolume(float value)
{
    vol = value;
}

float AudioSystem::getVolume()
{
    return vol;
}

void AudioSystem::play(SFX)
{

}

void AudioSystem::playSong(Song)
{

}

#else

#include "core/pokittolibextensions.h"
#include "Pokitto.h"
#include <LibAudio>

void AudioSystem::setVolume(float value)
{
    Audio::setVolume(value * 128.0f);
}

float AudioSystem::getVolume()
{
    uint8_t v;
    Audio::getVolume(&v);
    return float(v) / 128.0f;
}

#include <cstdint>

#include "sounds/menu_change.h"
#include "sounds/menu_select.h"
#include "sounds/player_attack.h"
#include "sounds/player_struck.h"
#include "sounds/player_destroyed.h"
#include "sounds/enemy_attack_new.h"
#include "sounds/base_struck.h"
#include "sounds/alert.h"

#include "sounds/tech_get.h"
#include "sounds/enemy_struck.h"
#include "sounds/enemy_destroyed.h"
#include "sounds/player_special.h"
#include "sounds/ball_bounce_1.h"

#include "Pokitto.h"
#include <LibAudio>

Audio::Sink<4, PROJ_AUD_FREQ> audio;

template <int N>
void playOnChannel(SFX sfx) {
    switch(sfx) {
        case MenuChange:
            Audio::play<N>(menu_change);
            break;
        case MenuSelect:
            Audio::play<N>(menu_select);
            break;
        case TechGet:
            Audio::play<N>(tech_get);
            break;
        case BallBounce:
            Audio::play<N>(ball_bounce_1);
            break;
        case EnemyStruck:
            Audio::play<N>(enemy_struck);
            break;
        case PlayerAttack:
            Audio::play<N>(player_attack);
            break;
        case PlayerStruck:
            Audio::play<N>(player_struck);
            break;
        case PlayerDestroyed:
            Audio::play<N>(player_destroyed);
            break;
        case PlayerSpecial:
            Audio::play<N>(player_special);
            break;
        case EnemyAttack:
            Audio::play<N>(enemy_attack_new);
            break;
        case EnemyDestroyed:
            Audio::play<N>(enemy_destroyed);
            break;
        case BaseStruck:
            Audio::play<N>(base_struck);
            break;
        case Alert:
            Audio::play<N>(alert);
            break;
        default:
            return;
    }
}

void AudioSystem::play(SFX sfx) {
    static int channel = 0;
    channel = (channel + 1) % 3;
    switch(channel) {
        case 0:
            playOnChannel<1>(sfx);
            break;
        case 1:
            playOnChannel<2>(sfx);
            break;
        case 2:
            playOnChannel<3>(sfx);
            break;
        default:
            break;
    }
}

void AudioSystem::playSong(Song song) {

    static Song activeSong = Song::None;
    if (activeSong == song) return;
    activeSong = song;

    static int title_counter = 0;

    switch(song) {
        case None:
            Audio::stop<0>();
        break;
        case TitleTheme:
            title_counter++;
            if (title_counter % 2 == 0) {
                Audio::play<0>("music/stardef0.raw");
            } else {
                Audio::play<0>("music/stardef1.raw");
            }
        break;
        case MainTheme:
            Audio::play<0>("music/stardef2.raw");
        break;
        case BossTheme:
            Audio::play<0>("music/stardef3.raw");
        break;
        default:
        break;
    }
}

#endif
