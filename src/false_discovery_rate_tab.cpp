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

#include "false_discovery_rate_tab.h"
#include "false_discovery_rate.h"

#include "progress.h"


//----------------------------------------------------------------------

using namespace std;

extern vector<sequence_t> orig_sequences;
extern vector<sequence_t> sequences;
extern Tree input_tree;
extern vector<Tree> trees;
extern MainWindow * main_window;

//----------------------------------------------------------------------

// FalseDiscoveryRateTab inherit QWidget and use its constructor.
FalseDiscoveryRateTab::FalseDiscoveryRateTab(QWidget *parent, const char *name) 
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


  label_ = new QLabel("False Discovery Rate for Predictions (Type I or Type II)", this);
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
  label2 = new QLabel("False Discovery Rate of Posterior Probability", this);


  QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding,
	  QSizePolicy::Minimum );

  h_box_layout->addWidget(label2);
  h_box_layout->addItem(spacer);

  top_layout->addLayout(h_box_layout);
  top_layout->addSpacing(2);


  list_ = new MethodResultsList(this);
  list_->setColumnText(0, "Divergence Type"); 
  top_layout->addWidget(list_);
  top_layout->addSpacing(10);

  gv_ = new FDRGraphView(this);
  top_layout->addWidget(gv_);

  top_layout->activate();
  

  this->clearFocus();

}

//----------------------------------------------------------------------
// deconstructor
FalseDiscoveryRateTab::~FalseDiscoveryRateTab() {}

//----------------------------------------------------------------------

void
	FalseDiscoveryRateTab::set_tip_info(QString tip) {
		tip_info_ = tip;
}

//----------------------------------------------------------------------

void FalseDiscoveryRateTab::calculate() 
{
	vector<string> t_names;
	if(!preprocess(t_names)) 
		return;

	int op_overhead = trees.size() + trees.size()*(trees.size()-1)/2  +  //GZ97_compute()
		trees.size() * (trees.size() - 1) /2 + //GZf2_compute
		(trees.size()*(trees.size()-1)/2) + //type_two_compute
		trees.size() * (trees.size() - 1) / 2; //process() overhead

	Progress::setup(0, op_overhead * 100,
		"Computing False Discovery Rate ..."); 
	Progress::show();

	vector<string> names;
	vector<summary_t> summary;
	vector<result_t> results1, results2, results3, results4;

	if(!process(trees, sequences, t_names, names, summary, results1, results2, results3, results4)) {
		Progress::hide();
		return;
	}

	set_results(names, names, results1, results2, results3, results4); 

	Progress::hide();  
}

//----------------------------------------------------------------------

bool
FalseDiscoveryRateTab::preprocess(vector<string> &names) 
{
  if(sequences.empty()) {
    QMessageBox::information(NULL, "False Discovery Rate", "Need to load aligned sequences first.");
    return false;
  }

  if(trees.size() < 2) {
    QMessageBox::information(NULL, "False Discovery Rate", "Need to create at least two clusters first.");
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
FalseDiscoveryRateTab::process(const vector<Tree> &trees,
		       const vector<sequence_t> &sequences,
		       const vector<string> &names,
		       vector<string> &names2,
		       vector<summary_t> &summary2,
		       vector<result_t> &results1, 
			   vector<result_t> &results2, 
			   vector<result_t> &results3, 
			   vector<result_t> &results4) 
{
  vector<vector<double> > summary, rets1, rets2, rets3, rets4;
  if (!false_discovery_rate_compute(trees, sequences, rets1, rets2, rets3, rets4)) 
  {
    Progress::hide();
    if(!Progress::wasCancelled()) {
      QMessageBox::warning(NULL, "False Discovery Rate", "False Discovery Rate method failed.  Please recheck input parameters.");
    }
    return false;
  }


  if(!rets1.empty()) 
  {

      size_t i, j, ngroups = names.size();

	  names2.push_back("Probability Cutoff"); 
      for(i = 0; i < ngroups; i++) 
	  {
          for(j = i + 1; j < ngroups; j++) 
		  {
	          string str = "FDR of " + names[i] + '/' + names[j];
              names2.push_back(str);
          }
      }

      size_t nrets = rets1[0].size();
    
      result_t r;
      r.values.resize(rets1.size());

      for(i = 0; i < nrets; i++) {
          r.pos = i;
          for(j = 0; j < rets1.size(); j++) {
	          r.values[j] = rets1[j][i];
          }
          results1.push_back(r);
      }

  }

  if(!rets2.empty()) 
  {

	  size_t i, j; 

	  if (rets1.empty())
	  {
		  names2.push_back("Probability Cutoff"); 
		  names2.push_back("False Discovery Rate"); 
	  }


	  size_t nrets = rets2[0].size();

	  result_t r;

	  r.values.resize(rets2.size());

	  for(i = 0; i < nrets; i++) {
		  r.pos = i;
		  for(j = 0; j < rets2.size(); j++) {
			  r.values[j] = rets2[j][i];
		  }
		  results2.push_back(r);
	  }

  }


  if(!rets3.empty()) 
  {
	  size_t i, j;

      size_t nrets = rets3[0].size();
    
      result_t r;
      r.values.resize(rets3.size());

      for(i = 0; i < nrets; i++) {
          r.pos = i;
          for(j = 0; j < rets3.size(); j++) {
	          r.values[j] = rets3[j][i];
          }
          results3.push_back(r);
      }
  }

  if(!rets4.empty()) 
  {
	  size_t i, j; 

	  size_t nrets = rets4[0].size();

	  result_t r;
	  r.values.resize(rets4.size());

	  for(i = 0; i < nrets; i++) {
		  r.pos = i;
		  for(j = 0; j < rets4.size(); j++) {
			  r.values[j] = rets4[j][i];
		  }
		  results4.push_back(r);
	  }
  }


  Progress::increment(summary.size() * 100);
  return true;

}

//----------------------------------------------------------------------

void
	FalseDiscoveryRateTab::set_results(const vector<string> &r_names, const vector<string> &s_names, 
	const vector<result_t> &results1, const vector<result_t> &results2, 
	const vector<result_t> &results3, const vector<result_t> &results4) {
		s_names_ = s_names;
		r_names_ = r_names;
		results1_ = results1;
		results2_ = results2;
		results3_ = results3;
		results4_ = results4;

		vector<result_t> results; 

		int num = 0; 

		result_t r;

		vector<result_t>::iterator i;
		for(i = results1_.begin(); i != results1_.end(); i++) {
			i->pos = 1;
			r.pos = i->pos; 
			r.values = i->values; 
			results.push_back(r); 
		}

		for(i = results2_.begin(); i != results2_.end(); i++) {
			i->pos = 2;
			r.pos = i->pos; 
			r.values = i->values; 
			results.push_back(r); 
		}

		results_ = results; 

		list_->set_results(s_names_, results_);

		gv_->clear();
		gv_->set_results(results3_, results4_);

		emit resultsChanged();
}

//----------------------------------------------------------------------
