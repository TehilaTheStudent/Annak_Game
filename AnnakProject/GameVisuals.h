#pragma once
#ifndef GAME_VISUALS_H
#define GAME_VISUALS_H 
#include <memory>
#include "GameObject.hpp"
#include "JsonFile.h"
#include "Command.h"
#include <vector>
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include "DirectionsEnum.h"



using namespace std;
using namespace cv;


class GameState;

class GameVisuals {
public:

	static Mat main_mat;
	static Mat secondary_mat;

	// Structure to hold rectangle properties
	struct Rectangle {
		int initX, initY, x, y;
		bool movedABit;
		cv::Scalar color;
		Rectangle(int initX, int  initY) :initX(initX), initY(initY), movedABit(false) {}

		void setXYMoved(int x, int y) {
			this->x = x;
			this->y = y;
			this->movedABit = true;
		}
	};
	struct Button { 
		Rect locationRect;  
		bool isPushed;
		string label; 
		Scalar onColor;
		Scalar offColor;
		bool toggle() {

			return isPushed = !isPushed;
		}
		void drawButton(Mat & copy) const{
			if (isPushed) {
				rectangle(copy, locationRect, onColor, -1);
			}
			else {
				rectangle(copy, locationRect, offColor, -1);
			}
			putText(copy, label, Point(locationRect.x + 2, locationRect.y + locationRect.height / 2),FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 255, 255), 2);
		}
		Button(Rect rect, string label,bool isPushed) :locationRect(rect), label(label),onColor(Scalar(0, 255, 0)),offColor(Scalar(0, 0, 255)),isPushed(isPushed) {}
		Button() {
			//cout << "def ctor" << endl;
		}
	};
	static unordered_map<string,Button> buttons; 
	struct MovingLabel {
		bool isBeingDraggedNow;
		Point holdOffset;
		Rect locationRect;
		vector<string> texts;
		void drawLabel(Mat& copy) {
				
			rectangle(copy, locationRect, Scalar(255, 0, 0), -1);
			for (int i = 0; i < texts.size(); i++) {
				cv::putText(copy, texts[i], locationRect.tl() + cv::Point(0,(i+1)* 20), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 2);
			}
		}
		MovingLabel() :isBeingDraggedNow(false) {
		}
	};
	static Rectangle* recBeing;
	static MovingLabel pointsLabel;
	static MovingLabel resourcesLabel;


	static  std::shared_ptr<GameState> gameState;
	static void fillChosenAreaDetails();
	static shared_ptr<JsonFile> jsonFilePtr;
	static double lastTime;
	static void drawGrid(cv::Mat& image, int cellSize, int cellsInTile);
	static 	unordered_map<string, Mat> category_image_map;
	static map<string, vector<Mat>>  category_states_map;
	static int currentFrameIndex;
	static bool isInMainMat(int x, int y);
	static bool isInSecondaryMat(int x, int y);

public:

	static bool inTheMiddleOfDrawingRectangle;
	static bool	userChoseDst;
	static Coordinates dest;
	static vector<shared_ptr<GameObject>> objectsInArea;

	

	static bool generateNewMoving; 
	static bool stopGame; 
	static bool mouseClicked; 
	static void fillResourcesLabelBySelected();
	static void fill_game_visuals(shared_ptr<GameState> gameState);
	static void addObject(Mat & copy,shared_ptr<GameObject> newObject);

	static void addObjectState(Mat& copy, shared_ptr<GameObject> newObject);

	static void addAllMovables(Mat& copy);

	static void addObject(shared_ptr<GameObject> newObject);



	static void recover(const Coordinates& coord, shared_ptr<GameObject> obj);
	static void draw();

	static void show();
	static void onMouse(int event, int x, int y, int, void*);

	static void drawInitMatByWorld(const vector < vector<shared_ptr<GameObject>>>& tiles);






};



#endif //GAME_VISUALS_H