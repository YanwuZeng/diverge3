#include <string>
#include <vector>
#include <list>
#include <algorithm>

#include <qfiledialog.h>
#include <qlayout.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qmessagebox.h>
#include <qinputdialog.h>
#include <qsplitter.h>


#include "ancestral_seq_tab.h"
#include "common.h"
#include "tree.h"
#include "tree_view.h"
#include "cluster.h"
#include "alignment_view.h"
#include "ancestral_compute.h"
#include "progress.h"
#include "mainwindow.h"


//----------------------------------------------------------------------

using namespace std;

extern vector<sequence_t> orig_sequences;
extern vector<sequence_t> sequences;
extern Tree input_tree;
extern vector<Tree> trees;
extern MainWindow * main_window;

//----------------------------------------------------------------------

//AncestralSeqTab inherit QWidget and use its constructor.
AncestralSeqTab::AncestralSeqTab(QWidget *parent, const char *name) 
  : QWidget(parent, name)
{
  QVBoxLayout *top_layout = new QVBoxLayout(this, 10, 10);
  top_layout->setAutoAdd(true);
  
  QSplitter *splitter = new QSplitter(Vertical, this);

  QHBox *tree_box = new QHBox(splitter);
  tree_box->setSpacing(10);

//define pb2 and pb3.
  QHBox *tree_btn_box = new QVBox(tree_box);
  tree_btn_box->setSpacing(10);
  QPushButton *pb2 = new QPushButton("Load &Tree", tree_btn_box);
  QPushButton *pb3 = new QPushButton("&NJ Tree-Making", tree_btn_box);
  //reroot_ = new QCheckBox("&Reroot", tree_btn_box);
  
  tree_view_ = new TreeView(tree_box);

//add signal to pb2-"Load Tree" and pb3-"NJ Tree_Making".
  connect(pb2, SIGNAL(clicked()), SLOT(load_tree()));
  connect(pb3, SIGNAL(clicked()), SLOT(nj_cluster()));
  
  QHBox *seq_box = new QHBox(splitter);
  seq_box->setSpacing(10);

//define pb1
  QVBox *seq_box_btns = new QVBox(seq_box);
  QPushButton *pb1 = new QPushButton("&Infer", seq_box_btns);

  alignment_view_ = new AlignmentView(seq_box);
    
//add signal to pb1-"infer"
  connect(pb1, SIGNAL(clicked()), SLOT(go()));
//add signal to tree_box
  connect(tree_view_, SIGNAL(node_selected(const node_t*)), SLOT(node_selected(const node_t*)));
}

//----------------------------------------------------------------------
//deconstructor
AncestralSeqTab::~AncestralSeqTab() {}

//----------------------------------------------------------------------

void
AncestralSeqTab::load_tree() {
  QString s(QFileDialog::getOpenFileName(QString::null, "Tree (*.tree *.ph);;All Files (*)", NULL));
  if(s.isNull()) return;

  load_tree((const char*)s);
}

//----------------------------------------------------------------------

bool
AncestralSeqTab::load_tree(string filename) {
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
AncestralSeqTab::clear() {
  Tree tree;
  set_tree(tree);
}

//----------------------------------------------------------------------

void
AncestralSeqTab::update_alignment() {
  alignment_view_->set_alignment(sequences);
}

//----------------------------------------------------------------------

void
AncestralSeqTab::nj_cluster() {
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
  ::nj_cluster(sequences, nj_tree, method, true);

  if(nj_tree.empty()) {
    QMessageBox::critical(NULL, appname, QString("NJ Clustering failed."));
    return;
  }

  Tree tree;
  
  if(!tree.import(nj_tree.c_str())) {
    QMessageBox::critical(NULL, appname, QString("Unable to import NJ tree."));
    return;
  }

  set_tree(tree);
  main_window->statusBar()->message("Right click for more tree options");
}

//----------------------------------------------------------------------

void
AncestralSeqTab::set_tree(Tree &tree) {
  tree.set_draw_root(false);
  tree.set_draw_node_id(true);
  
  tree_view_->set_tree(tree, false);
  alignment_view_->set_alignment(sequences);
}

//----------------------------------------------------------------------

void
AncestralSeqTab::node_selected(const node_t *node) {
  if(!node) return;

  bool state = (node->selected() && node->breadth() > 3 && (node->n_children() == 2 || node->n_children() == 3));
  selected_tree_ = node;

  /*if(reroot_->isChecked()) {
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
    selected_tree_ = node;
  }*/
}

//----------------------------------------------------------------------

void
AncestralSeqTab::go() {
  Progress::setup(0, 100,
		  "Inferring Ancestral State Assignments...");
  Progress::show();
  vector<sequence_t> sequencesOut;
  QString warn;
  Tree t = tree_view_->tree();
  if(!ancestral_compute(t, sequences, sequencesOut, warn)) {
	  Progress::hide();
	  QMessageBox::warning(NULL, appname, warn);
	  return;
  }
  alignment_view_->set_alignment(sequencesOut);
  Progress::hide();
}

//----------------------------------------------------------------------
