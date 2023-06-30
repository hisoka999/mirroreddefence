#pragma once
#include "engine/utils/vector2.h"
namespace world
{
    enum class TowerType
    {
        Canon = 5,
        MG = 20,
        Missle_Launcher = 100
    };

    class Tower
    {
    private:
        int level = 1;
        TowerType type;
        utils::Vector2 position;

    public:
        Tower(TowerType type,
              utils::Vector2 position);
        ~Tower();

        void upgrade();

        int getLevel();
        TowerType getType();
        const utils::Vector2 &getPosition();
    };

} // namespace world
