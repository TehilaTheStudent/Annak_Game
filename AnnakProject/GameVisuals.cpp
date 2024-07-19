#include "GameVisuals.h"
#include "GameUtils.hpp"
#include <opencv2/opencv.hpp>
#include <string> 
#include <unordered_map> 
#include <vector> 
#include <algorithm>
#include <ranges>
#include "Coordinates.h"
#include "GameState.hpp"
#include "Validations.hpp"
#include "CommandExecute.h"
#include "CreateCommand.h"

using namespace cv;
using namespace std;
// Define the static member variables
//std::shared_ptr<GameState> GameVisuals::game;

std::shared_ptr<JsonFile> GameVisuals::jsonFilePtr;
unordered_map<string, Mat> GameVisuals::category_image_map;
Mat GameVisuals::main_mat;
Mat GameVisuals::secondary_mat;
bool GameVisuals::mouseClicked = false;
int GameVisuals::currentFrameIndex = 0;
bool GameVisuals::userChoseDst = false;

Coordinates GameVisuals::dest;
vector<shared_ptr<GameObject>> GameVisuals::objectsInArea;
shared_ptr<GameState> GameVisuals::gameState;

double GameVisuals::lastTime;
bool GameVisuals::stopGame = false;
bool GameVisuals::inTheMiddleOfDrawingRectangle = false;
bool GameVisuals::generateNewMoving = false;

map<string, vector<Mat>> GameVisuals::category_states_map;
GameVisuals::Rectangle* GameVisuals::recBeing = nullptr;
GameVisuals::MovingLabel GameVisuals::pointsLabel;
GameVisuals::MovingLabel GameVisuals::resourcesLabel;

unordered_map<string, GameVisuals::Button> GameVisuals::buttons;

void GameVisuals::fillResourcesLabelBySelected() {


	resourcesLabel.texts.clear();
	if (gameState->currentSelectedGameObject == nullptr)
	{
		resourcesLabel.texts.push_back("nothing");
		resourcesLabel.texts.push_back("selected");

	}
	else {
		resourcesLabel.texts.push_back("you selected:");
		resourcesLabel.texts.push_back(gameState->currentSelectedGameObject->getCategory());
		resourcesLabel.texts.push_back("");
		for (int i = 0; i < 5; i++) {
			resourcesLabel.texts.push_back(jsonFilePtr->resourcesTypes[i] + " " + to_string(gameState->currentSelectedGameObject->getResources()[i]));
		}
	}

}



void GameVisuals::fill_game_visuals(shared_ptr<GameState> gameState)
{

	GameVisuals::gameState = gameState;
	GameVisuals::jsonFilePtr = JsonFile::getInstance();

	string base = "..\\images/";
	string baseTiles = base + "TILES\\";
	string baseSettlements = base + "Settlements/";
	string basePeople = base + "People/";
	string baseVehicles = base + "VEHICLES/";
	string baseWalking = base + "People\\WALKING ASTRONOUT\\";

	vector<string> imagesNamesTiles = { "","tile_ground.png","tile_water.png","tile_forest.png","tile_field.png","tile_iron_mine.png","tile_blocks_mine.png" };
	vector<string> imagesNamesSettlements = { "city.png","village.png","tile_road.png" };
	vector<string> imagesNamesVehicles = { "Car.png","truck.png" ,"helicopter.png" };
	vector<string> imagesNamesPaople = { "0.png" };


	category_image_map[GameUtils::GROUND] = imread(baseTiles + imagesNamesTiles[1]);
	category_image_map[GameUtils::WATER] = imread(baseTiles + imagesNamesTiles[2]);
	category_image_map[GameUtils::FOREST] = imread(baseTiles + imagesNamesTiles[3]);
	category_image_map[GameUtils::FIELD] = imread(baseTiles + imagesNamesTiles[4]);
	category_image_map[GameUtils::IRON_MINE] = imread(baseTiles + imagesNamesTiles[5]);
	category_image_map[GameUtils::BLOCKS_MINE] = imread(baseTiles + imagesNamesTiles[6]);

	category_image_map[GameUtils::CITY] = imread(baseSettlements + imagesNamesSettlements[0]);
	category_image_map[GameUtils::VILLAGE] = imread(baseSettlements + imagesNamesSettlements[1]);
	category_image_map[GameUtils::ROAD] = imread(baseSettlements + imagesNamesSettlements[2]);

	category_image_map[GameUtils::CAR] = imread(baseVehicles + imagesNamesVehicles[0]);
	category_image_map[GameUtils::TRUCK] = imread(baseVehicles + imagesNamesVehicles[1]);
	category_image_map[GameUtils::HELICOPTER] = imread(baseVehicles + imagesNamesVehicles[2]);

	category_image_map[GameUtils::PEOPLE] = imread(basePeople + imagesNamesPaople[0]);


	for (int i = 0; i < 11; i++) {
		category_states_map[GameUtils::PEOPLE].push_back(imread(baseWalking + to_string(i) + ".png"));
	}
	// Check if all images are loaded
	for (const auto& anim : category_states_map[GameUtils::PEOPLE]) {
		if (anim.empty()) {
			cerr << "Error: Could not load image for category WALKING PEOPLE" << endl;
		}
	}
	for (const auto& category : category_image_map) {
		if (category.second.empty()) {
			cerr << "Error: Could not load image for category " << category.first << endl;
		}
	}
}

void GameVisuals::addObject(shared_ptr<GameObject> newObject)
{
	//this function is called after a new object is born or an object is moving
	pointsLabel.texts[0] = "Score:" + to_string(gameState->points);
	int cellSize = 30;
	Mat obj;
	int objSize = newObject->getSize().first * cellSize;
	resize(category_image_map[newObject->getCategory()], obj, Size(objSize, objSize), INTER_LINEAR);
	Rect placeForNewObj((newObject->getCoordinates().x - 1) * cellSize, (newObject->getCoordinates().y - 1) * cellSize, objSize, objSize);
	obj.copyTo(main_mat(placeForNewObj));

}
void GameVisuals::addObjectState(shared_ptr<GameObject> newObject)
{
	int cellSize = 30;
	Mat obj;
	int objSize = newObject->getSize().first * cellSize;
	resize(category_states_map[newObject->getCategory()][currentFrameIndex], obj, Size(objSize, objSize), INTER_LINEAR);
	Rect placeForNewObj((newObject->getCoordinates().x - 1) * cellSize, (newObject->getCoordinates().y - 1) * cellSize, objSize, objSize);
	obj.copyTo(main_mat(placeForNewObj));
	currentFrameIndex++;
	currentFrameIndex %= category_states_map[newObject->getCategory()].size();


}
void GameVisuals::addObject(shared_ptr<GameObject> newObject, DirectionsEnum direction)
{
	//this function is called after a new object is born or an object is moving
	int cellSize = 30;
	Mat obj;
	int objSize = newObject->getSize().first * cellSize;
	resize(category_image_map[newObject->getCategory()], obj, Size(objSize, objSize), INTER_LINEAR);
	Rect placeForNewObj((newObject->getCoordinates().x - 1) * cellSize, (newObject->getCoordinates().y - 1) * cellSize, objSize, objSize);
	switch (direction) {
	case DirectionsEnum::RIGHT:
		cv::rotate(obj, obj, cv::ROTATE_90_CLOCKWISE);
		break;
	case DirectionsEnum::DOWN:
		cv::rotate(obj, obj, cv::ROTATE_180);
		break;
	case DirectionsEnum::LEFT:
		cv::rotate(obj, obj, cv::ROTATE_90_COUNTERCLOCKWISE);
		break;
	}

	obj.copyTo(main_mat(placeForNewObj));

}
void GameVisuals::recover(const Coordinates& coord, shared_ptr<GameObject> obj) {

	//this function is called after an object moved , to recover the image under him

	int cellSize = 30;
	//the x,y is part of obj,
	// take if out of obj
	Mat objAsImage;
	int objSize = obj->getSize().first;
	resize(category_image_map[obj->getCategory()], objAsImage, Size(objSize * cellSize, objSize * cellSize), INTER_LINEAR);
	Rect partInCoord((coord.x - obj->getCoordinates().x) * cellSize, (coord.y - obj->getCoordinates().y) * cellSize, cellSize, cellSize);
	Mat extractedSmallPart = objAsImage(partInCoord);
	Rect dstRect((coord.x - 1) * cellSize, (coord.y - 1) * cellSize, cellSize, cellSize);
	extractedSmallPart.copyTo(main_mat(dstRect));
}

void GameVisuals::fillChosenAreaDetails()
{
	vector<int> resources = { 0,0,0,0,0 };
	int cellSize = 30;
	int leftX = min(recBeing->initX, recBeing->x);
	int topY = min(recBeing->initY, recBeing->y);
	int rightX = max(recBeing->initX, recBeing->x);
	int bottomY = max(recBeing->initY, recBeing->y);

	Coordinates leftTop(leftX / cellSize + 1, topY / cellSize + 1);
	Coordinates rightBottom(rightX / cellSize + 1, bottomY / cellSize + 1);
	if (gameState->isOutOfRange(leftTop)) {
		leftTop = Coordinates(1, 1);
	}
	if (gameState->isOutOfRange(rightBottom)) {
		rightBottom = Coordinates(gameState->width, gameState->height);
	}

	vector<shared_ptr<GameObject>> objectsInRect;
	for (const pair<string, vector<shared_ptr<GameObject>>>& category : gameState->gameObjects) {
		if (category.first == GameUtils::PEOPLE || category.first == GameUtils::CAR || category.first == GameUtils::HELICOPTER || category.first == GameUtils::TRUCK) {
			//its a movable object category
			for (int i = 0; i < category.second.size(); i++) {
				shared_ptr<GameObject> movable = category.second[i];
				if (!(Validations::isInArea(leftTop, rightBottom, movable->getCoordinates(), movable->getRightBottomCoordinates())))
					continue;

				//its in the selected area! 
				objectsInArea.push_back(movable);
				for (int i = 0; i < 5; i++) {
					resources[i] += movable->getResources()[i];
				}
			}

		}

	}

	for (int i = 0; i < 5; i++) {
		resourcesLabel.texts.push_back(jsonFilePtr->resourcesTypes[i] + " " + to_string(resources[i]));
	}
}

void GameVisuals::drawGrid(cv::Mat& image, int cellSize, int cellsInTile) {
	int tileSize = cellSize * cellsInTile;
	int rows = image.rows / cellSize;
	int cols = image.cols / cellSize;

	// Draw the main grid (thick lines)
	for (int i = 0; i <= rows; ++i) {
		int thickness = (i % cellsInTile == 0) ? 2 : 1;
		cv::line(image, cv::Point(0, i * cellSize), cv::Point(image.cols, i * cellSize), cv::Scalar(0, 0, 0), thickness);
	}
	for (int j = 0; j <= cols; ++j) {
		int thickness = (j % cellsInTile == 0) ? 2 : 1;
		cv::line(image, cv::Point(j * cellSize, 0), cv::Point(j * cellSize, image.rows), cv::Scalar(0, 0, 0), thickness);
	}

	// Add labels for rows (y-axis) in the first column
	int fontFace = cv::FONT_HERSHEY_SIMPLEX;
	double fontScale = 0.5;
	int thickness = 1;
	int baseline = 0;

	for (int i = 1; i <= rows; ++i) {
		std::string label = std::to_string(i);
		cv::Size textSize = cv::getTextSize(label, fontFace, fontScale, thickness, &baseline);
		cv::putText(image, label, cv::Point(5, i * cellSize - cellSize / 2 + textSize.height / 2), fontFace, fontScale, cv::Scalar(0, 0, 0), thickness);
	}

	// Add labels for columns (x-axis) in the first row
	for (int j = 1; j <= cols; ++j) {
		std::string label = std::to_string(j);
		cv::Size textSize = cv::getTextSize(label, fontFace, fontScale, thickness, &baseline);
		cv::putText(image, label, cv::Point(j * cellSize - cellSize / 2 - textSize.width / 2, textSize.height + 5), fontFace, fontScale, cv::Scalar(0, 0, 0), thickness);
	}
}

void GameVisuals::draw()
{

	//gameState -> window
	//int cellSize = 30;
	//int tilesWidth = game->tiles[0].size();
	//int tilesHeight = game->tiles.size();
	//int tileCells = jsonFilePtr->sizes[GameUtils::TILE].first;
	//int tileSize = tileCells * cellSize;
	//cv::Mat main_image(tilesHeight * tileCells * cellSize, tilesWidth * tileCells * cellSize, CV_8UC3, cv::Scalar(255, 255, 255));





	//for (int i = 0; i < GameUtils::categories.size(); i++) {//each category
	//	string category = GameUtils::categories[i];
	//	if (!jsonFilePtr->isTile(category))
	//	{
	//		for (int j = 0; j < game->gameObjects[category].size(); j++) {//each object from that cetegory
	//			shared_ptr<GameObject>& gameObject = game->gameObjects[category][j];
	//			int objSize = cellSize * gameObject->getSize().first;
	//			Mat mat;
	//			resize(category_image_map[category], mat, Size(objSize, objSize), INTER_LINEAR);
	//			if (gameObject->isMoving()) {

	//			}
	//			else {
	//				cv::Rect roi((gameObject->getCoordinates().x - 1) * cellSize, (gameObject->getCoordinates().y - 1) * cellSize, objSize, objSize);//the place in main image i want the mat to be
	//				mat.copyTo(main_image(roi));
	//			}



	//		}
	//	}

	//}




}

void GameVisuals::show()
{
	static int callCount = 0;
	callCount++;

	int cellSize = 30;
	int tileCells = jsonFilePtr->sizes[GameUtils::TILE].first;
	int tileSize = tileCells * cellSize;


	Mat copyWithRectangleAndLabel = main_mat.clone();
	Mat secondary_copy = secondary_mat.clone();

	pointsLabel.drawLabel(secondary_copy);
	resourcesLabel.drawLabel(secondary_copy);

	if (recBeing != nullptr && recBeing->movedABit) {
		//show current state + the rectangle

		rectangle(copyWithRectangleAndLabel, Point(recBeing->initX, recBeing->initY), Point(recBeing->x, recBeing->y), Scalar(0, 0, 255), 2);
		if (!inTheMiddleOfDrawingRectangle)
			//resourcesLabel.drawLabel(secondary_copy);
			cout << "show with rect" << callCount << endl;
	}
	else {
		//cout << "show " << callCount << endl;
	}
	if (!stopGame) {
		double currentTime = getTickCount();
		double ticksPerFrame = (1.0 / 5) * getTickFrequency();
		if (currentTime - lastTime >= ticksPerFrame)
		{
			//its time to switch frame!
			lastTime = currentTime;
			for (const auto& movingObj : gameState->movingGameObjects) {
				if (movingObj.isPeople) {
					addObjectState(movingObj.gameObject);
				}
			}
		}
	}


	// Check if the heights are the same (they should be as per your description)
	if (copyWithRectangleAndLabel.rows != secondary_copy.rows) {
		std::cerr << "The heights of the matrices are not the same!" << std::endl;

	}
	if (buttons["Grid"].isPushed) {
		drawGrid(copyWithRectangleAndLabel, 30, jsonFilePtr->sizes[GameUtils::TILE].first);
	}


	// Concatenate the two images horizontally
	cv::Mat combined;
	cv::hconcat(copyWithRectangleAndLabel, secondary_copy, combined);

	// Display the combined image
	cv::imshow("world", combined);

	//cv::waitKey(10);
	//setWindowProperty("world", WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN); 
}

bool GameVisuals::isInMainMat(int x, int y) {
	return x >= 0 && x < main_mat.cols && y >= 0 && y < main_mat.rows;
}
bool GameVisuals::isInSecondaryMat(int x, int y) {
	return x >= main_mat.cols && x < main_mat.cols + secondary_mat.cols && y >= 0 && y < secondary_mat.rows;
}

// Mouse callback function
void GameVisuals::onMouse(int event, int x, int y, int, void*) {
	static int callCount = 0;
	int cellSize = 30;

	switch (event) {
	case  EVENT_LBUTTONDOWN:
	{
		cout << "event: l down" << endl;
		if (isInSecondaryMat(x, y)) {
			if (pointsLabel.locationRect.contains(Point(x - main_mat.cols, y))) {
				pointsLabel.isBeingDraggedNow = true;
				pointsLabel.holdOffset = Point(x - main_mat.cols, y) - pointsLabel.locationRect.tl();
			}
			for (unordered_map<string, Button>::iterator iter = buttons.begin(); iter != buttons.end(); iter++) {
				if (iter->second.locationRect.contains(Point(x - main_mat.cols, y))) {

					iter->second.toggle();
					iter->second.drawButton(secondary_mat);

					if (iter->first == "Pouse") {
						if (iter->second.isPushed) {
							stopGame = true;
						}
						else {
							stopGame = false;
						}
					}
					break;
				}
			}
			//if (recBeing != nullptr && !inTheMiddleOfDrawingRectangle) {
			//	//the area exists, maybe the resources label is being dragged
			if (resourcesLabel.locationRect.contains(Point(x - main_mat.cols, y))) {
				resourcesLabel.isBeingDraggedNow = true;
				resourcesLabel.holdOffset = Point(x - main_mat.cols, y) - resourcesLabel.locationRect.tl();
			}
			//}
		}
		else if (isInMainMat(x, y)) {
			if (stopGame) {
				if (recBeing == nullptr) {
					//new rect is starting:
					inTheMiddleOfDrawingRectangle = true;
					recBeing = new Rectangle(x, y);
				}
			}
			else {
				Coordinates coord(x / cellSize + 1, y / cellSize + 1); 

		 		if (gameState->currentSelectedGameObject!=nullptr&& gameState->currentSelectedGameObject->canMove()&&!gameState->currentSelectedGameObject->getIsMoving()) {
					CreateCommand::selecteDstForMovable(coord);   
				}

				CreateCommand::createSelectCommand(coord);
				fillResourcesLabelBySelected();
			}
		}



		break;
	}
	case EVENT_MOUSEMOVE:
	{
		callCount++;
		//cout << "event: move" <<callCount<< endl; 
		//if (!inTheMiddleOfDrawingRectangle && recBeing != nullptr) {
			//the area exists!

		if (resourcesLabel.isBeingDraggedNow) {
			resourcesLabel.locationRect = Rect(Point(x - main_mat.cols, y) - resourcesLabel.holdOffset, resourcesLabel.locationRect.size());
		}
		//}
		else if (pointsLabel.isBeingDraggedNow) {
			pointsLabel.locationRect = Rect(Point(x - main_mat.cols, y) - pointsLabel.holdOffset, pointsLabel.locationRect.size());
		}
		else if (inTheMiddleOfDrawingRectangle) {
			//cout << "updating rect" << endl;
			recBeing->setXYMoved(x, y);
			stopGame = true;//stop moving until a dest is chosen 

			//cout << "3" << endl;

		}
		break;
	}
	case EVENT_LBUTTONUP:
	{
		if (pointsLabel.isBeingDraggedNow)
			pointsLabel.isBeingDraggedNow = false;
		else if (resourcesLabel.isBeingDraggedNow)
			resourcesLabel.isBeingDraggedNow = false;

		cout << "event: l up" << endl;
		if (isInMainMat(x, y) && inTheMiddleOfDrawingRectangle)
		{
			//we just finished drawing the area!
			inTheMiddleOfDrawingRectangle = false;
			fillChosenAreaDetails();

		}
		break;
	}
	case EVENT_RBUTTONDOWN:
		cout << "event: r down" << endl;
		if (!userChoseDst && recBeing != nullptr && recBeing->movedABit) {
			// a rect exists, so this is the destination

			//Coordinates dest(x / cellSize + 1, y / cellSize + 1);

			dest = Coordinates(x / cellSize + 1, y / cellSize + 1);

			//delete recBeing;
			userChoseDst = true;
			//check:get the topLeft and bottom Right, the objects in the rect, than move them
			cout << "a dest is chosen" << endl;
			//  draw a circle at the position
			cv::circle(main_mat, cv::Point(x, y), 10, cv::Scalar(0, 0, 255), -1);
			//cv::imshow("world", main_mat);
		}
		break;
	}

}

void GameVisuals::drawInitMatByWorld(const vector < vector<shared_ptr<GameObject>>>& tiles)
{
	//this function is gets called after the "+World" part
	int cellSize = 30;
	int tilesWidth = tiles[0].size();
	int tilesHeight = tiles.size();
	int tileCells = jsonFilePtr->sizes[GameUtils::TILE].first;
	int tileSize = tileCells * cellSize;
	main_mat = Mat(tilesHeight * tileCells * cellSize, tilesWidth * tileCells * cellSize, CV_8UC3, cv::Scalar(255, 255, 255));

	secondary_mat = Mat(tilesHeight * tileCells * cellSize, 150, CV_8UC3, Scalar(255, 255, 255));

	for (int i = 0; i < tilesHeight; ++i) {
		for (int j = 0; j < tilesWidth; ++j) {
			string category = tiles[i][j]->getCategory();
			cv::Mat tile;
			resize(category_image_map[category], tile, Size(tileSize, tileSize), INTER_LINEAR);
			cv::Rect roi(j * tileSize, i * tileSize, tileSize, tileSize);//the place in main image i want the tile to be

			tile.copyTo(main_mat(roi));
		}
	}

	pointsLabel.texts.push_back("Score:" + to_string(gameState->points));
	resourcesLabel.texts.push_back("nothing");
	resourcesLabel.texts.push_back("selected");


	pointsLabel.locationRect = Rect(0, 0, 110, 30);
	resourcesLabel.locationRect = Rect(0, 250, 150, 200);

	buttons.emplace(std::piecewise_construct, std::forward_as_tuple("Grid"), std::forward_as_tuple(Rect(0, 50, 150, 50), "grid",true));
	buttons.emplace(std::piecewise_construct, std::forward_as_tuple("Pouse"), std::forward_as_tuple(Rect(0, 150, 150, 100), "pouse moving \n to choose area",false));


	for (const pair<string, Button>& button : buttons) {
		button.second.drawButton(secondary_mat);
	}

	cv::namedWindow("world", cv::WINDOW_NORMAL);

	cv::setMouseCallback("world", onMouse);

	lastTime = getTickCount();

}
