#ifndef ROADS_GRAPH_H
#define ROADS_GRAPH_H

#include <memory>
#include "GameObject.hpp"
#include "Coordinates.h"

#include "Validations.hpp"
#include <queue>
#include <algorithm>

using namespace std;
struct RoadsGraph {

	struct RoadVertice {

		shared_ptr<GameObject> road;
		vector<shared_ptr<RoadVertice>> adjacencyList;

		RoadVertice(shared_ptr<GameObject> road) :road(road) {}
	};

	vector<shared_ptr<RoadVertice>> roadVerticesList;

	void  addEdge(shared_ptr<GameObject> from, shared_ptr<GameObject> to) {

		int fromIndex = from->getRoadIndex(), toIndex = to->getRoadIndex();
		int verticesSize = roadVerticesList.size();

		if (fromIndex < 0 || fromIndex >= verticesSize || toIndex < 0 || toIndex >= verticesSize) {
			cout << "BUG" << endl;
			return;
		}

		shared_ptr<RoadVertice> fromVertice = roadVerticesList[fromIndex],
			toVertice = roadVerticesList[toIndex];

		fromVertice->adjacencyList.push_back(toVertice);
		toVertice->adjacencyList.push_back(fromVertice);

	}
	bool isInRange(int index)const   {
		return index >= 0 && index < roadVerticesList.size();
	}
	void addVertice(shared_ptr<GameObject> newRoad) {
		roadVerticesList.push_back(make_shared<RoadVertice>(newRoad));
	}

	const vector<shared_ptr<GameObject>> findWay(shared_ptr<GameObject> from, vector<shared_ptr<GameObject>> roadsTo) const {
		int fromIndex = from->getRoadIndex();
	
		if (!isInRange(fromIndex)) {
			cout <<endl<< "BUG: fromIndex out of range" << endl;
			return {};
		}
		
		
		vector<int>	toIndexes;
		for (shared_ptr<GameObject> road : roadsTo) {
			int toIndex = road->getRoadIndex();
			if (!isInRange(toIndex)) { 
				cout << endl<< "BUG: toIndex out of range" << endl;
				return {};
			}
			toIndexes.push_back(road->getRoadIndex());
		}

		int verticesSize = roadVerticesList.size();

	

		shared_ptr<RoadVertice> fromVertice = roadVerticesList[fromIndex];
			

		// BFS setup using vectors for visited and parent tracking
		queue<int> q;
		vector<bool> visited(verticesSize, false);
		vector<int> parent(verticesSize, -1);

		q.push(fromIndex);
		visited[fromIndex] = true;
		int current=-1;

		while (!q.empty()) {
			current = q.front();
			q.pop();

			if (find(toIndexes.begin(),toIndexes.end(), current) !=toIndexes.end()) { 
				//we reached! fuond one of the destinations
				//break couse we dont need short pathes for all the vertices only for this one
				break;
			}

			for (const shared_ptr<RoadVertice>& neighbor : roadVerticesList[current]->adjacencyList) {
				int neighborIndex = neighbor->road->getRoadIndex();
				if (!visited[neighborIndex]) {
					q.push(neighborIndex);
					visited[neighborIndex] = true;
					parent[neighborIndex] = current;
				}
			}
		}

		// Reconstruct path
		vector<shared_ptr<GameObject>> path;
		if (!isInRange(current)|| !visited[current]) { 
			cout << "No path found" << endl;
			//vector with size 0
			return {};
		}

		for (int v = current; v != fromIndex; v = parent[v]) { 
			path.push_back(roadVerticesList[v]->road); 
		}

		//include the from
		path.push_back(roadVerticesList[fromIndex]->road); 
		reverse(path.begin(), path.end());

		return path;
	}

};

#endif //ROADS_GRAPH_H