/****************************************************************************
** Gu99MethodTab meta object code from reading C++ file 'gu99_method_tab.h'
**
** Created: Tue Jan 10 16:33:53 2017
**      by: The Qt MOC ($Id: //depot/qt/main/src/moc/moc.y#178 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_Gu99MethodTab
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "gu99_method_tab.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *Gu99MethodTab::className() const
{
    return "Gu99MethodTab";
}

QMetaObject *Gu99MethodTab::metaObj = 0;

void Gu99MethodTab::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(MethodTab::className(), "MethodTab") != 0 )
	badSuperclassWarning("Gu99MethodTab","MethodTab");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString Gu99MethodTab::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("Gu99MethodTab",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* Gu99MethodTab::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) MethodTab::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(Gu99MethodTab::*m1_t0)();
    typedef void(Gu99MethodTab::*m1_t1)(int);
    m1_t0 v1_0 = Q_AMPERSAND Gu99MethodTab::calculate;
    m1_t1 v1_1 = Q_AMPERSAND Gu99MethodTab::bootstrap;
    QMetaData *slot_tbl = QMetaObject::new_metadata(2);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(2);
    slot_tbl[0].name = "calculate()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Protected;
    slot_tbl[1].name = "bootstrap(int)";
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl_access[1] = QMetaData::Protected;
    typedef void(Gu99MethodTab::*m2_t0)(QStringList,DMatrix2D);
    m2_t0 v2_0 = Q_AMPERSAND Gu99MethodTab::thetaValuesChanged;
    QMetaData *signal_tbl = QMetaObject::new_metadata(1);
    signal_tbl[0].name = "thetaValuesChanged(QStringList,DMatrix2D)";
    signal_tbl[0].ptr = *((QMember*)&v2_0);
    metaObj = QMetaObject::new_metaobject(
	"Gu99MethodTab", "MethodTab",
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

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL thetaValuesChanged
void Gu99MethodTab::thetaValuesChanged( QStringList t0, DMatrix2D t1 )
{
    // No builtin function for signal parameter type QStringList,DMatrix2D
    QConnectionList *clist = receivers("thetaValuesChanged(QStringList,DMatrix2D)");
    if ( !clist || signalsBlocked() )
	return;
    typedef void (QObject::*RT0)();
    typedef RT0 *PRT0;
    typedef void (QObject::*RT1)(QStringList);
    typedef RT1 *PRT1;
    typedef void (QObject::*RT2)(QStringList,DMatrix2D);
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
