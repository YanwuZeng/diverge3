#include <qlistview.h>
#include <qwidget.h>
#include <qheader.h>
#include <qpixmap.h>
#include <qbitmap.h>
#include <qstyle.h>
#include <qpainter.h>
#include "qlistview_wcs.h"

//----------------------------------------------------------------------

HeaderEventFilter::HeaderEventFilter(QWidget *parent, const char *name)
  : QWidget(parent, name)
{
  if(parent) {
	  parent->installEventFilter(this);
  }
}

//----------------------------------------------------------------------

bool
HeaderEventFilter::eventFilter(QObject *o, QEvent *e) {
  if(e->type() == QEvent::MouseButtonRelease) {
    QMouseEvent *me = (QMouseEvent*)e;
    emit mouseRelease(me->x(), me->y());
  }

  return QWidget::eventFilter(o, e);
}
 
//----------------------------------------------------------------------

QListViewWCS::QListViewWCS(QWidget *parent, const char *name) 
  : QListView(parent, name),
    mx_(0), my_(0),
    sort_column_(0),
    sort_dir_(1)
{

#ifdef WIN32
  {
    on_pix_.resize(style().indicatorSize());
    QPainter p(&on_pix_);
    style().drawIndicator(&p, 0, 0, on_pix_.width(), on_pix_.height(), colorGroup(), 2);
  }

  {
    off_pix_.resize(style().indicatorSize());
    QPainter p(&off_pix_);
    style().drawIndicator(&p, 0, 0, off_pix_.width(), off_pix_.height(), colorGroup(), 0);
  }
#else
  {
    //on_pix_.resize(s.width(), s.height());
    on_pix_.resize(12, 12);
    QPainter p(&on_pix_);

    QRect r(0, 0, off_pix_.width(), off_pix_.height());
    style().drawControl(QStyle::CE_CheckBox, &p, this, r, colorGroup(), QStyle::Style_On);
  }

  {
    //off_pix_.resize(style().sizeFromContents(QStyle::CT_CheckBox, this, QSize(10, 10)));
    off_pix_.resize(12, 12);
    QPainter p(&off_pix_);
    QRect r(0, 0, off_pix_.width(), off_pix_.height());
    style().drawControl(QStyle::CE_CheckBox, &p, this, r, colorGroup(), QStyle::Style_Off);
  }

#endif  

  connect(header(), SIGNAL(clicked(int)), SLOT(header_clicked(int)));

  HeaderEventFilter *hef = new HeaderEventFilter;
  header()->installEventFilter(hef);
  connect(hef, SIGNAL(mouseRelease(int, int)), SLOT(header_mouse_release(int, int)));
}

//----------------------------------------------------------------------

QListViewWCS::~QListViewWCS() {}

//----------------------------------------------------------------------

void
QListViewWCS::sort(int column, int dir) {
  if(dir == -1) {
    if(column == sort_column_) {
      sort_dir_ = !sort_dir_;
    } else {
      sort_column_ = column;
      sort_dir_ = true;
    }
  } else {
    sort_column_ = column;
    sort_dir_ = dir;
  }
  setSorting(sort_column_, sort_dir_);
  QListView::sort();
  setSorting(-1);
}

//----------------------------------------------------------------------

int
QListViewWCS::addColumn(const QString &label, int size) {
  int n = columns();
  if(n > 0) {
    c_states_.resize(n);
    c_states_[n-1] = false;
    return QListView::addColumn(off_pix_, label, size);
  } else {
    return QListView::addColumn(label, size);
  }
}

//----------------------------------------------------------------------

int
QListViewWCS::addColumn(const QIconSet &, const QString &label, int size) {
  return addColumn(label, size);
}
  
//----------------------------------------------------------------------

void
QListViewWCS::removeColumn(int index) {
  QListView::removeColumn(index);
  if(index < 1 || index >= columns()) return;
  c_states_[index-1] = false;
}

//----------------------------------------------------------------------

void
QListViewWCS::header_clicked(int section) {
  if(section == 0) {
    sort(section, -1);
    return;
  }
  
  int pos = header()->sectionPos(section);
  //int width = header()->sectionSize(section);
  
#if 0
  cout << "section: " << section << '\n'
       << "sectionPos(): " << pos << '\n'
       << "sectionSize(): " << width << '\n'
       << "mx_: " << mx_ << '\n'
       << "my_: " << my_ << '\n';
#endif

  if(mx_ - pos > 3 && mx_ - pos < on_pix_.width() + 3) {
    if(c_states_[section-1]) {
      setColumnText(section, QIconSet(off_pix_), columnText(section));
      c_states_[section-1] = false;
    } else {
      setColumnText(section, QIconSet(on_pix_), columnText(section));
      c_states_[section-1] = true;
    }

    emit header_changed();
  } else {
    sort(section, -1);
  }
}

//----------------------------------------------------------------------

void
QListViewWCS::header_mouse_release(int x, int y) {
  mx_ = x;
  my_ = y;
}

//----------------------------------------------------------------------

int
QListViewWCS::ncolumnSelected() const {
  int ns = 0;
  for(int i=0; i<int(c_states_.size()); i++) {
    if(c_states_[i]) ns++;
  }
  return ns;
}

//----------------------------------------------------------------------

bool
QListViewWCS::columnSelected(int column) const {
  if(column < 1 || column >= columns()) return false;
  return c_states_[column-1];
}

//----------------------------------------------------------------------

