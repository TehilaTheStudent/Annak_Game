
#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP
#pragma once
#include "JsonFile.h"
#include <string>
#include <vector>
#include "Structs.h"
#include <utility>
//#include "GameUtils.hpp"
#include <algorithm>
#include <map>
class GameObject {
private:
	std::vector<int> resources;
	Coordinates coordinates;
	Coordinates offsetFromCell;
	bool complete;
	bool robber;
	std::string category;
	int completedAlready;
	bool isMoving;

	shared_ptr<JsonFile> jsonFilePtr;
	map<string, int> transportationCounters;

public:
	//TODO vector of pointers to people in city
	// Constructors
	GameObject(std::string category) : category(std::move(category)), jsonFilePtr(JsonFile::getInstance()) {

		for (unordered_set<string>::const_iterator it = jsonFilePtr->transportationTypes.begin(); it != jsonFilePtr->transportationTypes.end(); ++it) {
			transportationCounters[*it] = 0;
		}
	}
	GameObject(
		const std::string& category,
		const std::vector<int>& resources,
		const Coordinates& coordinates,
		bool complete,
		bool robber
	) : category(category),
		resources(resources),
		coordinates(coordinates),
		complete(complete),
		robber(robber),
		completedAlready(0),
		jsonFilePtr(JsonFile::getInstance()),
		offsetFromCell(0,0),
		isMoving(false)
	{
		for (unordered_set<string>::const_iterator it = jsonFilePtr->transportationTypes.begin(); it != jsonFilePtr->transportationTypes.end(); ++it) {
			transportationCounters[*it] = 0;
		}
	}
#pragma region getset
	// Getter and Setter for resources

	std::vector<int>& getResources() {
		return resources;
	}
	const std::vector<int>& getCapacities() const {
		return jsonFilePtr->capacities[category];
	}
	void setResources(const std::vector<int>& resources) {
		this->resources = resources;
	}

	int getTransportationCount(string category) {
		return transportationCounters[category];
	}
	bool hasCapacities() {
		return jsonFilePtr->capacities.count(category) > 0;
	}
	double getSpeed() {
		if (jsonFilePtr->speeds.count(category) > 0) {
			return jsonFilePtr->speeds[category];
		}
		else {
			return -1;
		}
	}
	// Getter and Setter for coordinates
	const Coordinates& getCoordinates() const {
		return coordinates;
	}
	const Coordinates& getRightBottomCoordinates() const {
		int objSize = getSize().first;
		 Coordinates coord(coordinates.x+objSize-1,coordinates.y+objSize-1);
		 return coord;
	}

	void setCoordinates(const Coordinates& coordinates) {
		this->coordinates = coordinates;
		offsetFromCell = { 0,0 };
	}
	const Coordinates& getOffsetFromCell() const {
		return offsetFromCell;
	}
	void setOffsetFromCell(const Coordinates& offsetFromCell) {
		this->offsetFromCell = offsetFromCell;
	}


	// Getter and Setter for complete
	bool isComplete() const {
		return complete;
	}

	void setComplete(bool complete) {
		this->complete = complete;
	}
	bool getIsMoving() const{
		return isMoving;
	}
	void setIsMoving(bool isMoving) {
		this->isMoving = isMoving;
	}
	// Getter and Setter for robber
	bool isRobber() const {
		return robber;
	}

	void setRobber(bool robber) {
		this->robber = robber;
	}

	// Getter and Setter for category
	const std::string& getCategory() const {
		return category;
	}

	void setCategory(const std::string& category) {
		this->category = category;
	}

	void setTransportationCounter(string category, int count) {
		transportationCounters[category] = count;
	}
	void resetTransportationCounters() {
		for (const pair<string, int>& pair : transportationCounters) {
			transportationCounters[pair.first] = 0;
		}
	}

	pair<int, int> getSize() const {
		if (isTile()) {
			return jsonFilePtr->sizes["Tile"];
		}
		if (jsonFilePtr->sizes.count(category) > 0)
			return jsonFilePtr->sizes[category];
		return { -1,-1 };
	}
#pragma endregion
	void build() {
		completedAlready++;
		if (completedAlready == getTime()) {
			complete = true;
		}
	}


	bool canResource(const std::vector<int>& resourcesToAdd) {
		if (!hasCapacities())
			return true;
		for (int i = 0; i < resourcesToAdd.size(); i++) {
			if (resourcesToAdd[i] > getCapacity(i))//ASK > or >=?
			{
				return false;
			}
		}
		return true;
	}

	bool canSubtructResources(vector<int> costs) {
		for (int i = 0; i < costs.size(); i++) {
			if (resources[i] < costs[i])
				return false;
		}
		return true;
	}
	void subtructResources(vector<int> costs) {
		for (int i = 0; i < costs.size(); i++) {
			resources[i] -= costs[i];
		}
	}

	bool canResource(int resourceTypeIndex, int amount) {
		if (!hasCapacities())
			return true;
		return	resources[resourceTypeIndex] + amount <= getCapacity(resourceTypeIndex);//ASK > or >= ?
	}

	void resourceExisting(int resourceTypeIndex, int amount) {
		if (hasCapacities()) {
			if (amount > getCapacity(resourceTypeIndex)) {
				amount = getCapacity(resourceTypeIndex);
			}
		}
		resources[resourceTypeIndex] = amount;
	}
	int getCapacity(int resourceTypeIndex) {
		if (jsonFilePtr->capacities.count(category) > 0) {
			return jsonFilePtr->capacities[category][resourceTypeIndex];
		}
		return -1;
	}
	int getTime() {
		if (jsonFilePtr->times.count(category) > 0) {
			return jsonFilePtr->times[category];
		}
		return -1;
	}
	void resourceExisting(const std::vector<int>& resourcesToSet) {
		for (int i = 0; i < resourcesToSet.size(); i++) {
			if (hasCapacities() && resourcesToSet[i] > getCapacity(i)) {

				resources[i] = getCapacity(i);//put the maximum

			}
			else {
				resources[i] = resourcesToSet[i];
			}
		}
	}
	int getScore() const {
		if (jsonFilePtr->scores.count(category) > 0) {
			return jsonFilePtr->scores[category];
		}
		else {
			return 0;
		}
	}
	bool isPeople() {
		return category == "People";
	}
	bool isInfrustructure() {
		if (jsonFilePtr->infrustructureTypes.count(category) > 0) {
			return true;
		}
		return false;
	}
	bool isInfrusctuctureNotRoad() {
		return isInfrustructure() && !isRoad();
	}
	bool isTransportation() {
		if (jsonFilePtr->transportationTypes.count(category) > 0) {
			return true;
		}
		return false;
	}
	bool isTile() const{
		vector<string> tilesTypes = jsonFilePtr->tiles;
		if (find(tilesTypes.begin(), tilesTypes.end(), category) != tilesTypes.end()) {
			return true;
		}
		return false;
	}
	bool isRoad() {
		return category == "Road";
	}
	void addTransportation(string category) {

		transportationCounters[category]++;
	}
	int getMainResource() {
		if (jsonFilePtr->cateoryAmountResourceIndex.count(category) > 0) {
			int resourceTypeIndex = jsonFilePtr->cateoryAmountResourceIndex[category].second;
			return resources[resourceTypeIndex];
		}
		return 0;
	}
	int setMainResource(int amount) {
		if (jsonFilePtr->cateoryAmountResourceIndex.count(category) > 0) {
			int resourceTypeIndex = jsonFilePtr->cateoryAmountResourceIndex[category].second;
			resources[resourceTypeIndex] = amount;
		}
		return 0;
	}
	void transferResourcesToAnother(GameObject& other) {
		for (int i = 0; i < resources.size(); i++) {
			if (this->resources[i] > 0) {//i can give
				if (other.hasCapacities()) {//other has capacities
					if (other.canResource(i, 1)) {//other not full
						if (this->resources[i] > other.getCapacity(i)) {// i can give more, so give the maximum

							other.resources[i] = other.getCapacity(i);
							this->resources[i] -= other.getCapacity(i);
						}
						else {
							other.resources[i] = this->resources[i];
							this->resources[i] = 0;
						}
					}

				}
				else {//doesnt have capacities
					other.resources[i] = this->resources[i];
					this->resources[i] = 0;
				}
			}

		}
	}
};


#endif // GAMEOBJECT_HPP 