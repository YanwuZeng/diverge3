#include <qapplication.h>
#include <qprogressdialog.h>

#include "progress.h"

//Progress class is used to show a progress bar while executing 
//a long time job. All methods are made static, which means these
//methods can be used directly without instantizing any object.

//initialize the static variables to be NULL
QApplication *Progress::gui_ = NULL;
QWidget *Progress::parent_ = NULL;
QProgressDialog *Progress::dialog_ = NULL;

//Init() method is used to specify the parent widget of progress bar.
//This method must be called before using Progress class.
void
Progress::init(QApplication *gui, QWidget *parent) {
  gui_ = gui;
  parent_ = parent;
}

//Method setup() is to configure the variables of progress bar. 
//These variables include the label text, total number of steps,
//and current step. Whenever users want to customize a new progress
//bar, they have to call setup() once.
void
Progress::setup(int cur, int nsteps, const char *str) {
  if(!dialog_) {
    dialog_ = new QProgressDialog(parent_, NULL, true);
    dialog_->setAutoReset(false);
  }
#ifdef WIN32
  dialog_->setCaption("Qt Diverge");
#else
  dialog_->setCaption("Diverge 2.0");
#endif
  dialog_->reset();
  dialog_->setLabelText(str);
  dialog_->setTotalSteps(nsteps);
  dialog_->setProgress(cur);
  gui_->processEvents();
}

/*set_label() is used to configure the label of progress bar.

INPUT:
  str: the new label name.
OUTPUT:
  NONE
*/
void
Progress::set_label(const char *str) {
  if(!dialog_) return;
  dialog_->setLabelText(str);
  gui_->processEvents();
}

/*progress() is used to check the current of progress of
progress bar.

INPUT:
  NONE
OUTPUT:
  The current progress of progress bar.
*/
int
Progress::progress() {
  if(!dialog_) return 0;
  return dialog_->progress();
}

/*set_progress() is to customize the current progress.

INPUT:
  progress: the progress value.
OUTPUT:
  NONE
*/
void
Progress::set_progress(int progress) {
  if(!dialog_) return;
  dialog_->setProgress(progress);
  gui_->processEvents();
}

/*increment() is to increase the current progress by inc.

INPUT:
  inc: The increment value
OUTPUT:
  NONE
*/
void
Progress::increment(int inc) {
  if(!dialog_) return;
  dialog_->setProgress(dialog_->progress() + inc);
  gui_->processEvents();
}

/*Method show() makes progress bar appear.

INPUT:
  NONE
OUTPUT:
  NONE
*/
void
Progress::show() {
  if(!dialog_) return;
  dialog_->show();
  gui_->processEvents();
}

/*Method hide() makes progress bar disappear. Note that
dialog_ will be assigned NULL. 

INPUT:
  NONE
OUTPUT:
  NONE
*/
void
Progress::hide() {
  delete dialog_;
  dialog_ = NULL;
}

/*Check whether current progress has been cancelled by users. 

INPUT:
  NONE
OUTPUT:
  NONE
*/
bool
Progress::wasCancelled() {
  bool rv = false;
  if(dialog_) {
    rv = dialog_->wasCancelled();
    if(rv) {
      dialog_->reset();
    }
  }
  return rv;
}
