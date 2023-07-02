#pragma once

#include <vector>
#include <cstddef>
#include "engine/core/renderer.h"
#include "engine/graphics/texture.h"
#include "Tower.h"
#include "Sprite.h"

namespace world
{
    struct Enemie
    {
        size_t id = 0;
        std::shared_ptr<Sprite> sprite;
        int currentHealth;
        size_t maxHealth;
        size_t nextWaypoint;
        float progress = 0.0f;
        size_t reward = 10;
    };

    struct Bullet
    {
        int id = 0;
        std::shared_ptr<graphics::Texture> image;
        utils::Vector2 startPos;
        utils::Vector2 pos;
        utils::Vector2 target;
        int damage;
        int lifeTime;
        float rotation;
    };

    inline bool operator==(const Enemie &e1, const Enemie &e2)
    {
        return e1.id == e2.id;
    }

    inline bool operator==(const Bullet &e1, const Bullet &e2)
    {
        return e1.id == e2.id;
    }

    struct Player
    {
        int points = 0;
        int cash = 20;
        int lives = 5;
    };

    static int MAX_WAVE_TIMER = 30000;

    class World
    {
    private:
        std::vector<std::vector<size_t>> tiles;
        std::shared_ptr<graphics::Texture> tileset;
        std::shared_ptr<graphics::Texture> towerBase;
        size_t width;
        size_t height;
        std::vector<graphics::Rect> towerPlaces;
        std::vector<Tower> towers;
        std::vector<utils::Vector2> waypoints;
        std::vector<Enemie> enemies;
        Player player;
        size_t lastId = 0;
        void renderTower(Tower &tower, core::Renderer *renderer);
        std::vector<std::shared_ptr<graphics::Texture>> canonTextures;
        std::vector<std::shared_ptr<graphics::Texture>> mgTextures;
        std::vector<std::shared_ptr<graphics::Texture>> laucherTextures;
        std::vector<Bullet> bullets;
        std::shared_ptr<graphics::Texture> cannonBullet;
        std::shared_ptr<graphics::Texture> mgBullet;
        std::shared_ptr<graphics::Texture> launcherBullet;
        int enemiesLeft = 20;
        int lastEnemieSpawn = 0;
        int currentWave = 1;
        int waveTimer = MAX_WAVE_TIMER;
        int levelId = 1;

    public:
        World(int levelId, std::vector<std::vector<size_t>> &tiles, size_t width, size_t height, std::vector<graphics::Rect> towerPlaces,
              std::vector<utils::Vector2> waypoints);
        World(int levelId, std::vector<std::vector<size_t>> &tiles, size_t width, size_t height, std::vector<graphics::Rect> towerPlaces,
              std::vector<utils::Vector2> waypoints, Player player);
        ~World();

        Player &getPlayer();
        void setPlayer(Player player);
        void render(core::Renderer *renderer);
        void update(float delta);
        bool canBuildTower(graphics::Rect &towerRect);
        void buildTower(const utils::Vector2 &position, TowerType towerType);
        void tryUpgradeBuilding(utils::Vector2 &mousePos);
        int getCurrentWave();
        int getWaveTimerInSeconds();
        int getLevelId();

        bool isWorldFinished();
    };

} // namespace world
