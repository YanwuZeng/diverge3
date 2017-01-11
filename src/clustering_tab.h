#ifndef _CLUSTERS_TAB_H_
#define _CLUSTERS_TAB_H_

#include <string>

#include <qwidget.h>

#include "tree.h"

class QListView;
class QListViewItem;
class QPushButton;
class QCheckBox;
class TreeView;

class ClusteringTab : public QWidget {
  Q_OBJECT
public:
  ClusteringTab(QWidget *parent = NULL, const char *name = NULL);
  ~ClusteringTab();

  bool load_tree(std::string filename);

signals:
  void clustersChanged();
  
public slots:
  void clear();
 
private slots:	
  void load_tree();
  void nj_cluster();
  void add_cluster();
  void delete_cluster();
  void view_cluster(QListViewItem*);
  void sc_cluster(QListViewItem*);
  void node_selected(const node_t*);

private:
  QListView *clusters_list_;
  TreeView *tree_view_;
  QPushButton *pb_add_cluster_;
  QPushButton *pb_delete_cluster_;
  Tree selected_tree_;
  //QCheckBox *lengths_;
  //QCheckBox *reroot_;
  int cluster_num_;
  
  static bool containsPolytomy(const node_t *node, bool root);
  void set_tree(Tree&);
};

#endif
