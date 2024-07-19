#ifndef OBJECTS_IN_XY_HPP
#define OBJECTS_IN_XY_HPP

#include <string>
#include <memory>
#include "GameObject.hpp"
#include <cmath>
#include <iostream>
#include "JsonFile.h"
class ObjectsInXY {
public:

	shared_ptr<GameObject> infrastructure; // Corrected spelling from 'infrustructure' to 'infrastructure'
	shared_ptr<GameObject> transportation;
	shared_ptr<GameObject> person;
	shared_ptr<JsonFile> jsonFilePtr;
	bool hasTile;
	// Constructor


	ObjectsInXY() :hasTile(true), jsonFilePtr(JsonFile::getInstance()) {

	}
	//TODO change this structs tso it wont contain the tile, this should be separate as it is aligned 
	// Method to get the top-most object
	shared_ptr<GameObject> getTopMost() const {
		if (person) {
			return person;
		}
		if (transportation) {
			return transportation;
		}
		if (infrastructure) {
			return infrastructure;
		}
		return nullptr;
	}

	// Method to check if the object has infrastructure
	bool hasInfrastructure() const {
		return static_cast<bool>(infrastructure);
	}
	bool hasInfrustructureNotRoad() const {
		return hasInfrastructure() && !isRoad();
	}
	// Method to check if the object has person or transportation
	bool hasPerson() const {
		try {
			return static_cast<bool>(person);
		}
		catch (...) {
			cout << "waht" << endl;
		}

	}
	bool hasTransportation() const {
		return static_cast<bool>(transportation);
	}
	bool isRoad() const {
		if (hasInfrastructure() && infrastructure->getCategory() == "Road")
			return true;
		return false;
	}
	bool takesPlace() const {
		return hasPerson() || hasTransportation() || hasInfrastructure();
	}
	//bool placeToMoveToForVehicle() const {
	//	return !hasPerson()
	//		&& !hasTransportation()
	//		&& !(hasInfrastructure() && !isRoad())
	//		&& hasTile;
	//}
	bool placeToMoveToForVehicle() const {

		bool infrastructurePresent = hasInfrastructure();
		bool roadPresent = isRoad();
		bool tilePresent = hasTile;

		return !hasPerson()
			&& !hasTransportation()
			&& !(infrastructurePresent && !roadPresent)
			&& tilePresent;
	}

	bool placeToMoveToForPerson() const {

		int peopleResourceIndex = jsonFilePtr->resourceTypeIndex["People"];
		return !hasPerson()
			&& !(hasInfrastructure()
				&& !isRoad()
				//&&!infrastructure->canResource(peopleResourceIndex, 1)
				)
			//&& !(hasTransportation() && !transportation->canResource(peopleResourceIndex, 1))
			&& hasTile
			&& !hasTransportation()
			// for now cant go to transportation?...
			;
	}
	bool occupiedByMovingObj() const {
		bool hasMovingPerson = hasPerson() && person->getIsMoving();
		bool hasMovingTransportation = hasTransportation() && transportation->getIsMoving();
		return hasMovingPerson || hasMovingTransportation;
	}
};
#endif // OBJECTS_IN_XY_HPP