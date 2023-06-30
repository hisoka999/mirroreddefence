#pragma once

#include <vector>
#include <cstddef>
#include "engine/core/renderer.h"
#include "engine/graphics/texture.h"
#include "Tower.h"

namespace world
{
    class World
    {
    private:
        std::vector<size_t> tiles;
        std::shared_ptr<graphics::Texture> tileset;
        std::shared_ptr<graphics::Texture> towerBase;
        size_t width;
        size_t height;
        std::vector<graphics::Rect> towerPlaces;
        std::vector<Tower> towers;

    public:
        World(std::vector<size_t> &tiles, size_t width, size_t height, std::vector<graphics::Rect> towerPlaces);
        ~World();

        void render(core::Renderer *renderer);
        bool canBuildTower(graphics::Rect &towerRect);
        void buildTower(const utils::Vector2 &position, TowerType towerType);
    };

} // namespace world
