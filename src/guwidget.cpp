#include "guwidget.h"

//GuWidget inheritate QWidget, and use constructor of QWidget first.
GuWidget::GuWidget(QWidget *parent, const char *name) 
  : QWidget(parent, name)
{

//ust BoxLayout format and set up TopLayout.	
  QBoxLayout *top_layout = new QVBoxLayout(this, 0, 6);
  top_layout->setMargin(5);
	
//add TabWidget: sequence_tab, clustering_tab, gu99_tab, rvs_tab, ancestral_seq_tab, func_dist_tab,(gu2001_tab).
  QTabWidget *tabWidget = new QTabWidget(this);
  top_layout->addWidget(tabWidget, 5);
  

  sequences_tab_ = new SequencesTab(this);
  tabWidget->addTab(sequences_tab_, "Sequences");

  clustering_tab_ = new ClusteringTab(this);
  tabWidget->addTab(clustering_tab_, "Clustering");

  gu99_tab_ = new Gu99MethodTab(this);
  tabWidget->addTab(gu99_tab_, "Gu99");

  //enable or disable
  gu2001_tab_ = new Gu2001MethodTab(this);
  tabWidget->addTab(gu2001_tab_, "Gu2001");

  typeII_tab_ = new TypeIIMethodTab(this);
  tabWidget->addTab(typeII_tab_, "Type II Divergence");

  rvs_tab_ = new RVSTab(this);
  tabWidget->addTab(rvs_tab_, "Rate Variation among Sites(RVS)");

  ancestral_seq_tab_ = new AncestralSeqTab(this);
  tabWidget->addTab(ancestral_seq_tab_, "Ancestral Sequence Inference");

  func_dist_tab_ = new FuncDistTab(this);
  tabWidget->addTab(func_dist_tab_, "Functional Distance Analysis");


  typeI_tab_ = new TypeIMethodTab(this);
  tabWidget->addTab(typeI_tab_, "Type I Analysis");

  effective_number_sites_tab_ = new EffectiveNumberSitesTab(this);
  tabWidget->addTab(effective_number_sites_tab_, "Effective Number of Sites");

  asymmetric_test_one_tab_ = new AsymmetricTestOneTab(this);
  tabWidget->addTab(asymmetric_test_one_tab_, "Asymmetric Test for Type I");

  false_discovery_rate_tab_ = new FalseDiscoveryRateTab(this);
  tabWidget->addTab(false_discovery_rate_tab_, "False Discovery Rate");

  
  //add signal to each tab slot
  connect(sequences_tab_, SIGNAL(alignmentChanged()), clustering_tab_, SLOT(clear()));
  connect(sequences_tab_, SIGNAL(alignmentChanged()), gu99_tab_, SLOT(clear()));
  connect(sequences_tab_, SIGNAL(alignmentChanged()), rvs_tab_, SLOT(clear()));
  connect(sequences_tab_, SIGNAL(alignmentChanged()), ancestral_seq_tab_, SLOT(update_alignment()));
  connect(sequences_tab_, SIGNAL(alignmentChanged()), ancestral_seq_tab_, SLOT(clear()));
  connect(sequences_tab_, SIGNAL(alignmentChanged()), func_dist_tab_, SLOT(clear()));
  connect(sequences_tab_, SIGNAL(alignmentChanged()), gu2001_tab_, SLOT(clear()));
  connect(sequences_tab_, SIGNAL(alignmentChanged()), typeII_tab_, SLOT(clear()));
  connect(clustering_tab_, SIGNAL(clustersChanged()), gu99_tab_, SLOT(clear()));
  connect(clustering_tab_, SIGNAL(clustersChanged()), rvs_tab_, SLOT(clear()));
  connect(clustering_tab_, SIGNAL(clustersChanged()), func_dist_tab_, SLOT(clear()));
  //connect(clustering_tab_, SIGNAL(clustersChanged()), gu2001_tab_, SLOT(clear()));
  connect(gu99_tab_,       SIGNAL(thetaValuesChanged(QStringList, DMatrix2D)), func_dist_tab_, SLOT(updateResults(QStringList, DMatrix2D)));

  connect(sequences_tab_, SIGNAL(alignmentChanged()), typeI_tab_, SLOT(clear())); 
  connect(clustering_tab_, SIGNAL(clustersChanged()), typeI_tab_, SLOT(clear())); 
  connect(sequences_tab_, SIGNAL(alignmentChanged()), effective_number_sites_tab_, SLOT(clear())); 
  connect(clustering_tab_, SIGNAL(clustersChanged()), effective_number_sites_tab_, SLOT(clear())); 
  connect(sequences_tab_, SIGNAL(alignmentChanged()), asymmetric_test_one_tab_, SLOT(clear())); 
  connect(clustering_tab_, SIGNAL(clustersChanged()), asymmetric_test_one_tab_, SLOT(clear())); 
  connect(sequences_tab_, SIGNAL(alignmentChanged()), false_discovery_rate_tab_, SLOT(clear())); 
  connect(clustering_tab_, SIGNAL(clustersChanged()), false_discovery_rate_tab_, SLOT(clear())); 
   
  //always activate top_layout
  top_layout->activate();
  
}

//----------------------------------------------------------------------

//deconstructor
GuWidget::~GuWidget() {
}

//----------------------------------------------------------------------

//define two functions: load_alignment and load_tree.
bool
GuWidget::load_alignment(string filename) {
  return sequences_tab_->load_alignment(filename);
}

//----------------------------------------------------------------------

bool
GuWidget::load_tree(string filename) {
  return clustering_tab_->load_tree(filename);
}

//----------------------------------------------------------------------
