#ifndef _SEQUENCES_TAB_H_
#define _SEQUENCES_TAB_H_

#include <string>

#include <qwidget.h>

#include "tree.h"

class AlignmentView;

class SequencesTab : public QWidget {
  Q_OBJECT
public:
  SequencesTab(QWidget *parent = NULL, const char *name = NULL);
  ~SequencesTab();

  bool load_alignment(std::string filename);

signals:
  void alignmentChanged();
  
private slots:
  void load_alignment();
  void load_pdb();

private:
  AlignmentView *seq_entry_;
  AlignmentView *pdb_alignment_;

  bool process_pdb(std::string filename);
  void updatePDBMap();
};

#endif
