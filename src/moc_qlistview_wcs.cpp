/****************************************************************************
** HeaderEventFilter meta object code from reading C++ file 'qlistview_wcs.h'
**
** Created: Tue Jan 10 16:33:53 2017
**      by: The Qt MOC ($Id: //depot/qt/main/src/moc/moc.y#178 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_HeaderEventFilter
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "qlistview_wcs.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *HeaderEventFilter::className() const
{
    return "HeaderEventFilter";
}

QMetaObject *HeaderEventFilter::metaObj = 0;

void HeaderEventFilter::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QWidget::className(), "QWidget") != 0 )
	badSuperclassWarning("HeaderEventFilter","QWidget");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString HeaderEventFilter::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("HeaderEventFilter",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* HeaderEventFilter::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QWidget::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    QMetaData::Access *slot_tbl_access = 0;
    typedef void(HeaderEventFilter::*m2_t0)(int,int);
    m2_t0 v2_0 = Q_AMPERSAND HeaderEventFilter::mouseRelease;
    QMetaData *signal_tbl = QMetaObject::new_metadata(1);
    signal_tbl[0].name = "mouseRelease(int,int)";
    signal_tbl[0].ptr = *((QMember*)&v2_0);
    metaObj = QMetaObject::new_metaobject(
	"HeaderEventFilter", "QWidget",
	0, 0,
	signal_tbl, 1,
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

// SIGNAL mouseRelease
void HeaderEventFilter::mouseRelease( int t0, int t1 )
{
    // No builtin function for signal parameter type int,int
    QConnectionList *clist = receivers("mouseRelease(int,int)");
    if ( !clist || signalsBlocked() )
	return;
    typedef void (QObject::*RT0)();
    typedef RT0 *PRT0;
    typedef void (QObject::*RT1)(int);
    typedef RT1 *PRT1;
    typedef void (QObject::*RT2)(int,int);
    typedef RT2 *PRT2;
    RT0 r0;
    RT1 r1;
    RT2 r2;
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
	    case 2:
		r2 = *((PRT2)(c->member()));
		(object->*r2)(t0, t1);
		break;
	}
    }
}


const char *QListViewWCS::className() const
{
    return "QListViewWCS";
}

QMetaObject *QListViewWCS::metaObj = 0;

void QListViewWCS::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QListView::className(), "QListView") != 0 )
	badSuperclassWarning("QListViewWCS","QListView");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString QListViewWCS::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("QListViewWCS",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* QListViewWCS::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QListView::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(QListViewWCS::*m1_t0)(int);
    typedef void(QListViewWCS::*m1_t1)(int,int);
    m1_t0 v1_0 = Q_AMPERSAND QListViewWCS::header_clicked;
    m1_t1 v1_1 = Q_AMPERSAND QListViewWCS::header_mouse_release;
    QMetaData *slot_tbl = QMetaObject::new_metadata(2);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(2);
    slot_tbl[0].name = "header_clicked(int)";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Protected;
    slot_tbl[1].name = "header_mouse_release(int,int)";
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl_access[1] = QMetaData::Protected;
    typedef void(QListViewWCS::*m2_t0)();
    m2_t0 v2_0 = Q_AMPERSAND QListViewWCS::header_changed;
    QMetaData *signal_tbl = QMetaObject::new_metadata(1);
    signal_tbl[0].name = "header_changed()";
    signal_tbl[0].ptr = *((QMember*)&v2_0);
    metaObj = QMetaObject::new_metaobject(
	"QListViewWCS", "QListView",
	slot_tbl, 2,
	signal_tbl, 1,
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

// SIGNAL header_changed
void QListViewWCS::header_changed()
{
    activate_signal( "header_changed()" );
}
