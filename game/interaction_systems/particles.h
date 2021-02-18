#ifndef PARTICLES_H
#define PARTICLES_H

#include "game/components.h"
#include "core/babyecs.h"
#include "core/collisions/rect.h"
#include "game/factory.h"

void updateParticles() {
    static auto particles = BabyECS::createGroup(GroupMask({Component::Particle}));

    for(auto p : particles->members) {
        if (!BabyECS::alive(p)) continue;
        if (BabyECS::has(p, Component::Blast)) {
            Vector &vel = BabyECS::get(p).velocity;
            vel.x *= 0.7f;
            vel.y *= 0.7f;
        }
        Particle &particle = BabyECS::get(p).particle;
        particle.counter++;
        if (particle.counter > particle.counts_per_frame) {
            particle.counter = 0;
            particle.frame++;
            if (particle.frame >= particle.last_frame) {
                BabyECS::destroy(p);
            }
        }
    }
}

#endif // PARTICLES_H
