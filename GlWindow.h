 //! GlWindow.h
/*!
  This class handels the graphical openGL screen
  and serten inpusts from the keyboard.
*/
#pragma once

#include <QtOpenGL\QGLWidget>
#include <QGLWidget>
#include <QTimer>
#include <vector>
#include "s006d_2.h"
#include "Tile.h"
#include "NavMesh.h"
#include "GraphSearch.h"
#include <iostream>

class GlWindow : public QGLWidget
{
	Q_OBJECT
public:
	GlWindow(QWidget* parent = 0); ///< opens the openGL window with the chosen file
	~GlWindow();
	void PaintObject(Tile* t); ///< paints out the Tile on the openGL window
	void PaintPath(Tile* t, Tile* t2); ///< paints out the path when a graphical search has been done

private:
	Ui::S006D_2Class ui;
	QTimer timer; ///< run paintGL timer
	NavMesh *nv; 
	GraphSearch *gf;
	std::vector<Tile*> path;
	bool pathCreated;
	LARGE_INTEGER start, end, freq;

protected:
	void initializeGL(); ///< sets color off the screen and size of the window
	void paintGL(); ///< calls PaintObject and PaintPaths
	void resizeGL(int width, int height);
	void keyPressEvent(QKeyEvent *); ///< handels key press events from keyboard (a,b,d,r,1,2,3,space)
};
