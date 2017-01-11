#ifndef _STAR_VIEW_H_
#define _STAR_VIEW_H_

#include <qframe.h>
#include <qstringlist.h>

#include "matrix.h"

class StarView : public QFrame {
public:
  StarView(QWidget*);
  ~StarView();

  void setData(QStringList, DVector);
  void clear();
  
protected:
  QStringList names_;
  DVector dist_;

  void drawContents(QPainter*);
};

#endif
