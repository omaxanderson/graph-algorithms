#include "helpers.h"
#include "Graph.h"
#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <ctime>

Graph::Graph(int size) : _size(size) {
	init();
}

Graph::~Graph() {
	for (int i = 0; i < _size; i++) {
		delete[] adjMat[i];
	}
	delete[] adjMat;
}

void Graph::init() {
//	std::cout << "begin init" << std::endl;
	srand(time(NULL));
	std::vector< std::pair<int, int> > coords;

	adjMat = new double*[_size];
	for (int i = 0; i < _size; i++) {
		// create an array at i and create a new coordinate for the graph
		adjMat[i] = new double[_size];
		int x = rand() % 100;
		int y = rand() % 100;
//		std::cout << "rand nums: " << x << ", " << y << std::endl;
		coords.push_back(std::make_pair(x, y));
		unvisited.insert(i);
	}
	if (coords[0].first > 100 || coords[0].first < 0 || coords[0].second > 99 || 
			coords[0].second < 0) {
		// something got fucked up somehow
		coords[0] = std::make_pair(rand() % 100, rand() % 100);
	}
	/*
	int count = 0;
	for (std::pair<int, int> p : coords) {
		std::cout << count++ << ": " << p.first << ", " << p.second << std::endl;
	}
	*/
//	std::cout << "sizeof coords " << coords.size() << std::endl;

	// Initializing adjacency matrix and closest neighbors vector
//	std::cout << "init adj mat & closest neighbors vec" << std::endl;
	for (int i = 0; i < _size; i++) {
		int minDist = INT_MAX;
		int idx = -1;
		for (int j = 0; j < _size; j++) {
			if (i == j) {
				adjMat[i][j] = 0;
			} else {
				adjMat[i][j] = eucDist(coords[i], coords[j]);

				// initializing closest neighbors vector too for future optimizations
				if (adjMat[i][j] < minDist) {
					minDist = adjMat[i][j];
					idx = j;
				}
			}
		}
		closestNeighbors.push_back(std::make_pair(idx, minDist));
	}
	
	// debugging
	/*
	for (std::pair<int, int> coord : coords) {
		std::cout << coord.first << ", " << coord.second << std::endl;
	}
	*/
}

double Graph::dist(int i, int j) {
	return adjMat[i][j];
}

void Graph::printMatrix() {
	for (int i = 0; i < _size; i++) {
		for (int j = 0; j < _size; j++) {
			std::cout << adjMat[i][j] << "\t";
		}
		std::cout << std::endl;
	}
}

std::pair<int, int> Graph::getClosestNode() {
	double closest = INT_MAX;
	int idx = -1;
	int closestTo = -1;

	// debugging
//	std::clock_t start = std::clock();

	for (int i : visited) {
		// check if closestNeighbors[i] is in visited
		if (closestNeighbors[i].second < closest && 
				visited.find(closestNeighbors[i].first) == visited.end()) {
			closest = closestNeighbors[i].second;
			idx = closestNeighbors[i].first;
			closestTo = i;
		} else {
			// if not iterate through unvisited
			for (int j : unvisited) {
				/*
				if (visited.find(j) != visited.end()) {
					continue;
				} 
				*/
				
				if (i != j && adjMat[i][j] < closest) {
					closest = adjMat[i][j];
					idx = j;
					closestTo = i;
				}
			}
		}
	}
//	std::cout << (std::clock() - start) / (double)CLOCKS_PER_SEC << std::endl;
	return std::make_pair(idx, closestTo);
}

int Graph::getClosestNode(int idx) {
	// if the closestNeighbors thing is NOT in visited, return it
	if (visited.find(closestNeighbors[idx].first) == visited.end()) {
		return closestNeighbors[idx].first;
	} 

	double minDist = INT_MAX;
	int minIdx = -1;
	for (int i = 0; i < _size; i++) {
		// if i is in visited, continue
		if (visited.find(i) != visited.end()) {
			continue;
		}
		if (adjMat[idx][i] < minDist) {
			minDist = adjMat[idx][i];
			minIdx = i;
		}
	}
	return minIdx;
}

void Graph::cleanup() {
	visited.clear();
	unvisited.clear();
	for (int i = 0; i < _size; i++) {
		unvisited.insert(i);
	}
}

double Graph::minimumSpanningTree() {
	// create an adjacency list for the path
	std::vector< std::pair<int, int> > path;

	// start on idx 0
	visited.insert(0);
	unvisited.erase(unvisited.find(0));
	
	// find the closest node to 0, it's guaranteed to be unvisited in closestNeighbors vec
	visited.insert(closestNeighbors[0].first);
	unvisited.erase(unvisited.find(closestNeighbors[0].first));
	path.push_back(std::make_pair(0, closestNeighbors[0].first));

	// debugging
//	 std::cout << "0 " << closestNeighbors[0].first << std::endl;

	// iterate until we've visited every node
	while (visited.size() < _size) {
		// find the closest neighbor to the visited nodes
		std::pair<int, int> closest = getClosestNode();
		visited.insert(closest.first);
		unvisited.erase(unvisited.find(closest.first));
		path.push_back(std::make_pair(closest.first, closest.second));

		// debugging
//		 std::cout << closest.first << " " << closest.second << std::endl;
	}

	// calculate distance
	double distance = 0;
	for (std::pair<int, int> edge : path) { 
		distance += adjMat[edge.first][edge.second];
	}

	// clean up visited vector
	cleanup();

	return distance;
}

std::pair<double, std::vector<int> > Graph::randomPath(int iterations = 100) {
	double minDistance = INT_MAX;
	std::vector<int> bestPath;

	for (int iter = 0; iter < iterations; iter++) {
		std::vector<int> path;
		double distance = 0;

		// create an ordered vector and shuffle it
		for (int i = 0; i < _size; i++) {
			path.push_back(i);
		}

		std::random_shuffle(path.begin(), path.end());

		// find the distance
		for (int i = 1; i < path.size(); i++) {
			distance += adjMat[path[i]][path[i - 1]];
		}

		// connect the beginning and end of the path
		distance += adjMat[path[0]][path[path.size() - 1]];

		if (distance < minDistance) {
			minDistance = distance;
			bestPath = path;
		}
	}
	// we need to connect the final points

	return std::make_pair(minDistance, bestPath);
}

std::pair<double, std::vector<int> > Graph::greedyPath() {
	// start at 0, keep finding the closest node until no more unvisited nodes
	std::vector<int> path;
	path.push_back(0);
	visited.insert(0);
	double distance = 0;

	while (path.size() < _size) {
		path.push_back(getClosestNode(path[path.size() - 1]));
		visited.insert(path[path.size() - 1]);
	}

	for (int i = 1; i < path.size(); i++) {
		distance += adjMat[path[i]][path[i - 1]];
	}
	distance += adjMat[path[0]][path[path.size() - 1]];

	cleanup();

	return std::make_pair(distance, path);
}
