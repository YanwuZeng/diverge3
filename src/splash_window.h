#ifndef _SPLASH_WINDOW_H_
#define _SPLASH_WINDOW_H_

#include <qlabel.h>

class SplashWindow: public QLabel {
public:
  SplashWindow();
  
protected:
  void mousePressEvent(QMouseEvent *e);
  void keyPressEvent(QKeyEvent *e);
};
#endif
