#include <cmath>
#include <list>
#include <vector>
#include <iostream>
#include <string>
#include <limits>
#include <memory>

struct Node {
	int x;
	int y;
	int distanceToStart;
	double euclideanDistance;
	std::shared_ptr<Node> parent;
	bool operator==(const Node& other) const {
		return x == other.x && y == other.y;
	}
};

std::list<std::shared_ptr<Node>> aStarSearch(std::pair<int, int> startPosition, std::pair<int, int> targetPosition);
std::vector<std::shared_ptr<Node>> generateNeighborTiles(std::shared_ptr<Node> currentTile, std::shared_ptr<Node> targetTile);
std::list<std::shared_ptr<Node>>::iterator getBestTileFromHeuristic(std::list<std::shared_ptr<Node>>& unexploredTiles);
double getEuclideanDistanceToTarget(std::shared_ptr<Node> currentTile, std::shared_ptr<Node> targetTile);