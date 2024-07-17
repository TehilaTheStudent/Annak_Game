#ifndef OBJECT_MOVING_HPP
#define OBJECT_MOVING_HPP
#pragma once
#include "GameObject.hpp"
#include <memory>
#include <math.h>
using namespace std;


class ObjectMoving {
public:
	Coordinates src;
	Coordinates dst;
	string reason;
	std::shared_ptr<GameObject> gameObject;
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




	ObjectMoving(Coordinates dst, shared_ptr<GameObject> gameObject, const string& reason)
		:src(gameObject->getCoordinates()), dst(dst), gameObject(gameObject), speed(gameObject->getSpeed()), stepsDoneFromStartOfCell(0), reason(reason), current(src), isPeople(gameObject->isPeople())
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
	void changeDest(const Coordinates & newDst, const string& reason) {
		dst = newDst;
		src = gameObject->getCoordinates();
		stepsDoneFromStartOfCell = 0;
		this->reason = reason;
		current = src;
		stepsX = (src.x < dst.x) ? 1 : -1;     // Step direction in x 
		stepsY = (src.y < dst.y) ? 1 : -1;     // Step direction in y 
		if (isPeople) {
			deltaX = abs(dst.x - src.x);          // Difference in x (width) 
			deltaY = abs(dst.y - src.y);          // Difference in y (height) 

			error = deltaX - deltaY;              // Initial error term 
		}
	}
	bool willLeaveCellAfterNextMove() const {
		return (stepsDoneFromStartOfCell + 1) >= 1 / speed;
	}
	Coordinates calcNextStep() const {
		if (isPeople) {
			Coordinates next = current;
			int tmpError = error;
			int tmpError2 = error2;
			tmpError2 = 2 * tmpError;
			if (tmpError2 > -deltaY) {
				tmpError -= deltaY;
				next.x += stepsX;
			}
			if (tmpError2 < deltaX) {
				tmpError += deltaX;
				next.y += stepsY;
			}
			return next;
		}
		else {//transportation
			if (current.x != dst.x) {
				return Coordinates(current.x + stepsX, current.y);
			}
			else {
				return Coordinates(current.x, current.y + stepsY);
			}
		}
	}

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
				if (current.x != dst.x) {
					current.x += stepsX;
				}
				else {
					current.y += stepsY;
				}
				if (current.x != gameObject->getCoordinates().x && current.y != gameObject->getCoordinates().y) {
					cout << "something is wrong" << endl;
				}
				stepsDoneFromStartOfCell -= 1 / speed;
				return true;
			}
			return false;
		}

	}

	void changeToCurrentCell() {
		gameObject->setCoordinates(current);
	}
	bool reached() {
		return current == dst;
	}
};
#endif //OBJECT_MOVING_HPP