#ifndef PHYSICS_H
#define PHYSICS_H

#include "game/components.h"
#include "core/babyecs.h"
#include "core/collisions/rect.h"

const uint32_t physicsTimestepMs = 14;

void updatePhysics() {
    static auto all = BabyECS::createGroup(GroupMask({Component::Velocity, Component::Position }));
    const float timestep = physicsTimestepMs / 1000.0f;
    for (auto eid : all->members)
    {
        if (!BabyECS::alive(eid)) continue;
        Entity &e = BabyECS::get(eid);
        e.position.x += timestep * e.velocity.x;
        e.position.y += timestep * e.velocity.y;
    }
}


#endif // PHYSICS_H
