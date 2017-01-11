/****************************************************************************
** FalseDiscoveryRateTab meta object code from reading C++ file 'false_discovery_rate_tab.h'
**
** Created: Sun Sep 4 02:58:15 2016
**      by: The Qt MOC ($Id: //depot/qt/main/src/moc/moc.y#178 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_FalseDiscoveryRateTab
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "false_discovery_rate_tab.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *FalseDiscoveryRateTab::className() const
{
    return "FalseDiscoveryRateTab";
}

QMetaObject *FalseDiscoveryRateTab::metaObj = 0;

void FalseDiscoveryRateTab::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QWidget::className(), "QWidget") != 0 )
	badSuperclassWarning("FalseDiscoveryRateTab","QWidget");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString FalseDiscoveryRateTab::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("FalseDiscoveryRateTab",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* FalseDiscoveryRateTab::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QWidget::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(FalseDiscoveryRateTab::*m1_t0)();
    m1_t0 v1_0 = Q_AMPERSAND FalseDiscoveryRateTab::calculate;
    QMetaData *slot_tbl = QMetaObject::new_metadata(1);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(1);
    slot_tbl[0].name = "calculate()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Protected;
    typedef void(FalseDiscoveryRateTab::*m2_t0)();
    m2_t0 v2_0 = Q_AMPERSAND FalseDiscoveryRateTab::resultsChanged;
    QMetaData *signal_tbl = QMetaObject::new_metadata(1);
    signal_tbl[0].name = "resultsChanged()";
    signal_tbl[0].ptr = *((QMember*)&v2_0);
    metaObj = QMetaObject::new_metaobject(
	"FalseDiscoveryRateTab", "QWidget",
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
void FalseDiscoveryRateTab::resultsChanged()
{
    activate_signal( "resultsChanged()" );
}
