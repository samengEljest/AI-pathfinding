 //! GlWindow.cpp
#pragma once
#include "GlWindow.h"
#include <QtOpenGL>
#include <GL/glu.h>
#include <QKeyEvent>

GlWindow::GlWindow(QWidget* parent) : QGLWidget(parent)
{
	this->timer.setInterval(1);
	connect(&this->timer, SIGNAL(timeout()), this, SLOT(updateGL()));
	this->timer.start();

	pathCreated = false;
	nv = new NavMesh();
	nv->ReadMap("Map1.txt");
	nv->calcNodes();
	gf = new GraphSearch();
	
}

GlWindow::~GlWindow()
{}

void GlWindow::initializeGL()
{
	glClearColor(0, 0, 0, 1);
	glDisable(GL_DEPTH_TEST);
	resize(600,600);
}

void GlWindow::resizeGL( int width, int height )
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(width, height, 90.0f, 0);
	//glOrtho(0, width, height, 0, 0, -1);
	//glViewport(0,0,width, height);
}


void GlWindow::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
 
	glLoadIdentity();
 
	glMatrixMode(GL_MODELVIEW);

	std::vector<Tile*> list = nv->getList();

	for(int i = 0; i < list.size(); i++)
	{
		if(!list[i]->flagged)
		{
			PaintObject(list[i]);
		}
	}
	if(pathCreated)
	{
		for(int i = 0; i < path.size()-1; i++)
		{
			PaintPath(path[i], path[i+1]);
		}
	}
}

void GlWindow::PaintObject(Tile* t)
{
	float vertexULX = t->x * 16;
	float vertexULY = t->y * 16 * -1;
	float vertexURX = vertexULX + (t->width * 16);
	float vertexURY = vertexULY;
	float vertexLLX = vertexULX;
	float vertexLLY = vertexULY - (t->height * 16);
	float vertexLRX = vertexURX;
	float vertexLRY = vertexLLY;

	float xMod = 400;
	float yMod = 200;
	
	if(t->goal)
	{
		glColor3f(1.0f, .0f, 0.0f);
	}
	else if(t->start)
	{
		glColor3f(0.0f, 1.0f, 0.0f);
	}
	else if(t->walkable)
	{
		glColor3f(1.0f, 1.0f, 0.0f);
	}
	else
	{
		glColor3f(0.0f, 0.0f, 1.0f);
	}

	glBegin(GL_QUADS);
		glVertex2f(vertexULX/xMod-1.0f, vertexULY/yMod+1.0f);
		glVertex2f(vertexLLX/xMod-1.0f, vertexLLY/yMod+1.0f);
		glVertex2f(vertexLRX/xMod-1.0f, vertexLRY/yMod+1.0f);
		glVertex2f(vertexURX/xMod-1.0f, vertexURY/yMod+1.0f);
	glEnd();
	float nodeX = t->nodeX;
	float nodeY = t->nodeY;
	if(t->walkable)
	{
		glColor3f(0.4f, 0.0f, 0.8f);
		glBegin(GL_LINES);
			glVertex2f(vertexULX/xMod-1.0f, vertexULY/yMod+1.0f);
			glVertex2f(vertexLLX/xMod-1.0f, vertexLLY/yMod+1.0f);
			glVertex2f(vertexLLX/xMod-1.0f, vertexLLY/yMod+1.0f);
			glVertex2f(vertexLRX/xMod-1.0f, vertexLRY/yMod+1.0f);
			glVertex2f(vertexLRX/xMod-1.0f, vertexLRY/yMod+1.0f);
			glVertex2f(vertexURX/xMod-1.0f, vertexURY/yMod+1.0f);
			glVertex2f(vertexURX/xMod-1.0f, vertexURY/yMod+1.0f);
			glVertex2f(vertexULX/xMod-1.0f, vertexULY/yMod+1.0f);
		glEnd();

		glColor3f(0.1f, 0.1f, 0.1f);
		glBegin(GL_POINTS);
			glVertex2f(nodeX/xMod-1.0f, nodeY/yMod+1.0f);
		glEnd();
	}
}

void GlWindow::PaintPath(Tile* t, Tile* t2)
{
	float xMod = 400;
	float yMod = 200;
	float nodeX = t->nodeX;
	float nodeY = t->nodeY;
	float nodeX2 = t2->nodeX;
	float nodeY2 = t2->nodeY;
	glColor3f(0.8f, 0.3f, 0.9f);
	glBegin(GL_LINES);
		glVertex2f(nodeX/xMod-1.0f, nodeY/yMod+1.0f);
		glVertex2f(nodeX2/xMod-1.0f, nodeY2/yMod+1.0f);
	glEnd();
}

void GlWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space)
    {
		pathCreated = false;
		for(int i = 0; i <nv->getList().size(); i++)
		{
			nv->Expand(i);
		}
		nv->calcNodes();
    }
	if(event->key() == Qt::Key_D)
    {
		delete gf;
		gf = new GraphSearch();
		nv->UpdateList();

		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&start); 
		path = gf->findPathDepthFirst(nv->getList());
		QueryPerformanceCounter(&end);

		pathCreated = true;
		  std::cout << "Time to calculate DepthFirst: " 
            << (end.QuadPart - start.QuadPart) * 1000000 / freq.QuadPart 
            << " microSeconds" << std::endl;
    }
	if(event->key() == Qt::Key_B)
    {
		delete gf;
		gf = new GraphSearch();
		nv->UpdateList();

		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&start); 
		path = gf->findPathBreadthFirst(nv->getList());
		QueryPerformanceCounter(&end);

		pathCreated = true;
		  std::cout << "Time to calculate BreadthFirst: "
            << (end.QuadPart - start.QuadPart) * 1000000 / freq.QuadPart 
            << " microSeconds" << std::endl;
    }
	if(event->key() == Qt::Key_R)
    {
		delete gf;
		gf = new GraphSearch();
		nv->UpdateList();

		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&start); 
		path = gf->findPathRNG(nv->getList());
		QueryPerformanceCounter(&end);

		pathCreated = true;
		  std::cout << "Time to calculate RNG: " 
            << (end.QuadPart - start.QuadPart) * 1000000 / freq.QuadPart 
            << " microSeconds" << std::endl;
    }
	if(event->key() == Qt::Key_A)
    {
		delete gf;
		gf = new GraphSearch();
		nv->UpdateList();
  
		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&start); 
		path = gf->findAStar(nv->getList());
		QueryPerformanceCounter(&end);

		pathCreated = true;
		  std::cout << "Time to calculate A*: " 
            << (end.QuadPart - start.QuadPart) * 1000000 / freq.QuadPart 
            << " microSeconds" << std::endl;
    }
	if(event->key() == Qt::Key_1)
    {
		delete nv;
		nv = new NavMesh();
		nv->ReadMap("Map1.txt");
		nv->calcNodes();
		pathCreated = false;
    }
	if(event->key() == Qt::Key_2)
    {
		delete nv;
		nv = new NavMesh();
		nv->ReadMap("Map2.txt");
		nv->calcNodes();
		pathCreated = false;
    }
	if(event->key() == Qt::Key_3)
    {
		delete nv;
		nv = new NavMesh();
		nv->ReadMap("Map3.txt");
		nv->calcNodes();
		pathCreated = false;
    }
}