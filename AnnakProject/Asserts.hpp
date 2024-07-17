#ifndef ASSERTS_HPP
#define ASSERTS_HPP
#pragma once
#include <string>
#include <memory>
#include "GameState.hpp"
#include "json.hpp"
#include <functional>
using namespace std;
class Asserts {
public:
	static void fill_assert(std::shared_ptr<GameState> game);
	static  void executeAsserts(const std::vector<std::string>& assertions);
private:

	// Private constructor and destructor to prevent instantiation
	Asserts() = delete;
	~Asserts() = delete;

	// Delete copy constructor and copy assignment operator
	Asserts(const Asserts&) = delete;
	Asserts& operator=(const Asserts&) = delete;

	static   std::shared_ptr<GameState> game;
	static   std::unordered_map<std::string, std::function<void(const std::string&)>> assertMap;
	static shared_ptr<JsonFile> jsonFilePtr;


	static const std::string SELECTED_CATEGORY;
	static const std::string SELECTED_RESOURCE;
	static const std::string SELECTED_PEOPLE;
	static const std::string SELECTED_CAR;
	static const std::string SELECTED_TRUCK;
	static const std::string SELECTED_COORDINATES;
	static const std::string SELECTED_COMPLETE;
	static const std::string SELECTED_ROBBER;
	static const std::string ROAD_COUNT;
	static const std::string VILLAGE_COUNT;
	static const std::string CITY_COUNT;
	static const std::string CAR_COUNT;
	static const std::string TRUCK_COUNT;
	static const std::string HELICOPTER_COUNT;
	static const std::string PEOPLE_COUNT;
	static const std::string POINTS;


	static void selectedCategory(const std::string& args);
	static  void selectedResource(const std::string& args);
	static  void selectedPeople(const std::string& args);
	static  void selectedCar(const std::string& args);
	static  void selectedTruck(const std::string& args);
	static  void selectedCoordinates(const std::string& args);
	static  void selectedComplete(const std::string& args);
	static  void selectedRobber(const std::string& args);
	static  void roadCount(const std::string& args);
	static  void villageCount(const std::string& args);
	static void cityCount(const std::string& args);
	static void carCount(const std::string& args);
	static  void truckCount(const std::string& args);
	static  void helicopterCount(const std::string& args);
	static void peopleCount(const std::string& args);
	static void points(const std::string& args);
};

#endif //ASSERTS_HPP
