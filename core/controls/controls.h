#ifndef __CONTROLS
#define __CONTROLS

struct ControlStatus {
  float x = 0.0f, y = 0.0f;
  bool pu = false, pd = false, pl = false, pr = false;
  bool a = false,b = false,c = false;
  bool pa = false;
  bool pb = false;
  bool pc = false;
};

class Controls {
    ControlStatus m_stats;
public:
    ControlStatus getStatus(bool normalize_dir = false);
};

extern bool previousUp, previousDown, previousLeft, previousRight;

#ifdef POKITTO_SFML
extern bool previousA, previousB, previousC;
#else
extern unsigned int upCount, downCount, leftCount, rightCount;
#endif

#endif
