#include "Tower.h"
#include "engine/utils/logger.h"
namespace world
{
    Tower::Tower(TowerType type,
                 utils::Vector2 position) : type(type), position(position)
    {
    }
    Tower::~Tower()
    {
    }
    bool Tower::upgrade()
    {
        if (level == 2)
            return false;
        level++;
        APP_LOG_TRACE("upgraded tower");
        return true;
    }

    int Tower::getLevel()
    {
        return level;
    }

    utils::Vector2 &Tower::getPosition()
    {
        return position;
    }

    TowerType Tower::getType()
    {
        return type;
    }

    int Tower::getTargetDistance()
    {
        switch (type)
        {
        case TowerType::Canon:
            return 120 + (5 * level);
        case TowerType::MG:
            return 200 + (10 * level);
        case TowerType::Missle_Launcher:
            return 250 + (20 * level);
        default:
            break;
        }
        return 0;
    }

    float Tower::getRotation()
    {
        return rotation;
    }
    void Tower::setRotation(float r)
    {
        rotation = r;
    }

    size_t Tower::getBulletSpawnTime()
    {
        switch (type)
        {
        case TowerType::Canon:
            return 400 * 3 / (level + 1);
        case TowerType::MG:

            return 200 * 3 / (level + 1);
        case TowerType::Missle_Launcher:
            return 1000 * 3 / (level + 1);
        default:
            break;
        }
        return 0;
    }
    size_t Tower::getRemainingBulletTime()
    {
        return remainingBulletTime;
    }
    void Tower::setRemainingBulletTime(size_t time)
    {
        remainingBulletTime = time;
    }

    size_t Tower::getBulletDamage()
    {
        switch (type)
        {
        case TowerType::Canon:
            return 10;
        case TowerType::MG:

            return 5;
        case TowerType::Missle_Launcher:
            return 40;
        default:
            break;
        }
        return 0;
    }
} // namespace world
