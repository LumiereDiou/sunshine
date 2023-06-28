#pragma once
#include "Tilemap.h"
#include <unordered_map>

class Pathfinder
{
public:
	Tilemap* map = nullptr;

private:
	std::unordered_map<TileCoord, float, std::hash<TileCoord>, std::equal_to<TileCoord>> unvisited;
	std::unordered_map<TileCoord, float, std::hash<TileCoord>, std::equal_to<TileCoord>> visited;
	std::unordered_map<TileCoord, float, std::hash<TileCoord>, std::equal_to<TileCoord>> cheapestEdgeTo;

	TileCoord startNode;
	TileCoord goalNode;
	TileCoord currentNode;

public:
	Pathfinder() {};

	Pathfinder(Tilemap* levelToNavigate, TileCoord startTile, TileCoord endTile)
	{
		map = levelToNavigate;
		startNode = startTile;
		goalNode = endTile;
		currentNode = startNode;
		Restart();
	}

	bool IsVisited(TileCoord pos) const { return visited.count(pos); }
	
	bool IsSolved() const { return IsVisited(goalNode); }
	
	bool IsCompleted() { return IsVisited(goalNode) || GetLowerCostIn(unvisited).second == INFINITY; }

	void Restart()
	{
		visited.clear();
		cheapestEdgeTo.clear();

		unvisited.clear();
		for (TileCoord position : map->GetAllTraversableTiles())
		{
			unvisited[position] = INFINITY;
		}

		unvisited[startNode] = 0;
	}

	void MoveToVisitedSet(TileCoord node)
	{
		visited[currentNode] = unvisited[currentNode];
		unvisited.erase(currentNode);
	}

	void ProcessNextIterationFunctional()
	{
		if (IsCompleted()) return;
		currentNode = GetLowerCostIn(unvisited).first;

		//Todo: evaluate cost to reach this nodes neigbors and update

		MoveToVisitedSet(currentNode);
	}

	bool SolvePath()
	{
		Restart();
		while (!IsCompleted())
		{
			ProcessNextIterationFunctional();
		}
		return IsSolved();
	}

	std::pair<TileCoord, float> GetLowerCostIn(std::unordered_map<TileCoord, float> set)
	{
		TileCoord cheapestPosition = { -1,-1 };
		float lowestCost = INFINITY;
		for (auto nodeValuePair : set)
		{
			TileCoord tile = nodeValuePair.first;
			float cost = nodeValuePair.second;

			if (cost < lowestCost)
			{
				cheapestPosition = tile;
				lowestCost = cost;
			}
		}
		return { cheapestPosition, lowestCost };
	}
};