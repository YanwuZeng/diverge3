#include <stdio.h>
#include <math.h>

#include <qlayout.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qmessagebox.h>

#include "func_dist_tab.h"
#include "common.h"
#include "star_view.h"
#include "matrix.h"

//----------------------------------------------------------------------

FuncDistTab::FuncDistTab(QWidget *parent, const char *name) 
  : QWidget(parent, name)
{
  QVBoxLayout *top_layout = new QVBoxLayout(this, 10, 10);
  
  star_view_ = new StarView(this);
  top_layout->addWidget(star_view_);

  top_layout->activate();

#if 0
  int n = 4;
  DVector dist(n);
  dist(0) = 0.308827;
  dist(1) = -0.125694;
  dist(2) = 0.260831;
  dist(3) = -0.288524;
  QStringList names;
  names.append("Cluster 1");
  names.append("Cluster 2");
  names.append("Cluster 3");
  names.append("Cluster 4");
  star_view_->setData(names, dist);
#endif
}

//----------------------------------------------------------------------

FuncDistTab::~FuncDistTab() {}

//----------------------------------------------------------------------

void
FuncDistTab::clear() {
  star_view_->update();
}

//----------------------------------------------------------------------

void
FuncDistTab::updateResults(QStringList names, DMatrix2D theta) {
  double B = 0.0;
  int n = theta.cols();
  star_view_->clear();
  if(n < 3) return;
  DMatrix2D d(n, n);
  int i, j, k;
  for(i=0; i<n-1; i++) {
    for(j=i+1; j<n; j++) {
      d(j, i) = d(i, j) = -log(1.0 - theta(i, j));
#ifdef DEBUG_DIVERGE
      printf("i = %d, j = %d, theta(i, j) = %f, dij = %f\n", i, j, theta(i, j), d(j, i));
#endif
      B += d(i, j) / (n - 1);
    }
  }
  DVector dist(n);
  for(k=0; k<n; k++) {
    double bk = 0.0;
    for(i=0; i<n-1; i++) {
      if(i == k) continue;
      for(j=i+1; j<n; j++) {
	if(j == k) continue;
	bk += d(i, j) / (n - 2);
      }
    }
    dist(k) = B - bk;

#ifdef DEBUGE_DIVERGE
    printf("%d %s %f\n", k, (const char*)names[k], dist(k));
#endif

  }
  star_view_->setData(names, dist);
}

//----------------------------------------------------------------------
