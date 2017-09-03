 //! s006d_2.h
/*!
  Just a base class for 
*/
#ifndef S006D_2_H
#define S006D_2_H

#include <QtGui/QMainWindow>
#include "ui_s006d_2.h"

class S006D_2 : public QMainWindow
{
	Q_OBJECT

public:
	S006D_2(QWidget *parent = 0, Qt::WFlags flags = 0);
	~S006D_2();

private:
	Ui::S006D_2Class ui;
};

#endif // S006D_2_H
