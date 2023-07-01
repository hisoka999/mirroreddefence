#pragma once
#include "engine/utils/vector2.h"
#include <cstddef>

namespace world
{
    enum class TowerType
    {
        Canon = 10,
        MG = 40,
        Missle_Launcher = 200
    };

    class Tower
    {
    private:
        int level = 0;
        TowerType type;
        utils::Vector2 position;
        float rotation = 90;
        size_t remainingBulletTime = 0;

    public:
        Tower(TowerType type,
              utils::Vector2 position);
        ~Tower();

        bool upgrade();

        int getLevel();
        TowerType getType();
        utils::Vector2 &getPosition();
        int getTargetDistance();
        float getRotation();
        void setRotation(float r);
        void setRemainingBulletTime(size_t time);
        size_t getRemainingBulletTime();
        size_t getBulletSpawnTime();
        size_t getBulletDamage();
        };

} // namespace world
