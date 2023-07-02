#include "WorldLoader.h"
#include "engine/utils/json/parser.h"
#include <fstream>
#include "engine/utils/exceptions.h"
#include "engine/utils/base64.h"
#include <vector>
#include "engine/utils/string.h"

namespace world
{
    WorldLoader::WorldLoader(/* args */)
    {
    }

    std::unique_ptr<world::World> WorldLoader::loadWorld(const std::string &path, int levelId)
    {
        utils::JSON::Parser parser;

        std::ifstream file;

        std::string s;
        std::string group;

        file.open(path, std::ios::in);
        if (!file.is_open())
        {
            throw IOException(path, "file does not exists");
        }
        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        std::string buffer(size, ' ');
        file.seekg(0);
        file.read(&buffer[0], size);

        auto object = parser.parseObject(buffer);
        auto layers = object->getArray("layers");
        int width = object->getIntValue("width");
        int height = object->getIntValue("height");
        std::vector<std::vector<size_t>> tiles;
        std::vector<graphics::Rect> towerPlaces;
        std::vector<utils::Vector2> waypoints;

        for (auto &layer : layers)
        {

            auto layerObj = std::get<std::shared_ptr<utils::JSON::Object>>(layer);
            auto layerType = layerObj->getStringValue("type");
            auto layerName = layerObj->getStringValue("name");
            std::vector<size_t> tileLayer;
            if (layerObj->hasArray("data"))
            {
                auto data = layerObj->getArray("data");

                for (size_t i = 0; i < size_t(width * height); ++i)
                {
                    tileLayer.push_back(std::get<int>(data[i]));
                }

                tiles.push_back(tileLayer);
            }
            else if (layerType == "objectgroup" && layerName == "Tower Positions")
            {
                auto objects = layerObj->getArray("objects");
                for (auto &obj : objects)
                {
                    auto ob = std::get<std::shared_ptr<utils::JSON::Object>>(obj);
                    towerPlaces.push_back({float(ob->getFloatValue("x")), float(ob->getFloatValue("y")), float(ob->getFloatValue("width")), float(ob->getFloatValue("height"))});
                }
            }
            else if (layerType == "objectgroup" && layerName == "Waypoints")
            {
                auto objects = layerObj->getArray("objects");

                auto waypoint = std::get<std::shared_ptr<utils::JSON::Object>>(objects[0]);
                float baseX = waypoint->getFloatValue("x");
                float baseY = waypoint->getFloatValue("y");
                for (auto &pos : waypoint->getArray("polyline"))
                {
                    auto position = std::get<std::shared_ptr<utils::JSON::Object>>(pos);

                    waypoints.push_back({position->getFloatValue("x") + baseX, position->getFloatValue("y") + baseY});
                }
            }
        }
        return std::make_unique<World>(levelId, tiles, width, height, towerPlaces, waypoints);
    }

    WorldLoader::~WorldLoader()
    {
    }
} // namespace world
