#ifndef VOLUME_H
#define VOLUME_H

#include "game/systems.h"

#include <cstdint>

#include "core/audio/audiosystem.h"

bool muted = false;
float volume = 0.5f;

void initVolume() {
    volume = AudioSystem::getVolume();
}

void toggleMute() {
    muted = !muted;
    if (muted) {
        AudioSystem::setVolume(0);
    } else {
        AudioSystem::setVolume(volume);
    }
}

void incrementVolume(float percentage) {
    if (muted && percentage > 0) {
        volume = 0.0f;
        muted = false;
    }
    float inc = percentage / 100.0f;
    volume = std::fmin(std::fmax(volume + inc, 0.0f), 1.0f);
    if (!muted) {
        AudioSystem::setVolume(volume);
    }
}

void drawVolume(RenderSystem * renderer, std::uint8_t x, std::uint8_t y, bool only_if_muted) {
    if (muted) {
        SpriteData icon(ui_icon_off);
        renderer->sprite(x, y-1, &icon);
    } else if (!only_if_muted) {
        SpriteData bar(ui_volume_bar);
        renderer->sprite(x, y, &bar, 0);
        uint8_t span = uint8_t(volume * 52.0f);
        renderer->setClip(true, span, 7);
        renderer->sprite(x, y, &bar, 1);
        renderer->setClip(false);
    }
}

#endif // VOLUME_H
