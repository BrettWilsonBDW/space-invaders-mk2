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
    data.trajectory = {{100, 20}, {800, 20}};

    m_levels.push_back({data.width, data.height, data.gap, data.enemyCount, data.enemySpeed, data.trajectory});

    data.width = 50;
    data.height = 50;
    data.gap = 20;
    data.enemyCount = 3;
    data.enemySpeed = 300;
    data.trajectory = {{100, 20}, {800, 20}};

    m_levels.push_back({data.width, data.height, data.gap, data.enemyCount, data.enemySpeed, data.trajectory});

    data.width = 50;
    data.height = 50;
    data.gap = 20;
    data.enemyCount = 4;
    data.enemySpeed = 300;
    data.trajectory = {{0, 20}, {900, 20}};

    m_levels.push_back({data.width, data.height, data.gap, data.enemyCount, data.enemySpeed, data.trajectory});

    data.width = 50;
    data.height = 50;
    data.gap = 20;
    data.enemyCount = 8;
    data.enemySpeed = 500;
    data.trajectory = {{0, 50}, {900, 50}};

    m_levels.push_back({data.width, data.height, data.gap, data.enemyCount, data.enemySpeed, data.trajectory});

    data.width = 50;
    data.height = 50;
    data.gap = 20;
    data.enemyCount = 3;
    data.enemySpeed = 600;
    data.trajectory = {{0, 50}, {900, 50}, {300, 60}, {600, 60}, {0, 50}};

    m_levels.push_back({data.width, data.height, data.gap, data.enemyCount, data.enemySpeed, data.trajectory});

    data.width = 50;
    data.height = 50;
    data.gap = 20;
    data.enemyCount = 15;
    data.enemySpeed = 300;
    data.trajectory = {{100, 20}, {800, 20}};

    m_levels.push_back({data.width, data.height, data.gap, data.enemyCount, data.enemySpeed, data.trajectory});

    data.width = 50;
    data.height = 50;
    data.gap = 20;
    data.enemyCount = 10;
    data.enemySpeed = 300;
    data.trajectory = {{100, 20}, {800, 20}};

    m_levels.push_back({data.width, data.height, data.gap, data.enemyCount, data.enemySpeed, data.trajectory});
}