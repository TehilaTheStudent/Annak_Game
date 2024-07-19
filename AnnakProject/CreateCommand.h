#ifndef CREATE_COMMAND_H
#define CREATE_COMMAND_H

#include <memory>
#include "GameState.hpp"
#include "CommandExecute.h"
#include "Command.h"
#include <vector>

class CreateCommand {

	static  std::shared_ptr<GameState> gameState;
	static  shared_ptr<JsonFile> jsonFilePtr;

public:
	static  void fill_create_command(const std::shared_ptr<GameState>& game);

	static void createSelectCommand(const Coordinates& coord) {

		shared_ptr<Command> select = make_shared<Command>("Select");
		coordIntoArgs(coord, select);
		CommandExecute::executeCommand(select, false);
	}
	static void selecteDstForMovable(const Coordinates& coord) {
		//movable can be: 
		//people->road/tile(work),car,infrus(deposit) ,
		//car->road(move)/ifrus(deposit)

		shared_ptr<Command> move = make_shared<Command>("Move");
		coordIntoArgs(coord, move);
		CommandExecute::executeCommand(move, false);

	}
	static void coordIntoArgs(const Coordinates& coord, shared_ptr<Command> command) 
	{
		command->arguments.push_back(to_string(coord.x));
		command->arguments.push_back(to_string(coord.y));
	}

};


#endif // CREATE_COMMAND_H
