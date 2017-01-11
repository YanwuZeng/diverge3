#include <string>
#include <list>
#include <vector>

#include <stdio.h>
#include <stdlib.h>

#ifndef WIN32
#include <values.h>
#else
#include <float.h>
#endif

#include <qgl.h>
#include <qpixmap.h>
#include <qtimer.h>
#include <qdialog.h>
#include <qpushbutton.h>

#include "pdb.h"
#include "pdb_viewer.h"
#include "graphics_utils.h"

//----------------------------------------------------------------------

static char const * const rcsid_pdb_viewer_cpp = "$Id: pdb_viewer.cpp,v 1.1 2004/01/24 23:26:34 huangwei Exp $";

//----------------------------------------------------------------------

using namespace std;

//----------------------------------------------------------------------

ChainView::ChainView()
  : display_backbone_(true),
    display_balls_(true),

    point_mult_(6),
    tube_sides_(8),
    ball_slices_(14),
    ball_stacks_(8),
    r_(1.0),
    
    id_('_')
{
  set_backbone_color(QColor(170, 170, 170));
}

ChainView::~ChainView() {
}

void
ChainView::init() {
  backbone_ = glGenLists(1);
  glNewList(backbone_, GL_COMPILE);
  {
    int n = chain_.residues.size();
    if(n > 2) {
      int i;
      double *pts = new double[n*3];
      
      for(i=0; i<n; i++) {
	pts[i*3  ] = chain_.residues[i].x;
	pts[i*3+1] = chain_.residues[i].y;
	pts[i*3+2] = chain_.residues[i].z;
      }

#define SQR(x) ( (x) * (x) )  

      double px = pts[0*3 + 0], py = pts[0*3 + 1], pz = pts[0*3 + 2];
      int si = 0;
      for(i=1; i<n; i++) {
	double x = pts[i*3 + 0];
	double y = pts[i*3 + 1];
	double z = pts[i*3 + 2];
	if(SQR(x - px) + SQR(y - py) + SQR(z - pz) > SQR(4.5)) {
	  drawTube(pts+si*3, (i-1)-si+1, point_mult_, r_);
	  si = i;
	}
	px = x;
	py = y;
	pz = z;
      }

      drawTube(pts+si*3, (i-1)-si+1, point_mult_, r_);
      
      delete[] pts;
    }
  }
  glEndList();  
}

void
ChainView::draw(int chain_id) {
  if(display_balls_) {
    GLUquadricObj *qobj = gluNewQuadric();
    
    gluQuadricDrawStyle(qobj, GLU_FILL);
    gluQuadricNormals(qobj, GLU_SMOOTH);
    
    size_t r;
    for(r=0; r<chain_.residues.size(); r++) {
      Residue res = chain_.residues[r];
      GLfloat color[4] = { 0.0, 0.0, 0.0, 1.0 };
      lookupColor(res.type, color);
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
      
      glPushMatrix();
      glPushName(((chain_id & 0x0000ffff) << 16) | (r & 0x0000ffff));
      glTranslatef(res.x, res.y, res.z);
      gluSphere(qobj, 4*r_, ball_slices_, ball_stacks_);
      glPopName();
      glPopMatrix();
    }
  }

  if(!selected_residues_.empty()) {
    GLUquadricObj *qobj = gluNewQuadric();
    
    gluQuadricDrawStyle(qobj, GLU_FILL);
    gluQuadricNormals(qobj, GLU_SMOOTH);
    
    size_t i, r;
    for(i=0; i<selected_residues_.size(); i++) {
      r = selected_residues_[i];
      Residue res = chain_.residues[r];
      GLfloat color[4] = { 0.0, 0.0, 0.0, 1.0 };
      lookupColor(res.type, color);
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
      
      glPushMatrix();
      glPushName(((chain_id & 0x0000ffff) << 16) | (r & 0x0000ffff));
      glTranslatef(res.x, res.y, res.z);
      gluSphere(qobj, 4*r_, ball_slices_, ball_stacks_);
      glPopName();
      glPopMatrix();
    }
  }
  
  if(display_backbone_) {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, backbone_color_);
    glCallList(backbone_);
  }

#if 0
  if(bb_id_ != -1) {
    drawBoundingBox(ca_list_[bb_id_].x, ca_list_[bb_id_].y, ca_list_[bb_id_].z, .75);
  }
#endif
}

void
ChainView::lookupColor(char c, GLfloat color[4]) {
  color[3] = 1.0;
  switch(c) {
  case 'D': // ASP
  case 'E': // GLU
    color[0] = 230.0f/255.0f;
    color[1] =  10.0f/255.0f;
    color[2] =  10.0f/255.0f;
    break;
  case 'K': // LYS
  case 'R': // ARG
    color[0] =  20.0f/255.0f;
    color[1] =  90.0f/255.0f;
    color[2] = 255.0f/255.0f;
    break;
  case 'F': // PHE
  case 'Y': // TYR
    color[0] =  50.0f/255.0f;
    color[1] =  50.0f/255.0f;
    color[2] = 170.0f/255.0f;
    break;
  case 'G': // GLY
    color[0] = 235.0f/255.0f;
    color[1] = 235.0f/255.0f;
    color[2] = 235.0f/255.0f;
    break;
  case 'A': // ALA
    color[0] = 200.0f/255.0f;
    color[1] = 200.0f/255.0f;
    color[2] = 200.0f/255.0f;
    break;
  case 'H': // HIS
    color[0] = 130.0f/255.0f;
    color[1] = 130.0f/255.0f;
    color[2] = 210.0f/255.0f;
    break;
  case 'C': // CYS
  case 'M': // MET
    color[0] = 230.0f/255.0f;
    color[1] = 230.0f/255.0f;
    color[2] =   0.0f/255.0f;
    break;
  case 'S': // SER
  case 'T': // THR
    color[0] = 250.0f/255.0f;
    color[1] = 150.0f/255.0f;
    color[2] =   0.0f/255.0f;
    break;
  case 'N': // ASN
  case 'Q': // GLN
    color[0] =   0.0f/255.0f;
    color[1] = 220.0f/255.0f;
    color[2] = 220.0f/255.0f;
    break;
  case 'L': // LEU
  case 'V': // VAL
  case 'I': // ILE
    color[0] =  15.0f/255.0f;
    color[1] = 130.0f/255.0f;
    color[2] =  15.0f/255.0f;
    break;
  case 'W': // TRP
    color[0] = 180.0f/255.0f;
    color[1] =  90.0f/255.0f;
    color[2] = 180.0f/255.0f;
    break;
  case 'P': // PRO
    color[0] = 220.0f/255.0f;
    color[1] = 150.0f/255.0f;
    color[2] = 130.0f/255.0f;
    break;
  default:
    color[0] = 190.0f/255.0f;
    color[1] = 160.0f/255.0f;
    color[2] = 110.0f/255.0f;
    break;
  };
}

char
ChainView::id() const {
  return chain_.name;
}

void
ChainView::update(const Chain &chain) {
  chain_ = chain;
  selected_residues_.clear();  
}

QColor
ChainView::backbone_color() const {
  return QColor(int(backbone_color_[0] * 255 + .5),
		int(backbone_color_[1] * 255 + .5),
		int(backbone_color_[2] * 255 + .5));
}

void
ChainView::set_backbone_color(const QColor &color) {
  backbone_color_[0] = color.red()   / 255.0;
  backbone_color_[1] = color.green() / 255.0;
  backbone_color_[2] = color.blue()  / 255.0;
  backbone_color_[3] = 1.0;
}

void
ChainView::set_random_color() {
#ifdef WIN32
  backbone_color_[0] = rand() / double(RAND_MAX);
  backbone_color_[1] = rand() / double(RAND_MAX);
  backbone_color_[2] = rand() / double(RAND_MAX);
#else
  backbone_color_[0] = random() / double(RAND_MAX);
  backbone_color_[1] = random() / double(RAND_MAX);
  backbone_color_[2] = random() / double(RAND_MAX);
#endif
  backbone_color_[3] = 1.0;
}

void
ChainView::set_values(int point_mult, double R,
		      int ball_slices, int ball_stacks,
		      int tube_sides,
		      bool display_backbone, bool display_balls) {
  point_mult_ = point_mult;
  r_ = R;
  ball_slices_ = ball_slices;
  ball_stacks_ = ball_stacks;
  tube_sides_ = tube_sides;
  display_backbone_ = display_backbone;
  display_balls_ = display_balls;

  init();
}

void
ChainView::select(const list<string> &sel_res) {
  selected_residues_.clear();
  list<string>::const_iterator i;
  int j;
  for(i = sel_res.begin(), j = 0; i != sel_res.end(); i++, j++) {
    int ind = chain_.find_res_index(*i);
    if(ind != -1) {
      selected_residues_.push_back(ind);
    }
  }
}

void
ChainView::unselect_all() {
  selected_residues_.clear();
}

PDBView::PDBView(QWidget *parent, const char *name)
  : QGLWidget(parent, name),
    draw_labels_(false),
    display_balls_(false),

    wireframe_(false),
    
    point_mult_(6),
    tube_sides_(8),
    ball_slices_(14),
    ball_stacks_(8),

    fov_(45),
    aspect_(1.0),
    diam_(1.0),
    r_(.20),
    
    bb_chain_id_(-1),
    bb_res_id_(-1)
{
  setBackgroundMode(NoBackground);

  set_background_color(QColor(0, 0, 0));

  reset();

  timer_ = new QTimer(this);
}

PDBView::~PDBView() {
}

void
PDBView::initializeGL() {
  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  //glDepthFunc(GL_LEQUAL);
  //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  //glShadeModel(GL_SMOOTH);

  size_t i;
  for(i=0; i<chains_.size(); i++) {
    chains_[i].init();
    chains_[i].set_values(point_mult_, r_, ball_slices_, ball_stacks_,
			  tube_sides_, true, display_balls_);
  }
}

void
PDBView::resizeGL(int width, int height) {
  aspect_ = width/(double)height;
  
  glViewport(0, 0, width, height);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fov_, aspect_, .25*diam_, 1.0+3.0*diam_);

  glMatrixMode(GL_MODELVIEW);
}

void
PDBView::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  size_t i;
  for(i=0; i<chains_.size(); i++) {
    chains_[i].draw(i);
  }

  if(bb_chain_id_ != -1 && bb_res_id_ != -1) {
    drawBoundingBox((GLfloat)chains_[bb_chain_id_].chain_.residues[bb_res_id_].x,
		    (GLfloat)chains_[bb_chain_id_].chain_.residues[bb_res_id_].y,
		    (GLfloat)chains_[bb_chain_id_].chain_.residues[bb_res_id_].z,
		    (GLfloat)r_*4);
  }

  glFlush();
}

bool
PDBView::load(const string &filename) {
  if(filename_ != filename) {
    name_ = "";
    PDB tmp;
    if(!tmp.load(filename)) {
      return false;
    }
    return load(tmp);
  }
  
  return true;
}

bool
PDBView::load(const PDB &pdb) {
  name_ = pdb.name;
  filename_ = pdb.filename;
  chains_.clear();
  size_t n = 0, i, j;
  for(i=0; i<pdb.chains.size(); i++) {
    if(!pdb.chains[i].residues.empty()) {
      n++;
    }
  }
  chains_.resize(n);
  for(i=0, j=0; i<pdb.chains.size(); i++) {
    if(!pdb.chains[i].residues.empty()) {
      chains_[j].update(pdb.chains[i]);
      chains_[j].set_random_color();
      j++;
    }
  }

  reset();

  return true;
}

bool
PDBView::select(char chain_ID, const list<string> &sel_res) {
  size_t i;

  for(i=0; i<chains_.size(); i++) {
    if(chains_[i].id() == chain_ID) {
      chains_[i].select(sel_res);
      break;
    }
  }

  if(i == chains_.size()) return false;

  redraw();

  return true;
}

void
PDBView::unselect_all() {
  for(size_t i=0; i<chains_.size(); i++) {
    chains_[i].unselect_all();
  }
  redraw();
}

void
PDBView::reset() {
  initializeGL();
  reset_camera();
  initializeGL();
}

void
PDBView::redraw(bool complete) {
  updateGL();
}

void
PDBView::reset_camera() {
  fov_ = 45;
  
  double avg_x = 0.0, avg_y = 0.0, avg_z = 0.0;
  double min_x = DBL_MAX, min_y = DBL_MAX, min_z = DBL_MAX;
  double max_x = -DBL_MAX, max_y = -DBL_MAX, max_z = -DBL_MAX;
  size_t i, r, n = 0;
  for(i=0; i<chains_.size(); i++) {
    for(r=0; r<chains_[i].chain_.residues.size(); r++) {
      double x = chains_[i].chain_.residues[r].x;
      double y = chains_[i].chain_.residues[r].y;
      double z = chains_[i].chain_.residues[r].z;
    
      avg_x += x;
      avg_y += y;
      avg_z += z;
      if(x < min_x) min_x = x;
      if(y < min_y) min_y = y;
      if(z < min_z) min_z = z;
      if(x > max_x) max_x = x;
      if(y > max_y) max_y = y;
      if(z > max_z) max_z = z;
    }
    n += chains_[i].chain_.residues.size();
  }
  if(n != 0) {
    avg_x /= (double)n;
    avg_y /= (double)n;
    avg_z /= (double)n;
  }

  diam_ = 0.0;
  for(i=0; i<chains_.size(); i++) {
    size_t r;
    for(r=0; r<chains_[i].chain_.residues.size(); r++) {
      double dx = chains_[i].chain_.residues[r].x - avg_x;
      double dy = chains_[i].chain_.residues[r].y - avg_y;
      double dz = chains_[i].chain_.residues[r].z - avg_z;
      double d = dx*dx + dy*dy + dz*dz;
      if(d > diam_) diam_ = d;
    }
  }
  diam_ = 2.0*sqrt(diam_);
  
#if 1
  for(i=0; i<chains_.size(); i++) {
    size_t r;
    for(r=0; r<chains_[i].chain_.residues.size(); r++) {
      chains_[i].chain_.residues[r].x -= avg_x;
      chains_[i].chain_.residues[r].y -= avg_y;
      chains_[i].chain_.residues[r].z -= avg_z;
    }
  }
  avg_x = 0.0;
  avg_y = 0.0;
  avg_z = 0.0;
#endif
#if 1
  eye_[0] = avg_x;
  eye_[1] = avg_y;
  eye_[2] = avg_z - 1.75*diam_;
#else
  eye_[0] = 0;
  eye_[1] = 0;
  eye_[2] = 0 - 1.75*diam_;
#endif
  
#if 1
  center_[0] = avg_x;
  center_[1] = avg_y;
  center_[2] = avg_z;
#else
  center_[0] = 0;
  center_[1] = 0;
  center_[2] = 0;
#endif
  
  lookup_[0] = 0;
  lookup_[1] = 1;
  lookup_[2] = 0;

  right_[0] = 1;
  right_[1] = 0;
  right_[2] = 0;
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fov_, aspect_, .25*diam_, 1.0+3.0*diam_);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(eye_[0], eye_[1], eye_[2],
	    center_[0], center_[1], center_[2],
	    lookup_[0], lookup_[1], lookup_[2]);
  
  updateGL();
}

void
PDBView::force_redraw() {
  redraw(true);
}

void
PDBView::set_display_labels(bool state) {
  draw_labels_ = state;
  redraw();
}

void
PDBView::set_display_balls(bool state) {
  display_balls_ = state;
  redraw();
}

bool
PDBView::save_bitmap(const char *filename) {
  QPixmap pix = renderPixmap();
  return pix.save(filename, "PNG", 0);
}

QColor
PDBView::background_color() const {
  return QColor(int(background_color_[0] * 255 + .5),
		int(background_color_[1] * 255 + .5),
		int(background_color_[2] * 255 + .5));
}

void
PDBView::set_background_color(const QColor &color) {
  background_color_[0] = color.red()   / 255.0;
  background_color_[1] = color.green() / 255.0;
  background_color_[2] = color.blue()  / 255.0;
  makeCurrent();
  glClearColor(background_color_[0], background_color_[1],
	       background_color_[2], background_color_[3]);
  redraw();
}

string
PDBView::name() const {
  return name_;
}

void
PDBView::mousePressEvent(QMouseEvent *e) {
  if(e->button() & LeftButton) {
    connect(timer_, SIGNAL(timeout()), SLOT(rotateCamera()));
  }
  if(e->button() & RightButton) {
    connect(timer_, SIGNAL(timeout()), SLOT(panCamera()));
  }
  if(e->button() & MidButton) {
    connect(timer_, SIGNAL(timeout()), SLOT(zoomCamera()));
  }
  curPos = e->pos();
  //timer_->start(25);
  timer_->start(0);
}

void
PDBView::mouseReleaseEvent(QMouseEvent *e) {
  if(e->stateAfter() == NoButton) {
    timer_->stop();
    disconnect(timer_, NULL, this, NULL);
  } else {
    if(e->button() & LeftButton) {
      disconnect(timer_, NULL, this, SLOT(rotateCamera()));
    }
    if(e->button() & RightButton) {
      disconnect(timer_, NULL, this, SLOT(panCamera()));
    }
    if(e->button() & MidButton) {
      disconnect(timer_, NULL, this, SLOT(zoomCamera()));
    }
  }
}

void
PDBView::mouseMoveEvent(QMouseEvent *e) {
  curPos = e->pos();
}

void
PDBView::rotateCamera() {
  makeCurrent();
  float px = (width()/2.0 - curPos.x()) / (width()/2.0);
  float py = (height()/2.0 - curPos.y()) / (height()/2.0);
  clamp(px, -1.0, 1.0);
  clamp(py, -1.0, 1.0);
  px *= 2 * PI / 32.0;
  py *= 2 * PI / 32.0;
  eye_[0] -= center_[0];
  eye_[1] -= center_[1];
  eye_[2] -= center_[2];
  normalize(eye_);
  normalize(right_);
  normalize(lookup_);
  {
    double c = cos(px), s = sin(px);
    double x = lookup_[0], y = lookup_[1], z = lookup_[2];
    double mat[9] = { x*x*(1-c)+c,   x*y*(1-c)-z*s, x*z*(1-c)+y*s,
		      y*x*(1-c)+z*s, y*y*(1-c)+c,   y*z*(1-c)-x*s,
		      x*z*(1-c)-y*s, y*z*(1-c)+x*s, z*z*(1-c)+c   };
    multiply(mat, eye_);
    multiply(mat, right_);
  }
  {
    double c = cos(-py), s = sin(-py);
    double x = right_[0], y = right_[1], z = right_[2];
    double mat[9] = { x*x*(1-c)+c,   x*y*(1-c)-z*s, x*z*(1-c)+y*s,
		      y*x*(1-c)+z*s, y*y*(1-c)+c,   y*z*(1-c)-x*s,
		      x*z*(1-c)-y*s, y*z*(1-c)+x*s, z*z*(1-c)+c   };
    multiply(mat, eye_);
    multiply(mat, lookup_);
  }
  eye_[0] *= 1.75*diam_;
  eye_[1] *= 1.75*diam_;
  eye_[2] *= 1.75*diam_;
  eye_[0] += center_[0];
  eye_[1] += center_[1];
  eye_[2] += center_[2];
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(eye_[0], eye_[1], eye_[2],
	    center_[0], center_[1], center_[2],
	    lookup_[0], lookup_[1], lookup_[2]);
  updateGL();
}

void
PDBView::panCamera() {
  makeCurrent();
  float px = (width()/2.0 - curPos.x()) / (width()/2.0);
  float py = (height()/2.0 - curPos.y()) / (height()/2.0);
  clamp(px, -1.0, 1.0);
  clamp(py, -1.0, 1.0);
  double off[3] = { px * right_[0] + py * lookup_[0],
		    px * right_[1] + py * lookup_[1],
		    px * right_[2] + py * lookup_[2] };
  eye_[0] -= off[0];
  eye_[1] -= off[1];
  eye_[2] -= off[2];
  center_[0] -= off[0];
  center_[1] -= off[1];
  center_[2] -= off[2];
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(eye_[0], eye_[1], eye_[2],
	    center_[0], center_[1], center_[2],
	    lookup_[0], lookup_[1], lookup_[2]);
  updateGL();
}

void
PDBView::zoomCamera() {
  makeCurrent();
  float py = (height()/2.0 - curPos.y()) / (height()/2.0);
  clamp(py, -1.0, 1.0);
  fov_ *= (1.0 + py/16.0);
  clamp(fov_, 1.0, 140.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fov_, aspect_, .25*diam_, 1.0+3.0*diam_);
  glMatrixMode(GL_MODELVIEW);
  updateGL();
}

void
PDBView::keyPressEvent(QKeyEvent *e) {
  char c = (char)e->ascii();
  if(c == 'p') {
    pick(curPos.x(), curPos.y());
  } else if(c == 'w') {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    updateGL();
  } else if(c == 's') {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    updateGL();
  } else if(c == 'r') {
    reset_camera();
  } else {
    e->ignore();
  }
}

void
PDBView::enterEvent(QEvent*) {
  setFocus();
  setMouseTracking(true);
}

void
PDBView::leaveEvent(QEvent*) {
  clearFocus();
  setMouseTracking(false);
}

#define BUFSIZE 512

void
PDBView::pick(int x, int y) {
  makeCurrent();

  GLuint selectBuf[BUFSIZE];
  GLint hits;
  GLint viewport[4];

  glGetIntegerv(GL_VIEWPORT, viewport);

  glSelectBuffer(BUFSIZE, selectBuf);
  glRenderMode(GL_SELECT);
  glInitNames();
  
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluPickMatrix((GLdouble)x, (GLdouble) (viewport[3] - y),
		5.0, 5.0, viewport);
  gluPerspective(fov_, aspect_, .25*diam_, 1.0+3.0*diam_);
  
  glMatrixMode(GL_MODELVIEW);
  updateGL();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glFlush();

  hits = glRenderMode(GL_RENDER);
  processHits(hits, selectBuf);
  glMatrixMode(GL_MODELVIEW);
  if(bb_chain_id_ != -1 && bb_res_id_ != -1) {
    double x = chains_[bb_chain_id_].chain_.residues[bb_res_id_].x;
    double y = chains_[bb_chain_id_].chain_.residues[bb_res_id_].y;
    double z = chains_[bb_chain_id_].chain_.residues[bb_res_id_].z;
    eye_[0] += x - center_[0];
    eye_[1] += y - center_[1];
    eye_[2] += z - center_[2];
    center_[0] = x;
    center_[1] = y;
    center_[2] = z;
    glLoadIdentity();
    gluLookAt(eye_[0], eye_[1], eye_[2],
	      center_[0], center_[1], center_[2],
	      lookup_[0], lookup_[1], lookup_[2]);
  }
  updateGL();
}

void
PDBView::processHits(GLint hits, GLuint buffer[]) {
  GLint i, j;
  GLuint names, *ptr;

  bb_chain_id_ = -1;
  bb_res_id_ = -1;
  
  int bb_id = -1;
  double min_z = 0;
  
  ptr = (GLuint*) buffer;
  for(i=0; i<hits; i++) {
    names = *ptr; ptr++;
    float z1 = ((float) *ptr)/0xffffffff; ptr++;
    ptr++;
    for(j=0; j<names; j++) {
      if(j == 0) {
	if(bb_id == -1 || min_z > z1) {
	  bb_id = *ptr;
	  min_z = z1;
	  bb_chain_id_ = (bb_id & 0xffff0000) >> 16;
	  bb_res_id_   = (bb_id & 0x0000ffff);
	}
      }
      ptr++;
    }
  }
}   

PDBViewer::PDBViewer(QWidget *parent)
  : QDialog(parent, NULL)
{
  // Workaround for a Qt bug
  move(0, 0);

  setMinimumSize(400, 400);
  setCaption("PDB Viewer");

  view_ = new PDBView(this);

  ok_ = new QPushButton("OK", this);
  connect(ok_, SIGNAL(clicked()), SLOT(accept()));
}

bool
PDBViewer::load(const string &filename) {
  if(!view_->load(filename)) {
    setCaption("PDB Viewer");
    return false;
  }
  string str = string("PDB Viewer - ") + view_->name();
  setCaption(str.c_str());
  return true;
}

bool
PDBViewer::load(const PDB &pdb) {
  if(!view_->load(pdb)) {
    setCaption("PDB Viewer");
    return false;
  }
  string str = string("PDB Viewer - ") + view_->name();
  setCaption(str.c_str());
  return true;
}

bool
PDBViewer::select(char chain_ID, const list<string> &sel_res) {
  return view_->select(chain_ID, sel_res);
}

void
PDBViewer::unselect_all() {
  view_->unselect_all();
}

void
PDBViewer::resizeEvent(QResizeEvent *e) {
  int w = e->size().width();
  int h = e->size().height();
  
  view_->setGeometry(10, 10, w-20, h-20-10-30);
  ok_->setGeometry((w-100)/2, h-10-30, 100, 30);
}

static PDBViewer *pdb_viewer = NULL;

bool
pdb_load(const string &filename) {
  if(!pdb_viewer) {
    pdb_viewer = new PDBViewer;
  }
  bool rv = pdb_viewer->load(filename);
  pdb_viewer->show();
  return rv;
}

bool
pdb_load(const PDB &pdb) {
  if(!pdb_viewer) {
    pdb_viewer = new PDBViewer;
  }
  bool rv = pdb_viewer->load(pdb);
  pdb_viewer->show();
  return rv;
}

bool
pdb_select(char chain_ID, const string &sel_res) {
  list<string> tmp;
  tmp.push_back(sel_res);
  return pdb_select(chain_ID, tmp);
}

bool
pdb_select(char chain_ID, const list<string> &sel_res) {
  if(!pdb_viewer) {
    pdb_viewer = new PDBViewer;
  }
  bool rv = pdb_viewer->select(chain_ID, sel_res);
  
  pdb_viewer->show();
  return rv;
}

bool
pdb_select(char chain_ID, const vector<string> &sel_res) {
  list<string> sel_res_list;
  vector<string>::const_iterator i;
  for(i=sel_res.begin(); i!=sel_res.end(); i++) {
    sel_res_list.push_back(*i);
  }
  return pdb_select(chain_ID, sel_res_list);
}

void
pdb_unselect_all() {
  if(!pdb_viewer) {
    pdb_viewer = new PDBViewer;
  }
  pdb_viewer->unselect_all();
  pdb_viewer->show();
}
