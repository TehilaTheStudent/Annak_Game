#ifndef GAME_UTILS_HPP
#define GAME_UTILS_HPP
#pragma once
#include <string>
#include <map>
#include <vector>
#include "ObjectsInXY.h"
#include <iomanip> // For std::setw/=
#include "Structs.h"
using namespace std;
class GameUtils {
public:
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
	static const vector<string> categories;

	
	static std::string getColorCode(const std::string& category)  {
		static const std::map<std::string, std::string> colorCodes = {
			{"Ground", "\033[32m"},    // Green
			{"Water", "\033[34m"},     // Blue
			{"Forest", "\033[35m"},    // Magenta
			{"Field", "\033[33m"},     // Yellow
			{"IronMine", "\033[31m"},  // Red
			{"BlocksMine", "\033[36m"}, // Cyan
			{"Village", "\033[32m"},    // Green
			{"City", "\033[31m"},      // Red
			{"Road", "\033[34m"},       // Blue
			  {"Car", "\033[32m"},       // Green
		 {"Truck", "\033[34m"},     // Blue
		{"Helicopter", "\033[35m"} // Magenta
		};
		auto it = colorCodes.find(category);
		return it != colorCodes.end() ? it->second : "\033[0m";
	}
	static void printInfrastructure(const std::vector<std::vector<shared_ptr<ObjectsInXY>>>& worldMatrix, const Coordinates& selected);
	static void printTransportation(const std::vector<std::vector<shared_ptr<ObjectsInXY>>>& worldMatrix, const Coordinates& selected);
	static void printWorld(const vector<vector<shared_ptr<GameObject>>>& tiles, const std::vector<std::vector<shared_ptr<ObjectsInXY>>>& worldMatrix, const Coordinates& selected);
	static void printGameObjects(const std::unordered_map<std::string, std::vector<std::shared_ptr<GameObject>>>& gameObjects);

	static void usePrintUtils(const vector<vector<shared_ptr<GameObject>>> & tiles, const std::vector<std::vector<shared_ptr<ObjectsInXY>>>& worldMatrix, const std::unordered_map<std::string, std::vector<std::shared_ptr<GameObject>>>& gameObjects,const Coordinates & selected);
};
#endif //GAME_UTILS_HPP