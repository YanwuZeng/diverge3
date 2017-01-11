/****************************************************************************
** TreeView meta object code from reading C++ file 'tree_view.h'
**
** Created: Tue Jan 10 16:33:53 2017
**      by: The Qt MOC ($Id: //depot/qt/main/src/moc/moc.y#178 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_TreeView
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "tree_view.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *TreeView::className() const
{
    return "TreeView";
}

QMetaObject *TreeView::metaObj = 0;

void TreeView::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QScrollView::className(), "QScrollView") != 0 )
	badSuperclassWarning("TreeView","QScrollView");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString TreeView::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("TreeView",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* TreeView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QScrollView::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(TreeView::*m1_t0)(bool);
    typedef void(TreeView::*m1_t1)(const node_t*);
    typedef void(TreeView::*m1_t2)();
    typedef void(TreeView::*m1_t3)();
    typedef void(TreeView::*m1_t4)();
    typedef void(TreeView::*m1_t5)();
    typedef void(TreeView::*m1_t6)();
    typedef void(TreeView::*m1_t7)();
    typedef void(TreeView::*m1_t8)();
    typedef void(TreeView::*m1_t9)();
    typedef void(TreeView::*m1_t10)();
    typedef void(TreeView::*m1_t11)();
    m1_t0 v1_0 = Q_AMPERSAND TreeView::show_branch_lengths;
    m1_t1 v1_1 = Q_AMPERSAND TreeView::slotRightPressed;
    m1_t2 v1_2 = Q_AMPERSAND TreeView::slotReroot;
    m1_t3 v1_3 = Q_AMPERSAND TreeView::slotChangeFont;
    m1_t4 v1_4 = Q_AMPERSAND TreeView::slotFlip;
    m1_t5 v1_5 = Q_AMPERSAND TreeView::slotSaveTree;
    m1_t6 v1_6 = Q_AMPERSAND TreeView::copyTree;
    m1_t7 v1_7 = Q_AMPERSAND TreeView::topologyOnly;
    m1_t8 v1_8 = Q_AMPERSAND TreeView::slotChangeTipWidth;
    m1_t9 v1_9 = Q_AMPERSAND TreeView::slotFitWidth;
    m1_t10 v1_10 = Q_AMPERSAND TreeView::slotFitHeight;
    m1_t11 v1_11 = Q_AMPERSAND TreeView::slotActualSize;
    QMetaData *slot_tbl = QMetaObject::new_metadata(12);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(12);
    slot_tbl[0].name = "show_branch_lengths(bool)";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Public;
    slot_tbl[1].name = "slotRightPressed(const node_t*)";
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl_access[1] = QMetaData::Public;
    slot_tbl[2].name = "slotReroot()";
    slot_tbl[2].ptr = *((QMember*)&v1_2);
    slot_tbl_access[2] = QMetaData::Public;
    slot_tbl[3].name = "slotChangeFont()";
    slot_tbl[3].ptr = *((QMember*)&v1_3);
    slot_tbl_access[3] = QMetaData::Public;
    slot_tbl[4].name = "slotFlip()";
    slot_tbl[4].ptr = *((QMember*)&v1_4);
    slot_tbl_access[4] = QMetaData::Public;
    slot_tbl[5].name = "slotSaveTree()";
    slot_tbl[5].ptr = *((QMember*)&v1_5);
    slot_tbl_access[5] = QMetaData::Public;
    slot_tbl[6].name = "copyTree()";
    slot_tbl[6].ptr = *((QMember*)&v1_6);
    slot_tbl_access[6] = QMetaData::Public;
    slot_tbl[7].name = "topologyOnly()";
    slot_tbl[7].ptr = *((QMember*)&v1_7);
    slot_tbl_access[7] = QMetaData::Public;
    slot_tbl[8].name = "slotChangeTipWidth()";
    slot_tbl[8].ptr = *((QMember*)&v1_8);
    slot_tbl_access[8] = QMetaData::Public;
    slot_tbl[9].name = "slotFitWidth()";
    slot_tbl[9].ptr = *((QMember*)&v1_9);
    slot_tbl_access[9] = QMetaData::Public;
    slot_tbl[10].name = "slotFitHeight()";
    slot_tbl[10].ptr = *((QMember*)&v1_10);
    slot_tbl_access[10] = QMetaData::Public;
    slot_tbl[11].name = "slotActualSize()";
    slot_tbl[11].ptr = *((QMember*)&v1_11);
    slot_tbl_access[11] = QMetaData::Public;
    typedef void(TreeView::*m2_t0)(const node_t*);
    typedef void(TreeView::*m2_t1)(const node_t*);
    m2_t0 v2_0 = Q_AMPERSAND TreeView::node_selected;
    m2_t1 v2_1 = Q_AMPERSAND TreeView::rightButtonPressed;
    QMetaData *signal_tbl = QMetaObject::new_metadata(2);
    signal_tbl[0].name = "node_selected(const node_t*)";
    signal_tbl[0].ptr = *((QMember*)&v2_0);
    signal_tbl[1].name = "rightButtonPressed(const node_t*)";
    signal_tbl[1].ptr = *((QMember*)&v2_1);
    metaObj = QMetaObject::new_metaobject(
	"TreeView", "QScrollView",
	slot_tbl, 12,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    metaObj->set_slot_access( slot_tbl_access );
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    return metaObj;
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL node_selected
void TreeView::node_selected( const node_t* t0 )
{
    // No builtin function for signal parameter type const node_t*
    QConnectionList *clist = receivers("node_selected(const node_t*)");
    if ( !clist || signalsBlocked() )
	return;
    typedef void (QObject::*RT0)();
    typedef RT0 *PRT0;
    typedef void (QObject::*RT1)(const node_t*);
    typedef RT1 *PRT1;
    RT0 r0;
    RT1 r1;
    QConnectionListIt it(*clist);
    QConnection   *c;
    QSenderObject *object;
    while ( (c=it.current()) ) {
	++it;
	object = (QSenderObject*)c->object();
	object->setSender( this );
	switch ( c->numArgs() ) {
	    case 0:
		r0 = *((PRT0)(c->member()));
		(object->*r0)();
		break;
	    case 1:
		r1 = *((PRT1)(c->member()));
		(object->*r1)(t0);
		break;
	}
    }
}

// SIGNAL rightButtonPressed
void TreeView::rightButtonPressed( const node_t* t0 )
{
    // No builtin function for signal parameter type const node_t*
    QConnectionList *clist = receivers("rightButtonPressed(const node_t*)");
    if ( !clist || signalsBlocked() )
	return;
    typedef void (QObject::*RT0)();
    typedef RT0 *PRT0;
    typedef void (QObject::*RT1)(const node_t*);
    typedef RT1 *PRT1;
    RT0 r0;
    RT1 r1;
    QConnectionListIt it(*clist);
    QConnection   *c;
    QSenderObject *object;
    while ( (c=it.current()) ) {
	++it;
	object = (QSenderObject*)c->object();
	object->setSender( this );
	switch ( c->numArgs() ) {
	    case 0:
		r0 = *((PRT0)(c->member()));
		(object->*r0)();
		break;
	    case 1:
		r1 = *((PRT1)(c->member()));
		(object->*r1)(t0);
		break;
	}
    }
}


const char *TreeViewDialog::className() const
{
    return "TreeViewDialog";
}

QMetaObject *TreeViewDialog::metaObj = 0;

void TreeViewDialog::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QDialog::className(), "QDialog") != 0 )
	badSuperclassWarning("TreeViewDialog","QDialog");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString TreeViewDialog::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("TreeViewDialog",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* TreeViewDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QDialog::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    QMetaData::Access *slot_tbl_access = 0;
    metaObj = QMetaObject::new_metaobject(
	"TreeViewDialog", "QDialog",
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    metaObj->set_slot_access( slot_tbl_access );
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    return metaObj;
}
