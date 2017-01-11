/****************************************************************************
** RVSTab meta object code from reading C++ file 'rvs_tab.h'
**
** Created: Tue Jan 10 16:33:53 2017
**      by: The Qt MOC ($Id: //depot/qt/main/src/moc/moc.y#178 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_RVSTab
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "rvs_tab.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *RVSTab::className() const
{
    return "RVSTab";
}

QMetaObject *RVSTab::metaObj = 0;

void RVSTab::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(MethodTab::className(), "MethodTab") != 0 )
	badSuperclassWarning("RVSTab","MethodTab");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString RVSTab::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("RVSTab",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* RVSTab::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) MethodTab::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(RVSTab::*m1_t0)();
    typedef void(RVSTab::*m1_t1)(int);
    m1_t0 v1_0 = Q_AMPERSAND RVSTab::calculate;
    m1_t1 v1_1 = Q_AMPERSAND RVSTab::bootstrap;
    QMetaData *slot_tbl = QMetaObject::new_metadata(2);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(2);
    slot_tbl[0].name = "calculate()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Protected;
    slot_tbl[1].name = "bootstrap(int)";
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl_access[1] = QMetaData::Protected;
    metaObj = QMetaObject::new_metaobject(
	"RVSTab", "MethodTab",
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
