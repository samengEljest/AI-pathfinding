 //! main.cpp
#include "s006d_2.h"
#include "GlWindow.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GlWindow w;
	w.show();
	return a.exec();
}
