#ifndef TINYECS_H
#define TINYECS_H

#include <vector>
#include <initializer_list>
#include <limits>
#include "game/components.h"

typedef uint8_t Eid;
typedef uint32_t Cid;                 // set a type, get one component per bit (e.g., uint16 = 16 components)
const Eid maxEntityCount = 64;
const uint8_t maxGroupCount = 12;

const Eid nullEntity = std::numeric_limits<Eid>::max();
const uint8_t maxComponentId = sizeof(Cid) * 8 - 1;  // remainder bits can be used as tags

class GroupMask {
    Cid mask = 0;
    friend class BabyECS;

public:
    GroupMask(const std::initializer_list<Cid> &cList, const std::initializer_list<Cid> &tList = {});
};

class Group {
    Cid mask = 0;
    friend class BabyECS;

public:
    std::vector<Eid> members;
};

class BabyECS {

    static Group* s_groups[maxGroupCount];

    static std::vector<Eid> s_available_ids;

    static std::vector<Eid> s_id_queue;

    static uint8_t s_group_count;

    static Cid s_entity_components[maxEntityCount];

    static Entity s_entities[maxEntityCount];

    static Eid s_last_created;

public:

    static Group* createGroup(const GroupMask &group);

    static bool alive(Eid id);

    static Eid aliveCount();

    static Eid lastCreatedId();

    static void compress();

    static std::vector<Eid> getGroup(const GroupMask &group);

    static Eid getFirst(const GroupMask &group);

    static Eid getNext(Eid start, const GroupMask &group);

    static bool has(Eid id, Cid component);

    static bool hasTag(Eid id, Cid tag);

    static void remove(Eid id, Cid component);

    static void add(Eid id, Cid component);

    static void setTag(Eid id, Cid tag, bool val);

    static Entity &get(Eid id);

    static void initialize();

    static Entity &create(std::initializer_list<Cid> componentIds = {}, std::initializer_list<Cid> tagIds = {});

    static void destroy(Eid id);

    static void destroyAll();
};

GroupMask::GroupMask(const std::initializer_list<Cid> &componentList, const std::initializer_list<Cid> &tagList) {
    for (const auto &i : componentList) {
        mask |= (1 << i);
    }
    for (const auto &i : tagList) {
        mask |= (1 << (maxComponentId - i));
    }
}


Group* BabyECS::s_groups[maxGroupCount];

std::vector<Eid> BabyECS::s_available_ids;

std::vector<Eid> BabyECS::s_id_queue;

uint8_t BabyECS::s_group_count = 0;

Cid BabyECS::s_entity_components[maxEntityCount] = { 0 };

Entity BabyECS::s_entities[maxEntityCount] = { Entity() };

Eid BabyECS::s_last_created = 0;

Group *BabyECS::createGroup(const GroupMask &group)
{
    for (auto i = 0; i < s_group_count; i++) {
        if (s_groups[i]->mask == group.mask) {
            return s_groups[i];
        }
    }
    Group * g = new Group();
    g->mask = group.mask;
    s_groups[s_group_count] = g;
    s_group_count++;
    return g;
}

bool BabyECS::alive(Eid id)
{
    return id != nullEntity && s_entity_components[id] != 0;
}

Eid BabyECS::aliveCount()
{
    return maxEntityCount - s_available_ids.size();
}

Eid BabyECS::lastCreatedId()
{
    return s_last_created;
}

void BabyECS::compress()
{
    Eid alive_count = 0;
    Eid alive[maxEntityCount];
    for (Eid i = 0; i < maxEntityCount; i++) {
        if (s_entity_components[i] > 0) {
            alive[alive_count] = i;
            alive_count++;
        }
    }
    for (auto j = 0; j < s_group_count; j++) {
        s_groups[j]->members.clear();
        for (Eid i = 0; i < alive_count; i++) {
            if ((s_entity_components[alive[i]] & s_groups[j]->mask) == s_groups[j]->mask) {
                s_groups[j]->members.push_back(alive[i]);
            }
        }
    }
    for (auto id : s_id_queue) {
        s_available_ids.push_back(id);
    }
    s_id_queue.clear();
}

std::vector<Eid> BabyECS::getGroup(const GroupMask &group) {
    std::vector<Eid> ents;
    for (Eid i = 0; i < maxEntityCount; i++) {
        if ((s_entity_components[i] & group.mask) == group.mask) {
            ents.push_back(i);
        }
    }
    return ents;
}

Eid BabyECS::getFirst(const GroupMask &group) // don't use this Eid if == entityCount!
{
    for (Eid i = 0; i < maxEntityCount; i++) {
        if ((s_entity_components[i] & group.mask) == group.mask) {
            return i;
        }
    }
    return nullEntity;
}

Eid BabyECS::getNext(Eid start, const GroupMask &group)
{
    for (Eid i = start + 1; i < maxEntityCount; i++) {
        if ((s_entity_components[i] & group.mask) == group.mask) {
            return i;
        }
    }
    return nullEntity;
}

bool BabyECS::has(Eid id, Cid component) {
    return (s_entity_components[id] & (1 << component));
}

bool BabyECS::hasTag(Eid id, Cid tag)
{
    return (s_entity_components[id] & (1 << Cid(maxComponentId - tag)));
}

void BabyECS::remove(Eid id, Cid component) {
    s_entity_components[id] &= ~(1 << component);
}

void BabyECS::add(Eid id, Cid component) {
    s_entity_components[id] |= (1 << component);
}

void BabyECS::setTag(Eid id, Cid tag, bool val)
{
    if (val) {
        s_entity_components[id] |= (1 << int(maxComponentId-tag));
    } else {
        s_entity_components[id] &= ~(1 << int(maxComponentId-tag));
    }
}

Entity &BabyECS::get(Eid id)
{
    return s_entities[id];
}

void BabyECS::initialize()
{
    s_available_ids.clear();
    for (auto i = 0; i < maxEntityCount; i++) {
        s_available_ids.push_back(i);
    }
}

Entity &BabyECS::create(std::initializer_list<Cid> componentIds, std::initializer_list<Cid> tagIds) {
    if (s_available_ids.size() == 0) {
//        return Entity();
    }
    Eid id = s_available_ids.back();
    s_available_ids.pop_back();
    s_last_created = id;
    for (auto component : componentIds) {
        add(id, component);
    }
    for (auto tag : tagIds) {
        setTag(id, tag, true);
    }
    for (auto j = 0; j < s_group_count; j++) {
        if ((s_entity_components[id] & s_groups[j]->mask) == s_groups[j]->mask) {
            s_groups[j]->members.push_back(id);
        }
    }
    return s_entities[id];
}

void BabyECS::destroy(Eid id) {
    s_entity_components[id] = 0;
    s_id_queue.push_back(id);
}

void BabyECS::destroyAll()
{
    for (Eid i = 0; i < maxEntityCount; i++) {
        if (s_entity_components[i] > 0) {
            destroy(i);
        }
    }
    for (auto j = 0; j < s_group_count; j++) {
        s_groups[j]->members.clear();
    }
    s_id_queue.clear();
    initialize();
}

#endif // TINYECS_H
