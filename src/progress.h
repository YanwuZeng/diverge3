#ifndef _PROGRESS_H_
#define _PROGRESS_H_

class QApplication;
class QProgressDialog;
class QWidget;

//Progress class is used to show a progress bar while executing 
//a long time job. All methods are made static, which means these
//methods can be used directly without instantizing any object.

class Progress {
public:
  static void init(QApplication *gui, QWidget *parent);
  static void setup(int cur, int nsteps, const char *str);
  static void set_label(const char *str);
  static int  progress();
  static void set_progress(int cur);
  static void increment(int inc=1);
  static void show();
  static void hide();
  static bool wasCancelled();

private:
  static QApplication *gui_;
  static QWidget *parent_;
  static QProgressDialog *dialog_;
};

#endif
