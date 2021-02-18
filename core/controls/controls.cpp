#include "controls.h"

bool previousUp = false;
bool previousDown = false;
bool previousLeft = false;
bool previousRight = false;

#ifndef POKITTO_SFML

#include "Pokitto.h"
#include <limits>

unsigned int upCount = 0;
unsigned int downCount = 0;
unsigned int rightCount = 0;
unsigned int leftCount = 0;

ControlStatus Controls::getStatus(bool normalize_dir)
{
  upCount = Pokitto::Core::upBtn() ? upCount + 1 : 0;
  downCount = Pokitto::Core::downBtn() ? downCount + 1 : 0;
  leftCount = Pokitto::Core::leftBtn() ? leftCount + 1 : 0;
  rightCount = Pokitto::Core::rightBtn() ? rightCount + 1 : 0;

  m_stats.y = 0;
  if (upCount > 0 && downCount > 0) { // NOTE: dpad on pokitto can get stuck with both buttons down after quickly alternating
      m_stats.y = upCount < downCount ? -1 : 1;
  } else if (upCount > 0) {
      m_stats.y = -1;
  } else if (downCount > 0) {
      m_stats.y = 1;
  }

  m_stats.x = 0;
  if (rightCount > 0 && leftCount > 0) {
      m_stats.x = rightCount < leftCount ? 1 : -1;
  } else if (rightCount > 0) {
      m_stats.x = 1;
  } else if (leftCount > 0) {
      m_stats.x = -1;
  }

  if (normalize_dir && fabs(m_stats.x) > std::numeric_limits<float>::epsilon() && fabs(m_stats.y) > std::numeric_limits<float>::epsilon()) {
    m_stats.x *= 0.7071f;
    m_stats.y *= 0.7071f;
  }
  m_stats.pu = m_stats.y < 0 && !previousUp;
  previousUp = m_stats.y < 0;

  m_stats.pd = m_stats.y > 0 && !previousDown;
  previousDown = m_stats.y > 0;

  m_stats.pl = m_stats.x < 0 && !previousLeft;
  previousLeft = m_stats.x < 0;

  m_stats.pr = m_stats.x > 0 && !previousRight;
  previousRight = m_stats.x > 0;

  m_stats.pa = !m_stats.a && Pokitto::Core::aBtn();
  m_stats.a = Pokitto::Core::aBtn();

  m_stats.pb = !m_stats.b && Pokitto::Core::bBtn();
  m_stats.b = Pokitto::Core::bBtn();

  m_stats.pc = !m_stats.c && Pokitto::Core::cBtn();
  m_stats.c = Pokitto::Core::cBtn();
  return m_stats;
}

#else

#include "SFML/Window.hpp"

bool previousA = false;
bool previousB = false;
bool previousC = false;

ControlStatus Controls::getStatus(bool normalize_dir)
{
    m_stats.x = (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ? 1 : 0) - (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ? 1 : 0);
    m_stats.y = (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ? 1 : 0) - (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ? 1 : 0);

    if (normalize_dir && fabs(m_stats.x) > std::numeric_limits<float>::epsilon() && fabs(m_stats.y) > std::numeric_limits<float>::epsilon()) {
        m_stats.x *= 0.7071f;
        m_stats.y *= 0.7071f;
    }
    m_stats.pu = m_stats.y < 0 && !previousUp;
    previousUp = m_stats.y < 0;

    m_stats.pd = m_stats.y > 0 && !previousDown;
    previousDown = m_stats.y > 0;

    m_stats.pl = m_stats.x < 0 && !previousLeft;
    previousLeft = m_stats.x < 0;

    m_stats.pr = m_stats.x > 0 && !previousRight;
    previousRight = m_stats.x > 0;

    m_stats.a = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
    m_stats.pa = m_stats.a && !previousA;
    previousA = m_stats.a;

    m_stats.b = sf::Keyboard::isKeyPressed(sf::Keyboard::X);
    m_stats.pb = m_stats.b && !previousB;
    previousB = m_stats.b;

    m_stats.c = sf::Keyboard::isKeyPressed(sf::Keyboard::C);
    m_stats.pc = m_stats.c && !previousC;
    previousC = m_stats.c;

    return m_stats;
}

#endif


