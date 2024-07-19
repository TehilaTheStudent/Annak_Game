#include "CreateCommand.h"

void CreateCommand::fill_create_command(const std::shared_ptr<GameState>& game)
{
	CreateCommand::gameState = game;
	CreateCommand::jsonFilePtr = JsonFile::getInstance(); 
}


  std::shared_ptr<GameState> CreateCommand::gameState;
  shared_ptr<JsonFile>CreateCommand:: jsonFilePtr;
