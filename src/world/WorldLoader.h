#pragma once
#include "World.h"

namespace world
{
    class WorldLoader
    {
    private:
        /* data */
    public:
        std::unique_ptr<world::World> loadWorld(const std::string &path);

        WorldLoader(/* args */);
        ~WorldLoader();
    };

} // namespace world
