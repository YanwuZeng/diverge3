#include <string>
#include <sstream>
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

#include "type_one_method_tab.h"
#include "type_one.h"

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

// TypeIMethodTab inherit QWidget and use its constructor.
TypeIMethodTab::TypeIMethodTab(QWidget *parent, const char *name) 
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


  label_ = new QLabel("Gene-specific Type I Functional Divergence Analysis", this);
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
  label2 = new QLabel("Site-Specific Profile: Posterior Probability", this);


  QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding,
	  QSizePolicy::Minimum );

  h_box_layout->addWidget(label2);
  h_box_layout->addItem(spacer);

  top_layout->addLayout(h_box_layout);
  top_layout->addSpacing(2);


  list_ = new MethodResultsList(this);
  top_layout->addWidget(list_);
  top_layout->addSpacing(10);

  connect(list_, SIGNAL(header_changed()), SLOT(update_graph())); 

  gv_ = new GraphView(this);
  top_layout->addWidget(gv_);

  top_layout->activate();
  

  this->clearFocus();

}

//----------------------------------------------------------------------
// Deconstructor
TypeIMethodTab::~TypeIMethodTab() {}

//----------------------------------------------------------------------

void
	TypeIMethodTab::set_tip_info(QString tip) {
		tip_info_ = tip;
}

//----------------------------------------------------------------------

void TypeIMethodTab::calculate() 
{
	vector<string> t_names;
	if(!preprocess(t_names)) 
		return;


	Progress::setup(0, (trees.size() + trees.size()*(trees.size()-1)/2)*100, 
		"Computing Type I Divergence ... ");
	Progress::show();


	vector<string> names;
	vector<summary_t> summary;
	vector<result_t> results;

	if (!process(trees, sequences, t_names, names, summary, results)) 
	{
		Progress::hide();
		return;
	}

	Progress::hide(); 
	
	set_results(names, summary, names, results);
	
	int n = trees.size(), i, j, k;
	QStringList cluster_names;
	DMatrix2D theta_values(n, n);
	for(k=0, i=0; i<n; i++) {
		for(j=i+1; j<n; j++) {
			//theta_values(j, i) = theta_values(i, j) = summary[k++].values[5];
			theta_values(j, i) = theta_values(i, j) = summary[0].values[k++];
		}
		cluster_names.append(trees[i].filename().c_str());
	}
    emit thetaValuesChanged(cluster_names, theta_values);
	
}

//----------------------------------------------------------------------

bool
TypeIMethodTab::preprocess(vector<string> &names) 
{
  if(sequences.empty()) {
    QMessageBox::information(NULL, "Type I Analysis", "Need to load aligned sequences first.");
    return false;
  }
  if(trees.size() != 3) {
    QMessageBox::information(NULL, "Type I Analysis", "Need to create three clusters first.");
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
TypeIMethodTab::process(const vector<Tree> &trees,
		       const vector<sequence_t> &sequences,
		       const vector<string> &names,
		       vector<string> &names2,
		       vector<summary_t> &summary2,
		       vector<result_t> &results2) 
{
  vector<vector<double> > summary, rets;
  if (!type_one_compute(trees, sequences, summary, rets)) 
  {
    Progress::hide();
    if(!Progress::wasCancelled()) {
      QMessageBox::warning(NULL, "Type I Analysis", "Type I method failed.  Please recheck input parameters.");
    }
    return false;
  }
  
  if (!summary.empty()) 
  {
    /*char *names[21] = { "Da", "Db", "N", "C",
			"R", "p", "d", "W", "Z", "Alpha ML", "Theta-II", "Theta SE",
			"Gr", "Gc", "h", "Q", "Ar", "PIr", "F00,N", "F00,R", "F00,C"};*/

    char *names[21] = { "Da", "Db", "N", "C",
			"R", "Alpha ML", "Theta-II", "Theta SE", "Ar", "PIr",
			"p", "d", "W", "Z", "Gr", "Gc", "h", "Q", "F00,N", "F00,R", "F00,C" };

    size_t nsum = summary[0].size();
    //assert(nsum == 9);
    
    summary_t s;
    s.values.resize(summary.size());

    for(size_t i=0; i<nsum; i++) {
      s.name = names[i];
      for(size_t j=0; j<summary.size(); j++) {
	s.values[j] = summary[j][i];
      }
      summary2.push_back(s);
    }
  }
  
  if (!rets.empty()) 
  {
      size_t i, j, ngroups = 5; 
      for(i = 0; i < ngroups; i++) 
	  {
		  char tempStr[10];
		  sprintf(tempStr, "%d", i);             // 将整数转换成字符串

		  string str = "P(S"; 
		  str.append(tempStr); 
		  str.append("|X)"); 
	      names2.push_back(str);
      }

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
	TypeIMethodTab::set_results(const vector<string> &s_names, const vector<summary_t> &summary,
	const vector<string> &r_names, const vector<result_t> &results) {
		s_names_ = s_names;
		r_names_ = r_names;
		summary_ = summary;
		results_ = results;

		vector<result_t>::iterator i;
		for(i = results_.begin(); i != results_.end(); i++) {
			i->pos = pos_w_gaps(removed, i->pos) + 1;
		}

		list_->set_results(r_names_, results_);

		gv_->clear();

		emit resultsChanged();
}

//----------------------------------------------------------------------

void
	TypeIMethodTab::update_graph() {

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
