#include <string>
#include <vector>

#include <math.h>

#include <qframe.h>
#include <qpainter.h>

#include "star_view.h"
#include "matrix.h"

//----------------------------------------------------------------------

using namespace std;

//----------------------------------------------------------------------

StarView::StarView(QWidget *p)
  : QFrame(p)
{
  setBackgroundColor(white);
}

StarView::~StarView() {
}

void
StarView::setData(QStringList names, DVector d) {
  names_ = names;
  dist_ = d;
  update();
}

void
StarView::clear() {
  QStringList names;
  DVector d;
  setData(names, d);
}

void
StarView::drawContents(QPainter *p) {
  int n = dist_.size(), i;
  if(n < 3) {
    p->drawText(100, 100, "Please First Use Gu99 Method");
    p->drawText(100, 112, "With At Least Three clusters");
    return;
  }
    
  double max = 0.0;
  for(i=0; i<n; i++) {
    if(max < fabs(dist_(i))) max = fabs(dist_(i));
  }
  int w = width(), h= height();
  double dt = 2*3.14159 / n;
  int cx = w/2, cy = h/2, r;

  if(w < h) {
    r = w/2 - 50;
  } else {
    r = h/2 - 50;
  }

  p->setPen(QPen(gray, 5));
  p->setBrush(gray);
  p->drawPie(cx-5, cy-5, 10, 10, 0, 5760);
  for(i=0; i<n; i++) {
    double d = fabs(dist_(i));
    double r2 = d / max * r;
    int x = (int)(r2 * cos(dt * i) + 0.5) + cx;
    int y = (int)(r2 * sin(dt * i) + 0.5) + cy;
    p->setPen(QPen(gray, 5));
    p->drawPie(x-5, y-5, 10, 10, 0, 5760);
    p->drawLine(cx, cy, x, y);
    p->setPen(black);
    p->drawText(x-50, y-12, 100, 12, AlignCenter, names_[i]);
    QString tmp;
    tmp.setNum(dist_(i));
    p->drawText(x-50, y, 100, 12, AlignCenter, tmp);
  }
}
