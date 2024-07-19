#ifndef VALIDATIONS_HPP
#define VALIDATIONS_HPP
#pragma once
//#include "GameState.hpp"
#include <vector>
#include <memory>
#include "ObjectsInXY.h"
#include "Coordinates.h"

using namespace std;
class GameState;

class Validations {
public:
	static bool isEmptyGround(const vector<vector<shared_ptr<GameObject>>>& tiles, const std::vector<std::vector<shared_ptr<ObjectsInXY>>>& worldMatrix, int objSide, Coordinates coordinates);

	static bool isThereRoadAround(const std::vector<std::vector<shared_ptr<ObjectsInXY>>>& worldMatrix, int objSide, int roadSide, Coordinates coordinates);
	static bool isInArea(shared_ptr<GameObject> obj, const Coordinates& inside);
	static bool isInArea(const Coordinates& topLeftArea, const Coordinates& bottomRightAera, const Coordinates& obj);
	static bool isInArea(const shared_ptr<GameObject> big, const shared_ptr<GameObject> small);
	static 	bool isInArea(const shared_ptr<GameObject> big, const Coordinates& topLeft, int objSize);
	static 	bool isInArea(const Coordinates& topLeftArea, const Coordinates& bottomRightArea, const Coordinates& topLeftObj, const Coordinates& bottomRightObj);
};

#endif //VALIDATIONS_HPP