
#ifndef COORDINATES_H
#define COORDINATES_H



#pragma once
#include <string>
#include <utility>
#include <iostream>


using namespace std;


struct Coordinates {
	int x;
	int y;
	Coordinates() :x(-1), y(-1) {}
	Coordinates(int x, int y) :x(x), y(y) {
	}
	Coordinates(const std::string& x, const std::string& y) :x(std::stoi(x)), y(std::stoi(y))
	{


	}
	Coordinates(const Coordinates& other) :x(other.x), y(other.y) {
	}
	Coordinates& operator = (const Coordinates& other) {
		if (&other != this) {
			x = other.x;
			y = other.y;
		}
		return *this;
	}
	bool operator==(const Coordinates& other) const {
		return x == other.x && y == other.y;
	}
	pair<string,string> getAsString()const {
		return make_pair<string, string>(to_string(x), to_string(y));
	}
	Coordinates	operator+(const Coordinates& other) const {
		return Coordinates(x + other.x, y + other.y);
	}
};



#endif // COORDINATES_H