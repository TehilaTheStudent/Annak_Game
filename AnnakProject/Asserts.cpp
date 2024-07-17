#include "Asserts.hpp"
#include <memory>
#include <algorithm>
#include "GameUtils.hpp"
using namespace std;
//all assertions available




// Define the static member variables
std::shared_ptr<GameState> Asserts::game;
std::unordered_map<std::string, std::function<void(const std::string&)>> Asserts::assertMap;
std::shared_ptr<JsonFile> Asserts::jsonFilePtr;

void Asserts::fill_assert(std::shared_ptr<GameState> game) {
	Asserts::game = game;
	Asserts::jsonFilePtr = JsonFile::getInstance();	

	assertMap[SELECTED_CATEGORY] = &Asserts::selectedCategory;
	assertMap[SELECTED_RESOURCE] = &Asserts::selectedResource;
	assertMap[SELECTED_PEOPLE] = &Asserts::selectedPeople;
	assertMap[SELECTED_CAR] = &Asserts::selectedCar;
	assertMap[SELECTED_TRUCK] = &Asserts::selectedTruck;
	assertMap[SELECTED_COORDINATES] = &Asserts::selectedCoordinates;
	assertMap[SELECTED_COMPLETE] = &Asserts::selectedComplete;
	assertMap[SELECTED_ROBBER] = &Asserts::selectedRobber;
	assertMap[ROAD_COUNT] = &Asserts::roadCount;
	assertMap[VILLAGE_COUNT] = &Asserts::villageCount;
	assertMap[CITY_COUNT] = &Asserts::cityCount;
	assertMap[CAR_COUNT] = &Asserts::carCount;
	assertMap[TRUCK_COUNT] = &Asserts::truckCount;
	assertMap[HELICOPTER_COUNT] = &Asserts::helicopterCount;
	assertMap[PEOPLE_COUNT] = &Asserts::peopleCount;
	assertMap[POINTS] = &Asserts::points;
}

void Asserts::executeAsserts(const std::vector<std::string>& assertions) {
	for (const string& assert : assertions) {
		auto it = assertMap.find(assert);
		if (it != assertMap.end()) {
			cout << it->first << " ";
			it->second(assert);  // You can pass the actual arguments here
			cout << endl;
		}
		else {
			std::cerr << "Unknown assertion: " << assert << std::endl;
		}
	}
}
void Asserts::selectedCategory(const std::string& args) {
	if (game->currentSelectedGameObject == nullptr) {
		return;
	}
	cout << game->currentSelectedGameObject->getCategory();
}

void Asserts::selectedResource(const std::string& args) {
	if (game->currentSelectedGameObject == nullptr) {
		cout << 0 << " " << 0 << " " << 0 << " " << 0;
		return;
	}
	shared_ptr <GameObject> selectedGameObject = game->currentSelectedGameObject;
	vector<int> resources = selectedGameObject->getResources();

	for (int i = 0; i < resources.size() - 1; i++) {
		cout << resources[i] << " ";
	}
	//std::cout << "Executing SelectedResource assert with args: " << args << std::endl;
}

void Asserts::selectedPeople(const std::string& args) {
	if (game->currentSelectedGameObject == nullptr) {
		return;
	}
	int resourceTypeIndex = jsonFilePtr->resourceTypeIndex[GameUtils::PEOPLE];
	//if (game->currentSelectedGameObject->isInfrusctuctureNotRoad()) {
	//	cout << game->currentSelectedObjectsInXY()->infrastructure->getResources()[resourceTypeIndex];
	//	return;
	//}
	//else if (game->currentSelectedGameObject->isInfrusctuctureNotRoad()) {
	//	cout << game->currentSelectedObjectsInXY()->infrastructure->getResources()[resourceTypeIndex];
	//	return;
	//}
	cout << game->currentSelectedGameObject->getResources()[resourceTypeIndex];
	//	std::cout << "Executing SelectedPeople assert with args: " << args << std::endl;
}

void Asserts::selectedCar(const std::string& args) {
	//only on village/city!   
	if (game->currentSelectedGameObject == nullptr) {
		return;
	}
	if (game->currentSelectedGameObject->isInfrusctuctureNotRoad())
	{
		cout << game->currentSelectedGameObject->getTransportationCount(GameUtils::CAR);

	}
	//std::cout << "Executing SelectedCar assert with args: " << args << std::endl;
}

void Asserts::selectedTruck(const std::string& args) {
	//only on village/city!   
	if (game->currentSelectedGameObject == nullptr) {
		return;
	}
	if (game->currentSelectedGameObject->isInfrusctuctureNotRoad())
	{
		cout << game->currentSelectedGameObject->getTransportationCount(GameUtils::TRUCK);

	}
	//std::cout << "Executing SelectedTruck assert with args: " << args << std::endl;
}

void Asserts::selectedCoordinates(const std::string& args) {
	if (game->currentSelectedGameObject == nullptr) {
		cout << 0 << " " << 0;
		return;
	}
	//TODO prepare for that option
	//cout << (game->currentSelectedGameObject->getCoordinates().x - 1) / 5 << " " << (game->currentSelectedGameObject->getCoordinates().y - 1) / 5;
	cout << game->currentSelectedGameObject->getCoordinates().x<< " " << game->currentSelectedGameObject->getCoordinates().y;

	//std::cout << "Executing SelectedCoordinates assert with args: " << args << std::endl;
}

void Asserts::selectedComplete(const std::string& args) {
	if (game->currentSelectedGameObject == nullptr) {
		return;
	}
	if (game->currentSelectedGameObject->isComplete()) {
		cout << "true";
	}
	else {
		cout << "false";
	}
}

void Asserts::selectedRobber(const std::string& args) {
	std::cout << "Executing SelectedRobber assert with args: " << args << std::endl;
}

void Asserts::roadCount(const std::string& args) {
	//std::cout << "Executing RoadCount assert with args: " << args << std::endl;
	cout << game->counts["Road"];
}

void Asserts::villageCount(const std::string& args) {
	//std::cout << "Executing VillageCount assert with args: " << args << std::endl;
	cout << game->counts["Village"];
}

void Asserts::cityCount(const std::string& args) {
	//std::cout << "Executing CityCount assert with args: " << args << std::endl;
	cout << game->counts["City"];
}

void Asserts::carCount(const std::string& args) {
	cout << game->counts["Car"];
	//std::cout << "Executing CarCount assert with args: " << args << std::endl;
}

void Asserts::truckCount(const std::string& args) {
	cout << game->counts["Truck"];
	//std::cout << "Executing TruckCount assert with args: " << args << std::endl;
}

void Asserts::helicopterCount(const std::string& args) {
	cout << game->counts["Helicopter"];

	//std::cout << "Executing HelicopterCount assert with args: " << args << std::endl;
}

void Asserts::peopleCount(const std::string& args) {
	std::cout << "Executing PeopleCount assert with args: " << args << std::endl;
}

void Asserts::points(const std::string& args) {

	std::cout << (game->points > 100 ? 100 : game->points);
}




// SelectedCategory $exp
// Expects the category of the selected entity in the world to be $exp
const std::string Asserts::SELECTED_CATEGORY = "SelectedCategory";

// SelectedResource $exp
// Expects that the "Resource" property of the selected entity in the world to be $exp
const std::string Asserts::SELECTED_RESOURCE = "SelectedResource";

// SelectedPeople $exp
// Expects that the "People" property of the selected entity in the world to be $exp
const std::string Asserts::SELECTED_PEOPLE = "SelectedPeople";

// SelectedCar $exp
// Expects that the "Car" property of the selected entity in the world to be $exp
const std::string Asserts::SELECTED_CAR = "SelectedCar";

// SelectedTruck $exp
// Expects that the "Truck" property of the selected entity in the world to be $exp
const std::string Asserts::SELECTED_TRUCK = "SelectedTruck";

// SelectedCoordinates $x $y
// Expects that the position of the selected entity in the world expressed in zero based tile coordinates matches $x and $y
const std::string Asserts::SELECTED_COORDINATES = "SelectedCoordinates";

// SelectedComplete $exp
// Expects that the "Complete" property of the selected entity in the world to be $exp
const std::string Asserts::SELECTED_COMPLETE = "SelectedComplete";

// SelectedRobber $exp
// Expects that the "Robber" property of the selected entity in the world to be $exp
const std::string Asserts::SELECTED_ROBBER = "SelectedRobber";

// RoadCount $exp
// Expects that the model has $exp number of entities of the category "Road"
const std::string Asserts::ROAD_COUNT = "RoadCount";

// VillageCount $exp
// Expects that the model has $exp number of entities of the category "Village"
const std::string Asserts::VILLAGE_COUNT = "VillageCount";

// CityCount $exp
// Expects that the model has $exp number of entities of the category "City"
const std::string Asserts::CITY_COUNT = "CityCount";

// CarCount $exp
// Expects that the model has $exp number of entities of the category "Car"
const std::string Asserts::CAR_COUNT = "CarCount";

// TruckCount $exp
// Expects that the model has $exp number of entities of the category "Truck"
const std::string Asserts::TRUCK_COUNT = "TruckCount";

// HelicopterCount $exp
// Expects that the model has $exp number of entities of the category "Helicopter"
const std::string Asserts::HELICOPTER_COUNT = "HelicopterCount";

// PeopleCount $exp
// Expects that the model has $exp number of entities of the category "People"
const std::string Asserts::PEOPLE_COUNT = "PeopleCount";

// Points $exp
// Expects that the model has $exp number of points stored
const std::string Asserts::POINTS = "Points";