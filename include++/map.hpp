#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "vec2d.hpp"

namespace CastEngine
{

    class Map
    {

    private:
        
        // flat 2d array
        std::vector<int> mMapData;

        int mWidth;
        int mHeight;

        // stores file path to map on successful load
        std::string mFilePath;

        bool FillMapData(std::fstream& file);

    public:

        /// @brief stores extra information from the map
        struct LoadArgs
        {
            // start position of player in the map
            vec2d startPos;

            float maxSpeed;
            float rotateSpeed;

            // true if map loaded successfully
            bool success;

        };

        int& operator[] (int i);

        bool LoadMap(const std::string& pFilePath, LoadArgs& args);

        Map();

        void Destroy();

        void print();

    };

};