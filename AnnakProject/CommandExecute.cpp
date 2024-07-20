#include "CommandExecute.h"
#include <vector>
#include "Coordinates.h"
#include <memory>
#include "CategoriesConstants.h"
#include "Validations.hpp"
#include "GameVisuals.h" 
#include "ObjectMoving.h"

using namespace std;

// Define the static member variables
std::shared_ptr<GameState> CommandExecute::game;
std::unordered_map<std::string, std::function<void(const std::vector<std::string>&)>> CommandExecute::commandMap;
std::unordered_map<std::string, std::function<void(const std::vector<std::string>&)>> CommandExecute::specialForStartCommandMap;

std::shared_ptr<JsonFile> CommandExecute::jsonFilePtr;

void  CommandExecute::fill_command_execute(const std::shared_ptr<GameState>& game) {
	CommandExecute::game = game;
	CommandExecute::jsonFilePtr = JsonFile::getInstance();
	commandMap[WAIT] = [](const std::vector<std::string>& args) { wait(args); };
	commandMap[SELECT] = [](const std::vector<std::string>& args) { select(args); };
	commandMap[MOVE] = [](const std::vector<std::string>& args) { move(args); };
	commandMap[WORK] = [](const std::vector<std::string>& args) { work(args); };
	commandMap[DEPOSIT] = [](const std::vector<std::string>& args) { deposit(args); };
	commandMap[TAKE_RESOURCES] = [](const std::vector<std::string>& args) { takeResources(args); };
	commandMap[BUILD] = [](const std::vector<std::string>& args) { build(args); };
	commandMap[MANUFACTURE] = [](const std::vector<std::string>& args) { manufacture(args); };
	commandMap[PEOPLE] = [](const std::vector<std::string>& args) { people(args); };
	commandMap[RESOURCE] = [](const std::vector<std::string>& args) { resource(args); };
	commandMap[RESOURCES] = [](const std::vector<std::string>& args) { resources(args); };
	commandMap[MAKE_EMPTY] = [](const std::vector<std::string>& args) { makeEmpty(args); };
	commandMap[RAIN] = [](const std::vector<std::string>& args) { rain(args); };
	commandMap[ROBBER] = [](const std::vector<std::string>& args) { robber(args); };
	commandMap[MAKE_ROBBER] = [](const std::vector<std::string>& args) { makeRobber(args); };
	commandMap[SET_POINTS] = [](const std::vector<std::string>& args) { setPoints(args); };
	//commandMap[MOVE_TO_DESTINATION] = [](const std::vector<std::string>& args) { moveToDestination(args); };
	commandMap[MOVE_IT] = [](const std::vector<std::string>& args) { moveIt(args); };

	specialForStartCommandMap[BUILD] = [](const std::vector<std::string>& args) { buildStart(args); };
	specialForStartCommandMap[MANUFACTURE] = [](const std::vector<std::string>& args) { manufactureStart(args); };




}


void CommandExecute::executeCommand(shared_ptr<Command> command, bool start) {

	if (start) {
		auto it = specialForStartCommandMap.find(command->name);
		if (it != specialForStartCommandMap.end()) {
			it->second(command->arguments);
			return;
		}
	}

	const std::string& name = command->name;
	auto it = commandMap.find(name);
	if (it != commandMap.end()) {
		it->second(command->arguments);
	}
	else {
		//std::cerr << "Unknown command: " << command << std::endl;
	}

}

void CommandExecute::wait(const std::vector<std::string>& args) {
	//std::cout << "Executing wait with steps: " << args[0] << std::endl;
	//nothing?
}

void CommandExecute::select(const std::vector<std::string>& args) {
	// std::cout << "Executing select at coordinates: " << args[0] << ", " << args[1] << std::endl;


	int x = std::stoi(args[0]);
	int y = std::stoi(args[1]);
	// Logic to select the top-most entity at (x, y)

	shared_ptr<GameObject> gameObject = game->getTopMostGameObject(Coordinates(x, y));
	game->currentSelectedGameObject = gameObject;
	// std::cout << "Selected GameObject at (" << x << ", " << y << ")" << std::endl;


	//  std::cerr << "No GameObject found at (" << x << ", " << y << ")" << std::endl;


}

void CommandExecute::move(const std::vector<std::string>& args) {
	//car,helicopter,truck,people
	//Move 36 6



	Coordinates coordDst(args[0], args[1]);
	shared_ptr<GameObject> currentSelected = game->currentSelectedGameObject;


	moveObject(currentSelected, coordDst);




	//std::cout << "Executing move to coordinates: " << args[0] << ", " << args[1] << std::endl;
}

void CommandExecute::work(const std::vector<std::string>& args) {
	//people
	//Work 2 2
	shared_ptr<GameObject> currentSelected = game->currentSelectedGameObject;
	if (currentSelected == nullptr || !currentSelected->isPeople()) {
		return;
	}

	ObjectMoving workingPerson(Coordinates(args[0], args[1]), currentSelected, ReasonsEnum::Work);
	game->startMovingObject(workingPerson);
	//std::cout << "Executing work at coordinates: " << args[0] << ", " << args[1] << std::endl;
}

void CommandExecute::deposit(const std::vector<std::string>& args) {
	//from selected (car,people? ) to coord (infrustructure/transportation)
	//from selected to x,y
	Coordinates coord(args[1], args[1]);
	//subtruct from w,x

	shared_ptr<GameObject> selected = game->currentSelectedGameObject;
	if (selected == nullptr || !selected->canMove() || selected->getIsMoving()) {
		return;
	}

	if (!game->canDepositThere(coord)) {
		//is has to be villlage/city/transportation not moving
		return;
	}

	shared_ptr<GameObject> into;
	if (game->getObjectsInXYInCoordinates(coord)->hasInfrustructureNotRoad()) {
		//into infrustructure
		into = game->getObjectsInXYInCoordinates(coord)->infrastructure;
	}
	else {
		into = game->getObjectsInXYInCoordinates(coord)->transportation;
	}

	if (!into->canAddResource(selected->getResources())) {
		//it has to have anough capacity to deposit all my resources there
		return;
	}

	if (selected->isPeople()) {
		//people deposit into transportation/infrustructure 

		ObjectMoving movingObject(coord, selected, ReasonsEnum::Deposit,into); 
		game->startMovingObject(movingObject);
	}
	else {//its tansportation into infrustructure (not road!)
		if (!into->isInfrusctuctureNotRoad())
			return;
		shared_ptr<GameObject> roadUnderMe = game->getObjectsInXYInCoordinates(selected->getCoordinates())->infrastructure;
		vector<shared_ptr<GameObject>> roadsAroundDst = game->roadsAroundInfrustructure(into->getCoordinates(),roadUnderMe->getSize().first,into->getSize().first);
		if (roadUnderMe == nullptr || roadsAroundDst.size()==0) {  
			cout << endl << "ERROR no road under me, or no roads around destination!" << endl;
			return;
		}
		vector<shared_ptr<GameObject>> pathOfRoads = game->roadsGraph.findWay(roadUnderMe, roadsAroundDst); 
		if (pathOfRoads.size() == 0) {
			cout << endl << "ERROR no way of roads" << endl;
			return;
		}

		ObjectMoving movingObject(coord, selected, ReasonsEnum::Deposit, pathOfRoads,into); 
		game->startMovingObject(movingObject);
	}
	//selected->transferResourcesToAnother(*into); 



	//	std::cout << "Executing deposit at coordinates: " << args[0] << ", " << args[1] << std::endl;
}

void CommandExecute::takeResources(const std::vector<std::string>& args) {
	//from coord to selected (transportation)
	//from (infrustructure) into transportation (the selected) people also?
	Coordinates coord(args[0], args[1]);
	shared_ptr<GameObject> toSelected = game->currentSelectedGameObject;
	if (toSelected == nullptr) {
		return;
	}
	if (game->objectsInXYNullPtr(coord) || game->getObjectsInXYInCoordinates(coord)->getTopMost() == nullptr) {
		//nothing 
		return;
	}
	shared_ptr<GameObject> fromInCoord = game->getTopMostGameObject(coord);

	fromInCoord->transferResourcesToAnother(*toSelected);
	// 
	//just set the coord to have resources like the selected?
	//fromInCoord->setResources(toSelected->getResources());

	//std::cout << "Executing take resources from coordinates: " << args[0] << ", " << args[1] << std::endl;
}
//TODO check if area is ground before building
void CommandExecute::build(const std::vector<std::string>& args) {
	//Build City 1 1
	string category = args[0];
	Coordinates coord(args[1], args[2]);
	int objSide;
	objSide = jsonFilePtr->sizes[category].first;

	if (game->isOutOfRange(coord, objSide) || !Validations::isEmptyGround(game->tiles, game->worldMatrix, objSide, coord)) {
		//ERROR
		return;
	}
	int roadSide = jsonFilePtr->sizes[CategoriesConstants::ROAD].first;
	if (category != CategoriesConstants::ROAD && !Validations::isThereRoadAround(game->worldMatrix, objSide, roadSide, coord)) {
		//no road for village/ city
		return;

	}

	vector<int> resources = { 0,0,0,0,0 };
	if (jsonFilePtr->cateoryAmountResourceIndex.count(category) > 0) {
		//get initial resources
		resources[jsonFilePtr->cateoryAmountResourceIndex[category].second] = jsonFilePtr->cateoryAmountResourceIndex[category].first;
	}
	int time = 0;
	bool complete = true;
	if (jsonFilePtr->times.count(category) > 0) {
		//it should take some time!
		complete = false;
	}



	shared_ptr<GameObject> newInfrustructure = make_shared<GameObject>(category, resources, coord, complete, false);//ASK how much time //complete is false, so when certain time will pass it will be completed
	game->points += newInfrustructure->getScore();
	if (category == CategoriesConstants::ROAD) {
		game->roadsGraph.addVertice(newInfrustructure);
	}
	game->counts[newInfrustructure->getCategory()]++;//although its not completed yet  
	game->startBuildingObject(newInfrustructure);
	game->addNewGameObjectToGameObjects(newInfrustructure);
	game->addNewGameObjectToWorld(newInfrustructure, coord);
	//std::cout << "Input-Executing build of category " << category << " at coordinates: " << args[1] << ", " << args[2] << std::endl;



}
void CommandExecute::buildStart(const std::vector<std::string>& args) {
	//Build City 1 1
	string category = args[0];
	vector<int> resources = { 0,0,0,0,0 };
	Coordinates coord(args[1], args[2]);
	int objSide;
	objSide = jsonFilePtr->sizes[category].first;

	if (game->isOutOfRange(coord, objSide) || !Validations::isEmptyGround(game->tiles, game->worldMatrix, objSide, coord)) {
		//ERROR
		return;
	}
	if (jsonFilePtr->cateoryAmountResourceIndex.count(category) > 0) {
		//get initial resources
		resources[jsonFilePtr->cateoryAmountResourceIndex[category].second] = jsonFilePtr->cateoryAmountResourceIndex[args[0]].first;

	}


	shared_ptr<GameObject> newInfrustructure = make_shared<GameObject>(category, resources, coord, true, false);

	game->points += newInfrustructure->getScore();
	game->counts[newInfrustructure->getCategory()]++;
	game->addNewGameObjectToGameObjects(newInfrustructure);
	game->addNewGameObjectToWorld(newInfrustructure, coord);
	GameVisuals::addObject(newInfrustructure);

	if (category == CategoriesConstants::ROAD) {
		game->roadsGraph.addVertice(newInfrustructure);
		vector<shared_ptr<GameObject>> roadsAround = game->roadsAroundInfrustructure(coord, newInfrustructure->getSize().first, newInfrustructure->getSize().first);
		for (shared_ptr<GameObject> road : roadsAround) {
			game->roadsGraph.addEdge(newInfrustructure, road);
		}
	}

	//	std::cout << "Start---Executing build of category " << args[0] << " at coordinates: " << args[1] << ", " << args[2] << std::endl;
}
void CommandExecute::manufacture(const std::vector<std::string>& args) {
	//TODO add transportation obj into the infrustructure
	//Manufacture $category $x $y
	Coordinates coord(args[1], args[2]);

	if (game->objectsInXYNullPtr(coord) || !game->getObjectsInXYInCoordinates(coord)->hasInfrustructureNotRoad()) {
		return;
		//there is no infrustructure there? can i manufacture on road?
	}

	shared_ptr<GameObject> existing = game->worldMatrix[coord.y - 1][coord.x - 1]->infrastructure;

	string category = args[0];
	vector<int> cost = jsonFilePtr->access()["Costs"][category].get<vector<int>>();
	if (!existing->canSubtructResources(cost)) {
		return;
	}
	existing->subtructResources(cost);
	shared_ptr<GameObject> newTransportation = make_shared<GameObject>(category, vector<int>{0, 0, 0, 0, 0}, coord, true, false);
	existing->addResourceObject(newTransportation);

	//game->addNewGameObjectToWorld(newTransportation, coord);
	game->addNewGameObjectToGameObjects(newTransportation);
	game->counts[category]++;
	//std::cout << "input--Executing manufacture of category " << args[0] << " at coordinates: " << args[1] << ", " << args[2] << std::endl;
}

void CommandExecute::manufactureStart(const std::vector<std::string>& args)
{
	//Manufacture $category $x $y
	string category = args[0];
	Coordinates coord(args[1], args[2]);
	shared_ptr<GameObject> newTransportation = make_shared<GameObject>(category, vector<int>{0, 0, 0, 0, 0}, coord, true, false);
	game->addNewGameObjectToGameObjects(newTransportation);

	if (!game->objectsInXYNullPtr(coord) && game->getObjectsInXYInCoordinates(coord)->hasInfrustructureNotRoad()) {
		//there is an infrustructure not road
		shared_ptr<GameObject> existing = game->getObjectsInXYInCoordinates(coord)->infrastructure;
		existing->addResourceObject(newTransportation);
	}
	else {//not into infrustructure
		game->addNewGameObjectToWorld(newTransportation, coord);
		//GameVisuals::addObject(newTransportation);
	}

	game->counts[category]++;


	//std::cout << "start--Executing manufacture of category " << args[0] << " at coordinates: " << args[1] << ", " << args[2] << std::endl;

}

void CommandExecute::people(const std::vector<std::string>& args) {
	//People 1 6 6
	//People $number $x $y
	//whats in x,y? is there insfrustructure with capacity? check the capacity for: 
	//"Village"-infrustructure
	//"City"-infrustructure
	//"Car" -stansportation
	//"Truck" -stansportation
	//"Helicopter" -stansportation
	Coordinates coords(args[1], args[2]);
	if (!game->placeToMoveToForPerson(coords)) {
		//no tile/water tile/transportation/infrustructure no road that cant resource
		return;
	}
	shared_ptr<GameObject> gameObject = game->getTopMostGameObject(coords);//ASK and ssume resource is to top most

	int amount = stoi(args[0]);
	int resourceTypeIndex = jsonFilePtr->resourceTypeIndex[CategoriesConstants::PEOPLE];
	if (!gameObject->isInfrusctuctureNotRoad() && !gameObject->isTransportation() && amount == 1) {//probably 1? , creating 1 people
		shared_ptr<GameObject> newPerson = make_shared<GameObject>(CommandExecute::PEOPLE, vector<int>{0, 0, 0, 0, 0}, coords, true, false);
		game->counts[CategoriesConstants::PEOPLE]++;
		game->addNewGameObjectToGameObjects(newPerson);
		game->addNewGameObjectToWorld(newPerson, coords);
		//GameVisuals::addObject(newPerson);
	}
	else if (gameObject->isInfrusctuctureNotRoad() || gameObject->isTransportation()) {//infrustructure->except road!/ transporation, no creating new people???
		int peopleToAdd = amount;
		if (gameObject->hasCapacities()) {//car has no capacity
			int peopleCapacity = jsonFilePtr->access()["Capacities"][gameObject->getCategory()][resourceTypeIndex].get<int>();

			if (!gameObject->canAddResource(resourceTypeIndex, amount)) {
				//so put as most as i can
				peopleToAdd = peopleCapacity;

			}
		}
		gameObject->setMostResource(resourceTypeIndex, peopleToAdd);
		game->counts[CategoriesConstants::PEOPLE] += peopleToAdd;
		//ASK should i create new gameObject?????
	}

	//shoult it add people to tile and infrastructure? no
	//ASK


		//std::cout << "Executing people command with number: " << args[0] << " at coordinates: " << args[1] << ", " << args[2] << std::endl;

}

void CommandExecute::resource(const std::vector<std::string>& args) {
	//Resource
	//1 Wood 1 1
	//$amount $kind $x $y
	//get the entity to resource
	//check if has capacity
	//resource it
	int x = stoi(args[2]);
	int y = stoi(args[3]);
	Coordinates coord(x, y);
	shared_ptr<GameObject> gameObjectToResource = game->getTopMostGameObject(coord);
	if (gameObjectToResource == nullptr) {
		return;
	}
	int resourceTypeIndex = jsonFilePtr->resourceTypeIndex[args[1]];
	int amountToAdd = stoi(args[0]);
	if (jsonFilePtr->cateoryAmountResourceIndex.count(gameObjectToResource->getCategory()) > 0 || jsonFilePtr->access()["Capacities"].contains(gameObjectToResource->getCategory())) {
		//has stating or capacities
		//TODO check if this resource matches his
		gameObjectToResource->setMostResource(resourceTypeIndex, amountToAdd);

	}

	//std::cout << "Executing resource command with amount: " << args[0] << ", kind: " << args[1] << " at coordinates: " << args[2] << ", " << args[3] << std::endl;
}

void CommandExecute::resources(const std::vector<std::string>& args) {
	//Resource
// 0 0 5 0 1 1
//$amount $kind $x $y
//get the entity to resource
//check if has capacity
	//resource it
	int x = stoi(args[4]);
	int y = stoi(args[5]);
	vector<int> resources = { stoi(args[0]),stoi(args[1]),stoi(args[2]),stoi(args[3]),0 };
	Coordinates coord(x, y);
	shared_ptr<GameObject> gameObjectToResource = game->getTopMostGameObject(coord);
	if (gameObjectToResource == nullptr) {
		return;
	}

	gameObjectToResource->setMostResource(resources);





	//std::cout << "Executing resources command with wood: " << args[0] << ", wool: " << args[1] << ", iron: " << args[2] << ", blocks: " << args[3] << " at coordinates: " << args[4] << ", " << args[5] << std::endl;
}

void CommandExecute::makeEmpty(const std::vector<std::string>& args) {
	//MakeEmpty $x $y
	Coordinates coords(args[1], args[1]);
	if (!game->objectsInXYNullPtr(coords) && game->getObjectsInXYInCoordinates(coords)->hasInfrastructure()) {
		shared_ptr<GameObject> infrustructure = game->getObjectsInXYInCoordinates(coords)->infrastructure;
		infrustructure->setResources(vector<int>{0, 0, 0, 0, 0});
		infrustructure->resetTransportationCounters();
		//TODO set helicopter?
	}

	//	std::cout << "Executing make empty at coordinates: " << args[0] << ", " << args[1] << std::endl;
}

void CommandExecute::rain(const std::vector<std::string>& args) {
	game->startRaining(stoi(args[0]));
	//std::cout << "Executing rain for steps: " << args[0] << std::endl;
}

void CommandExecute::robber(const std::vector<std::string>& args) {
	std::cout << "Executing robber command" << std::endl;
}

void CommandExecute::makeRobber(const std::vector<std::string>& args) {
	std::cout << "Executing make robber at coordinates: " << args[0] << ", " << args[1] << std::endl;
}

void CommandExecute::setPoints(const std::vector<std::string>& args)
{
	game->points += stoi(args[0]);
}

void CommandExecute::moveToDestination(const Coordinates& dest, vector<shared_ptr<GameObject>> objectsInRect)
{

	//TODO should i check whats in the dst?


	for (shared_ptr<GameObject> movable : objectsInRect) {

		if (movable->getIsMoving()) {
			//its in the middle of moving... it has to change destinaton
			for (list<ObjectMoving>::iterator movingObj = game->movingGameObjects.begin(); movingObj != game->movingGameObjects.end(); movingObj++) {
				if (!(movingObj->gameObject == movable))
					continue;
				movingObj = game->movingGameObjects.erase(movingObj);
				moveObject(movable, dest);
				break;
			}
		}
		else {//its not moving now
			vector<string> args = { to_string(movable->getCoordinates().x),to_string(movable->getCoordinates().y),to_string(dest.x),to_string(dest.y) };
			moveIt(args);
		}
	}



}


void CommandExecute::moveObject(shared_ptr<GameObject> objToMove, const Coordinates& coordDst) {


	if (objToMove == nullptr || !objToMove->canMove()) {
		return;
	}



	if (objToMove->isPeople()) {//move / deposit
		if (!game->occupiedByMovingObject(coordDst) && !game->placeToMoveToForPerson(coordDst)) {
			return;
		}
		if (!game->objectsInXYNullPtr(coordDst) && game->getObjectsInXYInCoordinates(coordDst)->hasInfrastructure()) {//person moving to infrustructure
			shared_ptr<GameObject> theInfrustructureInDst = game->getObjectsInXYInCoordinates(coordDst)->infrastructure;
			int peopleResourceTypeIndex = jsonFilePtr->resourceTypeIndex[CategoriesConstants::PEOPLE];
			if (theInfrustructureInDst->hasCapacities() && theInfrustructureInDst->getCapacities()[peopleResourceTypeIndex] == theInfrustructureInDst->getResources()[peopleResourceTypeIndex]) {
				//cant move to infrustructure full?
				return;
			}
		}

		ObjectMoving movingObject(coordDst, objToMove, ReasonsEnum::Move);
		game->startMovingObject(movingObject);
	}
	else {
		if (!game->occupiedByMovingObject(coordDst) && !game->placeToMoveToForVehicle(coordDst,objToMove))
			return;
		shared_ptr<GameObject> roadUnderMe = game->getObjectsInXYInCoordinates(objToMove->getCoordinates())->infrastructure;
		shared_ptr<GameObject> roadInDst = game->getObjectsInXYInCoordinates(coordDst)->infrastructure;
		if (roadUnderMe == nullptr || roadInDst == nullptr) {
			cout << endl << "ERROR no road" << endl;
			return;
		}
		vector<shared_ptr<GameObject>> roadInDstList = { roadInDst };
		vector<shared_ptr<GameObject>> pathOfRoads = game->roadsGraph.findWay(roadUnderMe, roadInDstList);  
		if (pathOfRoads.size() == 0) {
			cout << endl << "ERROR no way of roads" << endl;
			return;
		}
		if (pathOfRoads.size() == 1) {
			cout << endl << "ERROR same road" << endl;
			return;
		}
		ObjectMoving movingObject(coordDst, objToMove, ReasonsEnum::Move, pathOfRoads);
		game->startMovingObject(movingObject);
	}


	//std::cout << "Executing move to coordinates: " << args[0] << ", " << args[1] << std::endl;
}

void CommandExecute::moveIt(const std::vector<std::string>& args)
{
	//car,helicopter,truck,people
//Movet 1 1 36 6-> selected, dst


	Coordinates coordFrom(args[0], args[1]);
	Coordinates coordDst(args[2], args[3]);
	shared_ptr<GameObject> inFrom = game->getTopMostGameObject(coordFrom);

	moveObject(inFrom, coordDst);
}


// Wait $steps
// The execution of following commands should not happen until the given number of $steps
const std::string CommandExecute::WAIT = "Wait";

// Select $x $y
// Should select the top-most entity on a given $x $y world location
const std::string CommandExecute::SELECT = "Select";

// Move $x $y
// Should issue a move command to the currently selected entity at the $x $y world location
const std::string CommandExecute::MOVE = "Move";

// Work $x $y
// Should issue a work command to the currently selected entity at the $x $y world location
const std::string CommandExecute::WORK = "Work";

// Deposit $x $y
// Should issue a deposit resources command to the currently selected entity at the $x $y world location
const std::string CommandExecute::DEPOSIT = "Deposit";

// TakeResources $x $y
// Should issue a take resources command to the currently selected entity from the $x $y world location
const std::string CommandExecute::TAKE_RESOURCES = "TakeResources";

// Build $category $x $y
// Should place an incomplete infrastructure of $category at the $x $y world location
// This command may be enforced, meaning it should place a complete infrastructure without resource usage
const std::string CommandExecute::BUILD = "Build";

// Manufacture $category $x $y
// Should place a transportation of $category manufactured at the $x $y world location
// The manufacturing should happen in the infrastructure at the given location if possible (have capacity and enough resources)
// This command may be enforced, meaning it should place the transportation without the appropriate infrastructure and without resource usage
const std::string CommandExecute::MANUFACTURE = "Manufacture";

// People $number $x $y
// Should place $number of people at the $x $y world location
// Should happen in the infrastructure at the given location if possible (have capacity) or simply at the world location if no infrastructure exists
const std::string CommandExecute::PEOPLE = "People";

// Resource $amount $kind $x $y
// Should set the $kind of resource to $amount for the entity at the given location if possible (have capacity)
const std::string CommandExecute::RESOURCE = "Resource";

// Resources $wood $wool $iron $blocks $x $y
// Should set the $wood, $wool, $iron, and $blocks for the entity at the given location if possible (have capacity)
const std::string CommandExecute::RESOURCES = "Resources";

// MakeEmpty $x $y
// Should set resources, people and transportation of an infrastructure to zero at the $x $y world location
const std::string CommandExecute::MAKE_EMPTY = "MakeEmpty";

// Rain $steps
// Should start raining for the given number of $steps
const std::string CommandExecute::RAIN = "Rain";

// Robber
// A robber should leave the robber city
const std::string CommandExecute::ROBBER = "Robber";

// MakeRobber $x $y
// Should turn the entity at the given location into a robber
const std::string CommandExecute::MAKE_ROBBER = "MakeRobber";
//const std::string CommandExecute::MOVE_TO_DESTINATION = "MoveToDestination";
const std::string CommandExecute::MOVE_IT = "MoveIt";
const std::string CommandExecute::SET_POINTS = "SetPoints";