/****************************************************************************
** Gu2001MethodTab meta object code from reading C++ file 'gu2001_method_tab.h'
**
** Created: Tue Jan 10 16:33:53 2017
**      by: The Qt MOC ($Id: //depot/qt/main/src/moc/moc.y#178 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_Gu2001MethodTab
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "gu2001_method_tab.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *Gu2001MethodTab::className() const
{
    return "Gu2001MethodTab";
}

QMetaObject *Gu2001MethodTab::metaObj = 0;

void Gu2001MethodTab::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(MethodTab::className(), "MethodTab") != 0 )
	badSuperclassWarning("Gu2001MethodTab","MethodTab");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString Gu2001MethodTab::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("Gu2001MethodTab",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* Gu2001MethodTab::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) MethodTab::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(Gu2001MethodTab::*m1_t0)();
    typedef void(Gu2001MethodTab::*m1_t1)(int);
    m1_t0 v1_0 = Q_AMPERSAND Gu2001MethodTab::calculate;
    m1_t1 v1_1 = Q_AMPERSAND Gu2001MethodTab::bootstrap;
    QMetaData *slot_tbl = QMetaObject::new_metadata(2);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(2);
    slot_tbl[0].name = "calculate()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Protected;
    slot_tbl[1].name = "bootstrap(int)";
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl_access[1] = QMetaData::Protected;
    metaObj = QMetaObject::new_metaobject(
	"Gu2001MethodTab", "MethodTab",
	slot_tbl, 2,
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
