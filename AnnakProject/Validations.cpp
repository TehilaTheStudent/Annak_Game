#include "Validations.hpp"
#include "GameUtils.hpp"
using namespace std;
bool Validations::isEmptyGround(const vector<vector<shared_ptr<GameObject>>>& tiles, const std::vector<std::vector<shared_ptr<ObjectsInXY>>>& worldMatrix, int objSide, Coordinates coordinates)
{
	//assume not out of range
	for (int i = coordinates.y; i < coordinates.y + objSide; i++) {
		for (int j = coordinates.x; j < coordinates.x + objSide; j++) {
			if (!(tiles[(i - 1) / 5][(j - 1) / 5] != nullptr &&
				tiles[(i - 1) / 5][(j - 1) / 5]->getCategory() == GameUtils::GROUND
				&& (worldMatrix[i - 1][j - 1] == nullptr || worldMatrix[i - 1][j - 1]->getTopMost() == nullptr)))
			{
				return false;
			}
		}
	}
	return true;
}

bool Validations::isThereRoadAround(const std::vector<std::vector<shared_ptr<ObjectsInXY>>>& worldMatrix, int objSide, int roadSide, Coordinates coordinates)
{
	//missing: check if ground underneath, chech if full road
	//TODO remove code duplication
	//hande rectangle road case
	int width = worldMatrix[0].size(); int height = worldMatrix.size();
	if (coordinates.x < 1 || coordinates.x + objSide - 1 > width) {
		//cout << "error- x axis out of range" << endl;
		return false;
	}
	if (coordinates.y < 1 || coordinates.y + objSide - 1 > height) {
		//cout << "error - x axis out of range" << endl;
		return false;
	}
	//TODO dvoras idea
	int count;
	//top
	if (coordinates.y != 1)//there is top
	{
		count = 0;
		for (int i = coordinates.x; i < objSide + coordinates.x; i++) {
			if (worldMatrix[coordinates.y - 2][i - 1] != nullptr && worldMatrix[coordinates.y - 2][i - 1]->isRoad())
			{
				count++;
				if (count == roadSide)
					return true;
			}
			else {
				count = 0;
			}
		}
	}


	//left
	if (coordinates.x != 1)//there is left
	{
		count = 0;
		for (int i = coordinates.y; i < objSide + coordinates.y; i++) {
			if (worldMatrix[i - 1][coordinates.x - 2] != nullptr && worldMatrix[i - 1][coordinates.x - 2]->isRoad())
			{
				count++;
				if (count == roadSide)
					return true;
			}
			else {
				count = 0;
			}
		}
	}

	//bottom
	if (coordinates.y + objSide < height)//there is bottom
	{
		count = 0;
		for (int i = coordinates.x; i < objSide + coordinates.x; i++) {
			if (worldMatrix[coordinates.y + objSide][i - 1] != nullptr && worldMatrix[coordinates.y + objSide][i - 1]->isRoad())
			{
				count++;
				if (count == roadSide)
					return true;
			}
			else {
				count = 0;
			}
		}
	}

	//right
	if (coordinates.x + objSide < width)
	{
		count = 0;
		for (int i = coordinates.y; i < objSide + coordinates.y; i++) {
			if (worldMatrix[i - 1][coordinates.x + objSide] != nullptr && worldMatrix[i - 1][coordinates.x + objSide]->isRoad())
			{
				count++;
				if (count == roadSide)
					return true;
			}
			else {
				count = 0;
			}
		}
	}

	return false;
}

bool Validations::isInArea(const Coordinates& topLeftArea, const Coordinates& bottomRightAera, const Coordinates& obj)
{
	if (obj.x >= topLeftArea.x && obj.x <= bottomRightAera.x && obj.y >= topLeftArea.y && obj.y <= bottomRightAera.y) {
		return true;
	}
	else
		return false;
}

bool Validations::isInArea(const Coordinates& topLeftArea, const Coordinates& bottomRightArea, const Coordinates& topLeftObj, const Coordinates& bottomRightObj)
{
	return isInArea(topLeftArea, bottomRightArea, topLeftObj) || isInArea(topLeftArea, bottomRightArea, bottomRightObj);

}