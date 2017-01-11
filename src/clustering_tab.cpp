#include <string>
#include <vector>
#include <list>
#include <algorithm>

#include <iostream>

#include <qfiledialog.h>
#include <qlayout.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qmessagebox.h>
#include <qinputdialog.h>

#include "clustering_tab.h"
#include "common.h"
#include "tree.h"
#include "tree_view.h"
#include "cluster.h"
#include "mainwindow.h"

//----------------------------------------------------------------------

using namespace std;

extern vector<sequence_t> orig_sequences;
extern vector<sequence_t> sequences;
extern Tree input_tree;
extern vector<Tree> trees;
extern MainWindow * main_window;

//----------------------------------------------------------------------

class ClusterListItem : public QListViewItem {
public:
  ClusterListItem(QListView *parent, string filename, string tree_str)
    : QListViewItem(parent),
      filename_(filename)
  {
    setText(0, filename_.c_str());
    setText(1, tree_str.c_str());
  }

  string filename() const { return filename_; }

protected:
  string filename_;
};

ClusteringTab::ClusteringTab(QWidget *parent, const char *name) 
  : QWidget(parent, name),
    cluster_num_(0)
{
  QVBoxLayout *top_layout = new QVBoxLayout(this, 10, 10);
  
  QHBox *tree_box = new QHBox(this);
  tree_box->setSpacing(10);

  QHBox *tree_btn_box = new QVBox(tree_box);
  tree_btn_box->setSpacing(10);
  QPushButton *pb2 = new QPushButton("Load &Tree", tree_btn_box);
  QPushButton *pb3 = new QPushButton("&NJ Tree-Making", tree_btn_box);
  //lengths_ = new QCheckBox("&Show Lengths", tree_btn_box);
  //lengths_->setChecked(true);
  //reroot_ = new QCheckBox("&Reroot", tree_btn_box);
  
  tree_view_ = new TreeView(tree_box);

  connect(pb2, SIGNAL(clicked()), SLOT(load_tree()));
  connect(pb3, SIGNAL(clicked()), SLOT(nj_cluster()));
  //connect(lengths_, SIGNAL(toggled(bool)), tree_view_, SLOT(show_branch_lengths(bool)));

  top_layout->addWidget(tree_box);

  QHBox *clusters_box = new QHBox(this);
  clusters_box->setSpacing(10);

  QVBox *clusters_box_btns = new QVBox(clusters_box);
  pb_add_cluster_ = new QPushButton("&Add Cluster", clusters_box_btns);
  pb_delete_cluster_ = new QPushButton("&Delete Cluster", clusters_box_btns);
  
  pb_add_cluster_->setEnabled(false);
  pb_delete_cluster_->setEnabled(false);

  clusters_list_ = new QListView(clusters_box);
  clusters_list_->setAllColumnsShowFocus(true);
  clusters_list_->addColumn("Cluster Name");
  clusters_list_->addColumn("Cluster Tree");

  clusters_box->setFixedHeight(pb_add_cluster_->height() +
			       pb_delete_cluster_->height() + 30);
  
  top_layout->addWidget(clusters_box);
  
  connect(pb_add_cluster_, SIGNAL(clicked()), SLOT(add_cluster()));
  connect(pb_delete_cluster_, SIGNAL(clicked()), SLOT(delete_cluster()));
  connect(clusters_list_, SIGNAL(selectionChanged(QListViewItem*)), SLOT(sc_cluster(QListViewItem*)));
  connect(clusters_list_, SIGNAL(doubleClicked(QListViewItem*)), SLOT(view_cluster(QListViewItem*)));

  connect(tree_view_, SIGNAL(node_selected(const node_t*)), SLOT(node_selected(const node_t*)));

  top_layout->activate();
}

//----------------------------------------------------------------------

ClusteringTab::~ClusteringTab() {}

//----------------------------------------------------------------------

void
ClusteringTab::load_tree() {
  QString s(QFileDialog::getOpenFileName(QString::null, "Tree (*.tree *.ph);;All Files (*)", NULL));
  if(s.isNull()) return;

  load_tree((const char*)s);
}

//----------------------------------------------------------------------

bool
ClusteringTab::load_tree(string filename) {
  Tree tree;
  
  if(!tree.load(filename)) {
    QMessageBox::critical(NULL, appname, QString("Unable to load ") +
			  QString(filename.c_str()));
    return false;
  }

  set_tree(tree);

  return true;
}

//----------------------------------------------------------------------

void
ClusteringTab::clear() {
  Tree tree;
  set_tree(tree);
  cluster_num_ = 0;
}

//----------------------------------------------------------------------

void
ClusteringTab::nj_cluster() {
  string nj_tree;
  clustering_method_t method;
  {
    QStringList lst;
    lst << "p-Distance" << "Poisson" << "Kimura";
    bool ok = false;
    QString res = QInputDialog::getItem(appname,
					"Please select distance measure",
					lst, 2, false, &ok, this);
    if(!ok) return;
    if(res == "p-Distance") {
      method = pDistance;
    } else if(res == "Poisson") {
      method = Poisson;
    } else if(res == "Kimura") {
      method = Kimura;
    } else {
      return;
    }
  }

  /*
  for(int i = 0; i < sequences.size(); i++) {
    cout << sequences[i].label << " ";
    cout << sequences[i].sequence << endl;
  }
  */

  ::nj_cluster(sequences, nj_tree, method, true);

  if(nj_tree.empty()) {
    QMessageBox::critical(NULL, appname, QString("NJ Clustering failed."));
    return;
  }

  Tree tree;

  /*
  string k = nj_tree.c_str();
  cout << k << endl;
  */
  
  if(!tree.import(nj_tree.c_str())) {
    QMessageBox::critical(NULL, appname, QString("Unable to import NJ tree."));
    return;
  }

  set_tree(tree);
  /*QPalette pal(main_window->statusBar()->palette());
  setPaletteForegroundColor(Qt::red);*/
  main_window->statusBar()->message("Right click for more tree options");
}

//----------------------------------------------------------------------

void
ClusteringTab::set_tree(Tree &tree) {
  tree.set_draw_root(false);
  tree.reorder_id();

  tree_view_->set_tree(tree, false);
  clusters_list_->clear();
  trees.clear();
  pb_add_cluster_->setEnabled(false);
  pb_delete_cluster_->setEnabled(false);
  input_tree = tree;
}

//----------------------------------------------------------------------

bool
ClusteringTab::containsPolytomy(const node_t *node, bool root) {
  if(!node) return false;
  if(node->n_children() > 3 || !root && node->n_children() > 2) return true;
  const list<node_t*> &children = node->children();
  list<node_t*>::const_iterator i;
  for(i=children.begin(); i!=children.end(); i++) {
    if(containsPolytomy(*i, false)) return true;
  }
  return false;
}

//----------------------------------------------------------------------

void
ClusteringTab::add_cluster() {
  if(selected_tree_.breadth() <= 3) return;

  if(containsPolytomy(selected_tree_.root(), true)) {
    QMessageBox::warning(NULL, appname, "Selected tree contains an unsupported polytomy.");
    return;
  }

  Tree tree;
  if(!selected_tree_.polyroot(tree)) abort();  

  vector<Tree>::const_iterator i;

  set<string> selected_clade, clade, intersection;
  selected_tree_.leaf_names(selected_clade);
  for(i = trees.begin(); i != trees.end(); i++) {
    i->leaf_names(clade);
    set_intersection(selected_clade.begin(), selected_clade.end(),
		     clade.begin(), clade.end(),
		     inserter(intersection, intersection.begin()));
    if(!intersection.empty()) {
      QMessageBox::warning(NULL, appname, "Selected tree is not disjoint with those already selected.");
      return;
    }
  }


  QString name;
  bool ok_name = false;
  do {
    QString tmp;
    tmp.sprintf("Cluster %d", ++cluster_num_);
    name = QInputDialog::getText("Cluster Name", "Enter name for cluster",
				 QLineEdit::Normal,tmp, &ok_name, this);
    if(!ok_name) return;

    name.stripWhiteSpace();
    ok_name = !name.isEmpty();
    if(ok_name) {
      for(i = trees.begin(); i != trees.end(); i++) {
	if(i->filename() == (const char*)name) {
	  QMessageBox::warning(NULL, appname, "Name already being used.");
	  ok_name = false;
	  break;
	}
      }
    }
  } while(!ok_name);
  
  tree.set_filename((const char*)name);
  tree.set_draw_root(false);
  
  tree.unselect_all();
  tree.set_has_branch_length(tree_view_->tree().has_branch_length());
  trees.push_back(tree);

  string tree_str = tree.gen_str();
  
  clusters_list_->insertItem(new ClusterListItem(clusters_list_, tree.filename(), tree_str));
  emit clustersChanged();
}

//----------------------------------------------------------------------

void
ClusteringTab::delete_cluster() {
  ClusterListItem *p = (ClusterListItem *)clusters_list_->selectedItem();
  if(!p) return;
  vector<Tree>::iterator i;
  for(i = trees.begin(); i != trees.end(); i++) {
    if(p->filename() == i->filename()) {
      trees.erase(i);
      break;
    }
  }
  delete p;
  pb_delete_cluster_->setEnabled(false);
  emit clustersChanged();
}

//----------------------------------------------------------------------

void
ClusteringTab::view_cluster(QListViewItem *p) {
  if(!p) return;
  ClusterListItem *q = (ClusterListItem *)p;
  vector<Tree>::iterator i;
  for(i = trees.begin(); i != trees.end(); i++) {
    if(q->filename() == i->filename()) {
      TreeViewDialog d(NULL, NULL, true);
      d.setTree(*i);
      d.exec();
      break;
    }
  }
}

//----------------------------------------------------------------------

void
ClusteringTab::sc_cluster(QListViewItem *p) {
  if(!p) return;
  pb_delete_cluster_->setEnabled(p->isSelected());
}

//----------------------------------------------------------------------

void
ClusteringTab::node_selected(const node_t *node) {
  if(!node) return;

  bool state = (node->selected() && node->breadth() > 3 && (node->n_children() == 2 || node->n_children() == 3));
  pb_add_cluster_->setEnabled(state);
  selected_tree_ = node;

	/*
  if(reroot_->isChecked()) {
    Tree tree = tree_view_->tree();
    if(tree.reroot(node->id())) {
      tree.unselect_all();
      set_tree(tree);
      reroot_->setChecked(false);
    }
  } else {
    bool state = (node->selected() &&
		  node->breadth() > 3 &&
		  (node->n_children() == 2 || node->n_children() == 3));
    pb_add_cluster_->setEnabled(state);
    selected_tree_ = node;
  }
  */
}

//----------------------------------------------------------------------
