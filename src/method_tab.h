#ifndef _METHOD_TAB_H_
#define _METHOD_TAB_H_

#include <vector>
#include <list>

#include <qlistview.h>
#include <qlabel.h>
#include <qstring.h>
#include <qhbox.h>

#include "common.h"
#include "qlistview_wcs.h"
#include "graph_view.h"
#include "alignment_view.h"
#include "tip_window.h"
#include "qpushbutton.h"

class MethodSummaryListItem : public QListViewItem {
public:
  MethodSummaryListItem(QListView *parent, const summary_t &s);

  summary_t summary() const { return summary_; }
  QString key(int column, bool ascending) const;
  
protected:
  summary_t summary_;
};

class MethodSummaryList : public QListView {
  //  Q_OBJECT
public:
  MethodSummaryList(QWidget *parent = NULL, const char *name = NULL);
  ~MethodSummaryList();

  void update();
  void set_summary(const std::vector<std::string> &names,
		   const std::vector<summary_t> &summary);
  
private:
  std::vector<std::string> names_;
  std::vector<summary_t> summary_;
};

class MethodResultsListItem : public QListViewItem {
public:
  MethodResultsListItem(QListView *parent, const result_t &r);

  result_t result() const { return result_; }
  QString key(int column, bool ascending) const;
  
protected:
  result_t result_;
};

class MethodResultsList : public QListViewWCS {
  Q_OBJECT
public:
  MethodResultsList(QWidget *parent = NULL, const char *name = NULL);
  ~MethodResultsList();

  void update();
  void set_results(const std::vector<std::string> &names,
		   const std::vector<result_t> &results);
  
protected slots:
  void selection_changed();

private:
  std::vector<std::string> names_;
  std::vector<result_t> results_;
};

class MethodTab : public QWidget {
  Q_OBJECT
public:
  MethodTab(QWidget *parent = NULL, const char *name = NULL, const char * desc1 = NULL, const char *desc2 = NULL, bool need_tip = false);
  ~MethodTab();

  void set_results(const std::vector<std::string> &s_names,
		   const std::vector<summary_t> &summary,
		   const std::vector<std::string> &r_names,
		   const std::vector<result_t> &results);
  void set_tip_info(QString tip);
  
signals:
  void resultsChanged();
  
public slots:
    void clear();
 
protected slots:
  virtual void calculate();
  virtual void bootstrap();
  virtual void bootstrap(int nsamples);
  virtual void display_on_alignment();
  virtual void display_on_structure() const;
  virtual bool export_data();
  void update_graph();
  void showTip(); 

protected:
  QLabel *label() { return label_; }
  QHBox *params_area() { return params_box_; }
  
private:
  MethodSummaryList *sum_list_;
  MethodResultsList *list_;
  GraphView *gv_;
  QLabel *label_;
  QHBox *params_box_;
  AlignmentViewDialog *avd_;
  std::vector<std::string> s_names_, r_names_;
  std::vector<summary_t> summary_;
  std::vector<result_t> results_;
  QString tip_info_;
  bool need_tip_info_;
};

#endif
