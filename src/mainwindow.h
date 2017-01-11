#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qmainwindow.h>
#include <qstring.h>
#include "guwidget.h"

class QToolBar;
class QPopupMenu;

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

protected slots:
    void fileNew();
    void fileOpen();
    void fileSave();
    void fileSaveAs();
    void filePrint();
    void closeWindow();
	void helpPaper();
	void helpAbout();

protected:
    void setupMenuBar();
    void setupStatusBar();
    void setupCentralWidget();
    void setupToolBar();
    void closeEvent( QCloseEvent * e );

    QToolBar *toolbar;
    GuWidget *guWidget;

};


#endif
