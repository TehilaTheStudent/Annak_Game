#ifndef JSON_FILE_H
#define JSON_FILE_H
#pragma once
#include "json.hpp"
#include <string>
#include <fstream>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <memory>
#include <utility>
#include <iostream>
using namespace std;
using json = nlohmann::json;

class JsonFile {
	json jsonData;
	static shared_ptr< JsonFile> instance;
	JsonFile(const string& fileName) {
		ifstream fileToRead(fileName);
		if (!fileToRead.is_open()) {
			throw std::runtime_error("Could not open file: " + fileName);
		}
		try {
			fileToRead >> jsonData;
		}
		catch (const json::parse_error& e) {
			throw std::runtime_error("Error parsing JSON: " + std::string(e.what()));
		}
		//now jsonData contains the data

		tiles.resize(jsonData["Tiles"].size() + 1);
		//populate tiles-> 0:[Ground]......5:[BlocksMine]
		for (const auto& tile : jsonData["Tiles"].items()) {
			tiles[tile.value()] = tile.key();
		}
		//populate resourceTypeIndex-><Wood:0>....<People:4>
		int i = 0;

		for (const auto& resourceType : jsonData["ResourceTypes"]) {
			resourceTypeIndex[resourceType] = i++;
		}
		//populate resourcesTypes: [Wood,..]
		resourcesTypes = jsonData["ResourceTypes"].get<vector<string>>();
		//populate cateoryAmountResourceIndex-> <Forest:<10,0>>...<City:<5,4>>
		i = 0;
		int nunOfResources = resourceTypeIndex.size();


		cateoryAmountResourceIndex["Forest"] = { 10,0 };
		cateoryAmountResourceIndex["Field"] = { 10,1 };
		cateoryAmountResourceIndex["IronMine"] = { 100,2 };
		cateoryAmountResourceIndex["BlocksMine"] = { 100,3 };
		cateoryAmountResourceIndex["Village"] = { 1,4 };
		cateoryAmountResourceIndex["City"] = { 5,4 };
		//size_t index = 0;
		//json s = jsonData["StartingResources"];
		//for (auto it = s.begin(); it != s.end(); ++it, ++index) {
		//	cout << it.key() << " " << it.value() << endl;
		//}

		//for (const auto& item : jsonData["StartingResources"].items()) {
		//	if (i == nunOfResources - 1)
		//	{
		//		cateoryAmountResourceIndex[item.key()] = { item.value(), i };
		//	}
		//	else {
		//		cateoryAmountResourceIndex[item.key()] = { item.value(), i++ };

		//	}
		//}
		//
		// 
		// 
		//  
		//populate speeds: <Car:0.04>
		speeds = jsonData["Speeds"].get < unordered_map<string, double>>();
		//populate infrustructureTypes-> {Road,...,City}
		//populate transportationTypes-> {Car,...,Helicopter}

		for (const auto& cost : jsonData["Costs"].items()) {
			if (speeds.count(cost.key()) == 0) {//category has cost and not speed
				infrustructureTypes.insert(cost.key());
			}
			else {//category has cost and  speed
				transportationTypes.insert(cost.key());
			}
		}
		//populate sizes: <Tile:<5,5>>....<City:<20,20>>
		for (const auto& item : jsonData["Sizes"].items()) {
			sizes[item.key()] = { item.value()[0],item.value()[1] };
		}
		//populate capacities:<Village: [10, 10, 5, 5, 10]>
		for (const auto& item : jsonData["Capacities"].items()) {
			capacities[item.key()] = item.value().get<vector<int>>();
		}
		//populate times:<City:100>
		for (const auto& item : jsonData["Times"].items()) {
			times[item.key()] = item.value().get<int>();
		}
		//populate scores <City:2>
		for (const auto& item : jsonData["Scores"].items()) {
			scores[item.key()] = item.value().get<int>();
		}
	}

public:
	// Delete copy constructor and assignment operator to prevent multiple instances
	JsonFile(const JsonFile&) = delete;
	JsonFile& operator=(const JsonFile&) = delete;


	static shared_ptr<JsonFile> getInstance(const string& fileName = "") {

		if (instance == nullptr) {
			instance = shared_ptr<JsonFile>(new JsonFile(fileName));
		}
		return instance;
	}
	bool isTile(string& category) const {
		if (find(tiles.begin(), tiles.end(), category) != tiles.end()) {
			return true;
		}
		return false;
	}

	const json& access() const {
		return jsonData;
	}



	vector<string> tiles;
	vector<string> resourcesTypes;
	unordered_map<string, int> resourceTypeIndex;
	unordered_map<string, pair< int, int>> cateoryAmountResourceIndex;
	unordered_set<string> transportationTypes;
	unordered_set<string> infrustructureTypes;
	unordered_map<string, pair<int, int>> sizes;//category, [width,height]
	unordered_map<string, vector<int>> capacities;
	unordered_map<string, int> times;
	unordered_map<string, double> speeds;
	unordered_map<string, int> scores;

};
#endif //JSON_FILE_H 