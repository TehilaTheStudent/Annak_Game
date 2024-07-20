// AnnakProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>


#include "GameControl.h"
using namespace std;





int main(int argc, char* argv[])
{
	GameControl gameControl;

	gameControl.doGameLoop();


//#ifndef VPL_MODE
//	// Check for a command-line argument
//	if (!(argc > 1 && std::string(argv[1]) == "--test")) {
//		GameUtils::usePrintUtils(gameState->tiles, gameState->worldMatrix, gameState->gameObjects, gameState->currentSelectedGameObject != nullptr ? gameState->currentSelectedGameObject->getCoordinates() : Coordinates(-1, -1));
//
//	}
//#endif 



}
