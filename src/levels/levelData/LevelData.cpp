#include "LevelData.hpp"

#include <iostream>

LevelData::LevelData()
{
    PopulateLevels();
}

LevelData::~LevelData()
{
}

void LevelData::PopulateLevels()
{
    // test level data for now
    Data data;
    data.width = 50;
    data.height = 50;
    data.gap = 20;
    data.enemyCount = 5;
    data.enemySpeed = 300;
    data.trajectory = {{100, 0}, {800, 0}};

    m_levels.push_back({data.width, data.height, data.gap, data.enemyCount, data.enemySpeed, data.trajectory});

    data.width = 50;
    data.height = 50;
    data.gap = 20;
    data.enemyCount = 3;
    data.enemySpeed = 300;
    data.trajectory = {{100, 0}, {800, 0}};

    m_levels.push_back({data.width, data.height, data.gap, data.enemyCount, data.enemySpeed, data.trajectory});
}