#include "Tower.h"

namespace world
{
    Tower::Tower(TowerType type,
                 utils::Vector2 position) : type(type), position(position)
    {
    }
    Tower::~Tower()
    {
    }
    void Tower::upgrade()
    {
        if (level == 3)
            return;
        level++;
    }

    int Tower::getLevel()
    {
        return level;
    }

    const utils::Vector2 &Tower::getPosition()
    {
        return position;
    }

    TowerType Tower::getType()
    {
        return type;
    }
} // namespace world
