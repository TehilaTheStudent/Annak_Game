
#include <iostream>
#include "GameUtils.hpp"
#include <iomanip>
#include <sstream>
const std::string GameUtils::GROUND = "Ground";
const std::string GameUtils::WATER = "Water";
const std::string GameUtils::FOREST = "Forest";
const std::string GameUtils::FIELD = "Field";
const std::string GameUtils::IRON_MINE = "IronMine";
const std::string GameUtils::BLOCKS_MINE = "BlocksMine";
const std::string GameUtils::ROAD = "Road";
const std::string GameUtils::VILLAGE = "Village";
const std::string GameUtils::CITY = "City";
const std::string GameUtils::CAR = "Car";
const std::string GameUtils::TRUCK = "Truck";
const std::string GameUtils::HELICOPTER = "Helicopter";
const std::string GameUtils::PEOPLE = "People";
const std::string GameUtils::TILE = "Tile";
const vector<string> GameUtils::categories = {
	GameUtils::WATER,
	GameUtils::GROUND,
	GameUtils::FOREST,
	GameUtils::FIELD,
	GameUtils::IRON_MINE,
	GameUtils::BLOCKS_MINE,
	GameUtils::CITY,
	GameUtils::ROAD,
	GameUtils::VILLAGE,
	GameUtils::PEOPLE,
	GameUtils::CAR,
	GameUtils::TRUCK,
	GameUtils::HELICOPTER
};

void GameUtils::printInfrastructure(const std::vector<std::vector<shared_ptr<ObjectsInXY>>>& worldMatrix, const Coordinates& selected)
{
	int width = worldMatrix[0].size(); int height = worldMatrix.size();
	const int columnWidth = 3; // Fixed width for each cell

	// Print top row of x coordinates
	std::cout << std::setw(columnWidth) << " ";
	for (int x = 1; x <= width; ++x) {
		std::cout << std::setw(columnWidth - 1) << x << " ";
	}
	std::cout << std::endl;

	// Print the matrix with y coordinates on the left
	for (int y = 0; y < height; ++y) {
		std::cout << std::setw(columnWidth) << y + 1 << " ";
		for (int x = 0; x < width; ++x) {
			std::string displayText = " ";
			std::string color = "\033[0m"; // Default color (no color)
			displayText = ".";
			if (worldMatrix[y][x] == nullptr) {
				displayText = "."; // Placeholder for empty cell
			}
			else {
				if (worldMatrix[y][x]->hasInfrastructure()) {
					std::string category = worldMatrix[y][x]->infrastructure->getCategory();
					displayText = category.substr(0, 1);
					if (category == "Village" || category == "City" || category == "Road") {
						displayText = category.substr(0, 2);
					}
					color = getColorCode(category);
				}
				if (worldMatrix[y][x]->hasPerson()) {
					displayText = "*";
				}

			}
			if (x + 1 == selected.x && y + 1 == selected.y) {
				displayText = "@";
				color = "\033[1;36m"; // Unique color for selected cell (bright cyan)

			}
			std::cout << color << std::setw(columnWidth - 1) << displayText << "\033[0m" << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "--------------------" << std::endl;
}

void GameUtils::printTransportation(const std::vector<std::vector<shared_ptr<ObjectsInXY>>>& worldMatrix, const Coordinates& selected)
{
	int width = worldMatrix[0].size(); int height = worldMatrix.size();

	const int columnWidth = 3; // Fixed width for each cell

	// Print top row of x coordinates
	std::cout << std::setw(columnWidth) << " ";
	for (int x = 1; x <= width; ++x) {
		std::cout << std::setw(columnWidth - 1) << x << " ";
	}
	std::cout << std::endl;

	// Print the matrix with y coordinates on the left
	for (int y = 0; y < height; ++y) {
		std::cout << std::setw(columnWidth) << y + 1 << " ";
		for (int x = 0; x < width; ++x) {
			std::string displayText;
			std::string color = "\033[0m"; // Default color (no color)
			displayText = ".";
			if (worldMatrix[y][x] == nullptr) {// Check if there's no transportation
				displayText = "."; // Placeholder for empty cell
			}
			else {
				if (worldMatrix[y][x]->hasTransportation()) {
					std::string category = worldMatrix[y][x]->transportation->getCategory();
					displayText = category.substr(0, 1);
					if (category == "Helicopter") {
						displayText = category.substr(0, 2);
					}
					color = getColorCode(category);
				}
				if (worldMatrix[y][x]->hasPerson()) {
					displayText = "*";
				}

			}
			if (x + 1 == selected.x && y + 1 == selected.y) {
				displayText = "@";
				color = "\033[1;36m"; // Unique color for selected cell (bright cyan)

			}
			std::cout << color << std::setw(columnWidth - 1) << displayText << "\033[0m" << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "--------------------" << std::endl;
}

void GameUtils::printWorld(const vector<vector<shared_ptr<GameObject>>>& tiles, const std::vector<std::vector<shared_ptr<ObjectsInXY>>>& worldMatrix, const Coordinates& selected)
{
	int width = tiles[0].size() * 5; int height = tiles.size() * 5;

	const int columnWidth = 3; // Fixed width for each cell

	// Print top row of x coordinates
	std::cout << std::setw(columnWidth) << " ";
	for (int x = 1; x <= width; ++x) {
		std::cout << std::setw(columnWidth - 1) << x << " ";
	}
	std::cout << std::endl;

	// Print the matrix with y coordinates on the left
	for (int y = 0; y < height; ++y) {
		std::cout << std::setw(columnWidth) << y + 1 << " ";
		for (int x = 0; x < width; ++x) {
			std::string displayText;
			std::string color = "\033[0m"; // Default color (no color)
			if (tiles[y / 5][x / 5] == nullptr) {
				displayText = "."; // Placeholder for empty cell
			}
			else {

				if (worldMatrix[y][x] != nullptr && worldMatrix[y][x]->hasPerson()) {
					displayText = "*";
				}
				else {
					std::string category = tiles[y / 5][x / 5]->getCategory();
					displayText = category.substr(0, 1);
					if (category == "Forest" || category == "Field") {
						displayText = category.substr(0, 2);
					}
					color = getColorCode(category);
				}
				if (selected.x != -1 && x + 1 == selected.x && y + 1 == selected.y) {
					displayText = "@";
					color = "\033[1;36m"; // Unique color for selected cell (bright cyan)

				}
			}

			std::cout << color << std::setw(columnWidth - 1) << displayText << "\033[0m" << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "--------------------" << std::endl;
}



void GameUtils::usePrintUtils(const vector<vector<shared_ptr<GameObject>>>& tiles, const std::vector<std::vector<shared_ptr<ObjectsInXY>>>& worldMatrix, const std::unordered_map<std::string, std::vector<std::shared_ptr<GameObject>>>& gameObjects, const Coordinates& selected)
{
	if (worldMatrix.size() == 0) {
		return;
		//no world
	}
	printWorld(tiles, worldMatrix, selected);
	printInfrastructure(worldMatrix, selected);
	printTransportation(worldMatrix, selected);
	printGameObjects(gameObjects);
}

void GameUtils::printGameObjects(const std::unordered_map<std::string, std::vector<std::shared_ptr<GameObject>>>& gameObjects) {
	const int categoryWidth = 15;
	const int resourcesWidth = 30;
	const int coordinatesWidth = 15;

	// Print the header
	std::cout << std::left << std::setw(categoryWidth) << "Category"
		<< std::setw(resourcesWidth) << "Resources"
		<< std::setw(coordinatesWidth) << "Coordinates" << std::endl;

	// Print the separator
	std::cout << std::left << std::setw(categoryWidth) << std::setfill('-') << ""
		<< std::setw(resourcesWidth) << std::setfill('-') << ""
		<< std::setw(coordinatesWidth) << std::setfill('-') << ""
		<< std::setfill(' ') << std::endl;

	// Print each GameObject
	for (const auto& pair : gameObjects) {
		const std::string& category = pair.first;
		const std::vector<std::shared_ptr<GameObject>>& objects = pair.second;

		for (const auto& gameObject : objects) {
			// Print category
			std::cout << std::left << std::setw(categoryWidth) << category;

			// Print resources
			const std::vector<int>& resources = gameObject->getResources();
			std::ostringstream resourcesStream;
			for (int resource : resources) {
				resourcesStream << resource << " ";
			}
			std::cout << std::left << std::setw(resourcesWidth) << resourcesStream.str();

			// Print coordinates
			const Coordinates& coords = gameObject->getCoordinates();
			std::ostringstream coordinatesStream;
			coordinatesStream << "(" << coords.x << ", " << coords.y << ")";
			std::cout << std::left << std::setw(coordinatesWidth) << coordinatesStream.str();

			std::cout << std::endl;


		}


	}
	std::cout << std::endl;		std::cout << std::endl;
}
