 //! GrapthSeatch.cpp
#include "GraphSearch.h"
#include <cmath>


GraphSearch::GraphSearch()
{
	goalFound = false;
}
GraphSearch::~GraphSearch(){}

std::vector<Tile*> GraphSearch::findPathDepthFirst(std::vector<Tile*> tileList)
{
	std::vector<Tile*> path;
	for(int i = 0; i < tileList.size(); i++)
	{
		if(tileList[i]->start)
		{
			searchNeighborsDepthFirst(tileList[i], &path);
			break;
		}
	}
	return path;
}

bool GraphSearch::searchNeighborsDepthFirst(Tile* t, std::vector<Tile*>* path)
{
	t->searched = true;
	if(t->goal)
	{
		path->push_back(t);
		this->goalFound = true;
	}
	else
	{
		for(int i = 0; i < t->neighbors.size(); i++)
		{
			if(!t->neighbors[i]->searched && !t->neighbors[i]->flagged)
			{
				searchNeighborsDepthFirst(t->neighbors[i], path);
			}
			if(goalFound)
			{
				path->push_back(t);
				break;
			}
		}
	}
	return true;
}

std::vector<Tile*> GraphSearch::findPathBreadthFirst(std::vector<Tile*> tileList)
{
	std::vector<Tile*> path;
	std::list<Tile*> bfQueue;
	Tile* pathTile;
	for(int i = 0; i < tileList.size(); i++)
	{
		if(tileList[i]->start)
		{
			pathTile = searchNeighborsBreadthFirst(tileList[i], NULL, &bfQueue);
			break;
		}
	}
	while(pathTile != NULL)
	{
		path.push_back(pathTile);
		pathTile = pathTile->parent;
	}
	return path;
}

Tile* GraphSearch::searchNeighborsBreadthFirst(Tile* t, Tile* t2, std::list<Tile*>* bfQueue)
{
	t->searched = true;
	if(t->goal)
	{
		return t;
	}

	if(t2 != NULL)
	{
		bfQueue->pop_front();
	}
	else
	{
		t->parent = NULL;
	}

	for(int i = 0; i < t->neighbors.size(); i++)
	{
		if(!t->neighbors[i]->searched && !t->neighbors[i]->flagged)
		{
			bfQueue->push_back(t->neighbors[i]);
			t->neighbors[i]->searched = true;
			t->neighbors[i]->parent = t;
		}
	}
	searchNeighborsBreadthFirst(bfQueue->front(), t, bfQueue);
}

std::vector<Tile*> GraphSearch::findPathRNG(std::vector<Tile*> tileList)
{
	std::vector<Tile*> path;
	for(int i = 0; i < tileList.size(); i++)
	{
		if(tileList[i]->start)
		{
			searchNeighborsRNG(tileList[i], NULL, &path);
			break;
		}
	}
	return path;
}

void GraphSearch::searchNeighborsRNG(Tile* t, Tile* lastT, std::vector<Tile*>* path)
{
	path->push_back(t);
	if(t->goal)
	{
		return;
	}
	int realNeighbors = 0;
	for(int i = 0; i < t->neighbors.size(); i++)
	{
		if(!t->neighbors[i]->flagged)
		{
			realNeighbors++;
		}
	}
	int direction = rand() % t->neighbors.size();
	if(realNeighbors > 1)
	{
		while(t->neighbors[direction] == lastT || t->neighbors[direction]->flagged)
		{
			direction = rand() % t->neighbors.size();
		}
	}

	searchNeighborsRNG(t->neighbors[direction], t, path);
}

std::vector<Tile*> GraphSearch::findAStar(std::vector<Tile*> tileList)
{
	std::vector<Tile*> openList;
	std::vector<Tile*> path;
	Tile* start;
	Tile* goal;
	for(int i = 0; i < tileList.size(); i++)
	{
		if(tileList[i]->start)
		{
			start = tileList[i];
		}
		if(tileList[i]->goal)
		{
			goal = tileList[i];
		}
	}
	Tile* pathTile = searchNeighborsAStar(start,goal, &openList);
	while(pathTile != NULL)
	{
		path.push_back(pathTile);
		pathTile = pathTile->parent;
	}
	return path;

	return openList;
}

Tile* GraphSearch::searchNeighborsAStar(Tile* t, Tile* goal, std::vector<Tile*>* open)
{
	bool goalFound = false;
	Tile* NowTile = t;
	Tile* temp;
	open->push_back(t);
	t->flaggdOpen = true;
	while(true)
	{
		Tile* LowfTile = new Tile(0,0,true);
		LowfTile->fValue = 99999;
		if(NowTile->goal)
		{
			goalFound = true;
			return NowTile;
		}
		else
		{
			for(int i = 0; i < NowTile->neighbors.size(); i++)
			{
				if(!NowTile->neighbors[i]->flaggdOpen && !NowTile->neighbors[i]->flagged) //
				{
					NowTile->neighbors[i]->flaggdOpen = true;
					NowTile->neighbors[i]->parent = NowTile;
					float Gx = std::abs(NowTile->neighbors[i]->nodeX - NowTile->nodeX);
					float Gy = std::abs(NowTile->neighbors[i]->nodeY - NowTile->nodeY);
					float G = std::sqrt(std::abs(std::pow(Gx,2)+std::pow(Gy,2)));
					NowTile->neighbors[i]->gValue = G;
					float Hx = std::abs(goal->nodeX - NowTile->neighbors[i]->nodeX);
					float Hy = std::abs(goal->nodeY - NowTile->neighbors[i]->nodeY);
					float H = std::sqrt(std::pow(Hx,2)+std::pow(Hy,2));
					NowTile->neighbors[i]->fValue = G+H;
					open->push_back(NowTile->neighbors[i]);
				}
			}
			for (int i = 0; i < open->size(); i++)
			{
				if(open->at(i)->fValue < LowfTile->fValue && !open->at(i)->flaggedClosed)
				{
					if(LowfTile->fValue == 99999)
					{
						temp = LowfTile;
						LowfTile = open->at(i);
						delete temp;
					}
					LowfTile = open->at(i);
				}
			}
			LowfTile->flaggedClosed = true;
			NowTile = LowfTile;
		}
	}
}