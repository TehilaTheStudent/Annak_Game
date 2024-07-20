
#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP
#pragma once
#include "JsonFile.h"
#include <string>
#include <vector>
#include "Coordinates.h"
#include <utility>
#include "CategoriesConstants.h"
#include <algorithm>
#include <map>
#include "DirectionsEnum.h"
class GameObject {
private:
	static int roadObjects;
	DirectionsEnum direction;
	std::vector<int> resources;
	Coordinates coordinates;
	bool complete;
	bool robber;
	std::string category;
	int completedAlready;
	bool isMoving;
	int roadIndex;
	bool isInsideAnother;
	shared_ptr<JsonFile> jsonFilePtr;
	

public:
	map<string, vector< shared_ptr<GameObject>>> resourcesObjectsInsideMe;//for city , village, car, truck, helicopter 
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
		isMoving(false),
		direction(DirectionsEnum::UP),
		isInsideAnother(false)
	{

		if (category == CategoriesConstants::ROAD) {
			roadIndex = roadObjects++;
		}
		//who can hold? 
		//city village->people transportation
		//car truck helicopter-> people
		//people, car, truck,helicopter
		if (isTransportation()) {
			resourcesObjectsInsideMe[CategoriesConstants::PEOPLE] = vector<shared_ptr<GameObject>>();

		}
		else if (isInfrusctuctureNotRoad()) {

			resourcesObjectsInsideMe[CategoriesConstants::PEOPLE] = vector<shared_ptr<GameObject>>();

			for (unordered_set<string>::const_iterator it = jsonFilePtr->transportationTypes.begin(); it != jsonFilePtr->transportationTypes.end(); ++it) {
				resourcesObjectsInsideMe[*it] = vector<shared_ptr<GameObject>>();
			}
		}

	}
#pragma region getset
	// Getter and Setter for resources

	DirectionsEnum getDirection() const {
		return direction;
	}
	bool getIsInsideAnother() const {
		return isInsideAnother;
	}

	void setIsInsideAnother(bool newValue) {
		isInsideAnother = newValue;
	}

	void setDirection(DirectionsEnum newDirection) {
		direction = newDirection;
	}
	std::vector<int>& getResources() {
		return resources;
	}
	const std::vector<int>& getCapacities() const {
		return jsonFilePtr->capacities[category];
	}
	void setResources(const std::vector<int>& resources) {
		this->resources = resources;
	}
	bool canContainAnotherObjects() const {
		return isInfrusctuctureNotRoad() || isTransportation();
	}
	int getResourceInsideMe(string category) {
		//return transportationCounters[category];
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
	const Coordinates getRightBottomCoordinates() const {
		int objSize = getSize().first;
		Coordinates coord(coordinates.x + objSize - 1, coordinates.y + objSize - 1);
		return coord;
	}
	const int getRoadIndex()const {
		return roadIndex;
	}
	void setCoordinates(const Coordinates& coordinates) {
		this->coordinates = coordinates;
	}




	// Getter and Setter for complete
	bool isComplete() const {
		return complete;
	}

	void setComplete(bool complete) {
		this->complete = complete;
	}
	bool getIsMoving() const {
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
	bool canMove() const {
		return isPeople() || isTransportation();
	}
	// Getter and Setter for category
	const std::string& getCategory() const {
		return category;
	}

	void setCategory(const std::string& category) {
		this->category = category;
	}

	void setTransportationCounter(string category, int count) {
		//transportationCounters[category] = count;
	}
	void resetTransportationCounters() {
		/*	for (const pair<string, int>& pair : transportationCounters) {
				transportationCounters[pair.first] = 0;
			}*/
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


	bool canAddResource(const std::vector<int>& resourcesToAdd) {
		if (!hasCapacities())
			return true;
		for (int i = 0; i < resourcesToAdd.size(); i++) {
			if (resourcesToAdd[i] + this->resources[i] >= getCapacity(i))
			{
				return false;
			}
		}
		return true;
	}
	bool canSetResource(const std::vector<int>& resourceToSet) {
		if (!hasCapacities())
			return true;
		for (int i = 0; i < resourceToSet.size(); i++) {
			if (resourceToSet[i] >= getCapacity(i))
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

	bool canSetResource(int resourceTypeIndex, int amount) {
		if (!hasCapacities())
			return true;
		return	 amount <= getCapacity(resourceTypeIndex);
	}
	bool canAddResource(int resourceTypeIndex, int amount) {
		if (!hasCapacities())
			return true;
		return	resources[resourceTypeIndex] + amount <= getCapacity(resourceTypeIndex);
	}
	void addResource(int resourceTypeIndex, int amount) {
		resources[resourceTypeIndex] += amount;
	}
	void addResource(const std::vector<int>& resoucesToAdd) {
		for (int i = 0; i < resoucesToAdd.size(); i++) {
			resources[i] += resoucesToAdd[i];
		}
	}
	bool isTileWithResources()const {
		return isTile() && category != CategoriesConstants::WATER && category != CategoriesConstants::GROUND;
	}
	void setMostResource(int resourceTypeIndex, int amount) {
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
	int getTime() const {
		if (jsonFilePtr->times.count(category) > 0) {
			return jsonFilePtr->times[category];
		}
		return -1;
	}
	void setMostResource(const std::vector<int>& resourcesToSet) {
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
	shared_ptr<GameObject> takeOutResourceObject(const string& category) {
		if (!hasKey(category) || resourcesObjectsInsideMe[category].size() == 0) {
			return nullptr;
		}
		shared_ptr<GameObject> popped = resourcesObjectsInsideMe[category].back();
		resourcesObjectsInsideMe[category].pop_back();
		return popped;

	}
	int resourceObjectsCount(const string& category)  {
		if (hasKey(category)) {
			return resourcesObjectsInsideMe[category].size();
		}
		return -1;
	}
	bool hasKey(const string& category) const {
		if (resourcesObjectsInsideMe.count(category) == 0) {
			cout << endl << "ERROR! dont have that key" << endl;
			return false;
		}
		return true;
	}
	bool isPeople()const {
		return category == CategoriesConstants::PEOPLE;
	}
	bool isInfrustructure()const {
		if (jsonFilePtr->infrustructureTypes.count(category) > 0) {
			return true;
		}
		return false;
	}
	bool isInfrusctuctureNotRoad() const {
		return isInfrustructure() && !isRoad();
	}
	bool isTransportation() const {
		if (jsonFilePtr->transportationTypes.count(category) > 0) {
			return true;
		}
		return false;
	}
	bool isTile() const {
		vector<string> tilesTypes = jsonFilePtr->tiles;
		if (find(tilesTypes.begin(), tilesTypes.end(), category) != tilesTypes.end()) {
			return true;
		}
		return false;
	}
	bool isRoad() const {
		return category == CategoriesConstants::ROAD;
	}
	void clearResources() {
		for (int& resource : resources) {
			resource = 0;
		}
	}
	void addResourceObject(shared_ptr<GameObject> resourceObject) {
		if (resourcesObjectsInsideMe.count(resourceObject->category) == 0) {
			cout << endl << "ERROR! cant add resourceobject to this!" << endl;
			return;
		}
		if (!canAddResource(resourceObject->resources)) {
			cout << endl << "ERROR! cant add resourceobject to this- i dont have anough capacity!" << endl;
			return;
		}
		resourceObject->transferResourcesToAnother(*this);
		resourcesObjectsInsideMe[resourceObject->category].push_back(resourceObject);
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
					if (other.canAddResource(i, 1)) {//other not full
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