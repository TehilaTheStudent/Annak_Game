#ifndef OBJECT_MOVING_HPP
#define OBJECT_MOVING_HPP
#pragma once
#include "GameObject.hpp"
#include "Validations.hpp"
#include <memory>
#include <math.h>
using namespace std;


enum ReasonsEnum { 
	Deposit, Work, Move 
}; 


class ObjectMoving {
public:
	Coordinates src;
	Coordinates dst;
	ReasonsEnum  reason;
	std::shared_ptr<GameObject> gameObject;
	shared_ptr<GameObject> depositIntoIt; 
	double speed;
	double stepsDoneFromStartOfCell;
	Coordinates current;

	int stepsX;
	int stepsY;

	//for person only:
	int deltaX;
	int deltaY;
	int error;
	int error2;
	bool isPeople;
	vector<shared_ptr<GameObject>> roadsInWay;

	int roadImIn;

	
	//for person moving/deposition, 
	ObjectMoving(Coordinates dst, shared_ptr<GameObject> gameObject, ReasonsEnum reason, shared_ptr<GameObject>depositInto = nullptr)
		:src(gameObject->getCoordinates()), dst(dst), gameObject(gameObject), speed(gameObject->getSpeed()), stepsDoneFromStartOfCell(0), reason(reason), current(src), isPeople(gameObject->isPeople()), depositIntoIt(depositInto)
	{
		//cout << "++++" << gameObject->getCategory() << "start move" << endl;
		gameObject->setIsMoving(true);
		stepsX = (src.x < dst.x) ? 1 : -1;     // Step direction in x
		stepsY = (src.y < dst.y) ? 1 : -1;     // Step direction in y
		if (gameObject->isPeople()) {
			deltaX = abs(dst.x - src.x);          // Difference in x (width)
			deltaY = abs(dst.y - src.y);          // Difference in y (height)

			error = deltaX - deltaY;              // Initial error term
		}
	}
	//for driver moving/depositing
	ObjectMoving(Coordinates dst, shared_ptr<GameObject> gameObject, ReasonsEnum reason, vector<shared_ptr<GameObject>> roadsInWay,shared_ptr<GameObject>depositInto=nullptr)
		:src(gameObject->getCoordinates()), dst(dst), gameObject(gameObject), speed(gameObject->getSpeed()), stepsDoneFromStartOfCell(0), reason(reason), current(src), isPeople(gameObject->isPeople()), roadsInWay(roadsInWay), roadImIn(0),depositIntoIt(depositInto)
	{
		gameObject->setIsMoving(true);
	


	}

	bool willLeaveCellAfterNextMove() const {
		return (stepsDoneFromStartOfCell + 1) >= 1 / speed;
	}
	//Coordinates calcNextStep() const {
	//	if (reached()) {
	//		return current;
	//	}
	//	Coordinates next = current;
	//	if (isPeople) {
	//		int tmpError = error;
	//		int tmpError2 = error2;
	//		tmpError2 = 2 * tmpError;
	//		if (tmpError2 > -deltaY) {
	//			tmpError -= deltaY;
	//			next.x += stepsX;
	//		}
	//		if (tmpError2 < deltaX) {
	//			tmpError += deltaX;
	//			next.y += stepsY;
	//		}
	//	}
	//	else {//transportation
	//		const Coordinates& roadIn = roadsInWay[roadImIn]->getCoordinates();
	//		const Coordinates& roadNext = roadsInWay[roadImIn + 1]->getCoordinates();
	//		if (roadIn.x != roadNext.x) {//move <-- or -->
	//			next.x += ((roadIn.x < roadNext.x) ? 1 : -1);
	//		}
	//		else {// move /\ or \/ 
	//			next.y += ((roadIn.y < roadNext.y) ? 1 : -1);
	//		}
	//		if (roadIn.x != roadNext.x && roadIn.y != roadNext.y) {
	//			cout << endl << "something is wrong" << endl;
	//		}
	//	}
	//	return next;
	//}

	bool move() {
		stepsDoneFromStartOfCell++;
		if (isPeople) {//person 

			if (stepsDoneFromStartOfCell >= 1 / speed) {//time to move to next cell   
				error2 = 2 * error;
				if (error2 > -deltaY) {
					error -= deltaY;
					current.x += stepsX;
				}
				if (error2 < deltaX) {
					error += deltaX;
					current.y += stepsY;
				}
				stepsDoneFromStartOfCell -= 1 / speed; 
				return true;
			}
			return false;
		}
		else {//transportation
			if (stepsDoneFromStartOfCell >= 1 / speed) {//time to move to next cell  
				if (reason == ReasonsEnum::Deposit && roadImIn == roadsInWay.size() - 1) {
					//im in the last road before the infrustricture
					const Coordinates& roadIn = roadsInWay[roadImIn]->getCoordinates(); 
					const Coordinates& infrTopLeft = depositIntoIt->getCoordinates();
					const Coordinates& infrBottomRight = depositIntoIt->getRightBottomCoordinates();

					if (roadIn.y < infrTopLeft.y) {
						//down
						current.y++;
					}
					else if (roadIn.y > infrBottomRight.y) {
						//up
						current.y--;
					}
					else if (roadIn.x < infrTopLeft.x) {
						//right
						current.x++;
					}
					else {
						//left
						current.x--;
					}
					stepsDoneFromStartOfCell -= 1 / speed; 
					return true;
				}
				else {
					const Coordinates& roadIn = roadsInWay[roadImIn]->getCoordinates();
					const Coordinates& roadNext = roadsInWay[roadImIn + 1]->getCoordinates();
					if (roadIn.x != roadNext.x) {//move <-- or -->
						current.x += ((roadIn.x < roadNext.x) ? 1 : -1);
					}
					else {// move /\ or \/ 
						current.y += ((roadIn.y < roadNext.y) ? 1 : -1);
					}

					if (roadIn.x != roadNext.x && roadIn.y != roadNext.y) {
						cout << endl << "something is wrong" << endl;
					}
					stepsDoneFromStartOfCell -= 1 / speed;

					if (Validations::isInArea(roadsInWay[roadImIn + 1], current, gameObject->getSize().first)) {
						//im fully inside the next road 
						roadImIn++;
					}

					return true;
				}
				
			}
			return false;
		}

	}

	void changeToCurrentCell() {
		gameObject->setCoordinates(current);
	}
	bool reached() const {
		if (reason == ReasonsEnum::Move) {
			if (isPeople) {
				return current == dst;
			}
			else {
				return roadImIn == roadsInWay.size() - 1;
			}
		}
		else if (reason == ReasonsEnum::Deposit) {
			if (isPeople) {
				
			}
			else {
				return Validations::isInArea(depositIntoIt, gameObject); 
			}
		}

	}
};


#endif //OBJECT_MOVING_HPP