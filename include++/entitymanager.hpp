#pragma once

#include <vector>

namespace CastEngine
{

    class Entity;
    class EntityManager
    {

    private:

        std::vector<Entity> mEntities;

    public:

        EntityManager() {}

    }

};