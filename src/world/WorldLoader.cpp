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
        loadWorld("maps/test.json.tmj");
    }

    std::unique_ptr<world::World> WorldLoader::loadWorld(const std::string &path)
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
        std::vector<size_t> tiles;
        std::vector<graphics::Rect> towerPlaces;
        for (auto &layer : layers)
        {

            auto layerObj = std::get<std::shared_ptr<utils::JSON::Object>>(layer);
            auto layerType = layerObj->getStringValue("type");

            if (layerObj->hasArray("data"))
            {
                auto data = layerObj->getArray("data");

                for (size_t i = 0; i < size_t(width * height); ++i)
                {
                    tiles.push_back(std::get<int>(data[i]));
                }
            }
            else if (layerType == "objectgroup")
            {
                auto objects = layerObj->getArray("objects");
                for (auto &obj : objects)
                {
                    auto ob = std::get<std::shared_ptr<utils::JSON::Object>>(obj);
                    towerPlaces.push_back({float(ob->getIntValue("x")), float(ob->getIntValue("y")), float(ob->getIntValue("width")), float(ob->getIntValue("height"))});
                }
            }
        }
        return std::make_unique<World>(tiles, width, height, towerPlaces);
    }

    WorldLoader::~WorldLoader()
    {
    }
} // namespace world
