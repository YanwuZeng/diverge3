#ifndef _FDR_GRAPH_VIEW_H_
#define _FDR_GRAPH_VIEW_H_

#include <string>
#include <vector>

#include <qscrollview.h>
#include <qdialog.h>

#include "common.h"

#include "result.h"

class QPushButton;

class FDRGraphView : public QScrollView {
public:
  FDRGraphView(QWidget *parent, const char *name=NULL, WFlags f=0);
  
  void set_results(const std::vector<result_t> &results1, const std::vector<result_t> &results2);
  void clear();
  
protected:
  void drawContents(QPainter *paint, int cx, int cy, int cw, int ch);
  void viewportResizeEvent (QResizeEvent *e);
  void contentsMousePressEvent(QMouseEvent *e);
  void keyPressEvent(QKeyEvent *e);
  
private:
  std::vector<result_t> results1_, results2_;
  int tw, th;
  int label_max_w, label_max_h;
  int seq_max_w, seq_max_h;
  int min_axesnum, max_axesnum;
  int min_points, max_points;
  double min_value, max_value;
  QColor colors[13];
};


#endif
