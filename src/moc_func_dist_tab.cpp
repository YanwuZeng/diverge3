/****************************************************************************
** FuncDistTab meta object code from reading C++ file 'func_dist_tab.h'
**
** Created: Tue Jan 10 16:33:53 2017
**      by: The Qt MOC ($Id: //depot/qt/main/src/moc/moc.y#178 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_FuncDistTab
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "func_dist_tab.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *FuncDistTab::className() const
{
    return "FuncDistTab";
}

QMetaObject *FuncDistTab::metaObj = 0;

void FuncDistTab::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QWidget::className(), "QWidget") != 0 )
	badSuperclassWarning("FuncDistTab","QWidget");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString FuncDistTab::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("FuncDistTab",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* FuncDistTab::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QWidget::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(FuncDistTab::*m1_t0)();
    typedef void(FuncDistTab::*m1_t1)(QStringList,DMatrix2D);
    m1_t0 v1_0 = Q_AMPERSAND FuncDistTab::clear;
    m1_t1 v1_1 = Q_AMPERSAND FuncDistTab::updateResults;
    QMetaData *slot_tbl = QMetaObject::new_metadata(2);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(2);
    slot_tbl[0].name = "clear()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Public;
    slot_tbl[1].name = "updateResults(QStringList,DMatrix2D)";
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl_access[1] = QMetaData::Public;
    metaObj = QMetaObject::new_metaobject(
	"FuncDistTab", "QWidget",
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
