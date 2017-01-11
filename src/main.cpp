#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mainwindow.h"
#include "guwidget.h"
#include "resources.h"
#include "global.h"
#include "splash_window.h"

//----------------------------------------------------------------------

using namespace std;

//Define the global variables.
//These variables will be used in other modules
//Check the sequence_tab.cpp for details.
vector<sequence_t> orig_sequences; //the original gene sequences (for alignment_view)
vector<sequence_t> sequences; //the original sequence
vector<int> removed; //the sequence with gaps removed
vector<Tree> trees; //the definition of tree structure
Tree input_tree;
PDB pdb; //protein data file
char pdb_chain_ID;
vector<string> pdb_map; 
Resources * resources;
SplashWindow * splash_window;
MainWindow * main_window;

//----------------------------------------------------------------------


void configureEnv(QWidget * window) {
	int width = WINDOW_WIDTH;
	int height = WINDOW_HEIGHT;

	QWidget * desktop = QApplication::desktop();
	int desktopW = desktop->width();
	int desktopH = desktop->height();

	int x = (width <= desktopW)? ((desktopW - width)/2):0;
	int y = (height <= desktopH)?((desktopH - height)/2):0;

	width = (width <= desktopW)?width:desktopW;
	height = (height <= desktopH)?height:desktopH;

	window->setGeometry(x, y, width, height);

}

//setup a Mainwindow to show the GUI
int main(int argc, char *argv[]) {
	//Qt application must have exactly one object of class QApplication
	QApplication *gui = new QApplication(argc, argv); 
	
	main_window = new MainWindow();
	main_window->resize(850, 550);
    configureEnv(main_window);
	//main widget must show (default is hide).
	main_window->show();
	Progress::init(gui, main_window);
	gui->setMainWidget(main_window);

	splash_window = new SplashWindow();
	splash_window->show();
	
	//generate a random number use diff method in WIN32 or other OS
#ifndef WIN32
	srandom(time(NULL));    
#else
	srand(time(NULL));
#endif

	//show window
	return gui->exec();
}


//----------------------------------------------------------------------

/*#include "main.moc"*/

//----------------------------------------------------------------------
