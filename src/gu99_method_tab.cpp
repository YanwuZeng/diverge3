#include <string>
#include <vector>

#include <stdio.h>
#include <assert.h>

#include <qmessagebox.h>
#include <qvalidator.h>
#include <qfiledialog.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qlineedit.h>
#include <qcombobox.h>

#include "gu99_method_tab.h"
#include "common.h"
#include "gu99.h"
#include "tree.h"
#include "progress.h"
#include "cluster.h"
#include "tree.h"

//----------------------------------------------------------------------

using namespace std;

extern vector<sequence_t> sequences;
extern vector<int> removed;
extern vector<Tree> trees;

extern vector<vector<double> > summary; 
extern vector<vector<double> > rets2; 
extern vector<result_t> results2; 
extern int runTypeInt; 

//----------------------------------------------------------------------

//Gu99MethodTab inherits MethodTab's constructor

Gu99MethodTab::Gu99MethodTab(QWidget *parent, const char *name) 
  : MethodTab(parent, name, NULL, NULL, false)
{
  label()->setText("Gu99 Estimation of the Coefficient of Functional Divergence");  
}

//----------------------------------------------------------------------

//deconstructor
Gu99MethodTab::~Gu99MethodTab() {}

//----------------------------------------------------------------------

void
Gu99MethodTab::calculate() {
  vector<string> t_names;
  if(!preprocess(t_names)) return;
  
  Progress::setup(0, (trees.size() + trees.size()*(trees.size()-1)/2)*100,
		  "Computing Gu99...");
  Progress::show();

  vector<string> names;
  vector<summary_t> summary;
  vector<result_t> results;

  if(!process(trees, sequences, t_names, names, summary, results)) return;

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

void
Gu99MethodTab::bootstrap(int nsamples) {
  vector<string> t_names;
  if(!preprocess(t_names)) return;

  bool use_resampled_sequences = false;

  switch(QMessageBox::information(NULL, "Bootstrapped Gu99 Method",
				  "Should the resampled sequences used\n"
				  "to build the new trees be used in\n"
				  "the Gu99 method?\n\n",
				  "Yes", "No", QString::null, 1)) {
  case 0:
    use_resampled_sequences = true;
    break;
  case 1:
    use_resampled_sequences = false;
    break;
  case -1:
    return;
    break;
  }

  int i, j, k;

  vector<int> bs_values(trees.size());
  vector<string> names;
  vector<summary_t> summary, avg_summary;
  vector<result_t> results, avg_results;

  for(i=0; i<bs_values.size(); i++) {
    bs_values[i] = 0;
  }

  int progress_ps = (trees.size() + trees.size()*(trees.size()-1)/2)*100;
  Progress::setup(0, progress_ps * (nsamples+1), "Computing Bootstraped Gu99...");
  Progress::show();

  if(!process(trees, sequences, t_names, names, summary, results)) return;

  int nsamples_used = 0;

  for(i=0; i<nsamples; i++) {
    Progress::set_progress(progress_ps * (i+1));

    vector<sequence_t> ss;
    sample(sequences, ss);
    string bs_tree;
    nj_cluster(ss, bs_tree, Poisson, true, false);
    
    vector<Tree> bs_trees(trees.size());
    {
      bool complete = true;
      for(int j=0; j<trees.size(); j++) {
	set<string> clade;
	trees[j].leaf_names(clade);
	Tree tmp;
	string tmp2;
	if(!tmp.import(bs_tree)) {
	  abort();
	}
	if(!tmp.clade_topology(clade, tmp2)) {
	  complete = false;
	  continue;
	}
	if(!tmp.import(tmp2)) {
	  abort();
	}
	if(!tmp.polyroot(bs_trees[j])) abort();
	bs_trees[j].set_filename(trees[j].filename());
	bs_values[j]++;
      }
      if(!complete) continue;
      nsamples_used++;
    }

    names.clear();
    vector<summary_t> summary;
    vector<result_t> results;

    if(use_resampled_sequences) {
      if(!process(bs_trees, ss, t_names, names, summary, results)) return;
    } else {
      if(!process(bs_trees, sequences, t_names, names, summary, results)) return;
    }
    
    if(nsamples_used == 1) {
      avg_summary = summary;
      avg_results = results;
    } else {
      assert(avg_summary.size() == summary.size());
      for(j=0; j<summary.size(); j++) {
	assert(avg_summary[j].name == summary[j].name);
	assert(avg_summary[j].values.size() == summary[j].values.size());
	for(k=0; k<summary[j].values.size(); k++) {
	  avg_summary[j].values[k] += summary[j].values[k];
	}
      }
      assert(avg_results.size() == results.size());
      for(j=0; j<results.size(); j++) {
	assert(avg_results[j].pos == results[j].pos);
	assert(avg_results[j].values.size() == results[j].values.size());
	for(k=0; k<results[j].values.size(); k++) {
	  avg_results[j].values[k] += results[j].values[k];
	}
      }
    }
  }

  int n=names.size();
  vector<string> s_names = names, r_names = names;
  for(j=0; j<n; j++) {
    s_names.push_back("Bootstrapped " + names[j]);
    if(!use_resampled_sequences) {
      r_names.push_back("Bootstrapped " + names[j]);
    }
  }
  for(j=0; j<avg_summary.size(); j++) {
    for(k=0; k<avg_summary[j].values.size(); k++) {
      avg_summary[j].values[k] /= nsamples_used;
      summary[j].values.push_back(avg_summary[j].values[k]);
    }
  }
  if(!use_resampled_sequences) {
    for(j=0; j<avg_results.size(); j++) {
      for(k=0; k<avg_results[j].values.size(); k++) {
	avg_results[j].values[k] /= nsamples_used;
	results[j].values.push_back(avg_results[j].values[k]);
      }
    }
  }

  Progress::hide();

  if(nsamples_used > 0) {
    set_results(s_names, summary, r_names, results);
  }

  QString str = "Bootstrap results\n";
  for(j=0; j<bs_values.size(); j++) {
    str += tr("        %1\t%2/%3\n").arg(t_names[j].c_str()).arg(bs_values[j]).arg(nsamples);
  }
  str += tr("        Joint\t%1/%2\n").arg(nsamples_used).arg(nsamples);

  QMessageBox::information(NULL, "Bootstrapped Gu99 Method", str);
}

//----------------------------------------------------------------------

bool
Gu99MethodTab::preprocess(vector<string> &names) {
  if(trees.size() < 2) {
    QMessageBox::information(NULL, "Gu99 Method", "Need to create at least two clusters first.");
    return false;
  }
  if(sequences.empty()) {
    QMessageBox::information(NULL, "Gu99 Method", "Need to load aligned sequences first.");
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
Gu99MethodTab::process(const vector<Tree> &trees,
		       const vector<sequence_t> &sequences,
		       const std::vector<std::string> &names,
		       std::vector<std::string> &names2,
		       std::vector<summary_t> &summary2,
		       std::vector<result_t> &results2) {
  vector<vector<double> > summary, rets;
  
  if(!gu99_compute(trees, sequences, summary, rets)) {
    Progress::hide();
    if(!Progress::wasCancelled()) {
      QMessageBox::warning(NULL, "Gu99 Method", "Gu99 method failed.  Please recheck input parameters.");
    }
    return false;
  }
  
  if(!summary.empty()) {
    char *names[9] = { "MFE Theta", "MFE se", "MFE r X", "MFE r max",
		       "MFE z score", "ThetaML", "AlphaML", "SE Theta",
		       "LRT Theta" };

    size_t nsum = summary[0].size();
    assert(nsum == 9);
    
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
  
  if(!rets.empty()) {
    size_t i, j, ngroups = names.size();
    for(i = 0; i < ngroups; i++) {
      for(j = i + 1; j < ngroups; j++) {
	string str = names[i] + '/' + names[j];
	names2.push_back(str);
      }
    }
    
    size_t nrets = rets[0].size();
    
    result_t r;
    r.values.resize(rets.size());

    for(i = 0; i < nrets; i++) {
      r.pos = i;
      for(j = 0; j < rets.size(); j++) {
	r.values[j] = rets[j][i];
      }
      results2.push_back(r);
    }
  }

  return true;
}

//----------------------------------------------------------------------
