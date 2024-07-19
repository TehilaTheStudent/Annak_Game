#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP
#pragma once
#include <memory>
#include "GameObject.hpp"
#include "Coordinates.h"
#include "GameUtils.hpp"
#include "JsonFile.h"
#include "Input.h"
#include <algorithm>
#include <map>
#include <utility>
#include <list>
#include "ObjectMoving.h"
#include "ObjectsInXY.h"
#include "GameVisuals.h"
#include "DirectionsEnum.h"
#include <unordered_map>
#include <utility>
#include "RoadsGraph.h"

using namespace std;
class GameState {
	//tile, inft=rustructure,movingObject

public:
	//fill worldMatrix and gameObjects with tiles
	void addWorldToWorldMatrix(const Input& input) {
		if (input.world->data.size() == 0) {
			return;
			//no world to build?
		}
		int tileSide = jsonFilePtr->sizes["Tile"].first;//assume square

		height = input.world->data.size() * tileSide;
		width = input.world->data[0].size() * tileSide;
		//resize nullptr
		worldMatrix.resize(height, std::vector<shared_ptr<ObjectsInXY>>(width));
		tiles.resize(input.world->data.size(), vector<shared_ptr<GameObject>>(input.world->data[0].size()));
		//1 2 3
		//4 5 5
		//create 6 GameObjects with properties, 
		//add them to the worldMatrix

		shared_ptr<GameObject> newGameObject;
		//add each tile
		for (int i = 0; i < tiles.size(); i++) {
			for (int j = 0; j < tiles[i].size(); j++) {
				if (input.world->data[i][j] == "0") {
					//stays nullptr
				}
				else {
					string category = jsonFilePtr->tiles[stoi(input.world->data[i][j])];
					vector<int> resources = { 0,0,0,0,0 };//add initial resources
					Coordinates coord(j * tileSide + 1, i * tileSide + 1);//base 1!!!
					//TODO addd checking when querying the json with .contains etc.
					if (jsonFilePtr->cateoryAmountResourceIndex.count(category) > 0) {//if this is of category with initial rsources

						resources[jsonFilePtr->cateoryAmountResourceIndex[category].second] = jsonFilePtr->cateoryAmountResourceIndex[category].first;

					}
					newGameObject = make_shared<GameObject>(category, resources, coord, true, false);
					tiles[i][j] = newGameObject;
					gameObjects[newGameObject->getCategory()].push_back(newGameObject);
					counts["Tiles"]++;
				}

			}
		}
		//printWorld();
	}
	bool hasTile(const Coordinates& coord) {
		if (!isOutOfRange(coord))
			return tiles[(coord.y - 1) / 5][(coord.x - 1) / 5] != nullptr;
		else
			return false;
	}
	bool objectsInXYNullPtr(const Coordinates& coord) {
		return getObjectsInXYInCoordinates(coord) == nullptr;
	}
	shared_ptr<GameObject> getTile(const Coordinates& coord) {
		return  tiles[(coord.y - 1) / 5][(coord.x - 1) / 5];
	}
	bool isOutOfRange(const Coordinates& coord) {
		if (coord.x<1 || coord.x>width || coord.y<1 || coord.y>height)
			return true;
		return false;
	}
	bool isOutOfRange(const Coordinates& coordinates, int objSide) {

		if (coordinates.x < 1 || coordinates.x + objSide - 1 > width) {
			//cout << "error- x axis out of range" << endl;
			return true;
		}
		if (coordinates.y < 1 || coordinates.y + objSide - 1 > height) {
			//cout << "error - x axis out of range" << endl;
			return true;
		}
		return false;
	}
	shared_ptr<ObjectsInXY> getObjectsInXYInCoordinates(const Coordinates& coord) {
		//not the time
		if (coord.y > worldMatrix.size() || coord.x > worldMatrix[0].size() || coord.x < 1 || coord.y < 1) {
			return nullptr;
		}
		return worldMatrix[coord.y - 1][coord.x - 1];
	}
	shared_ptr<ObjectsInXY> currentSelectedObjectsInXY() {
		return getObjectsInXYInCoordinates(currentSelectedGameObject->getCoordinates());
	}
	void startRaining(int steps) {
		rainLeftToFall = steps;//will start raining from next iteration
	}
	bool rain() {
		if (rainLeftToFall > 0) {//its raining
			rainLeftToFall--;
			for (map<string, pair<int, int>>::iterator it = rainsNeedToRegrow.begin(); it != rainsNeedToRegrow.end(); ++it) {
				//resourceType: <rained,need>
				it->second.first++;
				if (it->second.second == it->second.first) {//a resource needs to grow!
					//can grow now the resourceType for which category? 

					int resourceTypeIndex = jsonFilePtr->resourceTypeIndex[it->first];

					for (unordered_map<string, pair<int, int>>::const_iterator withResource = jsonFilePtr->cateoryAmountResourceIndex.begin(); withResource != jsonFilePtr->cateoryAmountResourceIndex.end(); withResource++) {
						//category:<starting,index>
						if (withResource->second.second == resourceTypeIndex) {//found the categpory that should get +1 resource
							for (int i = 0; i < gameObjects[withResource->first].size(); i++) {//grow resource to all gameObjects of this category
								if (gameObjects[withResource->first][i]->getCategory() == withResource->first) {
									gameObjects[withResource->first][i]->resourceExisting(resourceTypeIndex, 1);
								}
							}
						}
					}
					//resource was given
					it->second.first = 0;
				}
			}
			return true;
		}
		else {//not rainning now
			return false;
		}
	}
	void removeExistingGameObjectFromWorldMatrix(shared_ptr<GameObject> existingGameObject) {
		//ASK and then make it more efficient
		Coordinates coord = existingGameObject->getCoordinates();
		//existingGameObject->setCoordinates(Coordinates(-1, -1));//its nowhere
		int objSide;
		//get size
		bool isTile = existingGameObject->isTile();

		if (isTile) {
			//its a tile
			objSide = jsonFilePtr->sizes["Tile"].first;//assume square
		}
		else {
			objSide = jsonFilePtr->sizes[existingGameObject->getCategory()].first;
		}

		string category = existingGameObject->getCategory();
		//TODO chagne for case of rectangle objects
		for (int i = coord.y; i < objSide + coord.y; i++) {
			for (int j = coord.x; j < objSide + coord.x; j++) {


				if (existingGameObject->isInfrustructure()) {
					worldMatrix[i - 1][j - 1]->infrastructure = nullptr;
				}
				else {
					if (existingGameObject->isPeople()) {
						worldMatrix[i - 1][j - 1]->person = nullptr;

					}
					else {

						worldMatrix[i - 1][j - 1]->transportation = nullptr;

					}
				}
			}
		}



	}
	void addNewGameObjectToWorld(shared_ptr<GameObject> newGameObject, Coordinates coordinates) {
		//ASK and then make it more efficient
		newGameObject->setCoordinates(coordinates);
		int objSide;
		//get size


		objSide = jsonFilePtr->sizes[newGameObject->getCategory()].first;


		string category = newGameObject->getCategory();
		//TODO chagne for case of rectangle objects
		for (int i = coordinates.y; i < objSide + coordinates.y; i++) {
			for (int j = coordinates.x; j < objSide + coordinates.x; j++) {
				if (worldMatrix[i - 1][j - 1] == nullptr) {
					worldMatrix[i - 1][j - 1] = make_shared<ObjectsInXY>();

				}
				if (newGameObject->isInfrustructure()) {//TODO...
					//infrusturcture
					worldMatrix[i - 1][j - 1]->infrastructure = newGameObject;
				}
				else {
					if (newGameObject->isPeople()) {
						worldMatrix[i - 1][j - 1]->person = newGameObject;

					}
					else {

						worldMatrix[i - 1][j - 1]->transportation = newGameObject;

					}
				}
			}
		}
		//	printWorld();


	}
	void addNewGameObjectToGameObjects(shared_ptr<GameObject> newGameObject) {
		gameObjects[newGameObject->getCategory()].push_back(newGameObject);
	}
	bool isThereRoad(const Coordinates& coord) {
		if (!objectsInXYNullPtr(coord) && getObjectsInXYInCoordinates(coord)->isRoad()) {
			return true;
		}
		else {
			return false;
		}
	}

	void selectTopMostObject(Coordinates coordinates) {
		//TODO error handling- no object, invalid coordinates
		currentSelectedGameObject = getTopMostGameObject(coordinates);
	}

	void startMovingObject(ObjectMoving newMovingObject) {
		
		movingGameObjects.push_back(newMovingObject);
	}

	void startBuildingObject(shared_ptr<GameObject> beingBuilt) {
		buildingObjects.push_back(beingBuilt);
	}
	shared_ptr<GameObject> getTopMostGameObject(const  Coordinates& coordinates) {
		if (objectsInXYNullPtr(coordinates) || getObjectsInXYInCoordinates(coordinates)->getTopMost() == nullptr) {
			if (hasTile(coordinates)) {
				return getTile(coordinates);
			}
			else {
				return nullptr;
			}
		}
		return getObjectsInXYInCoordinates(coordinates)->getTopMost();
	}

	bool placeToMoveToForVehicle(const Coordinates& coord) {

		bool isHasTile = hasTile(coord); 
		bool isNotWater = getTile(coord)->getCategory()!=GameUtils::WATER; 
		bool isHasRoad = isThereRoad(coord); 
		bool isPlaceFree = getObjectsInXYInCoordinates(coord) != nullptr && getObjectsInXYInCoordinates(coord)->placeToMoveToForVehicle();

		

		return isHasTile && isNotWater && isHasRoad && isPlaceFree;
	}

	bool placeToMoveToForPerson(const Coordinates& coord) {
		return  hasTile(coord) &&
			getTile(coord)->getCategory() != "Water" &&
			(getObjectsInXYInCoordinates(coord) == nullptr || getObjectsInXYInCoordinates(coord)->placeToMoveToForPerson());
	}
	bool occupiedByMovingObject(const Coordinates & dst) {
		if (!objectsInXYNullPtr(dst) && getObjectsInXYInCoordinates(dst)->occupiedByMovingObj()) {
			return true;
		}
		else {
			return false;
		}
	}
	void moveMovingGameObjects() {
		bool movedThisTime = false;
		for (list<ObjectMoving>::iterator movingObj = movingGameObjects.begin(); movingObj != movingGameObjects.end(); movingObj++) {

			/*	Coordinates next = movingObj->calcNextStep();
				if (!objectsInXYNullPtr(next) && getObjectsInXYInCoordinates(next)->occupiedByMovingObj()) {
					cout << "continue" << endl;
					continue;
				}*/

			bool movedCell = movingObj->move();

			if (!movedCell)
				continue;

			//check if can move to next cell:  
			bool isPerson = movingObj->gameObject->isPeople();
			const Coordinates to = movingObj->current;
			const Coordinates in = movingObj->gameObject->getCoordinates();

			DirectionsEnum direction;
			bool shouldComtinue = false;
			if (!isPerson && to.x != in.x && to.y != in.y) {
				cout << "something is wrong" << endl;
			}
			int objSize = movingObj->gameObject->getSize().first;
			if (isPerson &&
				(!placeToMoveToForPerson(movingObj->current))) {
				//collision of person with: water, full infrustructure,full transportation
				//this person finished moving
				movingObj->gameObject->setIsMoving(false);
				movingObj = movingGameObjects.erase(movingObj);
				if (movingObj == movingGameObjects.end())
					return;
				continue;
			}
			else if (!isPerson
				//&& movingObj->gameObject->getCategory() != GameUtils::HELICOPTER
				)
			{


				if (to.x > in.x) {//-->
					direction = DirectionsEnum::RIGHT;
					for (int i = in.y; i < in.y + objSize; i++) {
						Coordinates coord(to.x + objSize - 1, i);
						if (!placeToMoveToForVehicle(coord)) {
							//the place i want to take is full, 
							movingObj->gameObject->setIsMoving(false);
							movingObj = movingGameObjects.erase(movingObj);
							if (movingObj == movingGameObjects.end())
								return;
							shouldComtinue = true;
							break;
						}
					}
				}
				else if (to.x < in.x)//<--
				{
					direction = DirectionsEnum::LEFT;
					for (int i = in.y; i < in.y + objSize; i++) {
						Coordinates coord(to.x, i);
						if (!placeToMoveToForVehicle(coord)) {

							//the place i want to take is full, 
							movingObj->gameObject->setIsMoving(false);
							movingObj = movingGameObjects.erase(movingObj);
							if (movingObj == movingGameObjects.end())
								return;
							shouldComtinue = true;
							break;
						}
					}
				}
				else if (to.y > in.y)//    \/
				{
					direction = DirectionsEnum::DOWN;
					for (int i = in.x; i < in.x + objSize; i++) {
						Coordinates coord(i, to.y + objSize - 1);
						if (!placeToMoveToForVehicle(coord)) {
							//the place i want to take is full, 
							movingObj->gameObject->setIsMoving(false);
							movingObj = movingGameObjects.erase(movingObj);
							if (movingObj == movingGameObjects.end())
								return;
							shouldComtinue = true;
							break;
						}
					}
				}
				else {// /\ /
					direction = DirectionsEnum::UP;
					for (int i = in.x; i < in.x + objSize; i++) {
						Coordinates coord(i, to.y);
						if (!placeToMoveToForVehicle(coord)) {
							//the place i want to take is full, 
							movingObj->gameObject->setIsMoving(false);
							movingObj = movingGameObjects.erase(movingObj);
							if (movingObj == movingGameObjects.end())
								return;
							shouldComtinue = true;
							break;
						}
					}
				}
			}
			if (shouldComtinue)
				continue;
			//doing a move, for vehicle / person
			removeExistingGameObjectFromWorldMatrix(movingObj->gameObject);

			//in the place that he left, get the top most and recover it

			if (isPerson) {//its a person
				GameVisuals::recover(in, getTopMostGameObject(in));
			}
			else {//its transportation
				if (direction == DirectionsEnum::RIGHT) {//-->
					for (int y = in.y; y < in.y + objSize; y++) {
						Coordinates coord(in.x, y);
						GameVisuals::recover(coord, getTopMostGameObject(coord));
					}
				}
				else if (direction == DirectionsEnum::LEFT) {//<-- 
					for (int y = in.y; y < in.y + objSize; y++) {
						Coordinates coord(to.x + objSize, y);
						GameVisuals::recover(coord, getTopMostGameObject(coord));
					}
				}
				else if (direction == DirectionsEnum::DOWN) {// \/ 
					for (int x = in.x; x < in.x + objSize; x++) {
						Coordinates coord(x, in.y);
						GameVisuals::recover(coord, getTopMostGameObject(coord));
					}
				}
				else {// /\       /
					for (int x = in.x; x < in.x + objSize; x++) {
						Coordinates coord(x, to.y + objSize);
						GameVisuals::recover(coord, getTopMostGameObject(coord));
					}
				}
			}
			addNewGameObjectToWorld(movingObj->gameObject, movingObj->current);
			if (isPerson) {
				GameVisuals::addObject(movingObj->gameObject);
			}
			else {
				GameVisuals::addObject(movingObj->gameObject, direction);
			}

			//	GameUtils::usePrintUtils(tiles, worldMatrix, gameObjects, currentSelectedGameObject != nullptr ? currentSelectedGameObject->getCoordinates() : Coordinates(-1, -1));
			movedThisTime = true;

			if (!movingObj->reached())
				continue;

			Coordinates& dst = movingObj->dst;

			if (movingObj->reason == "Work") {
				//take resource from tile ASK to where?

				shared_ptr<GameObject> tileToWorkOn = getTile(Coordinates(dst.x - 1, dst.y - 1));
				int resourceTypeIndex = jsonFilePtr->cateoryAmountResourceIndex[tileToWorkOn->getCategory()].second;
				if (tileToWorkOn->getResources()[resourceTypeIndex] > 0)//cant take from empty
				{
					tileToWorkOn->resourceExisting(resourceTypeIndex, tileToWorkOn->getResources()[resourceTypeIndex] - 1);//subtract 1 from tile
					movingObj->gameObject->resourceExisting(resourceTypeIndex, movingObj->gameObject->getResources()[resourceTypeIndex] + 1);//add 1 to the person?ASK

				}
			}
			else if (movingObj->reason == "Move") {
				if (movingObj->gameObject->getCategory() == "People") {//if its a person moving
					if (getObjectsInXYInCoordinates(movingObj->dst)->hasInfrustructureNotRoad()) {//if moves to infrustructure that is not road
						shared_ptr<GameObject> theInfrustructureInDst = getObjectsInXYInCoordinates(movingObj->dst)->infrastructure;
						int peopleResourceTypeIndex = jsonFilePtr->resourceTypeIndex[GameUtils::PEOPLE];
						if (theInfrustructureInDst->hasCapacities() && theInfrustructureInDst->getCapacities()[peopleResourceTypeIndex] == theInfrustructureInDst->getResources()[peopleResourceTypeIndex]) {
							// 
						}
						theInfrustructureInDst->getResources()[peopleResourceTypeIndex]++;
						//TODO add person to infrustructure objects...
						removeExistingGameObjectFromWorldMatrix(movingObj->gameObject);//remove from world map
					}


				}
			}
			//this person/transportation finished moving
			movingObj->gameObject->setIsMoving(false);
			movingObj = movingGameObjects.erase(movingObj);
			if (movingObj == movingGameObjects.end())
				return;
		}
		//TODO remove code repeatition...!!!

		/*if (movedThisTime) {
			GameVisuals::show();
		}*/

	}
	vector<shared_ptr<GameObject>> roadsAroundRoad( const Coordinates& roadCoord, int roadSize)
	{
		vector<shared_ptr<GameObject>> roadsAround;

		vector<Coordinates > places;
		places.push_back(roadCoord + Coordinates(0, -1));// /\ /
		places.push_back(roadCoord + Coordinates(0, roadSize));// \/ 
		places.push_back(roadCoord + Coordinates(roadSize, 0)); //-->
		places.push_back(roadCoord + Coordinates(-1, 0));//<--

		

		for (const Coordinates& coord : places) { 
			if (isThereRoad(coord)) {
				roadsAround.push_back(getObjectsInXYInCoordinates(coord)->infrastructure); 
			} 
		}




		return roadsAround;
	}
	void rainMoveBuild() {
		rain();
		moveMovingGameObjects();
		buildBuildingGameObjects();
	}

	void buildBuildingGameObjects() {
		for (list<shared_ptr<GameObject>>::iterator buildingObj = buildingObjects.begin(); buildingObj != buildingObjects.end(); buildingObj++) {
			(*buildingObj)->build();
			if ((*buildingObj)->isComplete()) {
				//printWorld();

				//counts[(*buildingObj)->getCategory()]++;was done already

				buildingObj = buildingObjects.erase(buildingObj);
				if (buildingObj == buildingObjects.end())
					return;
			}

		}
	}
	GameState() :rainLeftToFall(0), jsonFilePtr(JsonFile::getInstance()), points(0) {


		for (unordered_map<string, pair<int, int>>::const_iterator it = jsonFilePtr->sizes.begin(); it != jsonFilePtr->sizes.end(); ++it) {
			counts[it->first] = 0;
		}
		//"Tile": [5, 5] ,
		//	"People" : [1, 1] ,
		//	"Car" : [2, 2] ,
		//	"Truck" : [3, 3] ,
		//	"Helicopter" : [2, 2] ,
		//	"Road" : [5, 5] ,
		//	"Village" : [10, 10] ,
		//	"City" : [20, 20]


		map<string, int> rains = jsonFilePtr->access()["Rains"].get<map<string, int>>();
		for (map<string, int>::const_iterator it = rains.begin(); it != rains.end(); ++it) {
			rainsNeedToRegrow[it->first] = pair<int, int>(0, it->second);
			//resouceType: <alreadyRained,need>
		}
	}

	shared_ptr<GameObject> currentSelectedGameObject;
	vector<vector<shared_ptr<ObjectsInXY>>> worldMatrix;// structure(city/road/village)/people 
	map<string, int> counts;
	unordered_map<string, vector<shared_ptr<GameObject>>> gameObjects;
	int rainLeftToFall;
	map<string, pair<int, int>> rainsNeedToRegrow;
	vector < vector<shared_ptr<GameObject>>> tiles;
	RoadsGraph roadsGraph; 
	list<ObjectMoving> movingGameObjects;
	list<shared_ptr<GameObject>> buildingObjects;
	shared_ptr<JsonFile> jsonFilePtr;
	int height, width;
	int points;

};
#endif//GAME_STATE_HPP
