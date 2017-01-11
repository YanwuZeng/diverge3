#ifndef _QLISTVIEW_WCS_H_
#define _QLISTVIEW_WCS_H_

#include <vector>

#include <qwidget.h>
#include <qlistview.h>
#include <qpixmap.h>

class HeaderEventFilter : public QWidget {
  Q_OBJECT
public:
  HeaderEventFilter(QWidget *parent = NULL, const char *name = NULL);

protected:
  bool eventFilter(QObject *, QEvent *);

signals:
  void mouseRelease(int x, int y);
};

class QListViewWCS : public QListView {
  Q_OBJECT
public:
  QListViewWCS(QWidget *parent = NULL, const char *name = NULL);
  ~QListViewWCS();

  int addColumn(const QString &label, int size = -1);
  int addColumn(const QIconSet &iconset, const QString &label, int size = -1);
  void removeColumn(int index);

  int ncolumnSelected() const;
  bool columnSelected(int column) const;

protected:
  void sort(int column, int dir);  

protected slots:
  void header_clicked(int section);
  void header_mouse_release(int x, int y);

signals:
  void header_changed();
  
private:
  QPixmap on_pix_;
  QPixmap off_pix_;
  int mx_, my_;
  std::vector<bool> c_states_;
  int sort_column_;
  bool sort_dir_;
};

#endif
