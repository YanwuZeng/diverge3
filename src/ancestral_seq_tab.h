#ifndef _ANCESTRAL_SEQ_TAB_H_
#define _ANCESTRAL_SEQ_TAB_H_

#include <string>

#include <qwidget.h>

#include "tree.h"

class AlignmentView;
class QPushButton;
class QCheckBox;
class TreeView;

class AncestralSeqTab : public QWidget {
  Q_OBJECT
public:
  AncestralSeqTab(QWidget *parent = NULL, const char *name = NULL);
  ~AncestralSeqTab();

  bool load_tree(std::string filename);

public slots:
  void clear();
  void update_alignment();
 
private slots:
  void load_tree();
  void nj_cluster();
  void node_selected(const node_t*);
  void go();
  
private:
  AlignmentView *alignment_view_;
  TreeView *tree_view_;
  Tree selected_tree_;
  //QCheckBox *reroot_;

  void set_tree(Tree&);
};

#endif
