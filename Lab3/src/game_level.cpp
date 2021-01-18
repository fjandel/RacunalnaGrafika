#include "game_level.h"

#include <fstream>
#include <sstream>


GameLevel::GameLevel(const char* file, unsigned int width, unsigned int height)
    : levelFile(file), levelWidth(width), levelHeight(height)
{
    unsigned int tileCode;
    std::string line;
    std::ifstream fstream(file);
    if (fstream)
    {
        while (std::getline(fstream, line))
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> tileCode)
                row.push_back(tileCode);
            tileData.push_back(row);
        }

        init();
    }
}

void GameLevel::Draw(Renderer& renderer)
{
    for (Brick& tile : this->Bricks)
        if (!tile.Destroyed)
            tile.Draw(renderer);
}

bool GameLevel::IsCompleted()
{
    for (Brick& tile : this->Bricks)
        if (!tile.IsSolid && !tile.Destroyed)
            return false;
    return true;
}

void GameLevel::Reset()
{
    this->Bricks.clear();
    init();
}

void GameLevel::init()
{
    unsigned int height = tileData.size();
    unsigned int width = tileData[0].size();
    float unit_width = levelWidth / static_cast<float>(width);
    float unit_height = levelHeight / height;
    
    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            if (tileData[y][x] == 3)
            {
                glm::vec2 position(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                this->Bricks.push_back(Brick(position, size, glm::vec3(0.1f, 0.1f, 0.1f), 3, true));
            }
            else if (tileData[y][x] > 1)
            {
                glm::vec2 position(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                this->Bricks.push_back(Brick(position, size, glm::vec3(0.8f, 0.0f, 0.0f), 2, false, false));
            }
        }
    }
}