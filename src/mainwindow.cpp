#include "mainwindow.h"
#include "guwidget.h"

#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qapplication.h>
#include <qfiledialog.h>

#include "resources.h"
#include "splash_window.h"
extern Resources * resources;
extern SplashWindow * splash_window;

//constructor
MainWindow::MainWindow() : QMainWindow( 0, "Diverge 3.1 Beta 1" )
{
	resources = new Resources();
	//setupToolBar();
    setupMenuBar();
    setupStatusBar();
    setupCentralWidget();
	
}//MainWindow has 3 part: MenuBar, StatusBar, CentralWidget

//deconstructor
MainWindow::~MainWindow()
{
}

//define setupMenuBar function
void MainWindow::setupMenuBar()
{
#ifdef WIN32
	setCaption("Qt Diverge 3.1 Beta 1");
#else
	setCaption("Diverge 3.1 Beta 1");
#endif

//insert item in MenuBar's "File": New, open, save, save as, print, close, exit.
    QPopupMenu *fileMenu = new QPopupMenu( this );
    menuBar()->insertItem( "&File", fileMenu );

    /*fileMenu->insertItem( "New", this, SLOT( fileNew() ), CTRL + Key_N );
    fileMenu->insertItem( QPixmap( "fileopen.xpm" ), "Open", this, SLOT( fileOpen() ), CTRL + Key_O );
    fileMenu->insertSeparator();
    fileMenu->insertItem( QPixmap( "filesave.xpm" ), "Save", this, SLOT( fileSave() ), CTRL + Key_S );
    fileMenu->insertItem( "Save As...", this, SLOT( fileSaveAs() ) );
    fileMenu->insertSeparator();
    fileMenu->insertItem( QPixmap( "fileprint.xpm" ), "Print...", this, SLOT( filePrint() ), CTRL + Key_P );
    fileMenu->insertSeparator();
    fileMenu->insertItem( "Close", this, SLOT( closeWindow() ), CTRL + Key_W );*/
    fileMenu->insertItem( "Quit", this, SLOT( closeWindow() ), CTRL + Key_Q );

//insert item in MenuBar's "Help":related paper, about diverge. 
	QPopupMenu * helpMenu = new QPopupMenu( this );
    menuBar()->insertItem( "&Help", helpMenu );

    helpMenu->insertItem( "Related &Paper", this, SLOT( helpPaper() ), Key_F1 );
    helpMenu->insertSeparator();
    helpMenu->insertItem( "&About Diverge", this, SLOT( helpAbout() ));

	
    guWidget = new GuWidget(this, "Main Widget");
	
	setCentralWidget(guWidget);
}

//---------------------------------------------------------------------------------------
void MainWindow::setupToolBar()
{
	toolbar = new QToolBar( this, "Operations" );
    //fileTools->setLabel( tr( "File Operations" ) );

    /*openIcon = QPixmap( fileopen );
    QToolButton * fileOpen
	= new QToolButton( openIcon, "Open File", QString::null,
			   this, SLOT(load()), toolbar, "open file" );

    saveIcon = QPixmap( filesave );
    QToolButton * fileSave
	= new QToolButton( saveIcon, "Save File", QString::null,
			   this, SLOT(save()), toolbar, "save file" );
	
    printIcon = QPixmap( fileprint );
    QToolButton * filePrint
	= new QToolButton( printIcon, "Print File", QString::null,
			   this, SLOT(print()), toolbar, "print file" );

	toolbar->addSeparator();*/

    QToolButton * topologyButton
	= new QToolButton( resources->topologyIcon, "Display Only Topology", QString::null,
			   this, SLOT(topology()), toolbar, "display only topology" );
	
	topologyButton->setToggleButton(true);

    QToolButton * vertfitButton
	= new QToolButton(resources->vertfitIcon, "Fit Width", QString::null,
			   this, SLOT(vertfit()), toolbar, "fit width" );

    QToolButton * horifitButton
	= new QToolButton(resources->horifitIcon, "Fit Height", QString::null,
			   this, SLOT(horifit()), toolbar, "fit height" );

	toolbar->addSeparator();

    QToolButton * logwinButton
	= new QToolButton(resources->logwinIcon, "Log Window", QString::null,
			   this, SLOT(showLog()), toolbar, "log window" );
	logwinButton->setToggleButton(true);
	connect(logwinButton, SIGNAL(toggled(bool)), this, SLOT(showLog(bool)));
}

//define setupStatusBar function
void MainWindow::setupStatusBar()
{
    statusBar()->message( "Ready", 2000 );//StatusBar shows "ready" in 2000 ms (2s) when excute diverge.
}

//define setupCentralWidget function
void MainWindow::setupCentralWidget()
{
    
}

void MainWindow::closeWindow()
{
	/*switch( QMessageBox::warning( this, "Quit",
		"Do you really want to quit Diverge?",
		QMessageBox::Yes,
		QMessageBox::No) ) {
	case QMessageBox::Yes:
		qApp->quit();
        break;
	case QMessageBox::No: 
        break;
	}*/
	switch( QMessageBox::warning( this, "Quit",
		"Do you really want to quit Diverge?",
		"Yes",
		"No", QString::null, 0, 1) ) {
	case 0:
		qApp->quit();
        break;
	case 1: 
        break;
	}
}

void MainWindow::fileNew()
{
}

void MainWindow::fileOpen()
{
   
}

void MainWindow::fileSave()
{
   
}

void MainWindow::fileSaveAs()
{
   
}

void MainWindow::filePrint()
{
}

//Help Menu item "about diverge" will popup a window shows authors and copyright.
void MainWindow::helpAbout() {
/*	QMessageBox mb(appname,
                   QObject::tr("<center><b>%1 %2</b><br>"
                               "by<br>"
                               "Xun Gu, Dongping Xu, <br> Wei Huang, and Kent V. Velden<br>"
                               "<br>"
                               "Copyright ?2000-2004<br>"
                               "Iowa State University Research Foundation, Inc.<br>"
                               "<br>"
                               "All Rights Reserved<br>").arg(appname).arg(version),
                   QMessageBox::NoIcon,
                   QMessageBox::Yes | QMessageBox::Default,
                   0, 0);
    mb.exec();
	*/
	splash_window->show();
}

//Help Menu item "related paper" 
void MainWindow::helpPaper() {
	QMessageBox mb("Related paper",
                   "<p>[1] Gu X. (1999) <b>Statistical methods for testing functional divergence after gene duplication.</b> Molecular Biology and Evolution 16:1664-1674. <p> [2] Gu X. (2001) <b>Maximum likelihood approach for gene family evolution under functional divergence.</b> Molecular Biology and Evolution. 18:453-464. <p> [3] Gu X. (2013) <b>An Update of DIVERGE Software for Functional Divergence Analysis of Protein Family.</b> Mol Biol Evol, 30 (7): 1713-171",

                   QMessageBox::NoIcon,
                   QMessageBox::Ok | QMessageBox::Default,
                   0, 0);
    mb.exec();
	//statusBar()->message(QObject::tr("Gu X. (1999) <b>Statistical methods for testing functional divergence after gene duplication. </b> Molecular Biology and Evolution 16:1664-1674."));
}

void MainWindow::closeEvent( QCloseEvent * e )
{
  switch( QMessageBox::warning( this, "Quit",
				"Do you really want to quit Diverge?",
				"Yes",
				"No", QString::null, 0, 1) ) {
  case 0:
    qApp->quit();
    break;
  case 1: 
    break;
  }
}
