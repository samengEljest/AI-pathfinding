 //! GraphSearch.h
/*!
  This class four different Graph Search algorithms.
  A*, Deph First, Breath First and my own RNG(random number generator) search.
*/
#pragma once
#include <vector>
#include <stack>
#include "Tile.h"
#include <list>
#include <stdlib.h>
#include <windows.h>
class GraphSearch
{


public:
	GraphSearch(); ///< a constructor, sets the goalFound to false.
	~GraphSearch();///< a deconstructor

	std::vector<Tile*> findPathDepthFirst(std::vector<Tile*> tileList); ///< finds the start tile then calls the searchNeighborsDepthFirst with start as argument, returns the ready path to the goal
	bool searchNeighborsDepthFirst(Tile* t, std::vector<Tile*>* path); ///< recusive function to do Depth First seatch algorithm.

	std::vector<Tile*> findPathBreadthFirst(std::vector<Tile*> tileList); ///< finds the start tile then calls the searchNeighborsBreadthFirst with start as argument, returns the ready path to the goal
	Tile* searchNeighborsBreadthFirst(Tile* t, Tile* t2, std::list<Tile*>* bfQueue); ///< recusive function to do Breath First seatch algorithm.

	std::vector<Tile*> findPathRNG(std::vector<Tile*> tileList); ///< finds the start tile then calls the searchNeighborsRNG with start as argument, returns the ready path to the goal
	void searchNeighborsRNG(Tile* t,Tile* lastT, std::vector<Tile*>* path); ///< recusive function to do my own RNG algorithm.

	std::vector<Tile*> findAStar(std::vector<Tile*> tileList); ///< finds the start tile then calls the searchNeighborsAStar with start as argument, returns the ready path to the goal
	Tile* searchNeighborsAStar(Tile* t, Tile* goal, std::vector<Tile*>* open); ///< iterative function to do my A* algorithm.

private:
	bool goalFound;
};