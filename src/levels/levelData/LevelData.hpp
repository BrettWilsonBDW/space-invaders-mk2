#pragma once

#include <vector>
#include "../../entities/Entities.hpp"

class LevelData
{
public:
    struct Data
    {
        float width{};
        float height{};
        float gap{};
        int enemyCount{};
        int enemySpeed{};
        std::vector<Vector2> trajectory{};
    };

    Data GetLevelData(int i)
    {
        return m_levels[i];
    }

    std::vector<Data> GetAllLevels() { return m_levels; }

    LevelData();
    ~LevelData();

private:
    std::vector<Data> m_levels;

    void PopulateLevels();
};
