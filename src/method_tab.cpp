#include <string>
#include <vector>
#include <list>
#include <algorithm>

#include <stdio.h>
#include <limits.h>

#include <qlayout.h>
#include <qlistview.h>
#include <qtabwidget.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qhbox.h>
#include <qlineedit.h>
#include <qfiledialog.h>
#include <qinputdialog.h>
#include <qmessagebox.h>
#include <qtoolbutton.h>
#include <qiconset.h>

#include "method_tab.h"
#include "common.h"
#include "pdb_viewer.h"
#include "graph_view.h"
#include "alignment_view.h"
#include "tree.h"
#include "qlistview_wcs.h"
#include "pdb.h"
#include "resources.h"
#include "tip_window.h"

#include "matrices.h"
#include "matrix.h"
#include "Progress.h"
#include "gz97.h"

//----------------------------------------------------------------------

using namespace std;

extern vector<sequence_t> orig_sequences;
extern vector<sequence_t> sequences;
extern vector<int> removed;
extern vector<Tree> trees;
extern Tree input_tree;
extern PDB pdb;
extern char pdb_chain_ID;
extern vector<string> pdb_map;
extern Resources * resources;

extern double p0; 
extern DVector site_muta; 

//----------------------------------------------------------------------

MethodSummaryListItem::MethodSummaryListItem(QListView *parent, const summary_t &s)
  : QListViewItem(parent),
    summary_(s)
{
  QString str;
  int n=0;

  setText(n++, summary_.name.c_str());
  vector<double>::const_iterator i;
  for(i = summary_.values.begin(); i != summary_.values.end(); i++) {
    str.sprintf("%f", *i);
    setText(n++, str);
  }
}

//----------------------------------------------------------------------

QString
MethodSummaryListItem::key(int column, bool) const {
  QString str;

  switch(column) {
  case 0:
    str = summary_.name.c_str();
    break;
  default:
    str.sprintf("%08f", summary_.values[column - 1]);
    break;
  }

  return str;
}

//----------------------------------------------------------------------

MethodSummaryList::MethodSummaryList(QWidget *parent, const char *name) 
  : QListView(parent, name)
{
  setSelectionMode(NoSelection);
  setAllColumnsShowFocus(true);
  setSorting(-1);

  addColumn("Parameter");
}

//----------------------------------------------------------------------

MethodSummaryList::~MethodSummaryList() {}

//----------------------------------------------------------------------

void
MethodSummaryList::update() {
  clear();

  for(int k=columns()-1; k>0; k--) {
    removeColumn(k);
  }
  
  vector<string>::const_iterator i;
  //for(i = names_.begin(); i != names_.end(); i++) {
  for(i = names_.begin(); i != names_.end(); i+=1) { //to skip Cluster 1 Rk
    addColumn(i->c_str());
  }

  vector<summary_t>::reverse_iterator j;
  int l;
  for(l = 0, j = summary_.rbegin(); j != summary_.rend(); j++, l++) {
	  //if(l < 2 || l > 4) //to delete a, b, c
		new MethodSummaryListItem(this, *j);
  }
}

//----------------------------------------------------------------------

void
MethodSummaryList::set_summary(const vector<string> &names, const vector<summary_t> &summary) {
  names_ = names;
  summary_ = summary;
  
  update();
}

//----------------------------------------------------------------------

MethodResultsListItem::MethodResultsListItem(QListView *parent, const result_t &r)
  : QListViewItem(parent),
    result_(r)
{
  QString str;
  int n=0;

  str.sprintf("%d", result_.pos);
  setText(n++, str);

  vector<double>::const_iterator i;
  for(i = result_.values.begin(); i != result_.values.end(); i++) {
    str.sprintf("%f", *i);
    setText(n++, str);
  }
}

//----------------------------------------------------------------------

QString
MethodResultsListItem::key(int column, bool) const {
  QString str;

  switch(column) {
  case 0:
    str.sprintf("%08d", result_.pos);
    break;
  default:
    str.sprintf("%08f", result_.values[column-1]);
    break;
  }

  return str;
}

//----------------------------------------------------------------------

MethodResultsList::MethodResultsList(QWidget *parent, const char *name) 
  : QListViewWCS(parent, name)
{
  setSelectionMode(Extended);
  setAllColumnsShowFocus(true);
  //setShowSortIndicator(true);

  addColumn("Position");
  
  sort(0, true);
  
  connect(this, SIGNAL(selectionChanged()), SLOT(selection_changed()));
}

//----------------------------------------------------------------------

MethodResultsList::~MethodResultsList() {}

//----------------------------------------------------------------------

void
MethodResultsList::update() {
  clear();

  for(int k=columns()-1; k>0; k--) {
    removeColumn(k);
  }
  
  vector<string>::const_iterator i;
  for(i = names_.begin(); i != names_.end(); i++) {
    addColumn(i->c_str());
  }

  vector<result_t>::const_iterator j;
  for(j = results_.begin(); j != results_.end(); j++) {
    new MethodResultsListItem(this, *j);
  }

  sort(0, true);
}

//----------------------------------------------------------------------

void
MethodResultsList::set_results(const vector<string> &names, const vector<result_t> &results) {
  names_ = names;
  results_ = results;
  update();
}

//----------------------------------------------------------------------

void
MethodResultsList::selection_changed() {
  if(pdb.filename.empty()) return;

  list<string> select;

  MethodResultsListItem *p = (MethodResultsListItem *)firstChild();
  while(p) {
    if(p->isSelected()) {
      int pos = p->result().pos - 1;
      if(pos < (int)pdb_map.size() && !pdb_map[pos].empty()) {
			select.push_back(pdb_map[pos]);
      }
    }
    p = (MethodResultsListItem *)p->itemBelow();
  }

  pdb_select(pdb_chain_ID, select);
}

//----------------------------------------------------------------------
MethodTab::MethodTab(QWidget *parent, const char *name, const char *desc1, const char *desc2, bool need_tip) 
  : QWidget(parent, name),
    avd_(NULL)
{
  tip_info_ = "";
  need_tip_info_ = need_tip;
  QBoxLayout *top_layout = new QVBoxLayout(this);
  top_layout->setMargin(10);

  label_ = new QLabel("Method label", this);
  top_layout->addWidget(label_);
  top_layout->addSpacing(10);

  params_box_ = new QHBox(this);
  params_box_->setSpacing(10);
  top_layout->addWidget(params_box_);
  top_layout->addSpacing(10);

  QHBox *btn_box = new QHBox(this);
  QPushButton *pb1 = new QPushButton("&Calculate", btn_box);
  connect(pb1, SIGNAL(clicked()), SLOT(calculate()));
#ifdef BOOTSTRAP
  QPushButton *pb2 = new QPushButton("&Bootstrap", btn_box);
  connect(pb2, SIGNAL(clicked()), SLOT(bootstrap()));
#endif
  top_layout->addWidget(btn_box);
  top_layout->addSpacing(10);

  QHBoxLayout * h_box_layout = new QHBoxLayout(top_layout, 0);
  h_box_layout->setSpacing( 2 );
  h_box_layout->setMargin( 0 );

  QLabel *label2;
  if (desc1 != NULL)
    label2 = new QLabel(desc1, this, "tip horizontal layout");
  else
    label2 = new QLabel("Results From Statistical Analysis", this);
  
  QIconSet icon(resources->tipIcon);

  //QToolButton * pushButton
  //= new QToolButton(this, "a" );
  //QToolButton * tipButton = new QToolButton(top_layout, "asdf");
  QPushButton * tipButton;
  if ( need_tip_info_ ) {
    tipButton = new QPushButton(icon, "", this, "Tip Button");
    tipButton->setFlat(true);
    tipButton->setFixedSize(15, 15);
    connect(tipButton, SIGNAL(clicked()), this, SLOT(showTip()));
  }
  

  QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding,
					 QSizePolicy::Minimum );

  h_box_layout->addWidget(label2);
  if ( need_tip_info_ )
    h_box_layout->addWidget(tipButton);
  h_box_layout->addItem(spacer);
  

  top_layout->addLayout(h_box_layout);
  top_layout->addSpacing(2);
  
  sum_list_ = new MethodSummaryList(this);
  top_layout->addWidget(sum_list_);
  top_layout->addSpacing(10);

  QLabel *label3;
  if (desc1 != NULL)
    label3 = new QLabel(desc2, this);
  else
    label3 = new QLabel("Site-Specific Profile: Posterior Probability", this);

  top_layout->addWidget(label3);
  top_layout->addSpacing(2);

  list_ = new MethodResultsList(this);
  top_layout->addWidget(list_);
  top_layout->addSpacing(10);

  QHBox *btn_box2 = new QHBox(this);
  QPushButton *pb1a = new QPushButton("Display on &Alignment", btn_box2);
  //pb1a->setFixedWidth(pb1a->sizeHint().width());
  QPushButton *pb2a = new QPushButton("Display on &Structure", btn_box2);
  //pb2a->setFixedWidth(pb2a->sizeHint().width());
  QPushButton *pb3a = new QPushButton("&Export", btn_box2);
  //pb3a->setFixedWidth(pb3a->sizeHint().width());
  top_layout->addWidget(btn_box2);
  top_layout->addSpacing(10);

  connect(pb1a, SIGNAL(clicked()), SLOT(display_on_alignment()));
  connect(pb2a, SIGNAL(clicked()), SLOT(display_on_structure()));
  connect(pb3a, SIGNAL(clicked()), SLOT(export_data()));

  connect(list_, SIGNAL(header_changed()), SLOT(update_graph()));

  gv_ = new GraphView(this);
  top_layout->addWidget(gv_);

  top_layout->activate();


  this->clearFocus();

}

//----------------------------------------------------------------------
void MethodTab::showTip()
{
  TipWindow * tip_window;
#ifdef WIN32
  tip_window = new TipWindow(this, "Qt Diverge Help Window");
#else
  tip_window = new TipWindow(this, "Diverge Help Window");
#endif
  
  tip_window->setText(tip_info_);

  tip_window->exec();
}
//----------------------------------------------------------------------

MethodTab::~MethodTab() {
  if(avd_) delete avd_;
}

//----------------------------------------------------------------------

void
MethodTab::set_results(const vector<string> &s_names, const vector<summary_t> &summary,
		       const vector<string> &r_names, const vector<result_t> &results) {
  s_names_ = s_names;
  r_names_ = r_names;
  summary_ = summary;
  results_ = results;

  vector<result_t>::iterator i;
  for(i = results_.begin(); i != results_.end(); i++) {
    i->pos = pos_w_gaps(removed, i->pos) + 1;
  }
  
  sum_list_->set_summary(s_names_, summary_);
  list_->set_results(r_names_, results_);
  
  gv_->clear();

  emit resultsChanged();
}

//----------------------------------------------------------------------
void
MethodTab::set_tip_info(QString tip) {
  tip_info_ = tip;
}
//----------------------------------------------------------------------

void
MethodTab::clear() {
  vector<string> names;
  vector<summary_t> summary;
  vector<result_t> results;
  set_results(names, summary, names, results);
}

//----------------------------------------------------------------------

void
MethodTab::calculate() {
}

//----------------------------------------------------------------------

void
MethodTab::bootstrap() {
  bool ok;
  int nsamples = QInputDialog::getInteger("Enter number of samples", "Number of samples",
					  100, 1, INT_MAX, 1, &ok);
  if(ok) bootstrap(nsamples);
}

//----------------------------------------------------------------------

void
MethodTab::bootstrap(int /*nsamples*/) {
}

//----------------------------------------------------------------------

void
MethodTab::display_on_alignment() {
  if(!list_->ncolumnSelected()) {
    QMessageBox::warning(NULL, appname, "Need to select at least one column first.");
    return;
  }
  
  bool ok;

  double cut_off = QInputDialog::getDouble("Enter cutoff value", "Cutoff", 0.5, 0.0, 1.79769e+308, 8, &ok); 
  if(!ok) return;
  if(!avd_) {
    avd_ = new AlignmentViewDialog;
    avd_->resize(600, 500);
  }

  {
    vector<sequence_t> sorted_sequences, tmp_sequences = orig_sequences;
    vector<Tree>::const_iterator i;
    vector<sequence_t>::iterator k;
    for(i = trees.begin(); i != trees.end(); i++) {
      {
	sequence_t tmp;
	if(i != trees.begin()) {
	  sorted_sequences.push_back(tmp);
	}
	tmp.label = "Cluster " + i->filename();
	sorted_sequences.push_back(tmp);
      }
      vector<string> clade;
      i->leaf_names(clade);
      vector<string>::const_iterator j;
      for(j=clade.begin(); j!=clade.end(); j++) {
	for(k=tmp_sequences.begin(); k!=tmp_sequences.end(); k++) {
	  if(k->label == *j) {
	    sorted_sequences.push_back(*k);
		k = tmp_sequences.erase(k);
	    break;
	  }
	}
	if(k == tmp_sequences.end()) {
        
	}
      }
    }
    
    if(!tmp_sequences.empty()) {
      sequence_t tmp;
      sorted_sequences.push_back(tmp);
      tmp.label = "No cluster";
      sorted_sequences.push_back(tmp);
      for(k=tmp_sequences.begin(); k!=tmp_sequences.end(); k++) {
	      sorted_sequences.push_back(*k);
      }
    }

    avd_->setAlignment(sorted_sequences);
  }

  AlignmentView *av = avd_->alignment_view();

  vector<result_t>::const_iterator i;
  int start = 0;
  int len = 0;
  for(i = results_.begin(); i != results_.end(); i++) {
    bool b = false;
    for(int j=1; j<list_->columns(); j++) {
      if(list_->columnSelected(j)) {
	b = true;
	double v = i->values[j-1];
	if(v < 0.0) v = 0.0;
	if(v >= cut_off) {
	  if(i->pos - start > len) {
	    av->highlight_region(start - 1, len);
	    len = 0;
	  }
	  if(len == 0) {
	    start = i->pos;
	  }
	  len++;
	  break;
	}
      }
    }
    if(!b && len) {
      av->highlight_region(start - 1, len);
      len = 0;
    }
  }

  if(len) {
    av->highlight_region(start - 1, len);
  }

  avd_->show();
}

//----------------------------------------------------------------------

void
MethodTab::display_on_structure() const {
  if(!list_->ncolumnSelected()) {
    QMessageBox::warning(NULL, appname, "Need to select at least one column first.");
    return;
  }
  if(pdb.filename.empty()) {
    QMessageBox::warning(NULL, appname, "Need to specify a PDB structure file first.");
    return;
  }
  
  bool ok;

  double cut_off = QInputDialog::getDouble("Enter cutoff value", "Cutoff", 0.5, 0.0, 1.79769e+308, 8, &ok); 
  if(!ok) return;

  list<string> select;
  
  vector<result_t>::const_iterator i;
  for(i = results_.begin(); i != results_.end(); i++) {
    for(int j=1; j<list_->columns(); j++) {
      if(list_->columnSelected(j)) {
	double v = i->values[j-1];
	if(v < 0.0) v = 0.0;
	if(v >= cut_off) {
	  int pos = i->pos - 1;
          if(pos < (int)pdb_map.size() && !pdb_map[pos].empty()) {
	    select.push_back(pdb_map[pos]);
	  }
	  break;
	}
      }
    }
  }

  pdb_select(pdb_chain_ID, select);
}

//----------------------------------------------------------------------

bool
MethodTab::export_data() {
  QString s(QFileDialog::getSaveFileName(QString::null, "Text (*.txt);;All Files (*)", NULL));
  if(s.isNull()) return false;

  FILE *file = fopen(s, "w");
  if(!file) {
    QMessageBox::warning(NULL, "", "Could not open file for writting.");
    return false;
  }
  
  fprintf(file, "Main Tree:\t%s\n", input_tree.gen_str().c_str());

  vector<Tree>::const_iterator k2;
  for(k2 = trees.begin(); k2 != trees.end(); k2++) {
    fprintf(file, "Cluster %s:\t%s\n", k2->filename().c_str(), k2->gen_str().c_str()); 
  }
  putc('\n', file);

  vector<string>::const_iterator k;
  for(k = s_names_.begin(); k != s_names_.end(); k++) {
    fprintf(file, "\t%s", k->c_str());
  }
  putc('\n', file);


  vector<summary_t>::const_iterator k1;
  for(k1 = summary_.begin(); k1 != summary_.end(); k1++) {
	  fprintf(file, "%s", k1->name.c_str());
	  vector<double>::const_iterator j;
	  for(j = k1->values.begin(); j != k1->values.end(); j++) {
		  fprintf(file, "\t%f", *j);
	  }
	  putc('\n', file);
  }

  vector<result_t>::const_iterator zi;
  for(zi = results_.begin(); zi != results_.end(); zi++) {
	  fprintf(file, "%d", zi->pos);
	  vector<double>::const_iterator zj;
	  for(zj = zi->values.begin(); zj != zi->values.end(); zj++) {
		  fprintf(file, "\t%f", *zj);
	  }
	  putc('\n', file);
  }

  
  int ntrees = trees.size();
  int seqLength = sequences[0].sequence.size();
  vector<DVector> rets(ntrees);

  DVector freq(jtt_freq, 20);
  DMatrix2D prob(jtt_prob, 20, 20);

  vector< DVector > site_muta_groups;
  site_muta_groups.clear();

  for(int i = 0; i < ntrees; i++) {
    int treeE;

    if (Progress::wasCancelled()) 
		return false;
    
    vector<string> taxa;
    trees[i].leaf_names(taxa);

    CMatrix2D alignment(taxa.size(), sequences[0].sequence.size());

    {
      // generate sub_sequences and sub_seqNames
      vector<string>::const_iterator i;
      vector<sequence_t>::const_iterator j;
      int i2;
      for(i2 = 0, i = taxa.begin(); i != taxa.end(); i++) {
		  for(j = sequences.begin(); j != sequences.end(); j++) {
			  if(j->label == *i) {
				  for(int k=0; k<(int)j->sequence.size(); k++) {
					  alignment(i2, k) = j->sequence[k];
				  }
				  i2++;
				  break;
			  }
		  }

		  if(j == sequences.end()) {
			  abort();
		  } 
       }
    }

    string tree_str;
    if(!trees[i].gen_str_wrt_seq(taxa, tree_str)) 
		return false;
    
    IMatrix2D gu_tree;

    if(!parse_tree(tree_str.c_str(), alignment.rows(), treeE, gu_tree)) {
		return false;
    }
	if(!gz97_compute(true, 2.4, alignment, treeE, gu_tree, freq, prob, rets[i])) {
		return false;
    }

	site_muta_groups.push_back(site_muta);

  }
  
  double p000=0; 

  int iZero=0; 
  for (  i=0;i<seqLength;i++)
  {
	  bool zeroBl = true; 
	  for (int j=0;j<ntrees;j++)
	  {
	     if (site_muta_groups[j](i) >= 0.001)
			 zeroBl = false; 
	  }
	  if (zeroBl) iZero++; 
  }

  
  double D1 = 0.0; 
  double D2 = 0.0; 
  double D3 = 0.0; 

  vector<double> D(ntrees,0);
  for(  i=0;i<ntrees;i++)
  {
	  for(int j=0;j<seqLength;j++)
	  {
		  D[i] += site_muta_groups[i](j); 
	  }

	  D[i] = D[i] / seqLength; 
  }
  
  
  fprintf(file, "%s\n", ""); 
  fprintf(file, "%s\n", "----------------------------------------------------------------------------------------------------"); 
  fprintf(file, "%s\n", "----------------------------------------------------------------------------------------------------"); 
  fprintf(file, "%s\n", ""); 
  fprintf(file, "%s\n", "     Number of Sites :     "); 
  fprintf(file, "%s\n", ""); 
  
  fprintf(file, "Sites"); 
  for(int j=0;j<ntrees;j++)
  {
	  fprintf(file, "\tCluseter%d", (j + 1));
  }
  fprintf(file,"\n");

    j = 0; 
  for(zi = results_.begin(); zi != results_.end(); zi++) 
  {
	  fprintf(file, "%d", zi->pos); 

	  for(int ti=0; ti<ntrees; ti++)
	  {
		  fprintf(file, "\t%f", site_muta_groups[ti](j));
	  }
	  fprintf(file, "\n");

	  j++; 
  }
  fprintf(file, "\n");
  

  fclose(file);

}

//----------------------------------------------------------------------

void
MethodTab::update_graph() {
  vector<result_t> nr;

  if(list_->ncolumnSelected()) {
    nr.resize(results_.size());
    for(int i=0; i<int(results_.size()); i++) {
      nr[i].pos = results_[i].pos;
      for(int j=1; j<list_->columns(); j++) {
	if(list_->columnSelected(j)) {
	  double v = results_[i].values[j-1];
	  if(v < 0.0) v = 0.0;
	  nr[i].values.push_back(v);
	}
      }
    }
  }
  
  gv_->set_results(nr);
}

//----------------------------------------------------------------------
