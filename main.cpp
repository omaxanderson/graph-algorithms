#include "Graph.h"

#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

bool verbose = false;
bool debug = false;

int main(int argc, char* argv[]) {
	int numVertices = 10;
	int numAnnealIter = 1000;
	double rate = 0.99;
	if (argc > 1) {
		numVertices = std::stoi(argv[1]);
	}
	if (argc > 2) {
		numAnnealIter = std::stoi(argv[2]);
	}
	if (argc > 3) {
		rate = std::stod(argv[3]);
	}
	/*
	if (argc > 2) {
		std::string debugFlag = argv[2];
		if (debugFlag == "-d") {
			debug = true;
		} else if (debugFlag == "-v") {
			verbose = true;
			debug = true;
		}
	}
	*/

	Graph* graph = new Graph(numVertices);
	if (numVertices < 12) {
		graph->printMatrix();
	}
	const char* outfileName = "testoutfile.txt";
	graph->writeCoords(outfileName);
	std::cout << "done initializing" << std::endl;
	std::cout << "Minimum: " << graph->minimumSpanningTree() << std::endl;
	std::cout << "Random: " << graph->randomPath(2000).first << std::endl;
	std::pair<double, std::vector<int> > greedy = graph->greedyPath();
	std::cout << "Greedy: " << greedy.first << std::endl;
	std::cout << "Simulated Annealing: " << graph->simulatedAnneal(numAnnealIter, rate).first << std::endl;

	/*
	// ******************  testing *******************
	double** mat = new double*[5];
	for (int i = 0; i < 5; i++) {
		mat[i] = new double[5];
	}
	std::vector<std::pair<int, int> > coords;
	coords.push_back(std::make_pair(5, 95));
	coords.push_back(std::make_pair(95, 65));
	coords.push_back(std::make_pair(70, 55));
	coords.push_back(std::make_pair(50, 50));
	coords.push_back(std::make_pair(50, 45));
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (i == j) {
				mat[i][j] = 0;
			} else {
				mat[i][j] = std::sqrt(std::pow(coords[i].first - coords[j].first, 2) + std::pow(coords[i].second - coords[j].second, 2));
			}
		}
	}
	std::vector<int> testPath;
	testPath.push_back(0);
	testPath.push_back(1);
	testPath.push_back(2);
	testPath.push_back(3);
	testPath.push_back(4);
	Graph* testGraph = new Graph(mat, 5);
	std::cout << "******************  TESTING ********************\n";
	testGraph->printMatrix();
	double testDist1 = testGraph->getPathLength(testPath);
	testPath[3] = 4;
	testPath[4] = 3;
	double testDist2 = testGraph->getPathLength(testPath);
	if (testDist1 > 214.0 && testDist1 < 215.0 && testDist2 > 211.0 && testDist2 < 212.0) {
		std::cout << "distance tests passed" << std::endl;
	}
	std::pair<double, std::vector<int> > testRandomPath = testGraph->randomPath(100);
	std::pair<double, std::vector<int> > testGreedyPath = testGraph->greedyPath();
	std::vector<int> tp {4, 2, 0, 1, 3};
	std::cout << "\n\n\ntesting\n" << testGraph->getPathLength(tp) << std::endl;
	std::cout << "\n\n\n";

	std::cout << "Random Path: " << testRandomPath.first << std::endl << "Path: ";
	for (int i : testRandomPath.second) {
		std::cout << i << ", ";
	}
	std::cout << std::endl;
	std::cout << "Greedy Path: " << testGreedyPath.first << std::endl << "Path: ";
	for (int i : testGreedyPath.second) {
		std::cout << i << ", ";
	}
	std::cout << std::endl;

	// *************** END TESTING *******************
	*/

	return 0;
}

