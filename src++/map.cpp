#include "map.hpp"

#include <fstream>

#include "vec2d.hpp"
#include "logger.hpp"

#include <iostream>

namespace CastEngine
{
    bool Map::FillMapData(std::fstream &file)
    {
        int y = 0;
        std::string currentLine = "";

        if(!mWidth || !mHeight)
        {
            SetErrorf("invalid width or height (%ix%i)", mWidth, mHeight);
            return false;
        }

        while(std::getline(file, currentLine))
        {
            char* sanitised = strtok(currentLine.data(), " \n\r\t");
            if(!sanitised)
            {
                LogMsg(ERROR, "sanitised has nothing");
                break;
            }

            if((int)strlen(sanitised) != mWidth)
            {
                SetErrorf("Map width doesnt match dimensions set in map file %s\n", mFilePath);
                return false;
            }

            for(int x = 0; x < mWidth; x++)
            {
                if(sanitised[x] < '0' || sanitised[x] > '9')
                {
                    SetErrorf("Invalid character '%c' in map data at row %d column %d in map file %s\n", currentLine[x], y, x, mFilePath);
                    return false;
                }
                mMapData[y * mWidth + x] = sanitised[x] - '0';
            }

            y++;
        }

        if(y != mHeight)
        {
            SetErrorf("Rows dont match in map at '%s'\n", mFilePath);
            return false;
        }

        return true;
    }

    const int& Map::operator[] (int i) const
    {
        return mMapData[i];
    }

    bool Map::LoadMap(const std::string& pFilePath, LoadArgs& args)
    {
        std::fstream file(pFilePath);

        if(!file.is_open())
        {
            LogMsgf(ERROR, "failed to open file at path (%s)", pFilePath.c_str());
            return false;
        }

        mFilePath = pFilePath;

        args.success = false;

        int lineNum = 0;
        std::string currentLine = "";
        while(std::getline(file, currentLine))
        {
            lineNum++;
            if(currentLine[0] == '#' || currentLine[0] == '\n' || currentLine[0] == '\r' || currentLine.length() == 0)
                continue;
            char* token = strtok(currentLine.data(), " \t\n\r");

            if(!token)
                continue;

            if(strcmp(token, "mapdim") == 0)
            {
                // check map dimensions are correct
                mWidth = atoi(strtok(NULL, " \t\n\r"));
                mHeight = atoi(strtok(NULL, " \t\n\r"));
                if(mWidth <= 0 || mHeight <= 0)
                {
                    SetErrorf("Invalid map dimensions in map file '%s'", pFilePath.c_str());
                    Destroy();
                    return false;
                }

                mMapData.resize(mWidth * mHeight);
                mMapData.clear();

                // debug info
                LogMsgf(DEBUG, "Map dimensions: %d x %d\n", mWidth, mHeight);
                continue;
            }

            // read map data
            if(strcmp(token, "mapstart") == 0)
            {
                // the function sets an error on fail so no need to set the error here
                if(!FillMapData(file))
                {
                    return false;
                }
                // adjusts for lines skipped by M_FillMapData
                lineNum += mHeight;
                continue;
            }

            /* PLAYER DATA */

            // player start var
            if(strcmp(token, "playerstart") == 0)
            {
                float x = atof(strtok(NULL, " \t\n\r")) , y = atof(strtok(NULL, " \t\n\r"));
                if(x == 0 || y == 0)
                {
                    SetError("failed to load playerstart co-ords\n");
                    return false;
                }

                args.startPos = vec2d(x, y);
                continue;
            }

            // player max speed var
            if(strcmp(token, "playermaxspeed") == 0)
            {
                float maxSpeed = atof(strtok(NULL, " \t\n\r"));
                if(maxSpeed == 0.0f)
                {
                    SetError("failed to load maxSpeed for player");
                    return false;
                }

                args.maxSpeed = maxSpeed;
                continue;
            }

            // player rotate speed var
            if(strcmp(token, "playerrotatespeed") == 0)
            {
                float rotateSpeed = atof(strtok(NULL, " \t\n\r"));
                if(rotateSpeed == 0.0f)
                    LogMsg(INFO, "playerrotatespeed not set, defaulting to 0.1...\n");

                args.rotateSpeed = rotateSpeed;
                continue;
            }

            /* FILE END TAG*/
            if(strcmp(token, "fileend") == 0)
            {
                LogMsgf(DEBUG, "Reached end of map data in map file %s\n", pFilePath.c_str());
                break;
            }

            // if code execution reached here, the token is unrecognised
            SetErrorf("Unkown token ('%s') on line %i of map file '%s'", token, lineNum, pFilePath.c_str());
            return false;
        }

        file.close();

        args.success = true;
        return true;
    }
    

    Map::Map()
    {

    }

    void Map::Destroy()
    {
        mMapData.clear();
        mMapData.resize(0);

        mWidth = 0;
        mHeight = 0;

        mFilePath = "";
    }

    void Map::print()
    {
        for(int y = 0; y < mHeight; y++)
        {
            for (int x = 0; x < mWidth; x++)
            {
                std::cout << mMapData[y * mWidth + x];
            }
            std::cout << '\n';
        }
    }
};