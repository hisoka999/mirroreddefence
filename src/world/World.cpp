#include "World.h"
#include "engine/graphics/TextureManager.h"
#include <algorithm>
#include <cassert>
#include <random>

static const float PI = 3.1415926535;
namespace world
{

    void renderRotated(core::Renderer *ren, graphics::Texture *tex, const double angle,
                       const int x, const int y, const int pWidth, const int pHeight)
    {
        SDL_FPoint *center = nullptr;

        SDL_FRect dst;
        dst.x = float(x);
        dst.y = float(y);
        dst.w = float(pWidth);
        dst.h = float(pHeight);

        SDL_Rect src;
        src.x = 0;
        src.y = 0;
        src.w = tex->getWidth();
        src.h = tex->getHeight();

        int result = SDL_RenderCopyExF(ren->getRenderer(), tex->getSDLTexture(), &src, &dst, angle, center,
                                       SDL_FLIP_NONE);
        if (result != 0)
            throw SDLException("renderRotated::SDL_RenderCopyEx");
    }

    World::World(int levelId, std::vector<std::vector<size_t>> &tiles, size_t width, size_t height, std::vector<graphics::Rect> towerPlaces,
                 std::vector<utils::Vector2> waypoints, Player player)

        : tiles(tiles), width(width), height(height), towerPlaces(towerPlaces), waypoints(waypoints), player(player), levelId(levelId)
    {

        tileset = graphics::TextureManager::Instance().loadTexture("images/tileset.png");
        towerBase = graphics::TextureManager::Instance().loadTexture("images/Tower.png");
        canonTextures.push_back(graphics::TextureManager::Instance().loadTexture("images/Cannon.png"));
        canonTextures.push_back(graphics::TextureManager::Instance().loadTexture("images/Cannon2.png"));
        canonTextures.push_back(graphics::TextureManager::Instance().loadTexture("images/Cannon3.png"));

        mgTextures.push_back(graphics::TextureManager::Instance().loadTexture("images/MG.png"));
        mgTextures.push_back(graphics::TextureManager::Instance().loadTexture("images/MG2.png"));
        mgTextures.push_back(graphics::TextureManager::Instance().loadTexture("images/MG3.png"));

        laucherTextures.push_back(graphics::TextureManager::Instance().loadTexture("images/Missile_Launcher.png"));
        laucherTextures.push_back(graphics::TextureManager::Instance().loadTexture("images/Missile_Launcher2.png"));
        laucherTextures.push_back(graphics::TextureManager::Instance().loadTexture("images/Missile_Launcher3.png"));

        cannonBullet = graphics::TextureManager::Instance().loadTexture("images/Bullet_Cannon.png");

        mgBullet = graphics::TextureManager::Instance().loadTexture("images/Bullet_MG.png");

        launcherBullet = graphics::TextureManager::Instance().loadTexture("images/Missile.png");
    }

    World::World(int levelId, std::vector<std::vector<size_t>> &tiles, size_t width, size_t height, std::vector<graphics::Rect> towerPlaces,
                 std::vector<utils::Vector2> waypoints)
        : tiles(tiles), width(width), height(height), towerPlaces(towerPlaces), waypoints(waypoints), levelId(levelId)
    {

        tileset = graphics::TextureManager::Instance().loadTexture("images/tileset.png");
        towerBase = graphics::TextureManager::Instance().loadTexture("images/Tower.png");
        canonTextures.push_back(graphics::TextureManager::Instance().loadTexture("images/Cannon.png"));
        canonTextures.push_back(graphics::TextureManager::Instance().loadTexture("images/Cannon2.png"));
        canonTextures.push_back(graphics::TextureManager::Instance().loadTexture("images/Cannon3.png"));

        mgTextures.push_back(graphics::TextureManager::Instance().loadTexture("images/MG.png"));
        mgTextures.push_back(graphics::TextureManager::Instance().loadTexture("images/MG2.png"));
        mgTextures.push_back(graphics::TextureManager::Instance().loadTexture("images/MG3.png"));

        laucherTextures.push_back(graphics::TextureManager::Instance().loadTexture("images/Missile_Launcher.png"));
        laucherTextures.push_back(graphics::TextureManager::Instance().loadTexture("images/Missile_Launcher2.png"));
        laucherTextures.push_back(graphics::TextureManager::Instance().loadTexture("images/Missile_Launcher3.png"));

        cannonBullet = graphics::TextureManager::Instance().loadTexture("images/Bullet_Cannon.png");

        mgBullet = graphics::TextureManager::Instance().loadTexture("images/Bullet_MG.png");

        launcherBullet = graphics::TextureManager::Instance().loadTexture("images/Missile.png");
    }
    World::~World()
    {
    }

    void World::render(core::Renderer *renderer)
    {
        int tileset_width = tileset->getWidth() / 32;
        for (size_t layer = 0; layer < tiles.size(); layer++)
            for (size_t y = 0; y < height; ++y)
            {
                for (size_t x = 0; x < width; ++x)
                {
                    size_t tile = tiles[layer][x + (y * width)] - 1;
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
            renderTower(tower, renderer);
        }

        for (auto &enemy : enemies)
        {
            enemy.sprite->render(renderer);
        }

        for (auto &b : bullets)
        {
            b.image->renderRotated(renderer, b.rotation, b.pos.getX(), b.pos.getY(), 32, 32);
            // renderer->drawLine(b.pos, b.target);
        }
    }

    void World::renderTower(Tower &tower, core::Renderer *renderer)
    {
        towerBase->renderResized(renderer, tower.getPosition().getX(), tower.getPosition().getY(), 64, 64);

        switch (tower.getType())
        {
        case TowerType::Canon:
            renderRotated(renderer, canonTextures[tower.getLevel()].get(), tower.getRotation(), tower.getPosition().getX(), tower.getPosition().getY(), 64, 64);
            break;
        case TowerType::MG:
            renderRotated(renderer, mgTextures[tower.getLevel()].get(), tower.getRotation(), tower.getPosition().getX(), tower.getPosition().getY(), 64, 64);
            break;
        case TowerType::Missle_Launcher:
            renderRotated(renderer, laucherTextures[tower.getLevel()].get(), tower.getRotation(), tower.getPosition().getX(), tower.getPosition().getY(), 64, 64);
            break;
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

    double GetAngleBetweenPoints(utils::Vector2 endPt1, utils::Vector2 connectingPt, utils::Vector2 endPt2)
    {
        double x1 = endPt1.getX() - connectingPt.getX(); // Vector 1 - x
        double y1 = endPt1.getY() - connectingPt.getY(); // Vector 1 - y

        double x2 = endPt2.getX() - connectingPt.getX(); // Vector 2 - x
        double y2 = endPt2.getY() - connectingPt.getY(); // Vector 2 - y

        double angle = atan2(y1, x1) - atan2(y2, x2);
        angle = angle * 360 / (2 * PI);

        if (angle < 0)
        {
            angle += 360;
        }

        return angle;
    }

    float dot(utils::Vector2 a, utils::Vector2 b) // calculates dot product of a and b
    {
        return (a.getX() * b.getX()) + (a.getY() * b.getY());
    }

    float mag(utils::Vector2 a) // calculates magnitude of a
    {
        return std::sqrt((a.getX() * a.getX()) + (a.getY() * a.getY()));
    }

    void World::update(float delta)
    {
        if (lastEnemieSpawn <= 0 && enemiesLeft > 0)
        {
            unsigned long seed = std::chrono::system_clock::now().time_since_epoch().count();

            std::mt19937 gen(seed);

            lastId++;
            std::uniform_int_distribution<int> enemyDistribution(0, 100);
            int distibution = enemyDistribution(gen);
            Enemie e{.id = lastId, .sprite = nullptr, .currentHealth = 20, .maxHealth = 20, .nextWaypoint = 1};

            if (levelId > 1 && distibution < 10)
            {
                e.maxHealth = 80;
                e.currentHealth = 80;
                e.reward = 100;
                e.sprite = std::make_shared<Sprite>("images/BODY_skeleton_chain_armor.png", 4, 9);
            }
            else if (distibution < (10 * levelId))
            {
                e.maxHealth = 40;
                e.currentHealth = 40;
                e.reward = 30;
                e.sprite = std::make_shared<Sprite>("images/BODY_skeleton_leather_armor.png", 4, 9);
            }
            else
            {
                e.sprite = std::make_shared<Sprite>("images/BODY_skeleton.png", 4, 9);
            }
            e.sprite->setPosition(waypoints[0] - utils::Vector2{0.f, e.sprite->getRect().height * 0.5f});
            e.sprite->startAnimation();
            enemies.push_back(e);
            lastEnemieSpawn = 1000;
            enemiesLeft--;
        }
        lastEnemieSpawn -= delta;
        std::vector<Enemie> destroyedEnemies;
        for (auto &enemy : enemies)
        {
            enemy.sprite->update(delta);
            enemy.progress += 0.1f * delta / 400.f;

            auto spriteOffset = utils::Vector2{enemy.sprite->getRect().width * 0.5f, enemy.sprite->getRect().height};
            auto startPosition = waypoints[enemy.nextWaypoint - 1] - spriteOffset;

            auto targetPosition = waypoints[enemy.nextWaypoint] - spriteOffset;
            enemy.sprite->setPosition(utils::lerp(startPosition, targetPosition, enemy.progress));
            // float x = targetPosition.getX() - enemy.sprite->getPosition().getX();
            // float y = targetPosition.getY() - enemy.sprite->getPosition().getY();
            SpriteDirection direction = SpriteDirection::TOP;

            if (targetPosition.getY() > startPosition.getY())
            {
                direction = SpriteDirection::DOWN;
            }
            else if (targetPosition.getY() < startPosition.getY())
            {
                direction = SpriteDirection::TOP;
            }
            else if (targetPosition.getX() > startPosition.getX())
            {
                direction = SpriteDirection::RIGHT;
            }
            else
            {
                direction = SpriteDirection::LEFT;
            }
            enemy.sprite->setDirection(direction);
            if (enemy.progress >= 1)
            {
                enemy.nextWaypoint++;
                enemy.progress = 0.0;
            }

            if (enemy.nextWaypoint == waypoints.size())
            {
                player.points--;
                player.lives--;
                if (player.lives < 0)
                    player.lives = 0;
                destroyedEnemies.push_back(enemy);
            }
            if (enemy.currentHealth <= 0)
            {
                destroyedEnemies.push_back(enemy);
            }
        }
        std::vector<Bullet> bulletToDelete;
        for (auto &bullet : bullets)
        {
            bullet.lifeTime -= delta;
            if (bullet.lifeTime <= 0)
                bulletToDelete.push_back(bullet);

            for (auto &enemy : enemies)
            {
                if (enemy.sprite->getCollisionRect().intersects(bullet.pos))
                {
                    enemy.currentHealth -= bullet.damage;
                    bulletToDelete.push_back(bullet);
                    if (enemy.currentHealth <= 0)
                        player.cash += enemy.reward;
                    player.points++;
                    break;
                }
            }

            float currentDistance = bullet.pos.distance(bullet.startPos);
            float fullDistance = bullet.target.distance(bullet.startPos);
            float percentage = std::abs(currentDistance / fullDistance);
            percentage += (0.1f * delta / 100.f);
            if (percentage > 1.f)
            {
                percentage = 1.0f;
            }
            bullet.pos = utils::lerp(bullet.startPos, bullet.target, percentage);
        }

        for (auto &e : destroyedEnemies)
        {
            auto it = std::find(enemies.begin(), enemies.end(), e);
            enemies.erase(it);
        }

        for (auto &tower : towers)
        {
            tower.setRemainingBulletTime(tower.getRemainingBulletTime() - delta);
            Enemie target;
            auto towerPos = tower.getPosition();
            for (auto &enemy : enemies)
            {
                float distance = std::abs(towerPos.distance(enemy.sprite->getPosition()));
                if (distance <= tower.getTargetDistance())
                {
                    target = enemy;
                    break;
                }
            }
            if (target.id == 0)
                continue;
            auto towerCenter = tower.getPosition() + utils::Vector2{32, 32};
            auto enemyPosCenter = target.sprite->getPosition() + utils::Vector2{target.sprite->getRect().width / 2.f, target.sprite->getRect().height / 2.f};

            // α = arccos[(xa xb + ya yb) / (√(xa² + ya²) × √(xb² + yb²))]
            utils::Vector2 vecMin(towerCenter.getX(), towerCenter.getY() - PI);

            float b = enemyPosCenter.distance(towerCenter);
            float a = PI;
            float c = vecMin.distance(enemyPosCenter);

            float angle = std::acos((std::pow(a, 2.f) + std::pow(b, 2.f) - std::pow(c, 2.f)) / (2 * a * b)) * 180.f / PI;
            if (enemyPosCenter.getX() < towerCenter.getX())
            {
                angle = 360.f - angle;
            }

            tower.setRotation(angle);
            if (tower.getRemainingBulletTime() == 0)
            {
                Bullet bullet;
                bullet.damage = tower.getBulletDamage();
                bullet.lifeTime = 1000;
                bullet.startPos = towerCenter;
                bullet.pos = towerCenter;
                bullet.target = enemyPosCenter;
                bullet.id = lastId++;
                switch (tower.getType())
                {
                case TowerType::Canon:
                    bullet.image = cannonBullet;
                    break;
                case TowerType::MG:
                    bullet.image = mgBullet;
                    break;
                case TowerType::Missle_Launcher:
                    bullet.image = launcherBullet;
                    break;
                default:
                    break;
                }
                assert(bullet.image != nullptr);
                bullet.rotation = angle;

                bullets.push_back(bullet);
                tower.setRemainingBulletTime(tower.getBulletSpawnTime());
            }
        }

        for (auto &e : bulletToDelete)
        {
            auto it = std::find(bullets.begin(), bullets.end(), e);
            bullets.erase(it);
        }

        // wave timer

        waveTimer -= delta;

        if (waveTimer <= 0 && currentWave < 3)
        {
            waveTimer = MAX_WAVE_TIMER;
            currentWave++;
            enemiesLeft += 20;
        }
        else if (waveTimer < 0)
        {
            waveTimer = 0;
        }
    }

    Player &World::getPlayer()
    {
        return player;
    }

    void World::setPlayer(Player player)
    {
        this->player = player;
    }

    void World::tryUpgradeBuilding(utils::Vector2 &mousePos)
    {
        graphics::Rect towerRect{0, 0, 64, 64};
        for (auto &tower : towers)
        {
            towerRect.x = tower.getPosition().getX();
            towerRect.y = tower.getPosition().getY();
            if (towerRect.intersects(mousePos))
            {
                int neededCash = int(tower.getType()) * (tower.getLevel() + 1) * 10;
                if (player.cash > neededCash && tower.upgrade())
                {
                    player.cash -= neededCash;
                }

                return;
            }
        }
    }

    int World::getCurrentWave()
    {
        return currentWave;
    }

    int World::getWaveTimerInSeconds()
    {
        return waveTimer / 1000;
    }
    int World::getLevelId()
    {
        return levelId;
    }
    bool World::isWorldFinished()
    {
        return (currentWave == 3 && enemies.empty() && waveTimer < MAX_WAVE_TIMER) || player.lives == 0;
    }
} // namespace world
