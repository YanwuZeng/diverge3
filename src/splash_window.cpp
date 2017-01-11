#include <qlabel.h>
#include <qpixmap.h>
#include <qapplication.h>

#include "splash_window.h"
#include "splash.xpm"

SplashWindow::SplashWindow()
    : QLabel(0, "splash", Qt::WDestructiveClose | Qt::WStyle_Customize | Qt::WStyle_NoBorder)
{
  this->setFrameStyle( QFrame::NoFrame );
  //QPixmap pix( filename );
  QPixmap pix = QPixmap( splash );
  this->setPixmap( pix );
  this->adjustSize(); // This is needed to adjust the widget size to the image
  QRect r = QApplication::desktop()->geometry();
  this->move( r.center() - this->rect().center() );
  this->repaint(false);
  QApplication::flushX(); 
}
   
void SplashWindow::mousePressEvent(QMouseEvent *e) {
	hide();
}

void SplashWindow::keyPressEvent(QKeyEvent *e) {
	hide();
}
