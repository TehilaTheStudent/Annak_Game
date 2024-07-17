#ifndef COMMAND_EXECUTE_X
#define COMMAND_EXECUTE_H
#pragma once
#include "Command.h"
#include "Input.h"
#include <string>
#include <functional>
#include <unordered_map>
#include "Command.h"
#include <memory>
#include "GameState.hpp"
#include "JsonFile.h"
using namespace std;
class CommandExecute
{
public:
	static  void fill_command_execute(const std::shared_ptr<GameState>& game);
	static  void executeCommand(shared_ptr<Command> command, bool start);
	//IDEA another idea for start : member var 
	static const std::string WAIT;

	static const std::string SELECT;
	static const std::string MOVE;
	static const std::string WORK;
	static const std::string DEPOSIT;
	static const std::string TAKE_RESOURCES;
	static const std::string BUILD;
	static const std::string MANUFACTURE;
	static const std::string PEOPLE;
	static const std::string RESOURCE;
	static const std::string RESOURCES;
	static const std::string MAKE_EMPTY;
	static const std::string RAIN;
	static const std::string ROBBER;
	static const std::string MAKE_ROBBER;
	//static const std::string MOVE_TO_DESTINATION;
	static const std::string MOVE_IT;
	static const std::string SET_POINTS; 

	static	void moveToDestination(const Coordinates & coords, vector<shared_ptr<GameObject>> objectsInArea); 

private:
	static  unordered_map<std::string, std::function<void(const std::vector<std::string>&)>> commandMap;
	static  unordered_map<std::string, std::function<void(const std::vector<std::string>&)>> specialForStartCommandMap;

	static  std::shared_ptr<GameState> game;
	static  shared_ptr<JsonFile> jsonFilePtr;

	static void wait(const std::vector<std::string>& args);
	static void select(const std::vector<std::string>& args);
	static void move(const std::vector<std::string>& args);
	static void work(const std::vector<std::string>& args);
	static void deposit(const std::vector<std::string>& args);
	static void takeResources(const std::vector<std::string>& args);
	static void build(const std::vector<std::string>& args);
	static void buildStart(const std::vector<std::string>& args);

	static void manufacture(const std::vector<std::string>& args);
	static void manufactureStart(const std::vector<std::string>& args);

	static void people(const std::vector<std::string>& args);
	static void resource(const std::vector<std::string>& args);
	static void resources(const std::vector<std::string>& args);
	static void makeEmpty(const std::vector<std::string>& args);
	static void rain(const std::vector<std::string>& args);
	static void robber(const std::vector<std::string>& args);
	static void makeRobber(const std::vector<std::string>& args);
	static void setPoints(const std::vector<std::string>& args);

	static void moveIt(const std::vector<std::string>& args);




};

#endif //COMMAND_EXECUTE_H