#include "AStar_Pathfinding.h"

std::vector<std::pair<int, int>> directions = {
	   {1, 0},
	   {1, 1},
	   {0, 1},
	   {-1, 1},
	   {-1, 0},
	   {-1, -1},
	   {0, -1},
	   {1, -1}
};

std::list<std::shared_ptr<Node>> aStarSearch(std::pair<int, int> startPosition, std::pair<int, int> targetPosition) {
	auto start = std::make_shared<Node>();
	start->x = startPosition.first;
	start->y = startPosition.second;
	start->distanceToStart = 0;

	auto target = std::make_shared<Node>();
	target->x = targetPosition.first;
	target->y = targetPosition.second;

	std::list<std::shared_ptr<Node>> unexploredTiles;
	unexploredTiles.push_back(start);
	auto it = unexploredTiles.begin();
	std::list<std::shared_ptr<Node>> exploredTiles;
	while (!unexploredTiles.empty()) {
		auto result = getBestTileFromHeuristic(unexploredTiles);
		auto selectedTile = *result;
		std::cout << (*selectedTile).x << (*selectedTile).y << std::endl;

		//If path found
		if (*selectedTile == *target) {
			std::cout << "Found." << std::endl;
			std::list<std::shared_ptr<Node>> path;
			auto current = selectedTile;
			while (current->parent != nullptr) {
				path.push_front(current);
				std::cout << (*current).x << ", " << (*current).y << std::endl;
				current = current->parent;
			}
			return path;
		}

		auto neighbors = generateNeighborTiles(selectedTile, target);
		exploredTiles.push_back(selectedTile);
		if (result != unexploredTiles.end()) {
			std::cout << "Deleting." << std::endl;
			unexploredTiles.erase(result);
		}
		for (auto neighbor : neighbors) {
			//If explored tiles list contains neighbor, skip it and continue with the loop
			if (std::find_if(exploredTiles.begin(), exploredTiles.end(),
				[&neighbor](const std::shared_ptr<Node>& n) { return *n == *neighbor; }) != exploredTiles.end()) {
				continue;
			}
			//If unexploredList contains neighbor, update the existing node with the new path (if shorter)
			auto it = std::find_if(unexploredTiles.begin(), unexploredTiles.end(),
				[&neighbor](const std::shared_ptr<Node>& n) { return *n == *neighbor; });
			if (it != unexploredTiles.end()) {
				auto node = *it;
				if (neighbor->distanceToStart < node->distanceToStart) {
					node->distanceToStart = neighbor->distanceToStart;
					node->parent = neighbor->parent;
				}
			}
			else {
				//if node is not in the unexplored or explored list,
				//calculate heuristic distance to target and push it to the unexplored list
				neighbor->euclideanDistance = getEuclideanDistanceToTarget(neighbor, target);
				unexploredTiles.push_back(neighbor);
			}
		}
	}
	return {};
}

//Generates a list of all neighboring tiles and returns it (positions cannot be negative)
std::vector<std::shared_ptr<Node>> generateNeighborTiles(std::shared_ptr<Node> currentTile, std::shared_ptr<Node> targetTile) {
	std::vector<std::shared_ptr<Node>> generatedNeighbors;
	for (auto p : directions) {
		int xPos = currentTile->x + p.first;
		int yPos = currentTile->y + p.second;
		if (xPos < 0 || yPos < 0) {
			continue;
		}
		auto n = std::make_shared<Node>();
		n->x = currentTile->x + p.first;
		n->y = currentTile->y + p.second;
		n->distanceToStart = currentTile->distanceToStart + std::sqrt(p.first*p.first + p.second*p.second);
		n->parent = currentTile;
		generatedNeighbors.push_back(n);
	}
	return generatedNeighbors;
}

//Calculates the euclidean distance from the current tile to the target tile
double getEuclideanDistanceToTarget(std::shared_ptr<Node> currentTile, std::shared_ptr<Node> targetTile) {
	return std::sqrt(std::pow(targetTile->x - currentTile->x, 2) + std::pow(targetTile->y - currentTile->y, 2));
}

//Obtains the best unexplored tile based on the sum of total distance from the starting tile and estimated euclidean distance
std::list<std::shared_ptr<Node>>::iterator getBestTileFromHeuristic(std::list<std::shared_ptr<Node>>& unexploredTiles) {
	if (unexploredTiles.empty()) {
		return unexploredTiles.end();
	}
	double currentLowestDistance = std::numeric_limits<double>::max();
	auto bestSoFar = unexploredTiles.begin();

	for (auto iterator = unexploredTiles.begin(); iterator != unexploredTiles.end(); ++iterator) {
		double estimatedTotalDistance = (*iterator)->distanceToStart + (*iterator)->euclideanDistance;
		if (estimatedTotalDistance < currentLowestDistance) {
			bestSoFar = iterator;
			currentLowestDistance = estimatedTotalDistance;
			
		}
	}

	return bestSoFar;
}
