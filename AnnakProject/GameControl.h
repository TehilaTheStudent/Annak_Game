#ifndef GAME_CONTROL_H
#define GAME_CONTROL_H

#include "CommandExecute.h"
#include "Asserts.hpp"
#include <memory>
#include "JsonFile.h"
#include <vector>
#include <string>
#include "HelpHeader.h"
#include "GameVisuals.h"
#include <random>
#include "CreateCommand.h"



//#define VPL_MODE
class GameControl {






public:
	GameControl() {

	}
	void doGameLoop() {


#ifdef VPL_MODE 
		Input inp("", "");
		JsonFile::getInstance("configuration.json");
#else
		JsonFile::getInstance("C:\\learn\\CrackTheCode\\Annak\\AnnakProject_Solution\\x64\\Debug\\configuration.json");
		Input inp("C:\\learn\\CrackTheCode\\Annak\\AnnakProject_Solution\\x64\\Debug\\test.txt", "");
#endif
		// Create a random number generator
		std::random_device rd;  // Seed generator 
		std::mt19937 gen(rd()); // Mersenne Twister engine seeded with rd() 



		inp.parse_and_store();

		shared_ptr<GameState>  gameState = make_shared<GameState>();
		GameVisuals::fill_game_visuals(gameState);
		CommandExecute::fill_command_execute(gameState);
		Asserts::fill_assert(gameState);
		CreateCommand::fill_create_command(gameState); 
		
		gameState->addWorldToWorldMatrix(inp);
		GameVisuals::drawInitMatByWorld(gameState->tiles);


		// Define the distribution range
		std::uniform_int_distribution<> disY(1, gameState->height);
		std::uniform_int_distribution<> disX(1, gameState->width);

		int leftToWait = 0;

		for (int i = 0; i < inp.start.size(); i++) {
			//execute start commands
			//cout << inp.start[i]->name << endl;
			CommandExecute::executeCommand(inp.start[i], true);

		}
	//	GameVisuals::generateNewMoving = true;
		while (true) {
			GameVisuals::show();
			if (waitKey(1) == 27)
				break;
			if (GameVisuals::stopGame) {
				//wating for use input, game sequence stops
				if (GameVisuals::userChoseDst) {
					CommandExecute::moveToDestination(GameVisuals::dest, GameVisuals::objectsInArea);

					//GameVisuals::objectsInArea.clear();
					//GameVisuals::userChoseDst =false;
					GameVisuals::stopGame = false;
					leftToWait = 1000;
				}
				//cout << "continue" << endl;
				continue;//do nothing, we are waiting for the user to choose dest
			}

			else if (leftToWait > 0) {
				leftToWait--;
			}
			else if (GameVisuals::generateNewMoving) {
				for (const pair<string, vector<shared_ptr<GameObject>>>& category : gameState->gameObjects) {
					if (JsonFile::getInstance()->speeds.count(category.first)>0) {
						//its a movable object category
						for (int i = 0; i < category.second.size(); i++) {
							shared_ptr<GameObject> movable = category.second[i];
							if (movable->getIsMoving() == false) {

								int x = disX(gen);
								int y = disY(gen);
								shared_ptr<Command> moveIt = make_shared<Command>(CommandExecute::MOVE_IT);
								moveIt->arguments.push_back(to_string(movable->getCoordinates().x));
								moveIt->arguments.push_back(to_string(movable->getCoordinates().y));

								moveIt->arguments.push_back(to_string(x));
								moveIt->arguments.push_back(to_string(y));
								CommandExecute::executeCommand(moveIt, false);
							}

						}
					}
				}
				leftToWait = 100;
			}

			gameState->rainMoveBuild();
		}
		//CategoriesConstants::usePrintUtils(gameState->tiles, gameState->worldMatrix, gameState->gameObjects, gameState->currentSelectedGameObject != nullptr ? gameState->currentSelectedGameObject->getCoordinates() : Coordinates(-1, -1));
		for (int i = 0; i < inp.steps.size() || leftToWait != 0; i++) {
			//execute steps commands
			//wait command- should wait
			if (leftToWait > 0) {//waiting
				i--;
				leftToWait--;
			}
			else {//command
				if (inp.steps[i]->name == CommandExecute::WAIT) {//wait

					leftToWait = stoi(inp.steps[i]->arguments[0]);

				}
				CommandExecute::executeCommand(inp.steps[i], false);
			}
			//actions that take time
			gameState->rainMoveBuild();

			/*	GameVisuals::show(); */

		}

		waitKey(0);
		destroyWindow("world");
		Asserts::executeAsserts(inp.asserts);

	}
};

#endif //GAME_CONTROL_H 