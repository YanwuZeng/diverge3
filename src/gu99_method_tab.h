#ifndef _GU99_METHOD_TAB_H_
#define _GU99_METHOD_TAB_H_

#include <string>
#include <vector>

#include <qstringlist.h>

#include "method_tab.h"
#include "tree.h"
#include "sequence.h"
#include "matrix.h"

class QLineEdit;
class QComboBox;

class Gu99MethodTab : public MethodTab {
  Q_OBJECT
public:
  Gu99MethodTab(QWidget *parent = NULL, const char *name = NULL);
  ~Gu99MethodTab();

signals:
  void thetaValuesChanged(QStringList, DMatrix2D);
  
protected slots:
  void calculate();
  void bootstrap(int nsamples);

private:
  bool preprocess(std::vector<std::string> &t_names);
  bool process(const std::vector<Tree> &trees,
	       const std::vector<sequence_t> &sequences,
	       const std::vector<std::string> &t_names,
	       std::vector<std::string> &names,
	       std::vector<summary_t> &summary,
	       std::vector<result_t> &results);
};

#endif
