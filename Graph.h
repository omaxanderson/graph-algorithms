#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>
#include <unordered_set>
#include <utility>

class Graph {
	public:
		// Public member functions
		Graph(int);
		Graph(double**, int);
		~Graph();
		double dist(int, int);
		void printMatrix();
		void writeCoords(const char* filename);
		double minimumSpanningTree();
		std::pair<double, std::vector<int> > randomPath(int);
		std::pair<double, std::vector<int> > greedyPath();
		std::pair<double, std::vector<int> > simulatedAnneal(int, double);
		double getPathLength(std::vector<int>&);

	private:
		// internal helper functions
		std::pair<int, int> getClosestNode();
		int getClosestNode(int);
		void init();
		void cleanup();
		
		// instance variables
		std::unordered_set<int> visited;
		std::unordered_set<int> unvisited;
		std::vector< std::pair<int, double> > closestNeighbors;
		std::vector< std::pair<int, int> > coords;
		int _size;
		double** adjMat;
};

#endif

