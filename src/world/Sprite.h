#pragma once

#include <engine/graphics/texture.h>
#include <engine/graphics/rect.h>
#include <engine/utils/vector2.h>

enum class SpriteDirection
{
    TOP = 0,
    LEFT = 1,
    DOWN = 2,
    RIGHT = 3
};
class Sprite
{
private:
    std::shared_ptr<graphics::Texture> m_texture;
    graphics::Rect m_rect;
    graphics::Rect m_srcRect;
    uint32_t m_lastUpdate;
    size_t m_rows;
    size_t m_cols;
    size_t spriteNum;
    SpriteDirection m_direction = SpriteDirection::LEFT;
    bool m_isMoving = false;

public:
    Sprite(const std::string &fileName, size_t rows, size_t cols);
    ~Sprite();
    void setPosition(float x, float y);
    void setPosition(const utils::Vector2 &pos);
    utils::Vector2 getPosition();
    void setDirection(SpriteDirection dir);
    void startAnimation();
    void stopAnimation();
    bool isMoving();

    void render(core::Renderer *pRender);
    void update(uint32_t deltaTime);
    graphics::Rect &getRect();
    graphics::Rect getCollisionRect();
};