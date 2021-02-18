#ifndef BALL_H
#define BALL_H

#include "game/components.h"
#include "core/babyecs.h"
#include "core/collisions/rect.h"
#include "game/factory.h"
#include "core/audio/audiosystem.h"

#include <cmath>
#include <cstdlib>

uint8_t ball_top = 0;

void setBallTop(uint8_t pos = 0) {
    ball_top = pos;
}

void updateBalls(GameData &playerInfo) {

    static auto balls = BabyECS::createGroup(GroupMask({Component::Position, Component::Velocity, Component::Size}, {Tag::Ball}));

    auto count = balls->members.size();
    if (count == 0) return;

    static uint8_t timer = 0;
    timer++;
    int timer_offset = 14 / count;
    int ct = 0;

    static auto bounce_targets = BabyECS::createGroup(GroupMask({Component::Position, Component::Size}, {Tag::BounceTarget}));

    for(auto b : balls->members) {
        if (!BabyECS::alive(b)) continue;

        Entity &ball = BabyECS::get(b);

        Rect ball_rect(ball.position.x,ball.position.y,ball.size.x,ball.size.y);

        if ((timer + ct * timer_offset) % 14*count == 0) {
            Factory::createNebulaParticle(ball_rect.centerX(), ball_rect.centerY());
        }
        ct++;

        if (ball_rect.left() < 0 && ball.velocity.x < 0) {
            ball.velocity.x = std::fabs(ball.velocity.x);
            AudioSystem::play(SFX::BallBounce);
        } else if (ball_rect.right() > 220 && ball.velocity.x > 0) {
            ball.velocity.x = std::fabs(ball.velocity.x) * -1.0f;
            AudioSystem::play(SFX::BallBounce);
        }
        if (ball_rect.top() < ball_top && ball.velocity.y < 0) {
            ball.velocity.y = std::fabs(ball.velocity.y);
            AudioSystem::play(BallBounce);
        } else if (ball_rect.bottom() > 176 && ball.velocity.y > 0) {
            ball.velocity.y = std::fabs(ball.velocity.y) * -1.0f;
            playerInfo.baseLife--;
            playerInfo.shake_frames = 10;
            AudioSystem::play(SFX::BaseStruck);
        }

        for (auto t : bounce_targets->members) {
            if (t == b || !BabyECS::alive(t)) continue;
            Entity &target = BabyECS::get(t);

            Size &esz = target.size;
            Vector &epos = target.position;
            Rect target_rect(epos.x,epos.y,esz.x,esz.y);

            bool blast = false;
            if (BabyECS::hasTag(t, Tag::Paddle) && playerInfo.blast_time > 0) {
                blast = true;
                float size_frac = 1.0f - float(playerInfo.blast_time) / float(playerInfo.max_blast_time);
                target_rect.grow(40 * size_frac, 70 * size_frac);
            }

            if (target_rect.overlaps(ball_rect)) {

                if (BabyECS::hasTag(t, Tag::Enemy)) {
                    Enemy &en = target.enemy;
                    if (en.iframes == 0) {
                        en.iframes = 10;
                        en.life--;
                        Factory::createExplosion(target_rect,ball.velocity.x/4.0f,ball.velocity.y/4.0f,80.0f);
                    }
                }

                bool bounce_up = BabyECS::hasTag(t, Tag::Paddle) || BabyECS::hasTag(t, Tag::Shot);
                if (bounce_up && (ball.velocity.y < 0 || ball_rect.centerY() > target_rect.centerY())) {
                    continue;
                }

                bool bounce_down = BabyECS::hasTag(t, Tag::EnemyShot);
                if (bounce_down && (ball.velocity.y > 0 || ball_rect.centerY() < target_rect.centerY())) {
                    continue;
                }

                float dx = target_rect.centerX() - ball_rect.centerX();
                float dy = target_rect.centerY() - ball_rect.centerY();
                float px = std::fabs(dx) - (ball.size.x + esz.x);
                float py = std::fabs(dy) - (ball.size.y + esz.y);
                if (bounce_up || bounce_down || px < py) {
                    ball.velocity.y = std::fabs(ball.velocity.y) * (dy > 0 ? -1 : 1);
                    AudioSystem::play(SFX::BallBounce);
                } else {
                    ball.velocity.x = std::fabs(ball.velocity.x) * (dx > 0 ? -1 : 1);
                    AudioSystem::play(SFX::BallBounce);
                }
                if (bounce_up || bounce_down) {
                    for (int i = 0; i < 2; i++) {
                        Factory::createNebulaParticle(ball_rect.centerX(), ball_rect.bottom());
                    }
                }
                if (BabyECS::hasTag(t, Tag::DestroyOnBounce)) {
                    BabyECS::destroy(t);
                }
            }
        }
    }
}

#endif // BALL_H
