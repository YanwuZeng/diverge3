#include <string>
#include <vector>
#include <algorithm>

#include <qscrollview.h>
#include <qapplication.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qdialog.h>
#include <qpushbutton.h>

#include <qpainter.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <minmax.h>
#endif

#include "alignment_view.h"

using namespace std;

SimpleAlignmentView::SimpleAlignmentView(QWidget *parent, const char *name, WFlags f)
  : QScrollView(parent, name, f),
    seq_max_len(0),
    char_max_w(0)
{
  viewport()->setBackgroundMode(NoBackground);
  setFocusPolicy(QWidget::StrongFocus);
  setHScrollBarMode(AlwaysOn);
  setVScrollBarMode(AlwaysOn);

  brushes[ 0] = QBrush(QColor(230,  10,  10));
  brushes[ 1] = QBrush(QColor( 20,  90, 255));
  brushes[ 2] = QBrush(QColor( 50,  50, 170));
  brushes[ 3] = QBrush(QColor(235, 235, 235));
  brushes[ 4] = QBrush(QColor(200, 200, 200));
  brushes[ 5] = QBrush(QColor(130, 130, 210));
  brushes[ 6] = QBrush(QColor(230, 230,   0));
  brushes[ 7] = QBrush(QColor(250, 150,   0));
  brushes[ 8] = QBrush(QColor(  0, 220, 220));
  brushes[ 9] = QBrush(QColor( 15, 130,  15));
  brushes[10] = QBrush(QColor(180,  90, 180));
  brushes[11] = QBrush(QColor(220, 150, 130));
  brushes[12] = QBrush(QColor(190, 160, 110));

  memset(cws, 0, sizeof(cws));

  QFontMetrics fm = viewport()->fontMetrics();
  for(int c=32; c<128; c++) {
    if(isprint(c)) {
      if(!cws[c]) cws[c] = fm.width(c);
      char_max_w = max(char_max_w, cws[c]);
    }
  }
}

void
SimpleAlignmentView::clear() {
  vector<sequence_t> empty;
  set_alignment(empty);
}


void
SimpleAlignmentView::set_alignment(const vector<sequence_t> &sequences) {
  sequences_ = sequences;
  regions.clear();

  seq_max_len = 0;
  
  if(sequences_.empty()) {
    viewport()->update();
    return;
  }

  //the following code will get the seq_max_len, the maximal number of 
  //gene in sequence data
  size_t num = sequences_.size();
  seq_max_len = sequences_[0].sequence.size();
  for(size_t j=1; j<num; j++) {
    seq_max_len = max(seq_max_len, sequences_[j].sequence.size());
  }

  QString str;
  str.setNum(seq_max_len);
  
  //caculate the maximal width of sequence (unit: pixel);
  int seq_max_w = char_max_w * seq_max_len;


  //caculate the width and height for this view
  QFontMetrics fm = viewport()->fontMetrics();
  int req_width = 5 + seq_max_w + 5;
  int req_height = 5 + fm.height()*(num+1) + 5;

  //build up the summary info. out of all sequences
  //such info. will be displayed at the bottom of SimpleAlignmentView
  build_summary();

  //inform the QT to adjust the size of SimpleAlignmentView
  if(contentsWidth() != req_width || contentsHeight() != req_height) {
    resizeContents(req_width, req_height);
  }// else {
  //    viewport()->update();
  //  }
  //inform QT to update the view content.
  viewport()->update();
}

/*append_alignment() method will append a new sequence to the end of current sequence.

INPUT
  sequences: the new sequences to append;

OUTPUT
  NONE

MECHANISM
1. insert the new sequence to the end of current sequences;
2. call the set_alignment() method to redraw the new sequences;
*/
void
SimpleAlignmentView::append_alignment(const vector<sequence_t> &sequences) {
  sequences_.insert(sequences_.end(), sequences.begin(), sequences.end());
  set_alignment(sequences_);
}

/*highlight_region() method will highlight a portion of current SimpleAlignmentView.

INPUT
  start: the starting position of highlight region;
  len: the width of highlighted region;

OUTPUT
  NONE
*/
void
SimpleAlignmentView::highlight_region(int start, int len) {
  regions.push_back(make_pair(start, len));
  int mw = char_max_w;
  int x1 = 5 + mw*start;
  int x2 = 5 + mw*(start+len);
  ensureVisible(x2, contentsY(), 12, 5);
  ensureVisible(x1, contentsY(), 12, 5);
  viewport()->update();
}

/*A utility method to get the brush for corresponding character c;

INPUT
  c: the character queried;

OUTPUT
  A QBrush object which will be used to draw character c;

*/
QBrush &
SimpleAlignmentView::get_brush(char c) {
  int bi = 0;
  switch(toupper(c)) {
  case 'D': // ASP
  case 'E': // GLU
    bi = 0;
    break;
  case 'K': // LYS
  case 'R': // ARG
    bi = 1;
    break;
  case 'F': // PHE
  case 'Y': // TYR
    bi = 2;
    break;
  case 'G': // GLY
    bi = 3;
    break;
  case 'A': // ALA
    bi = 4;
    break;
  case 'H': // HIS
    bi = 5;
    break;
  case 'C': // CYS
  case 'M': // MET
    bi = 6;
    break;
  case 'S': // SER
  case 'T': // THR
    bi = 7;
    break;
  case 'N': // ASN
  case 'Q': // GLN
    bi = 8;
    break;
  case 'L': // LEU
  case 'V': // VAL
  case 'I': // ILE
    bi = 9;
    break;
  case 'W': // TRP
    bi = 10;
    break;
  case 'P': // PRO
    bi = 11;
    break;
  default:
    bi = 12;
    break;
  }

  return brushes[bi];
}

/*drawContent() is the major method used to draw the site info in
the SimpleAlignmentView. This method will be called whenever there is a need to
redraw the SimpleAlignmentView region.

INPUT
  p: the QPainter object used to draw on.
  cx, cy, cw, ch: the coordinate of repaint region.

OUTPUT
  NONE
*/
void
SimpleAlignmentView::drawContents(QPainter *p, int cx, int cy, int cw, int ch) {
  /*
    cx, cy - starting position needing update
    cw, ch - width and height of area needing update
  */
  
  p->fillRect(cx, cy, cw, ch, white);

  if(sequences_.empty()) return;
  
  QFontMetrics fm = viewport()->fontMetrics();
  
  int mw = char_max_w;

  size_t j;
  
  vector<sequence_t>::const_iterator i;
  for(j=1, i = sequences_.begin(); i != sequences_.end(); j++, i++) {
    int x = 5;
    int y = 5 + fm.height()*j;

    const char *s = i->sequence.c_str();
    for(size_t k=0; k<i->sequence.size(); k++, s++) {
      x = 5 + mw*k;
      y = 5 + fm.height()*j;
      
      if(x+mw < cx ||   // y is bottom of char
	 y < cy) continue;
      if(x > cx+cw ||
	 y-fm.height() > cy+ch) break;
      
      char c = *s;
      if(!cws[c]) cws[c] = fm.width(c);

      int char_w = cws[c];
      int char_dis = (mw - char_w) / 2;

      p->fillRect(x, y, mw, -fm.height(), get_brush(c));

      char str[2] = { c, '\0' };
      p->drawText(x+char_dis, y-2, str);
    }
  }

  {
    int num = sequences_.size();
    for(size_t k=0; k<summary.size(); k++) {
      char c = summary[k];
      if(c == ' ') continue;
      
      int x = 5 + mw*k;
      int y = 5 + fm.height()*(num+1);
      
      if(x+mw < cx ||   // y is bottom of char
	 y < cy) continue;
      if(x > cx+cw ||
	 y-fm.height() > cy+ch) break;
      
      if(!cws[c]) cws[c] = fm.width(c);

      int char_w = cws[c];
      int char_dis = (mw - char_w) / 2;

      p->fillRect(x, y, mw, -fm.height(), get_brush(sequences_[0].sequence[k]));

      char str[2] = { c, '\0' };
      p->drawText(x+char_dis, y-2, str);
    }
  }

  if(!regions.empty()) {
    QPen cp = p->pen();
    int pen_width = 3;
    cp.setWidth(pen_width);
    p->setPen(cp);
    vector<pair<int, int> >::const_iterator i;
    for(i = regions.begin(); i != regions.end(); i++) {
      int x = 5 + mw*i->first;
      int y = pen_width/2;
      int w = i->second * mw;
      int h = contentsHeight()-2;
      p->drawRect(x, y, w, h);
    }
  }
}

/*build_summary() method is used to summarize the characteristics of sequence data.
Basically, for a specific column, if the gene data of the whole column is the same,
then we will put a '*' in the summary information. Otherwise, a ' ' will be stored.
This summary info will be draw at the bottom of SimpleAlignmentView.

INPUT
  NONE

OUTPUT
  NONE
*/
void
SimpleAlignmentView::build_summary() {
  size_t num = sequences_.size();

  summary.resize(seq_max_len);

  for(size_t i=0; i<seq_max_len; i++) {
    size_t j = 0;

    char c = ' '; 

	if (sequences_[0].sequence.size() > 0)
	{
		c = toupper(sequences_[0].sequence[i]); 
	} 
	else if (sequences_[1].sequence.size() > 0)
	{
		c = toupper(sequences_[1].sequence[i]); 
	}
	else if (sequences_[2].sequence.size() > 0)
	{
		c = toupper(sequences_[2].sequence[i]); 
	}

    if(c == 'D' || c == 'E' || c == 'K' ||
       c == 'R' || c == 'F' || c == 'Y' ||
       c == 'G' || c == 'A' || c == 'H' ||
       c == 'C' || c == 'M' || c == 'S' ||
       c == 'T' || c == 'N' || c == 'Q' ||
       c == 'L' || c == 'V' || c == 'I' ||
       c == 'W' || c == 'P') {
      for(j=1; j<num; j++) {
	if(i >= sequences_[j].sequence.size()) break;
	if(c != toupper(sequences_[j].sequence[i])) break;
      }
    }
    summary[i] = (j==num) ? '*' : ' ';
  }
}

/*contensMousePressEvent() will be called whenever the mouse is pressed in
SimpleAlignmentView. This method does not do any specific work.

INPUT
  e: QMouseEvent passed from QT;

OUTPUT
  NONE
*/
void
SimpleAlignmentView::contentsMousePressEvent(QMouseEvent *) {
  setFocus();
}

/*keyPressEvent() will be called whenever the key is pressed in
SimpleAlignmentView. The method will scroll this ailgnment view according to the
type of key press (up, down, left, right).

INPUT
  e: QKeyEvent passed from QT;

OUTPUT
  NONE
*/
void
SimpleAlignmentView::keyPressEvent(QKeyEvent *e) {
  switch(e->key()) {
  case Key_Up:
    scrollBy(0, -visibleHeight()/5);
    break;
  case Key_Down:
    scrollBy(0, visibleHeight()/5);
    break;
  case Key_Left:
    scrollBy(-visibleWidth()/5, 0);
    break;
  case Key_Right:
    scrollBy(visibleWidth()/5, 0);
    break;
  case Key_PageUp:
    scrollBy(0, -visibleHeight());
    break;
  case Key_PageDown:
    scrollBy(0, visibleHeight());
    break;
  default:
    e->ignore();
  }
}

////////////////////////////////////////////////////////////////
//The LabelView Class
////////////////////////////////////////////////////////////////
/*The constructor of LabelView. This constructor only configure
some basic properties of LabelView.
INPUT
  parent: parent widget of LabelView class;
  name: name of this LabelView;
  f: flags;
OUTPUT
  NONE
*/
LabelView::LabelView(QWidget *parent, const char *name, WFlags f)
  : QScrollView(parent, name, f),
    selectable_(false),
    label_max_w(0),
    cb_size_(0)
{
  viewport()->setBackgroundMode(NoBackground);
  //configure the scrollbar mode
  setHScrollBarMode(AlwaysOn);
  setVScrollBarMode(AlwaysOff);
}

/*Clear the content of current LabelView.
INPUT
  NONE

OUTPUT
  NONE

MECHANISM
The approach to clean LabelView is pretty simple. Basically, just create a empty
sequence and call the set_alignment() method.
*/
void
LabelView::clear() {
  vector<sequence_t> empty;
  set_alignment(empty);
}

/*set_alignment method is called whenever users want to display a new sequence. 
This method will configure some parameters (label_max_w, etc.) and inform
QT that widget size has changed. The job of drawing labels is implemented 
in another method - drawContent().

INPUT
  sequences: the new sequences to draw;

OUTPUT
  NONE

MECHANISM
1. caculate the maximal width of sequence (label_max_w);
2. inform the QT to resize the view;
3. call the update() method to redraw the content;
*/

void
LabelView::set_alignment(const vector<sequence_t> &sequences) {
  sequences_ = sequences;
  regions.clear();

  //if the sequence is empty, return immediately
  if(sequences_.empty()) {
    viewport()->update();
    return;
  }

  //get the total number of sequence
  size_t num = sequences_.size();
  states_.resize(num);
  
  //caculate the maximal width of LabelView (label_max_w);
  QFontMetrics fm = viewport()->fontMetrics();
  for(size_t i = 0; i < num; i++) {
    QRect r = fm.boundingRect(sequences_[i].label.c_str());
    label_max_w = max(label_max_w, r.width() - r.x());
    states_[i] = false;
  }

  cb_size_ = fm.height()-2;

  //caculate the 
  int req_width = 5 + label_max_w + 5;
  if(selectable_) {
    req_width += cb_size_ + 5;
  }
  // Need to add a buffer space for the scrollbar else the alignment view and
  // this view will get out of sync. 100 should be plenty...
  int req_height = 5 + (fm.height() + 0)*(num+1) + 5 + 100;

  if(contentsWidth() != req_width || contentsHeight() != req_height) {
    resizeContents(req_width, req_height);
  }// else {
  //    viewport()->update();
  //  }
  viewport()->update();
}

/*append_alignment() method will append a new sequence to the end of current sequence.

INPUT
  sequences: the new sequences to append;

OUTPUT
  NONE

MECHANISM
1. insert the new sequence to the end of current sequences;
2. call the set_alignment() method to redraw the new sequences;
*/
void
LabelView::append_alignment(const vector<sequence_t> &sequences) {
  sequences_.insert(sequences_.end(), sequences.begin(), sequences.end());
  set_alignment(sequences_);
}

/*highlight_region() method will highlight a portion of current LabelView.

INPUT
  start: the starting position of highlight region;
  len: the width of highlighted region;

OUTPUT
  NONE
*/
void
LabelView::highlight_region(int start, int len) {
  regions.push_back(make_pair(start, len));
  viewport()->update();
}

/*This method will configure LabelView to be selectable (s);

INPUT
  s: the selectable option

OUTPUT
  NONE
*/

void
LabelView::setSelectable(bool s) {
  if(selectable_ != s) {
    selectable_ = s;
    set_alignment(sequences_); //redraw the sequence again
  }
}

/*selectable() method will return the selectable property of LabelView.

INPUT
  NONE

OUTPUT
  The selectable property of current LabelView.
*/

bool
LabelView::selectable() const {
  return selectable_;
}

/*getStates() method will return the concurent states of LabelView.
The state information indicates which sequence has been selected. If
selected,the corresponding state will become true;

INPUT
  states: the states of current sequences;

OUTPUT
  NONE
*/
void
LabelView::getStates(vector<pair<string, bool> > &states) const {
  if(selectable_) {
    size_t num = sequences_.size();
    states.resize(num);
    for(size_t i = 0; i < num; i++) {
      states[i] = make_pair(sequences_[i].label, states_[i]);
    }
  }
}

/*contensMousePressEvent() will be called whenever the mouse is pressed in
LabelView. This method record the current mouse position in variable mp_.

INPUT
  e: QMouseEvent passed from QT;

OUTPUT
  NONE
*/
void
LabelView::contentsMousePressEvent(QMouseEvent *e) {
  mp_ = e->pos();
}

/*contensMouseReleaseEvent() will be called whenever the mouse button is released
in LabelView. This method will caculate the sequence where mouse
is pressed and update the correspodning states.

INPUT
  e: QMouseEvent passed from QT;

OUTPUT
  NONE
*/
void
LabelView::contentsMouseReleaseEvent(QMouseEvent *e) {
  if(1 /*5 < mp_.x() && mp_.x() < 5 + cb_size_ &&
         5 <  e->x() &&  e->x() < 5 + cb_size_*/) {
    QFontMetrics fm = viewport()->fontMetrics();
    int n1 = (mp_.y() - 5) / fm.height();
    int n2 = ( e->y() - 5) / fm.height();
    if(n1 == n2 && 0 <= n1 && n1 < sequences_.size()) {
      states_[n1] = !states_[n1];
      viewport()->update();
    }
  }
}

/*drawContent() is the major method used to draw the site info in
the LabelView. This method will be called whenever there is a need to
redraw the LabelView region.

INPUT
  p: the QPainter object used to draw on.
  cx, cy, cw, ch: the coordinate of repaint region.

OUTPUT
  NONE
*/
void
LabelView::drawContents(QPainter *p, int cx, int cy, int cw, int ch) {
  /*
    cx, cy - starting position needing update
    cw, ch - width and height of area needing update
  */
  
  p->fillRect(cx, cy, cw, ch, white);

  if(sequences_.empty()) return;
  
  QFontMetrics fm = viewport()->fontMetrics();

  QPen pen = p->pen();
  pen.setWidth(2);
  pen.setCapStyle(RoundCap);
  p->setPen(pen);

  for(size_t i = 0; i < sequences_.size(); i++) {
    int x = 5;
    int y = 5 + fm.height()*(i+1);

    if(selectable_) {
      x += cb_size_ + 5;
    }
	
    if(!(x + label_max_w < cx ||   // y is bottom of char
	 y+2 < cy ||               // +2 is needed to catch drop downs on y's, g's, etc.
	 x > cx+cw ||
	 y-fm.height() > cy+ch)) {
      if(selectable_) {
	p->drawRect(5, y-1, cb_size_, -cb_size_);
	if(states_[i]) {
	  p->drawLine(5+1,          y-1 - cb_size_ / 2,
		      5+2+1,  y-1 - cb_size_ / 2 + 2);
	  p->drawLine(5+cb_size_-4, y-1 - cb_size_+4,
		      5+2+1,  y-1 - cb_size_ / 2 + 2);
	}
      }
      
      p->drawText(x, y-2, sequences_[i].label.c_str());
    }
  }
}

//////////////////////////////////////////////////////////////
//class of SiteView
//////////////////////////////////////////////////////////////
/*The constructor of SiteView. This constructor only configure
some basic properties of SiteView.
INPUT
  parent: parent widget of SiteView class;
  name: name of this SiteView;
  f: flags;
OUTPUT
  NONE
*/
SiteView::SiteView(QWidget *parent, const char *name, WFlags f)
  : QScrollView(parent, name, f),
    nheader(0),
    seq_max_len(0),
    char_max_w(0)
{
  viewport()->setBackgroundMode(NoBackground);
  setHScrollBarMode(AlwaysOff);
  setVScrollBarMode(AlwaysOn);

  //the following section store the width info. of all printable
  //characters. Also, at the end, the maximal width is stored in 
  //char_max_w.
  memset(cws, 0, sizeof(cws)); //clean the memory of cws
  QFontMetrics fm = viewport()->fontMetrics(); //get the system font metrics
  for(int c=32; c<128; c++) { //for all printable characters
    if(isprint(c)) { //if c is printable character
      if(!cws[c]) cws[c] = fm.width(c); //store the font width of c
      char_max_w = max(char_max_w, cws[c]);
    }
  }
}

/*Clear the content of current SiteView.
INPUT
  NONE

OUTPUT
  NONE

MECHANISM
The approach to clean SiteView is pretty simple. Basically, just create a empty
sequence and call the set_alignment() method.
*/
void
SiteView::clear() {
  vector<sequence_t> empty;
  set_alignment(empty);
}

/*set_alignment method is called whenever users want to display a new sequence. 
This method will configure some parameters (seq_max_len, seq_max_w, etc.) and inform
QT that widge size has changed. The job of drawing site strings is implemented 
in another method - drawContent().

INPUT
  sequences: the new sequences to draw;

OUTPUT
  NONE

MECHANISM
1. caculate the maximal length of sequence (seq_max_len);
2. caculate the maximal width of sequence (seq_max_w);
3. inform the QT to resize the view;
4. call the update() method to redraw the content;
*/
void
SiteView::set_alignment(const vector<sequence_t> &sequences) {
  sequences_ = sequences;
  regions.clear(); //clean the regions

  seq_max_len = 0;
  
  //if sequences is empty, return immediately;
  if(sequences_.empty()) {
    viewport()->update();
    return;
  }

  //caculate the maximal length of sequence;
  size_t num = sequences_.size();
  seq_max_len = sequences_[0].sequence.size();
  for(size_t j=1; j<num; j++) {
    seq_max_len = max(seq_max_len, sequences_[j].sequence.size());
  }


  QString str;
  str.setNum(seq_max_len);
  nheader = str.length();
  
  //caculate the maximal width of gene sequence (in terms of pixel);
  QFontMetrics fm = viewport()->fontMetrics();
  int seq_max_w = char_max_w * seq_max_len;

  // Need to add a buffer space for the scrollbar else the alignment view and
  // this view will get out of sync. 100 should be plenty...
  int req_width = 5 + seq_max_w + 5 + 100;
  int req_height = 2 + fm.ascent()*nheader + 1;

  //inform QT to resize the view;
  if(contentsWidth() != req_width || contentsHeight() != req_height) {
    resizeContents(req_width, req_height);
  }// else {
  //    viewport()->update();
  //  }
  //the following function call (update()) will call drawContent eventually.
  viewport()->update();
}

/*append_alignment() method will append a new sequence to the end of current sequence.

INPUT
  sequences: the new sequences to append;

OUTPUT
  NONE

MECHANISM
1. insert the new sequence to the end of current sequences;
2. call the set_alignment() method to redraw the new sequences;
*/
void
SiteView::append_alignment(const vector<sequence_t> &sequences) {
  sequences_.insert(sequences_.end(), sequences.begin(), sequences.end());
  set_alignment(sequences_);
}

/*highlight_region() method will highlight a portion of current siteView.

INPUT
  start: the starting position of highlight region;
  len: the width of highlighted region;

OUTPUT
  NONE
*/
void
SiteView::highlight_region(int start, int len) {
  regions.push_back(make_pair(start, len));
  viewport()->update();
}

/*drawContent() is the major method used to draw the site info in
the SiteView. This method will be called whenever there is a need to
redraw the SiteView region.

INPUT
  p: the QPainter object used to draw on.
  cx, cy, cw, ch: the coordinate of repaint region.

OUTPUT
  NONE
*/
void
SiteView::drawContents(QPainter *p, int cx, int cy, int cw, int ch) {
  /*
    cx, cy - starting position needing update
    cw, ch - width and height of area needing update
  */
  
  p->fillRect(cx, cy, cw, ch, white);

  if(sequences_.empty()) return;
  
  QFontMetrics fm = viewport()->fontMetrics();
  
  int mw = char_max_w;

  if(!regions.empty()) {
    p->setBrush(lightGray);
    QPen cp = p->pen();
    QPen sp = cp;
    cp.setWidth(3);
    p->setPen(cp);
    vector<pair<int, int> >::const_iterator i;
    for(i = regions.begin(); i != regions.end(); i++) {
      int x = 5 + mw*i->first;
      int y = 0;
      int w = i->second * mw;
      int h = contentsHeight();
      p->drawRect(x, y, w, h);
    }
    p->setPen(sp);
  }

  size_t j;
  char fmt[16];
  sprintf(fmt, "%%%dd", nheader);
  for(j=0; j<seq_max_len; j++) {
    char str[16];
    sprintf(str, fmt, j+1);
    int x = 5 + mw*j;
    if(x+mw < cx) continue;
    if(x > cx+cw) break;
    for(int k=0; k<nheader; k++) {
      int y = 2 + fm.ascent()*(k+1);
      
      if(y < cy) continue;
      if(y-fm.height() > cy+ch) break;

      char c = str[k];
      if(!cws[c]) cws[c] = fm.width(c);

      int char_w = cws[c];
      int char_dis = (mw - char_w) / 2;
      
      char str[2] = { c, '\0' };

      p->drawText(x+char_dis, y-2, str);
    }
  }  
}

////////////////////////////////////////////////
//class of AlignmentView
////////////////////////////////////////////////

/*Constructor of AlignmentView. 
INPUT:
  parent: the parent widget of AlignmentView;
  name: the name of current AlignmentView;
  f: the flag indicator (passed to parent widget).
OUTPUT:
  NONE
*/
AlignmentView::AlignmentView(QWidget *parent, const char *name, WFlags f)
  : QWidget(parent, name, f),
    last_x_(0), last_y_(0)
{
	/*The focus polify is set to be StrongFocus, which means the widget accepts
	focus by both tabbing and clicking*/
  setFocusPolicy(QWidget::StrongFocus);
  
  /*The size policy is set to be expanding on both directions.*/
  setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
  
  //New the objects for three major components
  alignment_view_ = new SimpleAlignmentView(this);
  label_view_ = new LabelView(this);
  site_view_ = new SiteView(this);

  //connect the signals with the event handlers. The signals are 
  //contentMoving.
  connect(alignment_view_, SIGNAL(contentsMoving(int, int)), SLOT(alignmentViewMoving(int, int)));
  connect(label_view_, SIGNAL(contentsMoving(int, int)), SLOT(labelViewMoving(int, int)));
  connect(site_view_, SIGNAL(contentsMoving(int, int)), SLOT(siteViewMoving(int, int)));
}

/* Clean() is used to clean the content of AlignmentView. Internally, this function
call will clear three components respectively.

INPUT:
  NONE
OUTPUT:
  NONE
*/
void
AlignmentView::clear() {
  alignment_view_->clear();
  label_view_->clear();
  site_view_->clear();
  update_sizes();
}

/* set_alignment() is to set a new sequence for AlignmentView.

INPUT:
  sequence: the new sequence to be displayed in AlignmentView.
OUTPUT:
  NONE
*/
void
AlignmentView::set_alignment(const vector<sequence_t> &sequences) {
  alignment_view_->set_alignment(sequences);
  label_view_->set_alignment(sequences);
  site_view_->set_alignment(sequences);
  update_sizes();
}

/* set_alignment() is to append a new sequence to the end of current 
sequence in AlignmentView.

INPUT:
  sequence: the new sequence to be appended in AlignmentView.
OUTPUT:
  NONE
*/
void
AlignmentView::append_alignment(const vector<sequence_t> &sequences) {
  alignment_view_->append_alignment(sequences);
  label_view_->append_alignment(sequences);
  site_view_->append_alignment(sequences);
  update_sizes();
}

/* highlight_region() is used to highlight a portion of AlignmentView. It
will draw the black rectangle around this portion in AlignmentView.

INPUT:
  start: the position of highlighted region in AlignmentView.
  len: the span (width) of highlighted region.
OUTPUT:
  NONE
*/
void
AlignmentView::highlight_region(int start, int len) {
  alignment_view_->highlight_region(start, len);
  label_view_->highlight_region(start, len);
  site_view_->highlight_region(start, len);
  update_sizes();
}

/* update_sizes() is used to resize the layout of three major components.
This method should be called whenever the content of data changes. The major
function of this method is to constraint the height of siteview (totalHeight/10 <
height < totalHeight/4), and the width of labelView (totalWidth<10 < width < 
totalWidth<4). This will guarantee the space requirement of alignmenView.

INPUT:
  NONE
OUTPUT:
  NONE
*/
void
AlignmentView::update_sizes() {

	//get the width and height for current widget.
  int tw = width();
  int th = height();

  //get width and height for all components.
  int sw = site_view_->contentsWidth() + 5;
  int sh = site_view_->contentsHeight() + 5;

  int lw = label_view_->contentsWidth() + 5;
  int lh = label_view_->contentsHeight() + 5;

  int aw = alignment_view_->contentsWidth() + 5;
  int ah = alignment_view_->contentsHeight() + 5;

  //enfore the contraint of siteView height and labelView width
  if(sh < 10) sh=th/10;
  if(lw < 10) lw=tw/10;
  if(sh > th/4) sh=th/4;
  if(lw > tw/4) lw=tw/4;
  
  //configure the layout of all components.
  alignment_view_->setGeometry(lw, sh, tw-lw, th-sh);
  label_view_->setGeometry(0, sh, lw, th-sh);
  site_view_->setGeometry(lw, 0, tw-lw, sh);
}


/*Make the labelView to be selectable.

INPUT:
  s: whether the labelView is selectable.
OUTPUT:
  NONE
*/
void
AlignmentView::setLabelsSelectable(bool s) {
  label_view_->setSelectable(s);
}

/*Find out whether labelView is selectable.

INPUT:
  NONE
OUTPUT:
  The selectable property of labelView
*/
bool
AlignmentView::labelsSelectable() const {
  return label_view_->selectable();
}

/*Return the label status for all sequences. The status here means whether the
sequence has been selected.

INPUT:
  states: a vector of sequence status. Actually, this is the return value of 
  this function call.
OUTPUT:
  NONE
*/
void
AlignmentView::getLabelStates(std::vector<std::pair<std::string, bool> > &states) const {
  label_view_->getStates(states);
}

/*This method is the handler of GUI event - resizeEvent. Basically, whenever users
resize the window, this method will be called.

INPUT:
  NONE
OUTPUT:
  NONE
*/
void
AlignmentView::resizeEvent(QResizeEvent *e) {
  QWidget::resizeEvent(e);
  update_sizes();
}


/*The following three methods are the slots for view moving signals.
Whenever users move any component (labelView, siteView or alignmentView), the
other two components must move accordingly.
*/
void
AlignmentView::alignmentViewMoving(int x, int y) {
  if(last_y_ != y) {
    last_y_ = y;
    label_view_->setContentsPos(label_view_->contentsX(), y);
  }
  if(last_x_ != x) {
    last_x_ = x;
    site_view_->setContentsPos(x, site_view_->contentsY());
  }
}

void
AlignmentView::labelViewMoving(int x, int y) {
  if(last_y_ != y) {
    last_y_ = y;
    alignment_view_->setContentsPos(alignment_view_->contentsX(), y);
  }
}

void
AlignmentView::siteViewMoving(int x, int y) {
  if(last_x_ != x) {
    last_x_ = x;
    alignment_view_->setContentsPos(x, alignment_view_->contentsY());
  }
}

/*AlignmentViewDialog class provides a dialog with built-in AlignmentView inside.
All the methods are pretty similar to the definition of class AlignmentView.
INPUT
  parent: parent widget;
  name: the name of this dialog;
  modal: whether this dialog is modal or not;
  f: flags
OUTPUT
  NONE
*/
AlignmentViewDialog::AlignmentViewDialog(QWidget *parent, const char *name, bool modal, WFlags f)
  : QDialog(parent, name, modal, f)
{
  setMinimumSize(400, 200);
  setCaption("Alignment View");
  
  av_ = new AlignmentView(this);

  ok_ = new QPushButton("OK", this);
  connect(ok_, SIGNAL(clicked()), SLOT(accept()));
}

/*setAlignment() set the alignment sequence of AlignmentView.
INPUT
  alignment: new alignment sequence to be displayed.
OUTPUT
  NONE
*/
void
AlignmentViewDialog::setAlignment(const vector<sequence_t> &alignment) {
  av_->set_alignment(alignment);
}

/*The handler of dialog resize event.
INPUT
  e: the resize event passed from system.
OUTPUT
  NONE
*/
void
AlignmentViewDialog::resizeEvent(QResizeEvent *e) {
  int w = e->size().width();
  int h = e->size().height();
  
  av_->setGeometry(10, 10, w-20, h-20-10-30);
  ok_->setGeometry((w-100)/2, h-10-30, 100, 30);
}

