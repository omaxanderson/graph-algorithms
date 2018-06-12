#include "Graph.h"

#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

bool verbose = false;
bool debug = false;

std::vector<int> randomPath(double* adjMat[], int size) {
	std::vector<int> path;
	std::vector<int> unvisited;
	for (int i = 0; i < size; i++) {
		unvisited.push_back(i);
	}
	for (int i = 0; i < size; i++) {
		int next = rand() % unvisited.size();
		path.push_back(unvisited[next]);
		unvisited.erase(unvisited.begin() + next);
	}
	return path;
}

double getPathLength(double* adjMat[], const std::vector<int>& path, int size) {
	double distance = 0;
	for (int i = 0; i < path.size() - 1; i++) {
		distance += adjMat[path[i]][path[i + 1]];
	}
	return distance;
}

int main(int argc, char* argv[]) {
	int numVertices = 10;
	if (argc > 1) {
		numVertices = std::stoi(argv[1]);
	}
	if (argc > 2) {
		std::string debugFlag = argv[2];
		if (debugFlag == "-d") {
			debug = true;
		} else if (debugFlag == "-v") {
			verbose = true;
			debug = true;
		}
	}
	
	Graph* graph = new Graph(numVertices);
	if (numVertices < 12) {
		graph->printMatrix();
	}
	std::cout << "done initializing" << std::endl;
	std::cout << "Minimum: " << graph->minimumSpanningTree() << std::endl;
	std::cout << "Random: " << graph->randomPath(2000).first << std::endl;
	std::pair<double, std::vector<int> > greedy = graph->greedyPath();
	std::cout << "Greedy: " << greedy.first << std::endl;
	/*
	for (int i : greedy.second) {
		std::cout << i << " ";
	}
	std::cout << std::endl;
	*/
	
	// create random graph
	//double* adjMatrix[numVertices];
	//init(adjMatrix, numVertices);
	

	// print the matrix 
	if (verbose) {
//		printMatrix(adjMatrix, numVertices);
	}
	//double minSpanningDist = minSpanning1(adjMatrix, numVertices);
	//std::cout << "Cannot be better than: " << minSpanningDist << std::endl;

	/*
	double smallestRandom = INT_MAX;
	std::vector<int> path;
	for (int i = 0; i < 10000; i++) {
		std::vector<int> tempPath = randomPath(adjMatrix, numVertices);
		double d = getPathLength(adjMatrix, tempPath, numVertices);
		if (d < smallestRandom) {
			smallestRandom = d;
			path = tempPath;
			if (verbose) {
				std::cout << "Current smallest random found: " << smallestRandom << std::endl;
			}
		}
	}
	if (verbose) {
		for (int i : path) {
			std::cout << i << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "Random Path Length: " << smallestRandom << std::endl;
	*/


	return 0;
}

