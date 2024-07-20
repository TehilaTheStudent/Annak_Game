#ifndef CATEGORIES_CONSTANTS_H
#define CATEGORIES_CONSTANTS_H
#pragma once

#include <string>
#include <vector>

class CategoriesConstants {
public:
    // Declare the constants as static and const
    static const std::string GROUND;
    static const std::string WATER;
    static const std::string FOREST;
    static const std::string FIELD;
    static const std::string IRON_MINE;
    static const std::string BLOCKS_MINE;
    static const std::string ROAD;
    static const std::string VILLAGE;
    static const std::string CITY;
    static const std::string CAR;
    static const std::string TRUCK;
    static const std::string HELICOPTER;
    static const std::string PEOPLE;
    static const std::string TILE;

    static const std::vector<std::string> categories;
};

#endif // CATEGORIES_CONSTANTS_H
