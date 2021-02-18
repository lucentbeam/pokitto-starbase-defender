#ifndef GAMESPRITES_H
#define GAMESPRITES_H

#include "game/components.h"
#include "core/babyecs.h"
#include "core/rendering/rendersystem.h"
#include "game/factory.h"
#include "core/audio/audiosystem.h"

#include <cstdio>
#include <cstdlib>

const Vector stars[10] = { {26,75}, {167,90}, {190,40}, {55,32}, {110,73}, {7,16}, {204,58}, {170,10}, {137,62}, {110,98}};

void drawGameView(GameData &state, bool draw_bg = true, bool draw_sprites = true, bool draw_ui = true) {

    int8_t deltax = 0;
    int8_t deltay = 0;
    if (state.shake_frames > 0) {
        deltax = (std::rand() % 10) - 5;
        deltay = (std::rand() % 10) - 5;
    }

    if (draw_bg) {
        SpriteData bg(background);

        state.renderSystem->clear(0);
        state.renderSystem->sprite(deltax, 115 + deltay, &bg, 0);
        for(uint8_t i = 0; i < 10; i++) {
            state.renderSystem->pixel(int(stars[i].x + deltax), int(stars[i].y + deltay), 1 + (i % 3));
        }
    }

    if (draw_sprites) {
        static auto all = BabyECS::createGroup({ Component::SpriteComponent, Component::Position });

        std::vector<Eid> deferred;
        for (auto eid : all->members)
        {
            if (!BabyECS::alive(eid)) continue;
            if (!BabyECS::hasTag(eid, Tag::Background)) {
                deferred.push_back(eid);
                continue;
            }
            Sprites sprite = BabyECS::get(eid).sprite;
            Vector &pos = BabyECS::get(eid).position;
            int frame = 0;
            if (BabyECS::has(eid, Component::Particle)) {
                frame = BabyECS::get(eid).particle.frame;
            }
            state.renderSystem->sprite(int(pos.x + 0.5f + deltax), int(pos.y + 0.5f + deltay), sprite_list[sprite], frame);
        }

        SpriteData stun(particle_enemystun);
        for (auto eid : deferred)
        {
            Sprites sprite = BabyECS::get(eid).sprite;
            Vector &pos = BabyECS::get(eid).position;
            int frame = 0;
            if (BabyECS::has(eid, Component::Particle)) {
                frame = BabyECS::get(eid).particle.frame;
            }
            if (BabyECS::hasTag(eid, Tag::Ball)) {
                static uint8_t counter = 0;
                counter++;
                frame = (counter/15) % 2;
            }
            if (BabyECS::has(eid, Component::Enemy)) {
                frame = BabyECS::get(eid).enemy.sprite_index;
            }
            
            state.renderSystem->sprite(int(pos.x + 0.5f + deltax), int(pos.y + 0.5f + deltay), sprite_list[sprite], frame);

            if (BabyECS::hasTag(eid, Tag::Stunned) && (rand() % 2)) {
                state.renderSystem->sprite(int(pos.x + 0.5f + deltax), int(pos.y + 0.5f + deltay), &stun, rand() % 2);
            }
        }

        // PLAYER ON-PADDLE UI

        Eid playerId = BabyECS::getFirst(GroupMask({Component::Position},{Tag::Paddle}));
        if (BabyECS::alive(playerId)) {
            Vector &pos = BabyECS::get(playerId).position;

            SpriteData heat(playerheat);
            state.renderSystem->setClip(true, uint8_t(state.heat/100.0f * 20), 14);
            state.renderSystem->sprite(int(pos.x + 9.5f), int(pos.y + 5.5f), &heat);

            if (state.shot_flash > 0) {
                state.renderSystem->drawLine(int(pos.x + 9.5f), int(pos.y + 5.5f), int(pos.x + 9.5f) + 18, int(pos.y + 5.5f),11);
                state.renderSystem->drawLine(int(pos.x + 9.5f), int(pos.y + 6.5f), int(pos.x + 9.5f) + 18, int(pos.y + 6.5f),6);
            }

            SpriteData life(playerhealth);
            state.renderSystem->setClip(true, uint8_t(state.life/5.0f * 18), 14);
            state.renderSystem->sprite(int(pos.x + 9.5f), int(pos.y + 3.5f), &life);

            state.renderSystem->setClip(false);
        }
    }

    if (draw_ui) {
        if (state.shake_frames > 0) {
            state.renderSystem->drawLine(2,2,218,2,11);
            state.renderSystem->drawLine(2,2,2,169,11);
            state.renderSystem->drawLine(2,169,218,169,11);
            state.renderSystem->drawLine(218,2,218,169,11);
        }

        SpriteData uibar(ui_basehealth);
        state.renderSystem->sprite(0, 165, &uibar);

        SpriteData health(ui_healthbar);
        for (int i = 0; i < 6; i++) {
            if (i < state.baseLife) {
                state.renderSystem->sprite(1 + i * 10, 169, &health);
            }
        }

        uibar = SpriteData(ui_tokencount);
        state.renderSystem->sprite(178,165,&uibar);
        char buffer[4];
        int size = sprintf(buffer, "%d", state.token_counter);
        state.renderSystem->print(207-5*size,168,buffer,10);

        if (state.alert_timer > 0) {
            if (state.alert_timer % 60 > 36) {
                state.renderSystem->drawLine(20,2,20,174,state.alert_color);
                state.renderSystem->drawLine(200,2,200,174,state.alert_color);
                for (int i = 65; i <= 155; i+=45) {
                    state.renderSystem->drawLine(i,2,i,70,state.alert_color); // draw vertical lines
                    state.renderSystem->drawLine(i,100,i,174,state.alert_color); // draw vertical lines
                }
                for (int i = 20; i <= 155; i+=45) {
                    state.renderSystem->drawLine(2,i,218,i,state.alert_color); // draw horizontal lines
                }
                AudioSystem::play(SFX::Alert);
            }
            SpriteData warning(ui_warning);
            state.renderSystem->sprite(34, 61, &warning, state.alert_color == 13 ? 0 : 1);
            state.renderSystem->print(110-state.alert_offset, 84, state.alert_text, 10);
        }
    }
}

#endif // GAMESPRITES_H
