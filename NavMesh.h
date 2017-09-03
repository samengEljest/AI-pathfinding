 //! NavMesh.h
/*!
  This class handels the Map creation and NavMesh creation.
*/
#pragma once
#include <vector>
#include "Tile.h"
#include <string>
#include <stdlib.h>

class NavMesh
{
public:
	NavMesh(); ///< a constructor.
	~NavMesh(); ///< a deconstructor
	bool ReadMap(std::string s); ///< function for reading the txt file and creating different tils for X,0,S,G. Saves files in the vector tileList the n runs findNeighbors.
	void findNeighbors(); ///< a n^2 function for finding all walkabe neighboring tiles to ALL wakabel Tiles by compareing the x and y values for each tile. 
	std::vector<Tile*> getList(); ///< returns the tileList
	bool Expand(int x);  ///< the tile on index x in the tileList will try to merge with all its neghbors, runs neghborCheck.
	bool neighborCheck(Tile* neighbor, Tile* t); ///< compares two tiles to see if they already have it as a neighbor or if it is the same tile(used when we want to transfer neghbors during expand)
	int Intersect(Tile* t, Tile* t2); ///< compares two tiles to see if they are convex toghether, by comparing corners.
	void calcNodes(); ///< calculates the middle point of a tile, and sets it as a nodeX and nodeY value, does this off evety tile in tileList.
	void UpdateList(); ///< resets the some bools and parents pointer for every tile. This is reset before doing a search algorithems.

private:
	std::vector<Tile*> tileList;

};