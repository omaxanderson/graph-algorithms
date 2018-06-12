#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <iostream>
#include <cmath>
#include <vector>
#include <utility>

typedef std::pair<double, double> Coordinate;
extern bool debug;
extern bool verbose;

double eucDist(Coordinate c1, Coordinate c2) {
	return std::sqrt(std::pow(c1.first - c2.first, 2) + std::pow(c1.second - c2.second, 2));
}

// for each point give it a position and calculate dist to everything else
void init(double* mat[], int numVerts) {
	srand(time(NULL));

	std::vector<Coordinate> coords;
	for (int i = 0; i < numVerts; i++) {
		// assign position
		coords.push_back(std::make_pair(rand() % 100, rand() % 100));

		// also initialize an array at this pos
		mat[i] = new double[numVerts];
	}

	// now calculate distances and assign them
	for (int i = 0; i < numVerts; i++) {
		for (int j = 0; j < numVerts; j++) {
			if (i == j) {
				mat[i][j] = 0;
			} else {
				mat[i][j] = eucDist(coords[i], coords[j]);
				//std::cout << coords[i].first << ", " << coords[i].second << " : " << coords[j].first << ", " <<  coords[j].second << std::endl;
				//std::cout << eucDist(coords[i], coords[j]) << std::endl;
			}
		}
	}

	if (debug && verbose) {
		for (Coordinate c : coords) {
			std::cout << c.first << ", " << c.second << std::endl;
		}
	}
}

void initDistVec(std::vector<std::pair<int, double> >& closestNeighbors, double* adjMat[], int size) {
	for (int i = 0; i < size; i++) {
		int minDist = INT_MAX;
		int idx = -1;
		for (int j = 0; j < size; j++) {
			if (i != j && adjMat[i][j] < minDist) {
				minDist = adjMat[i][j];
				idx = j;
			}
		}
		closestNeighbors.push_back(std::make_pair(idx, minDist));
	}
}

std::pair<int, int> closestNode(double* adjMat[], const int size, const std::vector<int> &visited, const std::vector<int> &unvisited, std::vector<std::pair<int, double> > closestVec) {
	double closest = INT_MAX;
	int idx = -1;
	int closestTo = -1;
	for (int i : visited) {
		if (closestVec[i].second < closest && std::find(visited.begin(), visited.end(), closestVec[i].first) == visited.end()) {
			// closest node isn't in visited so we can just use this instead of iterating, nice!
			closest = closestVec[i].second;
			idx = closestVec[i].first;
			closestTo = i;
		}
		for (int j : unvisited) {
			if (i != j && adjMat[i][j] < closest) {
				closest = adjMat[i][j];
				idx = j;
				closestTo = i;
			}
		}
	}
	return std::make_pair(idx, closestTo);
}

double minSpanning1(double* adjMat[], int size) {
	// create a map with the closest values for each
	std::vector< std::pair<int, double> > closestNeighbors;
	initDistVec(closestNeighbors, adjMat, size);

	// first find the closest point 
	std::vector<int> visited;
	std::vector<int> unvisited;
	for (int i = 1; i < size; i++) {
		unvisited.push_back(i);
	}
	visited.push_back(0);
	if (verbose) {
		std::cout << visited[0];
	}

	// adj list for path
	std::vector< std::pair<int, int> > adjList;

	while (visited.size() < size) {
		// find the closest node to both this node and the previous--if applicable
		if (visited.size() == 1) {
			//visited.push_back(closestNode(adjMat, size, visited).first);
			visited.push_back(closestNeighbors[0].first);
			unvisited.erase(std::find(unvisited.begin(), unvisited.end(), closestNeighbors[0].first));
			adjList.push_back(std::make_pair(visited[0], visited[visited.size() - 1]));
			if (verbose) {
				std::cout << " " << visited[visited.size() - 1] << std::endl;
			}
		} else {
			// first check if closest node previously found has been visited or nah
			std::pair<int, int> closest = closestNode(adjMat, size, visited, unvisited, closestNeighbors);
			visited.push_back(closest.first);
			// is this correctly removing from unvisited?
			unvisited.erase(std::find(unvisited.begin(), unvisited.end(), closest.first));
			adjList.push_back(std::make_pair(closest.first, closest.second));
			if (verbose) {
				std::cout << closest.first << " " << closest.second << std::endl;
			}
		}

	}
	double distance = 0;
	for (std::pair<int, int> p : adjList) {
		distance += adjMat[p.first][p.second];
	}

	if (verbose) {
		std::cout << "sizeof visited: " << visited.size() << std::endl;
		std::cout << "sizeof unvisited: " << unvisited.size() << std::endl;
	}

	// get the distance
	return distance;
}

#endif
