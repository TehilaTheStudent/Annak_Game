#ifndef COMMAND_H
#define COMMAND_H
#pragma once

#include <string>
#include <vector>

class Command
{

public:
    Command(const std::string& _name): name(_name) { }

    std::string name;
    std::vector<std::string> arguments;//input / start
    std::vector< std::vector<std::string> > data;//world
	 
	static const std::string WORLD;
	static const std::string START;
	static const std::string INPUT;
	static const std::string ASSERTS;
};
#endif //COMMAND_H
