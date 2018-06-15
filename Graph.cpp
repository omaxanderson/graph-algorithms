#include "helpers.h"
#include "Graph.h"
#include <vector>
#include <algorithm>
#include <utility>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>

Graph::Graph(int size) : _size(size) {
	init();
}

Graph::Graph(double** adj, int size) : adjMat(adj), _size(size) {
	for (int i = 0; i < _size; i++) {
		unvisited.insert(i);
		int minDist = INT_MAX;
		int idx = -1;
		for (int j = 0; j < _size; j++) {
				// initializing closest neighbors vector too for future optimizations
			if (adjMat[i][j] < minDist) {
				minDist = adjMat[i][j];
				idx = j;
			}
		}
		closestNeighbors.push_back(std::make_pair(idx, minDist));
	}
}

Graph::~Graph() {
	for (int i = 0; i < _size; i++) {
		delete[] adjMat[i];
	}
	delete[] adjMat;
}

void Graph::writeCoords(const char* filename) {
	std::cout << "Writing coordinates to '" << filename << "'." << std::endl;
	std::ofstream outfile;
	outfile.open(filename);
	outfile << _size << std::endl;
	for (int i = 0; i < _size; i++) {
		outfile << coords[i].first << " " << coords[i].second << std::endl;
	}
	outfile.close();
}

void Graph::init() {
//	std::cout << "begin init" << std::endl;
	srand(time(NULL));
//	std::vector< std::pair<int, int> > coords;

	adjMat = new double*[_size];
	for (int i = 0; i < _size; i++) {
		// create an array at i and create a new coordinate for the graph
		adjMat[i] = new double[_size];
		int x = rand() % 1000;
		int y = rand() % 1000;
//		std::cout << "rand nums: " << x << ", " << y << std::endl;
		coords.push_back(std::make_pair(x, y));
		unvisited.insert(i);
	}

	/*
	if (coords[0].first > 100 || coords[0].first < 0 || coords[0].second > 99 || 
			coords[0].second < 0) {
		// something got fucked up somehow
		coords[0] = std::make_pair(rand() % 100, rand() % 100);
	}
	*/
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
	std::cout << "adjacency matrix" << std::endl;
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

		// create an ordered vector and shuffle it
		for (int i = 0; i < _size; i++) {
			path.push_back(i);
		}

		std::random_shuffle(path.begin(), path.end());

		double distance = getPathLength(path);

		if (distance < minDistance) {
			minDistance = distance;
			bestPath = path;
		}
	}

	return std::make_pair(minDistance, bestPath);
}

std::pair<double, std::vector<int> > Graph::greedyPath() {
	// start at 0, keep finding the closest node until no more unvisited nodes
	std::vector<int> path;
	path.push_back(0);
	visited.insert(0);

	while (path.size() < _size) {
		path.push_back(getClosestNode(path[path.size() - 1]));
		visited.insert(path[path.size() - 1]);
	}

	double distance = getPathLength(path);

	cleanup();

	return std::make_pair(distance, path);
}

double Graph::getPathLength(std::vector<int>& path) {
	double dist = 0;
	for (int i = 1; i < path.size(); i++) {
		dist += adjMat[path[i]][path[i - 1]];
	}
	dist += adjMat[path[0]][path[path.size() - 1]];
	return dist;
}

void neighbor1(std::vector<int>& path) {
	int a = rand() % path.size();
	int b = rand() % path.size();
	int temp = path[a];
	path[a] = path[b];
	path[b] = temp;
}

void neighbor2(std::vector<int>& path) {
	int a = rand() % path.size();
	int b = rand() % path.size();
	if (a > b) {
		// swap elements
		int range = (path.size() - a + b + 1) / 2;
		for (int i = 0; i < range; i++) {
			if (a == path.size()) {
				a = 0;
			}
			if (b < 0) {
				b = path.size() - 1;
			}
			std::iter_swap(path.begin() + a++, path.begin() + b--);
		}
	} else {
		std::reverse(path.begin() + a, path.begin() + b + 1);
	}
}

void neighbor3(std::vector<int>& path) {
	int a = rand() % path.size();
	if (a == path.size() - 1) {
		std::iter_swap(path.begin(), path.begin() + a);
	} else {
		std::iter_swap(path.begin() + a, path.begin() + a + 1);
	}
}

std::pair<double, std::vector<int> > Graph::simulatedAnneal(int iterations, double rate) {
	std::pair<double, std::vector<int> > path = greedyPath();
	double distance = path.first;
//	std::cout << distance << std::endl;
	double temperature = 0.95;
	int count = 0;
	int improve = 0;
	int worsen = 0;

	while (temperature > 0.00001) {
		for (int i = 0; i < iterations; i++) {
			// generate an edge swap
			std::vector<int> tempPath = path.second;
		//	std::pair<int, int> nodes = std::make_pair(rand() % path.second.size(), 
		//			rand() % path.second.size());
			if (temperature > 0.4) {
				if (i % 2) {
					neighbor1(tempPath);
				} else {
					neighbor2(tempPath);
				}
			} else {
				if (i % 2) {
					neighbor3(tempPath);
				} else if (i < iterations / 2) {
					neighbor2(tempPath);
				} else {
					neighbor1(tempPath);
				}
			}


			/*
			int temp = tempPath[nodes.first];
			tempPath[nodes.first] = tempPath[nodes.second];
			tempPath[nodes.second] = temp;
			*/

			// this is the expensive part
			double tempDist = getPathLength(tempPath);


			double p = std::pow(2.71828, (distance - tempDist) / temperature);
//			double p = 2.71828 * gain / temperature;
			if (p > (double)rand() / RAND_MAX) {
				if (tempDist < distance) {
					std::cout << "New Minimum: " << tempDist << std::endl;
				}
				if (tempDist < distance) {
					improve++;
				} else {
					worsen++;
				}
				path.second = tempPath;
				distance = tempDist;
				count++;
			}
		}

		temperature *= rate;
	}
	std::cout << "num changes: " << count << std::endl;
	std::cout << "improvements: " << improve << std::endl;
	std::cout << "downgrades: " << worsen << std::endl;

	return std::make_pair(distance, path.second);
}

