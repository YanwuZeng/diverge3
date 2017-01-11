#ifndef _TREE_VIEW_H_
#define _TREE_VIEW_H_

#include <qscrollview.h>
#include <qdialog.h>

#include "tree.h"

class QPushButton;

class TreeView : public QScrollView {
  Q_OBJECT
public:
  TreeView(QWidget *parent=NULL, const char *name=NULL, WFlags f=0);
  
  void set_tree(const Tree &tree, bool use_branch_lengths = true);
  Tree tree() const;
  void set_allow_selection(bool);
  void clear();
  
signals:
  void node_selected(const node_t *node);
  void rightButtonPressed(const node_t *node);

public slots:
  void show_branch_lengths(bool);
  void slotRightPressed(const node_t *node);
  void slotReroot();
  void slotChangeFont();
  void slotFlip();
  void slotSaveTree();
  void copyTree();
  void topologyOnly();
  void slotChangeTipWidth();
  void slotFitWidth();
  void slotFitHeight();
  void slotActualSize();
    
protected:
  void drawContents(QPainter *paint, int cx, int cy, int cw, int ch);
  void contentsMousePressEvent(QMouseEvent *e);
  void keyPressEvent(QKeyEvent *e);
  
private:
  QtTree tree_;
  enum TREESHAPE {ACTUALSIZE, FITWIDTH, FITHEIGHT};
  bool allow_selection;
  bool showTopology;
  TREESHAPE treeShape;
};

class TreeViewDialog : public QDialog {
  Q_OBJECT
public:
  TreeViewDialog(QWidget *parent=NULL, const char *name=NULL, bool modal=false, WFlags f=0);

  void setTree(const Tree &tree);

protected:
  TreeView *tv_;
  QPushButton *ok_;
  
  virtual void resizeEvent(QResizeEvent *);
};

#endif
