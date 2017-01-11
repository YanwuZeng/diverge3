#include <vector>

#include <stdio.h>

#include <qscrollview.h>
#include <qdialog.h>
#include <qpushbutton.h>
#include <qpainter.h>

#include "graph_view.h"
#include "common.h"

//----------------------------------------------------------------------

using namespace std;

//----------------------------------------------------------------------

GraphView::GraphView(QWidget *parent, const char *name, WFlags f)
  : QScrollView(parent, name, f),
    tw(0), th(0),
    label_max_w(0), label_max_h(0),
    seq_max_w(0), seq_max_h(0),
    min_pos(int(10e6)), max_pos(int(-10e6)),
    min_value(10e6), max_value(-10e6)
{
  viewport()->setBackgroundMode(NoBackground);
  setFocusPolicy(QWidget::StrongFocus);

  colors[ 0] = QColor(230,  10,  10);
  colors[ 1] = QColor( 20,  90, 255);
  colors[ 2] = QColor(150/*50*/,  50, 170);
  colors[ 3] = QColor(0/*235*/, 235, 235);
  colors[ 4] = QColor(0/*200*/, 100/*200*/, 200);
  colors[ 5] = QColor(130, 130, 210);
  colors[ 6] = QColor(230, 230,   0);
  colors[ 7] = QColor(250, 150,   0);
  colors[ 8] = QColor(  0, 220, 220);
  colors[ 9] = QColor( 15, 130,  15);
  colors[10] = QColor(180,  90, 180);
  colors[11] = QColor(220, 150, 130);
  colors[12] = QColor(190, 160, 110);
}

//----------------------------------------------------------------------

void
GraphView::set_results(const vector<result_t> &results) {
  results_ = results;

  if(results_.empty()) {
    resizeContents(5, 5);
    viewport()->update();
    return;
  }

  min_pos=1;
  max_pos=results.size();
  min_value=0.0;
  max_value=1.0;

  int bar_w = 8;
  int bar_dis = 2;
  int bar_dis2 = 16;

  int npos = results.size();
  int nbars = results[0].values.size();
  
  int req_width = (bar_w * nbars + bar_dis * (nbars - 1) + bar_dis2) * npos;
  int req_height = height() - 20;

  if(contentsWidth() != req_width || contentsHeight() != req_height) {
    resizeContents(req_width, req_height);
  }// else {
  //viewport()->update();
  //}
  viewport()->update();
}

//----------------------------------------------------------------------

void
GraphView::drawContents(QPainter *p, int cx, int cy, int cw, int ch) {
  /*
    cx, cy - starting position needing update
    cw, ch - width and height of area needing update
  */
  
  p->fillRect(cx, cy, cw, ch, white);

  if(results_.empty()) return;

  int nbars = results_[0].values.size();
  int bar_w = 8;
  int bar_dis = 2;

  int pos_label_reserve = 20;
  int value_label_reserve = 50;
  
  int width = contentsWidth();
  int height = contentsHeight();

  int plot_x = value_label_reserve;
  int plot_y = 5;
  int plot_width = width - plot_x - value_label_reserve;
  int plot_height = height - plot_y - pos_label_reserve;
  
  int npos = results_.size();
  int nvalue = 4;
  double pos_dx = plot_width / double(npos-1);
  double value_dx = plot_height / double(nvalue-1);

  QFontMetrics fm = viewport()->fontMetrics();

  int mh = fm.height();  

  int i;
  for(i=0; i<npos; i++) {
    result_t r = results_[i];

    for(int j=0; j<nbars; j++) {
      p->setPen(colors[j % 13]);
      p->setBrush(colors[j % 13]);

      int h = int(r.values[j] * plot_height + 0.5);
      int x = (bar_w + bar_dis) * j;
      p->fillRect(int(plot_x + x + pos_dx * i + 2 + .5), plot_y + plot_height, bar_w, -h, p->brush());
    }
  }

  p->setPen(black);
  
  p->drawLine(plot_x, plot_y, plot_x, plot_y + plot_height + 5);
  p->drawLine(plot_x - 5, plot_y + plot_height, plot_x + plot_width+20, plot_y + plot_height);

  for(i=0; i<npos; i++) {
    p->drawLine(int(plot_x + (bar_w + bar_dis)*nbars/2 + pos_dx * i + .5), plot_y + plot_height - 5,
		int(plot_x + (bar_w + bar_dis)*nbars/2 + pos_dx * i + .5), plot_y + plot_height + 5);
    char str[16];
    sprintf(str, "%d", results_[i].pos);
    p->drawText(int(plot_x + (bar_w + bar_dis)*nbars/2 + pos_dx * i - fm.width(str)/2 + .5),
		plot_y + plot_height + 20, str);
  } 

  for(i=0; i<nvalue; i++) {
    p->drawLine(plot_x - 5, int(plot_y + value_dx * i + .5),
	  	plot_x + 5, int(plot_y + value_dx * i + .5));
    char str[16];
    sprintf(str, "%.2f", (max_value - min_value) / double(nvalue-1) * i + min_value);
    p->drawText(5, int(plot_y + value_dx * (nvalue - 1 - i) + mh/2 + .5), str);
  }
}

//----------------------------------------------------------------------

void
GraphView::viewportResizeEvent (QResizeEvent *e) {
  QScrollView::viewportResizeEvent(e);
  resizeContents(contentsWidth(), e->size().height());
}

//----------------------------------------------------------------------

void
GraphView::clear () {
  vector<result_t> r;
  set_results(r);
}

//----------------------------------------------------------------------

void
GraphView::contentsMousePressEvent(QMouseEvent *) {
  setFocus();
}

//----------------------------------------------------------------------

void
GraphView::keyPressEvent(QKeyEvent *e) {
  switch(e->key()) {
  case Key_Up:
    scrollBy(0, -visibleHeight()/5);
    break;
  case Key_Down:
    scrollBy(0, visibleHeight()/5);
    break;
  case Key_Left:
    scrollBy(-visibleWidth()/5, 0);
    break;
  case Key_Right:
    scrollBy(visibleWidth()/5, 0);
    break;
  case Key_PageUp:
    scrollBy(0, -visibleHeight());
    break;
  case Key_PageDown:
    scrollBy(0, visibleHeight());
    break;
  default:
    e->ignore();
  }
}

//----------------------------------------------------------------------

GraphViewDialog::GraphViewDialog(QWidget *parent, const char *name, bool modal, WFlags f)
  : QDialog(parent, name, modal, f)
{
  setMinimumSize(400, 400);
  setCaption("Graph View");

  gv_ = new GraphView(this);

  ok_ = new QPushButton("OK", this);
  connect(ok_, SIGNAL(clicked()), SLOT(accept()));
}

void
GraphViewDialog::setResults(const vector<result_t> &results) {
  gv_->set_results(results);
}

void
GraphViewDialog::resizeEvent(QResizeEvent *e) {
  int w = e->size().width();
  int h = e->size().height();
  
  gv_->setGeometry(10, 10, w-20, h-20-10-30);
  ok_->setGeometry((w-100)/2, h-10-30, 100, 30);
}

//----------------------------------------------------------------------
