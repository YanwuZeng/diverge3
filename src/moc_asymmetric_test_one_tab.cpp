/****************************************************************************
** AsymmetricTestOneTab meta object code from reading C++ file 'asymmetric_test_one_tab.h'
**
** Created: Sun Sep 4 02:58:15 2016
**      by: The Qt MOC ($Id: //depot/qt/main/src/moc/moc.y#178 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_AsymmetricTestOneTab
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "asymmetric_test_one_tab.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *AsymmetricTestOneTab::className() const
{
    return "AsymmetricTestOneTab";
}

QMetaObject *AsymmetricTestOneTab::metaObj = 0;

void AsymmetricTestOneTab::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QWidget::className(), "QWidget") != 0 )
	badSuperclassWarning("AsymmetricTestOneTab","QWidget");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString AsymmetricTestOneTab::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("AsymmetricTestOneTab",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* AsymmetricTestOneTab::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QWidget::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(AsymmetricTestOneTab::*m1_t0)();
    m1_t0 v1_0 = Q_AMPERSAND AsymmetricTestOneTab::calculate;
    QMetaData *slot_tbl = QMetaObject::new_metadata(1);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(1);
    slot_tbl[0].name = "calculate()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Protected;
    typedef void(AsymmetricTestOneTab::*m2_t0)();
    m2_t0 v2_0 = Q_AMPERSAND AsymmetricTestOneTab::resultsChanged;
    QMetaData *signal_tbl = QMetaObject::new_metadata(1);
    signal_tbl[0].name = "resultsChanged()";
    signal_tbl[0].ptr = *((QMember*)&v2_0);
    metaObj = QMetaObject::new_metaobject(
	"AsymmetricTestOneTab", "QWidget",
	slot_tbl, 1,
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

// SIGNAL resultsChanged
void AsymmetricTestOneTab::resultsChanged()
{
    activate_signal( "resultsChanged()" );
}