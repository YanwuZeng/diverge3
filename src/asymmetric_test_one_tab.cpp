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


#include "common.h"
#include "tree.h"
#include "tree_view.h"
#include "cluster.h"
#include "alignment_view.h"
#include "ancestral_compute.h"
#include "progress.h"
#include "mainwindow.h"

#include "asymmetric_test_one_tab.h"
#include "asymmetric_test_one.h"

#include "progress.h"


//----------------------------------------------------------------------

using namespace std;

extern vector<sequence_t> orig_sequences;
extern vector<sequence_t> sequences;
extern Tree input_tree;
extern vector<Tree> trees;
extern MainWindow * main_window;

//----------------------------------------------------------------------

// AsymmetricTestOneTab inherit QWidget and use its constructor.
AsymmetricTestOneTab::AsymmetricTestOneTab(QWidget *parent, const char *name) 
  : QWidget(parent, name)
{

	tip_info_ = "";

  set_tip_info("<b>Da</b>: average number of substitutions of cluster a <br>"
	  "<b>Db</b>: average number of substitutions of cluster b <br>"
	  "<b>N</b>: number of sites with N (no change) in the early stage<br>"
	  "<b>C</b>: number of sites with C (conservative change) in the early stage<br>"
	  "<b>R</b>: number of sites with R (radical chage) in the early stage<br>"
	  "<b>Alpha ML</b>: ML estimate of Alpha<br>"
	  "<b>Theta-II</b>: Type II functional divergence<br>"
	  "<b>Theta SE</b>: standard error of theta<br>"
	  "<b>Ar</b>: Type II related radical change<br>"
	  "<b>PIr</b>: Type II unrelated radical change<br>"
	  );


  QBoxLayout *top_layout = new QVBoxLayout(this);
  top_layout->setMargin(10);


  label_ = new QLabel("Asymmetric Test for Type I Functional Divergence", this);
  top_layout->addWidget(label_);
  top_layout->addSpacing(10);

  params_box_ = new QHBox(this);
  params_box_->setSpacing(10);
  top_layout->addWidget(params_box_);
  top_layout->addSpacing(10);


//define PushButton1.
  QHBox *calculate_btn_box = new QVBox(this);
  calculate_btn_box->setSpacing(10);
  QPushButton * pb1 = new QPushButton(" Calculate ", calculate_btn_box);

//add signal to PushButton1-" Calculate ". 
  connect(pb1, SIGNAL(clicked()), SLOT(calculate()));

  top_layout->addWidget(calculate_btn_box);
  top_layout->addSpacing(10);


  QHBoxLayout * h_box_layout = new QHBoxLayout(top_layout, 0);
  h_box_layout->setSpacing(2);
  h_box_layout->setMargin(0); 


  QLabel *label2;
  label2 = new QLabel("Asymmetric Test: Delta Variation", this);


  QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding,
	  QSizePolicy::Minimum );

  h_box_layout->addWidget(label2);
  h_box_layout->addItem(spacer);

  top_layout->addLayout(h_box_layout);
  top_layout->addSpacing(2);


  list_ = new MethodResultsList(this);
  list_->setColumnText(0, "Cluster Number as Outgroup"); 
  top_layout->addWidget(list_);
  top_layout->addSpacing(10);

  top_layout->activate();
  

  this->clearFocus();

}

//----------------------------------------------------------------------
// Deconstructor
AsymmetricTestOneTab::~AsymmetricTestOneTab() {}

//----------------------------------------------------------------------

void
	AsymmetricTestOneTab::set_tip_info(QString tip) {
		tip_info_ = tip;
}

//----------------------------------------------------------------------

void AsymmetricTestOneTab::calculate() 
{
	vector<string> t_names;
	if (!preprocess(t_names)) 
		return;

	int op_overhead = trees.size() + trees.size()*(trees.size()-1)/2  +  //GZ97_compute()
		trees.size() * (trees.size() - 1) /2 + //GZf2_compute
		(trees.size()*(trees.size()-1)/2) + //type_two_compute
		trees.size() * (trees.size() - 1) / 2; //process() overhead

	Progress::setup(0, op_overhead * 100,
		"Testing Asymmetric Type I Divergence...");
	Progress::show();

	vector<string> names;
	vector<summary_t> summary;
	vector<result_t> results;

	if (!process(trees, sequences, t_names, names, summary, results)) {
		Progress::hide();
		return;
	}

	set_results(names, results);

	Progress::hide();  
}

//----------------------------------------------------------------------

bool
AsymmetricTestOneTab::preprocess(vector<string> &names) 
{
  if(sequences.empty()) {
    QMessageBox::information(NULL, "Asymmetric Test for Type I", "Need to load aligned sequences first.");
    return false;
  }
  if(trees.size() != 3) {
    QMessageBox::information(NULL, "Asymmetric Test for Type I", "Need to create three clusters first.");
    return false;
  }
  
  names.resize(trees.size());
  for(size_t i=0; i<trees.size(); i++) {
    string str = trees[i].filename();

    size_t j;

    j = str.find_last_of('/');
    if(j != string::npos) str.erase(0, j+1);

    j = str.find_last_of('.');
    if(j != string::npos) str.erase(j, string::npos);
    
    names[i] = str;
  }

  return true;
}

//----------------------------------------------------------------------

bool
AsymmetricTestOneTab::process(const vector<Tree> &trees,
		       const vector<sequence_t> &sequences,
		       const vector<string> &names,
		       vector<string> &names2,
		       vector<summary_t> &summary2,
		       vector<result_t> &results2) 
{
  vector<vector<double> > summary, rets;
  if (!asymmetric_test_one_compute(trees, sequences, rets)) 
  {
    Progress::hide();
    if(!Progress::wasCancelled()) {
      QMessageBox::warning(NULL, "Testing Asymmetric Type I Divergence", "Testing Asymmetric Type I method failed.  Please recheck input parameters.");
    }
    return false;
  }


  if(!rets.empty()) 
  {
	  size_t i, j; 
	  names2.push_back("Coefficient of Correlation of Theta"); 
	  names2.push_back("Delta Variation"); 

	  size_t nrets = rets.size();

	  result_t r;
	  r.values.resize(rets[0].size());

	  for(i = 0; i < nrets; i++) 
	  {
		  r.pos = i;
		  for(j = 0; j < rets[0].size(); j++) 
		  {
			  r.values[j] = rets[i][j];
		  }
		  results2.push_back(r);
	  }

  }

  Progress::increment(summary.size() * 100);
  return true;

}

//----------------------------------------------------------------------

void
	AsymmetricTestOneTab::set_results(const vector<string> &r_names, const vector<result_t> &results) 
{
		r_names_ = r_names;
		results_ = results;

		int num = 0; 

		vector<result_t>::iterator i;
		for(i = results_.begin(); i != results_.end(); i++) {
			i->pos = ++num;
		}

		list_->set_results(r_names_, results_);

		emit resultsChanged();
}

//----------------------------------------------------------------------
