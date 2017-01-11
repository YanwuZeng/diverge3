#ifndef _TYPEII_METHOD_TAB_H_
#define _TYPEII_METHOD_TAB_H_

#include "method_tab.h"
#include "tree.h"
#include "sequence.h"

class QLineEdit;
class QComboBox;

class TypeIIMethodTab : public MethodTab {
  Q_OBJECT
public:
  TypeIIMethodTab(QWidget *parent = NULL, const char *name = NULL);
  ~TypeIIMethodTab();

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
