 //! Tile.h
/*!
  A struct that handels the Tile, (unwakables or wakable areas).

*/
#pragma once
#include <vector>

struct Tile
{
public:
	int x;
	int y;
	int width;
	int height;
	bool walkable;
	bool start;
	bool goal;
	bool flagged; ///< this bool is true if we have merged it with another tile, basicly removed from the list
	bool searched;
	float nodeX;
	float nodeY;
	float fValue;
	float gValue;

	bool flaggdOpen;
	bool flaggedClosed;

	std::vector<Tile*> neighbors;
	Tile* parent;

	~Tile(){};

	Tile(int X, int Y, bool walk)
	{
		x = X;
		y = Y;
		width = 1;
		height = 1;
		walkable = walk;
		start = false;
		goal = false;
		flagged = false;
		searched = false;
		flaggdOpen = false;
		flaggedClosed = false;
		parent = NULL;

		nodeX = 0;
		nodeY = 0;
	}
};