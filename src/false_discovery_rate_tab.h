#ifndef _FALSE_DISCOVERY_RATE_H_
#define _FALSE_DISCOVERY_RATE_H_

#include <string>
#include <vector>
#include <list>

#include <qwidget.h>

#include "tree.h"
#include "sequence.h"

#include "common.h"
#include "qlistview_wcs.h"
#include "fdr_graph_view.h"
#include "tip_window.h"
#include "qpushbutton.h"

#include "method_tab.h"

class QPushButton;
class QCheckBox;

class QLineEdit;
class QComboBox;


class FalseDiscoveryRateTab : public QWidget {
	Q_OBJECT
public:
	FalseDiscoveryRateTab(QWidget *parent = NULL, const char *name = NULL);
	~FalseDiscoveryRateTab();

	void set_results(const std::vector<std::string> &s_names,
		const std::vector<std::string> &r_names,
		const std::vector<result_t> &results1, 
		const std::vector<result_t> &results2, 
		const std::vector<result_t> &results3, 
		const std::vector<result_t> &results4);
	void set_tip_info(QString tip);
	std::vector<result_t> get_results(); 

signals:
	void resultsChanged();

protected slots: 
	void calculate(); 

private:

	MethodSummaryList *sum_list_;
	MethodResultsList *list_;
	FDRGraphView *gv_;
	QLabel *label_;
	QHBox *params_box_;
	std::vector<std::string> s_names_, r_names_;
	std::vector<summary_t> summary_;
	std::vector<result_t> results_;
	std::vector<result_t> results1_;
	std::vector<result_t> results2_;
	std::vector<result_t> results3_;
	std::vector<result_t> results4_;
	QString tip_info_;
	bool need_tip_info_;

	bool preprocess(std::vector<std::string> &t_names);
	bool process(const std::vector<Tree> &trees,
		const std::vector<sequence_t> &sequences,
		const std::vector<std::string> &t_names,
		std::vector<std::string> &names, 
		std::vector<summary_t> &summary, 
		std::vector<result_t> &results1, 
		std::vector<result_t> &results2, 
		std::vector<result_t> &results3, 
		std::vector<result_t> &results4); 

};

#endif
