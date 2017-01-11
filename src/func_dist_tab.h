#ifndef _FUNC_DIST_TAB_H_
#define _FUNC_DIST_TAB_H_

#include <qwidget.h>
#include <qstringlist.h>

#include "matrix.h"

class StarView;

class FuncDistTab : public QWidget {
  Q_OBJECT
public:
  FuncDistTab(QWidget *parent = NULL, const char *name = NULL);
  ~FuncDistTab();

public slots:
  void clear();
  void updateResults(QStringList, DMatrix2D);
 
private:
  StarView *star_view_;
};

#endif
