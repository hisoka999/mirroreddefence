#include "World.h"
#include "engine/graphics/TextureManager.h"
namespace world
{
    World::World(std::vector<size_t> &tiles, size_t width, size_t height, std::vector<graphics::Rect> towerPlaces)
        : tiles(tiles), width(width), height(height), towerPlaces(towerPlaces)
    {

        tileset = graphics::TextureManager::Instance().loadTexture("images/tileset.png");
        towerBase = graphics::TextureManager::Instance().loadTexture("images/Tower.png");
    }
    World::~World()
    {
    }

    void World::render(core::Renderer *renderer)
    {
        int tileset_width = tileset->getWidth() / 32;
        for (size_t y = 0; y < height; ++y)
        {
            for (size_t x = 0; x < width; ++x)
            {
                size_t tile = tiles[x + (y * width)] - 1;
                float tile_x = ((tile - int((tile / tileset_width) * tileset_width))) * 32;
                float tile_y = int(tile / tileset_width) * 32;
                graphics::Rect src{tile_x, tile_y,
                                   32.f, 32.f};
                graphics::Rect dest{float(x * 32), float(y * 32), 32.f, 32.f};
                tileset->render(renderer, src, dest);
            }
        }

        for (auto &tower : towers)
        {
            towerBase->renderResized(renderer, tower.getPosition().getX(), tower.getPosition().getY(), 64, 64);
        }
    }

    bool World::canBuildTower(graphics::Rect &towerRect)
    {
        graphics::Rect tempRect{0, 0, 64, 64};
        for (auto &tower : towers)
        {
            tempRect.x = tower.getPosition().getX();
            tempRect.y = tower.getPosition().getY();
            if (towerRect.intersects(tempRect))
                return false;
        }

        for (auto &r : towerPlaces)
        {
            if (r.intersects(towerRect))
                return true;
        }
        return false;
    }

    void World::buildTower(const utils::Vector2 &position, TowerType towerType)
    {
        towers.push_back({towerType, position});
    }
} // namespace world
