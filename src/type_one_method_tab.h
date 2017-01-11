#ifndef _TYPEI_METHOD_TAB_H_
#define _TYPEI_METHOD_TAB_H_

#include <string>
#include <vector>
#include <list>

#include <qwidget.h>

#include "tree.h"
#include "sequence.h"
#include "matrix.h"

#include "common.h"
#include "qlistview_wcs.h"
#include "graph_view.h"
#include "tip_window.h"
#include "qpushbutton.h"

#include "method_tab.h"

class QPushButton;
class QCheckBox;

class QLineEdit;
class QComboBox;


class TypeIMethodTab : public QWidget {
	Q_OBJECT
public:
	TypeIMethodTab(QWidget *parent = NULL, const char *name = NULL);
	~TypeIMethodTab();

	void set_results(const std::vector<std::string> &s_names,
		const std::vector<summary_t> &summary,
		const std::vector<std::string> &r_names,
		const std::vector<result_t> &results);
	void set_tip_info(QString tip);
	std::vector<result_t> get_results(); 

signals:
	void resultsChanged();
	void thetaValuesChanged(QStringList, DMatrix2D);

protected slots: 
	void calculate(); 
	void update_graph();

private:

	MethodSummaryList *sum_list_;
	MethodResultsList *list_;
	GraphView *gv_;
	QLabel *label_;
	QHBox *params_box_;
	std::vector<std::string> s_names_, r_names_;
	std::vector<summary_t> summary_;
	std::vector<result_t> results_;
	QString tip_info_;
	bool need_tip_info_;

	bool preprocess(std::vector<std::string> &t_names);
	bool process(const std::vector<Tree> &trees,
		const std::vector<sequence_t> &sequences,
		const std::vector<std::string> &t_names,
		std::vector<std::string> &names,
		std::vector<summary_t> &summary,
		std::vector<result_t> &results);

};

#endif
