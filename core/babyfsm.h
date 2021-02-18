#ifndef BABYFSM_H
#define BABYFSM_H

#include "game/gamestate.h"

const uint8_t maxStates = 9;

class FSM {
    void (*updates[maxStates])(FSM&, GameData&);
    void (*draws[maxStates])(GameData&);

    uint8_t m_current;

    bool m_initialized = false;

public:
    FSM &add(uint8_t s, void (*upd)(FSM&, GameData&) = nullptr, void (*drw)(GameData&) = nullptr) {
        updates[s] = upd;
        draws[s] = drw;

        if (!m_initialized) {
            m_current = s;
            m_initialized = true;
        }
        return *this;
    }

    void go(uint8_t s) {
        if (s == m_current) {
            return;
        }
        m_current = s;
    }

    void update(GameData &state) {
        if (updates[m_current] != nullptr) {
            updates[m_current](*this, state);
        }
    }

    void draw(GameData &state) {
        if (draws[m_current] != nullptr) {
            draws[m_current](state);
        }
    }
};

#endif // BABYFSM_H
