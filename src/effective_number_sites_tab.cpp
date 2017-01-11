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

#include "effective_number_sites_tab.h"
#include "effective_number.h"

#include "progress.h"


//----------------------------------------------------------------------

using namespace std;

extern vector<sequence_t> orig_sequences;
extern vector<sequence_t> sequences;
extern Tree input_tree;
extern vector<int> removed;
extern vector<Tree> trees;
extern MainWindow * main_window;

//----------------------------------------------------------------------

// EffectiveNumberSitesTab inherit QWidget and use its constructor.
EffectiveNumberSitesTab::EffectiveNumberSitesTab(QWidget *parent, const char *name) 
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


  label1_ = new QLabel("Effective Number of Sites Related to Functional Divergence (Type I or Type II)", this);
  top_layout->addWidget(label1_);
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


  label2_ = new QLabel("Type I Site-Specific Profile: Theta and Standard Deviation", this);

  QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding,
	  QSizePolicy::Minimum );

  h_box_layout->addWidget(label2_);
  h_box_layout->addItem(spacer);


  top_layout->addLayout(h_box_layout);
  top_layout->addSpacing(2);

  list1_ = new MethodResultsList(this);
  list1_->setColumnText(0, "Number"); 
  top_layout->addWidget(list1_);
  top_layout->addSpacing(10);


  label3_ = new QLabel("Type II Site-Specific Profile: Theta and Standard Deviation", this);

  top_layout->addWidget(label3_);
  top_layout->addSpacing(2);

  list2_ = new MethodResultsList(this);
  list2_->setColumnText(0, "Number"); 
  top_layout->addWidget(list2_);
  top_layout->addSpacing(10);


  top_layout->activate();
  

  this->clearFocus();

}

//----------------------------------------------------------------------
// Deconstructor
EffectiveNumberSitesTab::~EffectiveNumberSitesTab() {}

//----------------------------------------------------------------------

void
	EffectiveNumberSitesTab::set_tip_info(QString tip) {
		tip_info_ = tip;
}

//----------------------------------------------------------------------

void EffectiveNumberSitesTab::calculate() 
{
	vector<string> t_names;
	if(!preprocess(t_names)) 
		return;

	int op_overhead = trees.size() + trees.size()*(trees.size()-1)/2  +  //GZ97_compute()
		trees.size() * (trees.size() - 1) /2 + //GZf2_compute
		(trees.size()*(trees.size()-1)/2) + //type_two_compute
		trees.size() * (trees.size() - 1) / 2; //process() overhead

	Progress::setup(0, op_overhead * 100,
		"Computing Effective Number of Sites ...");
	Progress::show();

	vector<string> names;
	vector<summary_t> summary;
	vector<result_t> results1, results2;

	int EffectiveNumberType1 = 0; 
	int EffectiveNumberType2 = 0; 

	if(!process(trees, sequences, t_names, names, results1, results2, EffectiveNumberType1, EffectiveNumberType2)) 
	{
		Progress::hide();
		return;
	}

	set_results(names, names, results1, results2); 

	Progress::hide();  

	char cstr[150]; 
	sprintf(cstr, "Type I Site-Specific Profile: Theta and Standard Deviation     ----------     Effective Number of Type I is %d .", EffectiveNumberType1); 
	label2_->setText(cstr); 

	sprintf(cstr, "Type II Site-Specific Profile: Theta and Standard Deviation     ----------     Effective Number of Type II is %d .", EffectiveNumberType2); 
	label3_->setText(cstr); 

}

//----------------------------------------------------------------------

bool
EffectiveNumberSitesTab::preprocess(vector<string> &names) 
{
  if(sequences.empty()) {
    QMessageBox::information(NULL, "Effective Number of Sites", "Need to load aligned sequences first.");
    return false;
  }
  if(trees.size() != 2) {
    QMessageBox::information(NULL, "Effective Number of Sites", "Need to create two clusters first.");
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
EffectiveNumberSitesTab::process(const vector<Tree> &trees,
		       const vector<sequence_t> &sequences,
		       const vector<string> &names,
		       vector<string> &names2,
		       vector<result_t> &results1,
		       vector<result_t> &results2,
			   int &EffectiveNumberType1, int &EffectiveNumberType2) 
{
  vector<vector<double> > rets1, rets2;
  if (!effective_number_compute(trees, sequences, rets1, rets2, EffectiveNumberType1, EffectiveNumberType2)) 
  {
    Progress::hide();
    if(!Progress::wasCancelled()) {
      QMessageBox::warning(NULL, "Effective Number of Sites", "Effective Number of Sites method failed.  Please recheck input parameters.");
    }
    return false;
  }
  
  
  if(!rets1.empty()) 
  {

	  size_t i, j; 

	  names2.push_back("Theta"); 
	  names2.push_back("Theta Standard Deviation"); 


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
		  names2.push_back("Theta"); 
		  names2.push_back("Theta Standard Deviation"); 
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

  Progress::increment((results1.size() + results2.size()) * 100);
  return true;

}

//----------------------------------------------------------------------

void
	EffectiveNumberSitesTab::set_results(const vector<string> &r_names, const vector<string> &s_names, 
	const vector<result_t> &results1, const vector<result_t> &results2) {
		s_names_ = s_names;
		r_names_ = r_names;
		results1_ = results1;
		results2_ = results2;

		int num = 0; 

		vector<result_t>::iterator i;
		for(i = results1_.begin(); i != results1_.end(); i++) {
			i->pos = ++num;
		}

		num = 0; 

		for(i = results2_.begin(); i != results2_.end(); i++) {
			i->pos = ++num;
		}

		list1_->set_results(r_names_, results1_);
		list2_->set_results(s_names_, results2_);

		emit resultsChanged();
}

//----------------------------------------------------------------------
