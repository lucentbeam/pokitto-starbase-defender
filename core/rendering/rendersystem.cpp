#include "rendersystem.h"

#include "game/palettes.h"

#include <cstdio>

#ifndef POKITTO_SFML

#include "Pokitto.h"
#include "core/pokittolibextensions.h"

Pokitto::Core game;

const uint16_t sfx_3_jump[] = { 0x05,0x05,0x05,0x05,0x05,0x00 };

uint32_t RenderSystem::getTimeMs()
{
    return game.getTime();
}

void RenderSystem::initialize()
{
  game.begin();
  game.display.textWrap = false;
  game.display.load565Palette(palette);
  game.display.setInvisibleColor(0);
  game.display.setColor(3, 0);
}

bool RenderSystem::running()
{
  return game.isRunning();
}

bool RenderSystem::update()
{
  return game.update();
}

void RenderSystem::print(int x, int y, const char * line, int color, int size, int bgcol)
{
  game.display.fontSize = size;
  game.display.invisiblecolor = bgcol;
  game.display.setColor(color, bgcol);
  game.display.setCursor(x, y);
  game.display.write(line);
}

void RenderSystem::pixel(int x, int y, uint8_t pixel_idx)
{
  game.display.drawPixel(x, y, pixel_idx);
}

void RenderSystem::sprite(int x, int y, SpriteData * spr, int frame, bool flip) {
    sprite(x, y, spr->pixels, frame, flip);
}

void RenderSystem::sprite(int x, int y, const uint8_t * sprite, int frame, bool flip) {
    if (m_clipping && (m_clip_width == 0 || m_clip_height == 0)) {
        return;
    }
    if (sprite[0] == 255) {
        Pokitto::DisplayExtensions::drawTile(x, y, sprite[1], sprite[2], sprite + 3 + frame * sprite[1] * sprite[2] / 2);
    }
    else if (m_clipping) {
        game.display.invisiblecolor = sprite[0];
        Pokitto::DisplayExtensions::drawClippedBitmap(x, y, sprite[1], sprite[2], m_clip_width, m_clip_height, sprite + 3 + frame * sprite[1] * sprite[2] / 2);
    } else {
        game.display.invisiblecolor = sprite[0];
        Pokitto::DisplayExtensions::drawBitmap(x, y, sprite + 1, frame, flip);
    }
}

void RenderSystem::setClip(bool clip, uint8_t clip_width, uint8_t clip_height)
{
    m_clipping = clip;
    m_clip_width = clip_width;
    m_clip_height = clip_height;
}

void RenderSystem::drawLine(int x0, int y0, int x1, int y1, int color)
{
  game.display.setColor(color);
  game.display.drawLine(x0, y0, x1, y1);
}


void RenderSystem::drawRect(int x0, int y0, int w, int h, int color)
{
  game.display.setColor(color);
  game.display.fillRect(x0, y0, w, h);
}

void RenderSystem::clear(int idx) {
    game.display.bgcolor = idx;
//    m_game.display.clear();
}

void RenderSystem::debug() {
    auto val = Pokitto::Core::leftBtn();
    char tmp[50];
    sprintf(tmp, "%d", val);
    print(30,4,tmp, 10);

    val = Pokitto::Core::rightBtn();
    sprintf(tmp, "%d", val);
    print(40,4,tmp, 10);

    static bool wasB = false;
    if (!wasB && Pokitto::Core::bBtn()) {
 //       snd.playPattern((uint16_t*)sfx_3_jump, 0);
        print(50,4,"sound!!",10);
    }
    wasB = Pokitto::Core::bBtn();

    static float time = 0;
    float delta = getTimeMs() - time;
    time = getTimeMs();
    static float fps = 0.0f;
    fps = 0.9f * fps + (0.1f) * 1000.0f / delta;
    char fpses[10];
    std::sprintf(fpses, "%d", int(fps));
    print(4, 20, fpses, 10);
}

#else

#include <SFML/Graphics.hpp>

struct SfmlSystem {
    sf::RenderWindow * window;

    sf::Image screenbuffer;

    sf::Color colors[16] = {sf::Color(0, 0, 0)};

    int transparent = 0;

    int clearcolor = 0;

    sf::Font font;

    std::vector<sf::Text> texts;

    sf::Clock clock;

    void create() {
        window = new sf::RenderWindow(sf::VideoMode(220*4,176*4), "Pokitto Testing");
        window->setView(sf::View(sf::Vector2f(110,88),sf::Vector2f(220,176)));
        window->setFramerateLimit(60);
        for (int i = 0; i < 16; i++) {
            uint16_t color = palette[i];
            uint8_t r = (color & 0b1111100000000000) >> 11;
            uint8_t g = (color & 0b0000011111100000) >> 5;
            uint8_t b = (color & 0b0000000000011111);
            colors[i] = sf::Color(r << 3, g << 2, b << 3);
            font.loadFromFile("VT323-Regular.ttf");
        }
        screenbuffer.create(220, 176, colors[0]);
    }
} sfSys;

uint32_t RenderSystem::getTimeMs()
{
    return sfSys.clock.getElapsedTime().asMilliseconds();
}

void RenderSystem::initialize() {
    sfSys.create();
}

bool RenderSystem::running() {
    return sfSys.window->isOpen();
}

bool RenderSystem::update() {
    sf::Event event;
    while (sfSys.window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed) {
            sfSys.window->close();
        }
    }
    sf::Texture texture;
    texture.loadFromImage(sfSys.screenbuffer);
    sf::Sprite sprite;
    sprite.setTexture(texture, true);
    sfSys.window->draw(sprite);
    for (auto &txt : sfSys.texts) {
        sfSys.window->draw(txt);
    }
    sfSys.texts.clear();
    sfSys.screenbuffer.create(220, 176, sfSys.colors[sfSys.clearcolor]);
    sfSys.window->display();
    return true;
}

void RenderSystem::print(int x, int y, const char *line, int color, int size, int bgcol)
{
    sf::Text text;
    text.setString(line);
    text.setPosition(x,y-1);
    text.setFillColor(sfSys.colors[color]);
    text.setCharacterSize(80);
    text.setScale(0.285f * float(size), 0.08f * float(size));
    text.setFont(sfSys.font);
    sfSys.texts.push_back(text);
}

void RenderSystem::pixel(int x, int y, uint8_t pixel_idx)
{
    sfSys.screenbuffer.setPixel(x, y, sfSys.colors[pixel_idx]);
}

void RenderSystem::sprite(int x, int y, SpriteData *spr, int frame, bool flip)
{
    if (spr->pixels == nullptr) return;
    sprite(x, y, spr->pixels, frame, flip);
}

void RenderSystem::sprite(int x, int y, const uint8_t *sprite, int frame, bool flip)
{
    if (m_clipping && (m_clip_width == 0 || m_clip_height == 0)) return;
    sfSys.transparent = sprite[0];
    uint8_t w = sprite[1];
    uint8_t width = w + (w % 2);
    uint8_t h = sprite[2];
    const uint8_t * start = sprite + (3 + frame*int(float(w)/2.0f + 0.5f)*h);
    for (int i = 0; i < width * h; i++) {
        int dx = i % width;
        if (dx >= w) continue;
        if (m_clipping && dx > m_clip_width) continue;
        int dy = i / width;
        if (m_clipping && dy > m_clip_height) continue;
        int px = x + dx;
        int py = y + dy;
        if (flip) {
            px = x + (w - dx);
        }
        if (px < 0 || py < 0 || px >= 220 || py >= 176) {
            continue;
        }
        int idx;
        int val = start[i/2];
        if (i % 2  == 0) {
            idx = (val & 0xf0) >> 4;
        } else {
            idx = val & 0x0f;
        }
        if (sfSys.transparent != 255 && idx == sfSys.transparent) {
            continue;
        }
        sfSys.screenbuffer.setPixel(px, py, sfSys.colors[idx]);
    }
}

void RenderSystem::setClip(bool clip, uint8_t clip_width, uint8_t clip_height)
{
    m_clipping = clip;
    m_clip_width = clip_width;
    m_clip_height = clip_height;
}


void plotLow(float x0, float y0, float x1, float y1, const sf::Color &color) {
    float dx = x1 - x0;
    float dy = y1 - y0;
    float delta = abs(dy/dx);
    float error = 0;
    int y = y0;
    for (int x = x0; x < x1; x++) {
        sfSys.screenbuffer.setPixel(x, y, color);
        error += delta;
        if (error >= 0.5) {
            y = y + copysign(1.0, dy);
            error--;
        }
    }
}

void plotHigh(float x0, float y0, float x1, float y1, const sf::Color &color) {
    float dx = x1 - x0;
    float dy = y1 - y0;

    float delta = abs(dx/dy);
    float error = 0;
    int x = x0;
    for (int y = y0; y < y1; y++) {
        sfSys.screenbuffer.setPixel(x, y, color);
        error += delta;
        if (error >= 0.5) {
            x = x + copysign(1.0, dx);
            error--;
        }
    }
}

void plotLine( float x0, float y0, float x1, float y1, const sf::Color &color) { // bresenham's algorithm
    if ((abs(y1-y0) < abs(x1-x0))) {
        if (x0 > x1) {
            plotLow(x1,y1,x0,y0,color);
        } else {
            plotLow(x0,y0,x1,y1,color);
        }
    } else {
        if (y0 > y1) {
            plotHigh(x1,y1,x0,y0,color);
        } else {
            plotHigh(x0,y0,x1,y1,color);
        }
    }
}

void RenderSystem::drawLine(int x0, int y0, int x1, int y1, int color)
{
    plotLine(x0,y0,x1,y1,sfSys.colors[color]);
}

void RenderSystem::drawRect(int x0, int y0, int w, int h, int color)
{
    sf::Color c = sfSys.colors[color];
    for (int i = x0; i < (x0 + w); i++) {
        for (int j = y0; j < (y0 + h); j++) {
            sfSys.screenbuffer.setPixel(i,j,c);
        }
    }
}

void RenderSystem::clear(int idx) {
    sfSys.clearcolor = idx;
}

void RenderSystem::debug()
{
    static float time = 0;
    float delta = getTimeMs() - time;
    time = getTimeMs();
    static float fps = 0.0f;
    fps = 0.9f * fps + (0.1f) * 1000.0f / delta;
    char fpstxt[10];
    std::sprintf(fpstxt, "%d", int(fps));
    print(4, 20, fpstxt, 10);
}

#endif
