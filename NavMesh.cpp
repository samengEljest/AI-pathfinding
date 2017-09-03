 //! NavMesh.cpp
#include "NavMesh.h"
#include "Tile.h"
#include <fstream>
#include <iostream>
using namespace std;

NavMesh::NavMesh()
{
}

NavMesh::~NavMesh()
{}


bool NavMesh::ReadMap(std::string s)
{
	int y = 0;
	int x = 0;
	std::vector<char> list;
	fstream loadFile;
    loadFile.open (s, ifstream::in);   
    while (loadFile.good())
	{        
		list.push_back(loadFile.get());
	}
	for(int j = 0; j < list.size(); j++)
	{
		const char noWak = char(88);
		const char wak = char(48);
		const char start = char(83);
		const char goal = char(71);
		if(list[j] == noWak)
		{
			Tile* t = new Tile(x, y, false);
			tileList.push_back(t);
		}
		else if(list[j] == wak)
		{
			Tile* t = new Tile(x, y, true);
			tileList.push_back(t);
		}
		else if(list[j] == start)
		{
			Tile* t = new Tile(x, y, true);
			t->start = true;
			tileList.push_back(t);
		}
		else if(list[j] == goal)
		{
			Tile* t = new Tile(x, y, true);
			t->goal = true;
			tileList.push_back(t);
		}
		else
		{
			y++;
			x = -1;
		}
		x++;
	}
	findNeighbors();
	return true;
}

void NavMesh::findNeighbors()
{
	for(int i = 0; i < tileList.size(); i++)
	{
		if(tileList[i]->walkable) //only walkables have neighbors
		{
			for(int j = 0; j < tileList.size(); j++) //find other tiles next to this tile
			{
				
				if(tileList[j]->x == tileList[i]->x+1 && tileList[j]->y == tileList[i]->y && tileList[j]->walkable)
				{
					tileList[i]->neighbors.push_back(tileList[j]);
				}
				else if(tileList[j]->x == tileList[i]->x-1 && tileList[j]->y == tileList[i]->y+1 && tileList[j]->walkable)
				{
					tileList[i]->neighbors.push_back(tileList[j]);
				}
				else if(tileList[j]->x == tileList[i]->x && tileList[j]->y == tileList[i]->y+1 && tileList[j]->walkable)
				{
					tileList[i]->neighbors.push_back(tileList[j]);
				}
				else if(tileList[j]->x == tileList[i]->x+1 && tileList[j]->y == tileList[i]->y+1 && tileList[j]->walkable)
				{
					tileList[i]->neighbors.push_back(tileList[j]);
				}
				else if(tileList[j]->x == tileList[i]->x-1 && tileList[j]->y == tileList[i]->y-1 && tileList[j]->walkable)
				{
					tileList[i]->neighbors.push_back(tileList[j]);
				}
				else if(tileList[j]->x == tileList[i]->x && tileList[j]->y == tileList[i]->y-1 && tileList[j]->walkable)
				{
					tileList[i]->neighbors.push_back(tileList[j]);
				}
				else if(tileList[j]->x == tileList[i]->x+1 && tileList[j]->y == tileList[i]->y-1 && tileList[j]->walkable)
				{
					tileList[i]->neighbors.push_back(tileList[j]);
				}
				else if(tileList[j]->x == tileList[i]->x-1 && tileList[j]->y == tileList[i]->y && tileList[j]->walkable)
				{
					tileList[i]->neighbors.push_back(tileList[j]);
				}
			}
		}
	}
}
void NavMesh::UpdateList()
{
	for(int i = 0; i < tileList.size(); i++)
	{
		tileList[i]->searched = false;
		tileList[i]->flaggdOpen = false;
		tileList[i]->flaggedClosed = false;
		tileList[i]->parent = NULL;
	}
}

std::vector<Tile*> NavMesh::getList()
{
	return tileList;
}

bool NavMesh::Expand(int x) 
{
	Tile* t = tileList[x]; //the tile we are looking at
	//dont expand on this if it ia non walkable, flaggd(removed from list), is a goal or is start
	if(t->walkable && !t->flagged && !t->goal && !t->start) 
	{
		int i = rand() % t->neighbors.size(); //takes an random neighbor to start expanding with
		for(i; i < t->neighbors.size(); i++)
		{
			Tile* t2 = t->neighbors[i%t->neighbors.size()]; //neghboring tile to t
			if(!t2->flagged && !t2->goal && !t2->start) //if the neighbor in not flagged, goal or start
			{
				int intersect = Intersect(t, t2); //check what way to expand
				//if expanding
				if(intersect == 1 || intersect == 2) 
				{
					//if expanding on x
					if(intersect == 1)
					{
						//if negbor is on left side
						if(t->x > t2->x)
						{
							for(int j = 0; j < t->neighbors.size(); j++) //relink steal negbor
							{
								if(!neighborCheck(t->neighbors[j], t2))
								{
									t2->neighbors.push_back(t->neighbors[j]);
									for(int k = 0; k < t->neighbors[j]->neighbors.size(); k++)
									{
										if(t->neighbors[j]->neighbors[k] == t)
										{
											t->neighbors[j]->neighbors[k] = t2;
										}
									}
								}
							}
							t2->width += t->width; //make width bigger
							tileList[x]->flagged = true; //flagg for removed from main list
							return true;
						}
						//if negbor is on right side
						else
						{
							for(int j = 0; j < t2->neighbors.size(); j++) //relink steal negbor
							{
								if(!neighborCheck(t2->neighbors[j], t))
								{
									t->neighbors.push_back(t2->neighbors[j]);
									for(int k = 0; k < t2->neighbors[j]->neighbors.size(); k++)
									{
										if(t2->neighbors[j]->neighbors[k] == t2)
										{
											t2->neighbors[j]->neighbors[k] = t;
										}
									}
								}
							}
							t->width += t2->width; //make width bigger
							for(int k = 0; k < tileList.size(); k++)
							{
								if(t2 == tileList[k])
								{
									tileList[k]->flagged = true; //flagged to true for removal from main list
									return true;
								}
							}
						}
					}
					//if expanding on y
					else
					{
						//if negbor is above
						if(t->y > t2->y)
						{
							for(int j = 0; j < t->neighbors.size(); j++) //relink steal negbor
							{
								if(!neighborCheck(t->neighbors[j], t2))
								{
									t2->neighbors.push_back(t->neighbors[j]);
									for(int k = 0; k < t->neighbors[j]->neighbors.size(); k++)
									{
										if(t->neighbors[j]->neighbors[k] == t)
										{
											t->neighbors[j]->neighbors[k] = t2;
										}
									}
								}
							}
							t2->height += t->height; //make height bigger
							tileList[x]->flagged = true; //flagg for removal from main list
							return true;
						}
						//if neghbor is below
						else
						{
							for(int j = 0; j < t2->neighbors.size(); j++) //relink steal negbor
							{
								if(!neighborCheck(t2->neighbors[j], t))
								{
									t->neighbors.push_back(t2->neighbors[j]);
									for(int k = 0; k < t2->neighbors[j]->neighbors.size(); k++)
									{
										if(t2->neighbors[j]->neighbors[k] == t2)
										{
											t2->neighbors[j]->neighbors[k] = t;
										}
									}
								}
							}
							t->height += t2->height; //make height bigger
							for(int k = 0; k < tileList.size(); k++)
							{
								if(t2 == tileList[k])
								{
									tileList[k]->flagged = true; //flagg this tile for removal from main list
									return true;
								}
							}
						}
					}
				}
			}
		}
	}
	return false;
}

bool NavMesh::neighborCheck(Tile* neighbor, Tile* t)
{
	for(int j = 0; j < t->neighbors.size(); j++)
	{
		//see if the naighbor list contains this naighbor
		if(t->neighbors[j] == neighbor) 
		{
			return true;
		}
		//make shureeee if the neighbor isent me
		if(neighbor == t)
		{
			return true;
		}
	}
	//return false puts this naighbor in naighber list
	return false;
}

int NavMesh::Intersect(Tile* t, Tile* t2)
{
	vector<float> test;
	vector<float> test2;
	//calculates the corners of t
	float vertexULX = t->x * 16;
	float vertexULY = t->y * 16 * -1;
	float vertexURX = vertexULX + (t->width * 16);
	float vertexURY = vertexULY;
	float vertexLLX = vertexULX;
	float vertexLLY = vertexULY - (t->height * 16);
	float vertexLRX = vertexURX;
	float vertexLRY = vertexLLY;

	test.push_back(vertexULX);
	test.push_back(vertexULY);
	test.push_back(vertexURX);
	test.push_back(vertexURY);
	test.push_back(vertexLLX);
	test.push_back(vertexLLY);
	test.push_back(vertexLRX);
	test.push_back(vertexLRY);

	//calculates the corners of t2
	float vertexULX2 = t2->x * 16;
	float vertexULY2 = t2->y * 16 * -1;
	float vertexURX2 = vertexULX2 + (t2->width * 16);
	float vertexURY2 = vertexULY2;
	float vertexLLX2 = vertexULX2;
	float vertexLLY2 = vertexULY2 - (t2->height * 16);
	float vertexLRX2 = vertexURX2;
	float vertexLRY2 = vertexLLY2;

	test2.push_back(vertexULX2);
	test2.push_back(vertexULY2);
	test2.push_back(vertexURX2);
	test2.push_back(vertexURY2);
	test2.push_back(vertexLLX2);
	test2.push_back(vertexLLY2);
	test2.push_back(vertexLRX2);
	test2.push_back(vertexLRY2);

	int same = 0;
	vector<float> vectorsaveX;
	vector<float> vectorsaveY;

	//finds corners that are the same between t and t2
	for(int i = 0; i < test.size(); i+=2)
	{
		for(int j = 0; j < test2.size(); j+=2)
		{
			if(test[i] == test2[j] && test[i+1] == test2[j+1])
			{
				same++;
				//saves the two corners in an vector for later use
				vectorsaveX.push_back(test[i]);
				vectorsaveY.push_back(test[i+1]);
			}
		}
	}
	if(same > 1) //if there is more then one corner that is the same(will always be two)
	{
		//cout<<"Not intersecting: "<<t2->x<<", "<<t2->y<<endl;
		if(vectorsaveX[0] == vectorsaveX[1]) //see if the tilees shares x values, to be able to know if it takes ower a tile on x or y side
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	else
	{
		//cout<<"Intersecting: "<<t2->x<<", "<<t2->y<<endl;
		return 0;
	}
}

void NavMesh::calcNodes()
{
	//sets the node in the middle of the tile
	for(int i = 0; i < tileList.size(); i++)
	{
		float vertexULX = tileList[i]->x * 16;
		float vertexULY = tileList[i]->y * 16 * -1;
		float vertexURX = vertexULX + (tileList[i]->width * 16);
		float vertexURY = vertexULY;
		float vertexLLX = vertexULX;
		float vertexLLY = vertexULY - (tileList[i]->height * 16);
		float vertexLRX = vertexURX;
		float vertexLRY = vertexLLY;

		tileList[i]->nodeX = (vertexULX+vertexLRX)/2;
		tileList[i]->nodeY = (vertexULY+vertexLRY)/2;
	}
}

