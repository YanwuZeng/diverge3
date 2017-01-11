#ifndef _TIP_WINDOW_H_
#define _TIP_WINDOW_H_

#include <qdialog.h>
#include <qstring.h>
#include <qtextview.h>

class TipWindow : public QDialog {
	Q_OBJECT
public:
  TipWindow(QWidget *parent = NULL, const char *name = NULL);
  ~TipWindow();

  void appendText(QString &str);
  void setText(QString &str);
  void clearText();

public slots:
  void ok_clicked();


private:
	QTextView * _text_view;

};

#endif 
 
