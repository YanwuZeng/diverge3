#ifndef _PDB_VIEWER_H_
#define _PDB_VIEWER_H_

#include <string>
#include <list>
#include <vector>

#include <qgl.h>
#include <qcolor.h>
#include <qdialog.h>

#include "pdb.h"

class QPushButton;
class QTimer;

class PDBView;

class ChainView {
public:
  ChainView();
  ~ChainView();

  void update(const Chain &chain);
  
  QColor backbone_color() const;
  void set_backbone_color(const QColor &color);
  void set_random_color();

  void set_values(int point_mult_, double R,
		  int ball_slices_, int ball_stacks,
		  int tube_sides, bool draw_backbone, bool display_balls);
  void select(const std::list<std::string> &sel_res);
  void unselect_all();
  char id() const;
  
private:
  //ChainView(const ChainView&);
  //ChainView &operator = (const ChainView &);
  Chain chain_;

  bool display_backbone_;
  bool display_balls_;
  
  int point_mult_;
  int tube_sides_;
  int ball_slices_, ball_stacks_;
  double r_;
  
  char id_;
  
  GLfloat backbone_color_[4];

  GLuint backbone_;

  std::vector<int> selected_residues_;

  void init();
  void draw(int chain_id);

  static void lookupColor(char c, GLfloat color[4]);

  friend class PDBView;
};

class PDBView : public QGLWidget {
  Q_OBJECT
public:
  PDBView(QWidget *parent = NULL, const char *name = NULL);
  ~PDBView();

  bool load(const std::string &filename);
  bool load(const PDB &pdb);
  bool select(char chain_ID, const std::list<std::string> &res);
  void unselect_all();
  
  bool save_bitmap(const char *filename);

  bool get_display_labels()    const { return draw_labels_; }
  bool get_display_balls()     const { return display_balls_; }
  void set_display_labels(bool state);
  void set_display_balls(bool state);
  void set_wireframe(bool state);

  std::string name() const;
  
  void redraw(bool complete=false);

  QColor background_color() const;
  void set_background_color(const QColor &color);

  void reset();
  
  void PDBView::pick(int x, int y);

public slots:
  void reset_camera();
  void force_redraw();
  void rotateCamera();
  void panCamera();
  void zoomCamera();
  void enterEvent(QEvent*);
  void leaveEvent(QEvent*);

protected:
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();
  void mousePressEvent(QMouseEvent*);
  void mouseReleaseEvent(QMouseEvent*);
  void mouseMoveEvent(QMouseEvent*);
  void keyPressEvent(QKeyEvent*);

private:
  std::string name_;
  std::string filename_;
  bool draw_labels_;
  bool display_balls_;

  bool wireframe_;

  int point_mult_;
  int tube_sides_;
  int ball_slices_, ball_stacks_;
  double r_;
  
  GLfloat background_color_[4];
  
  std::vector<ChainView> chains_;

  double fov_, aspect_;
  double eye_[3];
  double center_[3];
  double lookup_[3];
  double right_[3];
  double diam_;
  
  int bb_chain_id_, bb_res_id_;
  
  QTimer *timer_;
  QPoint curPos;

  void processHits(GLint hits, GLuint buffer[]);
};

class PDBViewer : public QDialog {
  Q_OBJECT
public:
  PDBViewer(QWidget *parent = NULL);
  
  bool load(const std::string &filename);
  bool load(const PDB &pdb);
  bool select(char chain_ID, const std::list<std::string> &sel_res);
  void unselect_all();

protected:
  PDBView *view_;
  QPushButton *ok_;
  
  virtual void resizeEvent(QResizeEvent *);
};

bool pdb_load(const std::string &filename);
bool pdb_load(const PDB &pdb);
bool pdb_select(char chain_ID, const std::string &sel_res);
bool pdb_select(char chain_ID, const std::list<std::string> &sel_res);
bool pdb_select(char chain_ID, const std::vector<std::string> &sel_res);
void pdb_unselect_all();

#endif
