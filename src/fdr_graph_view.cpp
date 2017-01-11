#include <vector>

#include <stdio.h>

#include <qscrollview.h>
#include <qdialog.h>
#include <qpushbutton.h>
#include <qpainter.h>

#include "fdr_graph_view.h"
#include "common.h"

#ifdef WIN32
#include <minmax.h>
#endif
//----------------------------------------------------------------------

using namespace std;

//----------------------------------------------------------------------

FDRGraphView::FDRGraphView(QWidget *parent, const char *name, WFlags f)
  : QScrollView(parent, name, f),
    tw(0), th(0),
    label_max_w(0), label_max_h(0),
    seq_max_w(0), seq_max_h(0),
	min_axesnum(int(10e6)), max_axesnum(int(-10e6)),
    min_points(int(10e6)), max_points(int(-10e6)),
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
FDRGraphView::set_results(const vector<result_t> &results1, const vector<result_t> &results2) 
{
  results1_ = results1;
  results2_ = results2;

  if (results1_.empty() && results2_.empty()) 
  {
    resizeContents(5, 5);
    viewport()->update();
    return;
  }


  min_axesnum = 1; 
  max_axesnum = 2; 
  min_points = 1; 
  max_points = max(results1.size(), results2.size()); 
  min_value = 0.0;
  max_value = 1.0;

  int axes_width = 600;
  int axes_dis = 50; 
  int margin_width = 50; 

  int naxes = 2;
  
  int req_width = axes_width * naxes + axes_dis * (naxes - 1) + margin_width * 2; 
  int req_height = height() - 20;

  if(contentsWidth() != req_width || contentsHeight() != req_height) 
  {
    resizeContents(req_width, req_height);
  }

  viewport()->update(); 

}

//----------------------------------------------------------------------

void
FDRGraphView::drawContents(QPainter *p, int cx, int cy, int cw, int ch) 
{
  /*
    cx, cy - starting position needing update
    cw, ch - width and height of area needing update
  */
  
  p->fillRect(cx, cy, cw, ch, white);

  if (results1_.empty() && results2_.empty()) return;

  min_value = 0.0;
  max_value = 1.0;

  double stepValue = 0.02; 

  int axes_width = 600;
  int axes_dis = 50; 
  int margin_width = 50; 
  int margin_height = 40; 

  int naxes = 2;
  int ncurves = 0;
  int npoints = 0; 

  int width = contentsWidth();
  int height = contentsHeight();

  int plot_x = margin_width;
  int plot_y = 5;
  int plot_width = width - plot_x - margin_width;
  int plot_height = height - plot_y - margin_height;


  int nyvalue = 5;
  double axes_dx = 0; 
  double axes_dy = plot_height / double(nyvalue-1); 

  QFontMetrics fm = viewport()->fontMetrics();

  int mh = fm.height();  


  ncurves = results1_[0].values.size();
  npoints = results1_.size();

  axes_dx = axes_width / double(npoints - 1); 

  int i = 0; 
  int j = 0; 
  int n = 0; 


  for (i = 0; i < ncurves; i ++)
  {
	  p->setPen(colors[i % 13]);
	  p->setBrush(colors[i % 13]);

	  result_t r1 = results1_[0];
	  result_t r2;

	  double x1Pos = int(plot_x + 0.5);
	  double x2Pos = 0; 
	  double y1Height = int(r1.values[i] * plot_height + 0.5);
	  double y2Height = 0; 

	  for (j = 1; j < npoints; j++)
	  {
		  r2 = results1_[j];

		  if (r2.values[i] >= 0)
		  {
			  x2Pos = int(plot_x + j * axes_width / (npoints - 1) + 0.5); 
			  y2Height = int(r2.values[i] * plot_height + 0.5);

			  p->drawLine(x1Pos, plot_y + plot_height - y1Height, x2Pos, plot_y + plot_height - y2Height); 

			  x1Pos = x2Pos; 
			  y1Height = y2Height; 
		  }
	  }
  }


  p->setPen(black);


  p->drawLine(plot_x, plot_y, plot_x, plot_y + plot_height + 5);
  p->drawLine(plot_x - 5, plot_y + plot_height, plot_x + axes_width + 10, plot_y + plot_height); 


  double tv = min_value; 
  for (i = 0; i < npoints; i++) 
  {
	  if (i % 5 == 0)
	  {
	      p->drawLine(int(plot_x + axes_dx * i + 0.5), plot_y + plot_height - 5,
		              int(plot_x + axes_dx * i + 0.5), plot_y + plot_height + 5); 
	  }
	  if (i % 20 == 0)
	  {
		  char str[16];
		  sprintf(str, "%d", i); 
		  p->drawText(int(plot_x + axes_dx * i - fm.width(str) / 2 + 0.5),
			  plot_y + plot_height + 20, str);
	  }

	  tv += stepValue; 
  }

  char cstr[100]; 
  sprintf(cstr, "Number of Sorted Sites be Selected"); 
  p->drawText(int(plot_x + axes_dx * npoints / 2 - fm.width(cstr) / 2 + 0.5), plot_y + plot_height + 35, cstr);

  for (i = 0; i < nyvalue; i++) 
  {
	  p->drawLine(plot_x - 5, int(plot_y + axes_dy * i + 0.5),
		  plot_x + 5, int(plot_y + axes_dy * i + 0.5));
	  char str[16];
	  sprintf(str, "%.2f", (max_value - min_value) / double(nyvalue - 1) * i + min_value); 
	  p->drawText(10, int(plot_y + axes_dy * (nyvalue - 1 - i) + mh/2 + 0.5), str); 
  }


  ncurves = results2_[0].values.size();
  npoints = results2_.size();

  axes_dx = axes_width / double(npoints - 1); 

  i = 0; 
  j = 0; 
  n = 0; 

  for (i = 0; i < ncurves; i ++)
  {
	  p->setPen(colors[i % 13]);
	  p->setBrush(colors[i % 13]);

	  result_t r1 = results2_[0];
	  result_t r2;

	  double x1Pos = int(plot_x * 1.5 + axes_width + axes_dis + 0.5); 
	  double x2Pos = 0; 
	  double y1Height = int(r1.values[i] * plot_height + 0.5);
	  double y2Height = 0; 

	  for (j = 1; j < npoints; j++)
	  {
		  r2 = results2_[j];

		  if (r2.values[i] >= 0)
		  {
			  x2Pos = int(plot_x * 1.5 + j * axes_width / (npoints - 1) + axes_width + axes_dis + 0.5); 
			  y2Height = int(r2.values[i] * plot_height + 0.5);

			  p->drawLine(x1Pos, plot_y + plot_height - y1Height, x2Pos, plot_y + plot_height - y2Height); 

			  x1Pos = x2Pos; 
			  y1Height = y2Height; 
		  }
	  }
  }
  

  p->setPen(black);


  p->drawLine(plot_x * 1.5 + axes_width + axes_dis, plot_y, plot_x * 1.5 + axes_width + axes_dis, plot_y + plot_height + 5);
  p->drawLine(plot_x * 1.5 + axes_width + axes_dis - 5, plot_y + plot_height, plot_x * 1.5 + 2 * axes_width + axes_dis + 10, plot_y + plot_height); 


  tv = min_value; 
  for (i = 0; i < npoints; i++) 
  {
	  if (i % 5 == 0)
	  {
		  p->drawLine(int(plot_x * 1.5 + axes_width + axes_dis + axes_dx * i + 0.5), plot_y + plot_height - 5,
			          int(plot_x * 1.5 + axes_width + axes_dis + axes_dx * i + 0.5), plot_y + plot_height + 5); 
	  }
	  if (i % 20 == 0)
	  {
	      char str[16];
		  sprintf(str, "%d", i); 
	      p->drawText(int(plot_x * 1.5 + axes_width + axes_dis + axes_dx * i - fm.width(str) / 2 + 0.5),
	      plot_y + plot_height + 20, str);
	  }

	  tv += stepValue; 
  }

  p->drawText(int(plot_x * 1.5 + axes_width + axes_dis + axes_dx * npoints / 2 - fm.width(cstr) / 2 + 0.5), plot_y + plot_height + 35, cstr);

  for (i = 0; i < nyvalue; i++) 
  {
	  p->drawLine(plot_x * 1.5 + axes_width + axes_dis - 5, int(plot_y + axes_dy * i + 0.5),
		          plot_x * 1.5 + axes_width + axes_dis + 5, int(plot_y + axes_dy * i + 0.5));
	  char str[16];
	  sprintf(str, "%.2f", (max_value - min_value) / double(nyvalue - 1) * i + min_value); 
	  p->drawText(plot_x * 0.5 + axes_width + axes_dis + 10, int(plot_y + axes_dy * (nyvalue - 1 - i) + mh/2 + 0.5), str); 
  }

}

//----------------------------------------------------------------------

void
FDRGraphView::viewportResizeEvent (QResizeEvent *e) {
  QScrollView::viewportResizeEvent(e);
  resizeContents(contentsWidth(), e->size().height());
}

//----------------------------------------------------------------------

void
FDRGraphView::clear () {
  vector<result_t> r1, r2;
  set_results(r1, r2);
}

//----------------------------------------------------------------------

void
FDRGraphView::contentsMousePressEvent(QMouseEvent *) {
  setFocus();
}

//----------------------------------------------------------------------

void
FDRGraphView::keyPressEvent(QKeyEvent *e) {
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
