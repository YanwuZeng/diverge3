#include <qscrollview.h>
#include <qdialog.h>
#include <qpushbutton.h>
#include <qpainter.h>
#include <qpopupmenu.h>
#include <qfontdialog.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qinputdialog.h>
#include <qapplication.h>
#include <qclipboard.h>
#include <qcursor.h>

#include "tree.h"
#include "tree_view.h"
#include "resources.h"

extern Resources * resources;

TreeView::TreeView(QWidget *parent, const char *name, WFlags f)
  : QScrollView(parent, name, f),
    allow_selection(true)
{
  viewport()->setBackgroundMode(NoBackground);
  setFocusPolicy(QWidget::StrongFocus);

  connect(this, SIGNAL(rightButtonPressed(const node_t *)), this, SLOT(slotRightPressed(const node_t *)));
  showTopology = false;
  treeShape = ACTUALSIZE;
}

void
TreeView::set_tree(const Tree &tree, bool use_branch_lengths) {
  /*tree_ = tree;

  int tw, th;
  tree_.max_label_dim(viewport()->fontMetrics(), tw, th);

  int req_width, req_height;
  tree_.req_size(tw, th, req_width, req_height);

  if(req_width != contentsWidth() || req_height != contentsHeight()) {
    resizeContents(req_width, req_height);
  }
  
  tree_.calc_position(contentsWidth(), contentsHeight(), tw, th,
		      use_branch_lengths);
  viewport()->update();*/
	tree_ = tree;

  int tw, th;
  tree_.max_label_dim(viewport()->fontMetrics(), tw, th);

  int req_width, req_height;
  tree_.req_size(tw, th, req_width, req_height);

  if(FITWIDTH == treeShape) {
	req_height = (int) ((((double)visibleWidth())/req_width)*req_height);
	req_width = visibleWidth();
  }
  else if(FITHEIGHT == treeShape) {
	req_width = (int) ((((double)visibleHeight())/req_height)*req_width);
	req_height = visibleHeight();
  }

  if(req_width != contentsWidth() || req_height != contentsHeight()) {
    resizeContents(req_width, req_height);
  }
  
  tree_.calc_position(contentsWidth(), contentsHeight(), tw, th,
		      !showTopology);

  viewport()->update();
}


void
TreeView::show_branch_lengths(bool s) {
  showTopology = !s;
  set_tree(tree_, s);
}

Tree
TreeView::tree() const {
  return tree_;
}

void
TreeView::set_allow_selection(bool s) {
  allow_selection = s;
}

void
TreeView::clear() {
  Tree tmp;
  set_tree(tmp);
}


void
TreeView::drawContents(QPainter *p, int cx, int cy, int cw, int ch) {
  /*
    cx, cy - starting position needing update
    cw, ch - width and height of area needing update
  */
  
  p->fillRect(cx, cy, cw, ch, white);

  tree_.draw(p, cx, cy, cx+cw, cy+ch);
}

void
TreeView::contentsMousePressEvent(QMouseEvent *e) {
  setFocus();
  
  if(allow_selection && e->button() != RightButton ) {
    const node_t *n = tree_.select(e->x(), e->y(), true);
    if(n) {
      // Can not update only the selected area if this was an exclusive select
#if 0
      updateContents(n->x() - 5, n->min_y() - 5,
		     n->max_x() - n->x() + 10, n->max_y() - n->min_y() + 10);
#else
      viewport()->update();
#endif

#ifdef DEBUG_DIVERGE
      printf("%d\n", n->id());
#endif
      
      emit node_selected(n);
    }
  }
  
  const node_t *nn = tree_.getSelectedRoot();
  emit node_selected(nn);

  if(e->button() == RightButton && tree_.breadth() > 0)
	emit rightButtonPressed(nn);
}


void
TreeView::slotRightPressed(const node_t *nn) {
	QPopupMenu menu( this );
	int id1, id2, id3;

	//id1 = menu.insertItem(resources->flipIcon, "F&lip", this, SLOT( slotFlip() ) );
	id2 = menu.insertItem( "&Reroot", this, SLOT( slotReroot() ) );

	if(nn == NULL) {
		menu.setItemEnabled(id1, false);
		menu.setItemEnabled(id2, false);
	}

	menu.insertSeparator();

	//topology menu item
	id1 = menu.insertItem(resources->topologyIcon, "&Topology Only", this, SLOT(topologyOnly()));
	menu.setCheckable(true);
	menu.setItemChecked(id1, showTopology);
	//connect(menu);

	id1 = menu.insertItem(resources->actualsizeIcon, "&Actual Size", this, SLOT( slotActualSize() ) );
	id2 = menu.insertItem(resources->horifitIcon, "Fit &Width", this, SLOT( slotFitWidth() ) );
	id3 = menu.insertItem(resources->vertfitIcon, "Fit &Height", this, SLOT( slotFitHeight() ) );

	switch(treeShape) {
	case ACTUALSIZE:
		menu.setItemChecked(id1, true);
		break;
	case FITWIDTH:
		menu.setItemChecked(id2, true);
		break;
	case FITHEIGHT:
		menu.setItemChecked(id3, true);
		break;
	}

	menu.insertSeparator();
	menu.insertItem( "&Copy to Clipboard", this, SLOT( copyTree() ));
	menu.insertSeparator();
	id1 = menu.insertItem(resources->saveIcon, "&Save Tree", this, SLOT(slotSaveTree()));
	menu.insertSeparator();
	menu.insertItem( "&Tip Width...", this, SLOT( slotChangeTipWidth() ) );
	menu.insertItem( "Tip Label &Font...", this, SLOT( slotChangeFont() ) );
	//menu.insertItem("Tree Info...", this, SLOT(slotTreeInfo()));

	menu.setMouseTracking( TRUE );
	menu.exec( QCursor::pos() );
}


void
TreeView::copyTree() {
	  
  int w = this->contentsWidth();
  int h = this->contentsHeight();
  QPixmap pm(w, h);
  QPainter painter(&pm);
  
  painter.fillRect(0, 0, w, h, white);
  tree_.draw(&painter, 0, 0, w, h);

  QClipboard *cb = QApplication::clipboard();

  cb->clear();
  cb->setPixmap(pm);
}

void
TreeView::slotReroot() {
	Tree tree = tree_;
    if(tree.reroot()) {
      tree.unselect_all();
      set_tree(tree);
    }
}
///////////////////////////////

void
TreeView::slotSaveTree() {
	QString s(QFileDialog::getSaveFileName(QString::null, "Tree (*.tree *.ph);;All Files (*)", NULL));
	if(s.isNull()) 
		return;

	if(!tree_.save(std::string(s.ascii()))) {
		QMessageBox::critical(NULL, NULL, QString("Unable to save ") +
			  s);
		return;
	}
}

///////////////////////////////

void
TreeView::slotFitWidth() {
	treeShape= FITWIDTH;
	set_tree(tree_ );
}


///////////////////////////////

void
TreeView::slotFitHeight() {
	treeShape= FITHEIGHT;
	set_tree(tree_);
}

///////////////////////////////

void
TreeView::slotActualSize() {
	treeShape= ACTUALSIZE;
	set_tree(tree_ );
}

///////////////////////////////

void
TreeView::slotChangeTipWidth() {
  bool ok;

  int n = QInputDialog::getInteger("Tree Tip Width", "Tree tip width",
                                          tree_.getTipWidth(), 1, 10, 1, &ok);
  if(ok) {
	  tree_.setTipWidth(n);
	  set_tree(tree_);
  }
}

///////////////////////////////

void
TreeView::slotChangeFont() {
	bool ok;
    QFont f = QFontDialog::getFont( &ok, tree_.getFont(), this );
    if ( ok ) {
       tree_.setFont(f);
	   set_tree(tree_);
    }
}

///////////////////////////////
void
TreeView::slotFlip() {
	Tree tree = tree_;
    if(tree.flipNode()) {
      tree.unselect_all();
      set_tree(tree);
    }
}

///////////////////////////////

void
TreeView::topologyOnly() {
	showTopology = !showTopology;
	show_branch_lengths(!showTopology);
}

void
TreeView::keyPressEvent(QKeyEvent *e) {
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

TreeViewDialog::TreeViewDialog(QWidget *parent, const char *name, bool modal, WFlags f)
  : QDialog(parent, name, modal, f)
{
  setMinimumSize(400, 400);
  setCaption("Tree View");
  
  tv_ = new TreeView(this);
  tv_->set_allow_selection(false);

  ok_ = new QPushButton("OK", this);
  connect(ok_, SIGNAL(clicked()), SLOT(accept()));
}

void
TreeViewDialog::setTree(const Tree &tree) {
  tv_->set_tree(tree);
}

void
TreeViewDialog::resizeEvent(QResizeEvent *e) {
  int w = e->size().width();
  int h = e->size().height();
  
  tv_->setGeometry(10, 10, w-20, h-20-10-30);
  ok_->setGeometry((w-100)/2, h-10-30, 100, 30);
}


