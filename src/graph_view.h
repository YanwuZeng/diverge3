#ifndef _GRAPH_VIEW_H_
#define _GRAPH_VIEW_H_

#include <string>
#include <vector>

#include <qscrollview.h>
#include <qdialog.h>

#include "common.h"

#include "result.h"

class QPushButton;

class GraphView : public QScrollView {
public:
  GraphView(QWidget *parent, const char *name=NULL, WFlags f=0);
  
  void set_results(const std::vector<result_t> &results);
  void clear();
  
protected:
  void drawContents(QPainter *paint, int cx, int cy, int cw, int ch);
  void viewportResizeEvent (QResizeEvent *e);
  void contentsMousePressEvent(QMouseEvent *e);
  void keyPressEvent(QKeyEvent *e);
  
private:
  std::vector<result_t> results_;
  int tw, th;
  int label_max_w, label_max_h;
  int seq_max_w, seq_max_h;
  int min_pos, max_pos;
  double min_value, max_value;
  QColor colors[13];
};

class GraphViewDialog : public QDialog {
  Q_OBJECT
public:
  GraphViewDialog(QWidget *parent=NULL, const char *name=NULL, bool modal=false, WFlags f=0);

  void setResults(const std::vector<result_t> &results);

protected:
  GraphView *gv_;
  QPushButton *ok_;
  
  virtual void resizeEvent(QResizeEvent *);
};

#endif
