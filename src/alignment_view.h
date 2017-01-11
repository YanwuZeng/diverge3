#ifndef _ALIGNMENT_VIEW_H_
#define _ALIGNMENT_VIEW_H_

#include <string>
#include <vector>

#include <qscrollview.h>
#include <qwidget.h>
#include <qdialog.h>

#include "sequence.h"

class QPushButton;
class QScrollView;

class SimpleAlignmentView : public QScrollView {
public:
  SimpleAlignmentView(QWidget *parent=NULL, const char *name=NULL, WFlags f=0);
  
  void clear();
  void set_alignment(const std::vector<sequence_t> &sequences);
  void append_alignment(const std::vector<sequence_t> &sequences);
  void highlight_region(int start, int len);
  
protected:
  void drawContents(QPainter *p, int cx, int cy, int cw, int ch);
  void contentsMousePressEvent(QMouseEvent *e);
  void keyPressEvent(QKeyEvent *e);

private:
  std::vector<sequence_t> sequences_;
  std::string summary;
  size_t seq_max_len;
  int char_max_w;
  int cws[256];
  std::vector<std::pair<int, int> > regions;
  QBrush brushes[13];

  QBrush &get_brush(char c);
  void build_summary();
};

class LabelView : public QScrollView {
public:
  LabelView(QWidget *parent=NULL, const char *name=NULL, WFlags f=0);
  
  void clear();
  void set_alignment(const std::vector<sequence_t> &sequences);
  void append_alignment(const std::vector<sequence_t> &sequences);
  void highlight_region(int start, int len);

  void setSelectable(bool s);
  bool selectable() const;

  void getStates(std::vector<std::pair<std::string, bool> > &states) const;

protected:
  void contentsMousePressEvent(QMouseEvent*);
  void contentsMouseReleaseEvent(QMouseEvent*);
  void drawContents(QPainter *p, int cx, int cy, int cw, int ch);

private:
  std::vector<sequence_t> sequences_;
  std::vector<bool> states_;
  bool selectable_;
  int label_max_w, cb_size_;
  QPoint mp_;
  std::vector<std::pair<int, int> > regions;
};

class SiteView : public QScrollView {
public:
  SiteView(QWidget *parent=NULL, const char *name=NULL, WFlags f=0);
  
  void clear();
  void set_alignment(const std::vector<sequence_t> &sequences);
  void append_alignment(const std::vector<sequence_t> &sequences);
  void highlight_region(int start, int len);
  
protected:
  void drawContents(QPainter *p, int cx, int cy, int cw, int ch);
    
private:
  std::vector<sequence_t> sequences_;
  int nheader;
  size_t seq_max_len;
  int char_max_w; //the maximal width of all printable characters;
  int cws[256]; //the width of all printable characters
  std::vector<std::pair<int, int> > regions;
};

class AlignmentView : public QWidget {
  Q_OBJECT
public:
  AlignmentView(QWidget *parent=NULL, const char *name=NULL, WFlags f=0);
  
  void clear();
  void set_alignment(const std::vector<sequence_t> &sequences);
  void append_alignment(const std::vector<sequence_t> &sequences);
  void highlight_region(int start, int len);
  void update_sizes();

  void setLabelsSelectable(bool s);
  bool labelsSelectable() const;
  void getLabelStates(std::vector<std::pair<std::string, bool> > &states) const;

protected slots:
  void alignmentViewMoving(int x, int y);
  void labelViewMoving(int x, int y);
  void siteViewMoving(int x, int y);
 
protected:
  void resizeEvent(QResizeEvent *e);
  
private:
  int last_x_, last_y_;
  SimpleAlignmentView *alignment_view_;
  LabelView *label_view_;
  SiteView *site_view_;
};

class AlignmentViewDialog : public QDialog {
public:
  AlignmentViewDialog(QWidget *parent=NULL, const char *name=NULL, bool modal=false, WFlags f=0);

  void setAlignment(const std::vector<sequence_t> &alignment);

  AlignmentView *alignment_view() { return av_; }
  
protected:
  AlignmentView *av_;
  QPushButton *ok_;
  
  virtual void resizeEvent(QResizeEvent *);
};

#endif
