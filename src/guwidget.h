#ifndef GUWIDGET_H
#define GUWIDGET_H

#include <string>


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <qapplication.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtabwidget.h>
#include <qmessagebox.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qstatusbar.h>
#include <qmainwindow.h>

#include <qwidget.h>
#include <qmenubar.h>
#include <qpopupmenu.h>


#include "sequences_tab.h"
#include "clustering_tab.h"
#include "gu99_method_tab.h"
#include "gu2001_method_tab.h"
#include "rvs_tab.h"
#include "ancestral_seq_tab.h"
#include "func_dist_tab.h"
#include "common.h"
#include "sequence.h"
#include "tree.h"
#include "pdb.h"
#include "progress.h"
#include "matrix.h"
#include "type_two_method_tab.h"

#include "type_one_method_tab.h"
#include "effective_number_sites_tab.h"
#include "asymmetric_test_one_tab.h"
#include "false_discovery_rate_tab.h"

using namespace std;

class GuWidget : public QWidget {
  Q_OBJECT
public:
  GuWidget(QWidget *parent = NULL, const char *name = NULL);
  ~GuWidget();

  bool load_alignment(string filename);
  bool load_tree(string filename);
  
private:
  SequencesTab *sequences_tab_;
  ClusteringTab *clustering_tab_;
  MethodTab *gu99_tab_;
  RVSTab *rvs_tab_;
  AncestralSeqTab *ancestral_seq_tab_;
  FuncDistTab *func_dist_tab_;
  MethodTab *gu2001_tab_;
  TypeIIMethodTab *typeII_tab_;

  TypeIMethodTab * typeI_tab_; 
  EffectiveNumberSitesTab * effective_number_sites_tab_; 
  AsymmetricTestOneTab * asymmetric_test_one_tab_; 
  FalseDiscoveryRateTab * false_discovery_rate_tab_; 

};

//----------------------------------------------------------------------
#endif
