#include "tip_window.h"

#include <qdialog.h>
#include <qlayout.h>
#include <qtextview.h>
#include <qstring.h>
#include <qpushbutton.h>

TipWindow::TipWindow(QWidget *parent, const char *name) : QDialog(parent, name, true, WStyle_Customize | WStyle_NormalBorder | WStyle_Title) 
{
	QVBoxLayout *top_layout = new QVBoxLayout(this);
	top_layout->setMargin(0);

	_text_view = new QTextView(this);
	_text_view->setHScrollBarMode(QScrollView::AlwaysOff);
	_text_view->setVScrollBarMode(QScrollView::AlwaysOn);
	top_layout->addWidget(_text_view);	
	_text_view->setTextFormat(RichText);
	
	QPushButton *pb1 = new QPushButton("&Close", this);
	top_layout->addWidget(pb1);
	pb1->setFixedWidth(pb1->sizeHint().width());
	connect(pb1, SIGNAL(clicked()), this, SLOT(ok_clicked()));
	top_layout->activate();
	this->resize(400, 400);
	this->setCaption(name);
}


TipWindow::~TipWindow() {
}

void TipWindow::ok_clicked() {
  hide();
}

void TipWindow::appendText(QString &text) {
	_text_view->append(text);
}

void TipWindow::setText(QString &text) {
  _text_view->setText(text);
}


void TipWindow::clearText() {
	_text_view->setText("");
}
