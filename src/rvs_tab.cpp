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

#include "rvs_tab.h"
#include "common.h"
#include "rvs.h"
#include "tree.h"
#include "progress.h"
#include "cluster.h"
#include "tree.h"

//----------------------------------------------------------------------

using namespace std;

extern vector<sequence_t> sequences;
extern vector<int> removed;
extern vector<Tree> trees;

//----------------------------------------------------------------------

//RVSTab inherit MethodTab and use its constructor
RVSTab::RVSTab(QWidget *parent, const char *name) 
  : MethodTab(parent, name, "Results From Statistical Analysis", "Site-Specific Profile (Xk: Number of Changes; Rk: Posterior Mean of Evolutionary Rate)", true)
{
  label()->setText("Rate Variation Among Sites (RVS)");  
    set_tip_info("<b>Alpha</b>: ML estimateof Gamma shape parameter<br>"
	       "<b>D</b>: mean number of substitutions<br>"
	       "<b>N</b>: number of sites"
	       );
}

//----------------------------------------------------------------------
//deconstructor
RVSTab::~RVSTab() {}

//----------------------------------------------------------------------

void
RVSTab::calculate() {
  vector<string> t_names;
  if(!preprocess(t_names)) return;
  
  Progress::setup(0, (trees.size() + trees.size()*(trees.size()-1)/2)*100,
		  "Computing RVS...");
  Progress::show();

  vector<string> s_names, r_names;
  vector<summary_t> summary;
  vector<result_t> results;

  if(!process(trees, sequences, t_names, s_names, summary, r_names, results)) return;

  Progress::hide();

  set_results(s_names, summary, r_names, results);
}

//----------------------------------------------------------------------

void
RVSTab::bootstrap(int nsamples) {
  vector<string> t_names;
  if(!preprocess(t_names)) return;

  bool use_resampled_sequences = false;

  switch(QMessageBox::information(NULL, "Bootstrapped RVS Method",
				  "Should the resampled sequences used\n"
				  "to build the new trees be used in\n"
				  "the RVS method?\n\n",
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
  vector<string> s_names, r_names;
  vector<summary_t> summary, avg_summary;
  vector<result_t> results, avg_results;

  for(i=0; i<bs_values.size(); i++) {
    bs_values[i] = 0;
  }

  int progress_ps = (trees.size() + trees.size()*(trees.size()-1)/2)*100;
  Progress::setup(0, progress_ps * (nsamples+1), "Computing Bootstraped RVS...");
  Progress::show();

  if(!process(trees, sequences, t_names, s_names, summary, r_names, results)) return;

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
      for(j=0; j<trees.size(); j++) {
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

    s_names.clear();
	r_names.clear();
    vector<summary_t> summary;
    vector<result_t> results;

    if(use_resampled_sequences) {
      if(!process(bs_trees, ss, t_names, s_names, summary, r_names, results)) return;
    } else {
      if(!process(bs_trees, sequences, t_names, s_names, summary, r_names, results)) return;
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

  int n1=s_names.size();
  int n2 = r_names.size();
  for(j=0; j<n1; j++) {
    s_names.push_back("Bootstrapped " + s_names[j]);	
  }

  if(!use_resampled_sequences) {
	  for(j = 0; j < n2; j++)
		r_names.push_back("Bootstrapped " + r_names[j]);
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

  QMessageBox::information(NULL, "Bootstrapped RVS Method", str);
}

//----------------------------------------------------------------------

bool
RVSTab::preprocess(vector<string> &names) {
  if(trees.size() < 1) {
    QMessageBox::information(NULL, "RVS Method", "Need to create at a single cluster first.");
    return false;
  }
  if(trees.size() > 1) {
    QMessageBox::information(NULL, "RVS Method", "Need to restricted to a single cluster.");
    return false;
  }
  if(sequences.empty()) {
    QMessageBox::information(NULL, "RVS Method", "Need to load aligned sequences first.");
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
RVSTab::process(const vector<Tree> &trees,
		const vector<sequence_t> &sequences,
		const std::vector<std::string> &names,
		std::vector<std::string> &s_names,
		std::vector<summary_t> &summary2,
		std::vector<std::string> &r_names,
		std::vector<result_t> &results2) {
  vector<vector<double> > summary, rets;
  int i;

  if(!rvs_compute(trees, sequences, summary, rets)) {
    Progress::hide();
    if(!Progress::wasCancelled()) {
      QMessageBox::warning(NULL, "RVS Method", "RVS method failed.  Please recheck input parameters.");
    }
    return false;
  }
  
  if(!summary.empty()) {
	for(i = 0; i < names.size(); i++) {
	  s_names.push_back(names[i]);
	}

    //char *names[7] = { "Alpha", "SE Alpha", "D", "N" };
     char *names[7] = { "Alpha", "D", "N" };

    size_t nsum = summary[0].size();
    assert(nsum == 3);
    
    summary_t s;
    s.values.resize(summary.size());

    for(i=0; i<nsum; i++) {
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
      r_names.push_back(names[i] + " Xk");
      r_names.push_back(names[i] + " Rk");
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
