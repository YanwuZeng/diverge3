/****************************************************************************
** MethodResultsList meta object code from reading C++ file 'method_tab.h'
**
** Created: Tue Jan 10 16:33:53 2017
**      by: The Qt MOC ($Id: //depot/qt/main/src/moc/moc.y#178 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_MethodResultsList
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "method_tab.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *MethodResultsList::className() const
{
    return "MethodResultsList";
}

QMetaObject *MethodResultsList::metaObj = 0;

void MethodResultsList::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QListViewWCS::className(), "QListViewWCS") != 0 )
	badSuperclassWarning("MethodResultsList","QListViewWCS");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString MethodResultsList::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("MethodResultsList",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* MethodResultsList::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QListViewWCS::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(MethodResultsList::*m1_t0)();
    m1_t0 v1_0 = Q_AMPERSAND MethodResultsList::selection_changed;
    QMetaData *slot_tbl = QMetaObject::new_metadata(1);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(1);
    slot_tbl[0].name = "selection_changed()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Protected;
    metaObj = QMetaObject::new_metaobject(
	"MethodResultsList", "QListViewWCS",
	slot_tbl, 1,
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


const char *MethodTab::className() const
{
    return "MethodTab";
}

QMetaObject *MethodTab::metaObj = 0;

void MethodTab::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QWidget::className(), "QWidget") != 0 )
	badSuperclassWarning("MethodTab","QWidget");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString MethodTab::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("MethodTab",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* MethodTab::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QWidget::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(MethodTab::*m1_t0)();
    typedef void(MethodTab::*m1_t1)();
    typedef void(MethodTab::*m1_t2)();
    typedef void(MethodTab::*m1_t3)(int);
    typedef void(MethodTab::*m1_t4)();
    typedef void(MethodTab::*m1_t5)()const;
    typedef bool(MethodTab::*m1_t6)();
    typedef void(MethodTab::*m1_t7)();
    typedef void(MethodTab::*m1_t8)();
    m1_t0 v1_0 = Q_AMPERSAND MethodTab::clear;
    m1_t1 v1_1 = Q_AMPERSAND MethodTab::calculate;
    m1_t2 v1_2 = Q_AMPERSAND MethodTab::bootstrap;
    m1_t3 v1_3 = Q_AMPERSAND MethodTab::bootstrap;
    m1_t4 v1_4 = Q_AMPERSAND MethodTab::display_on_alignment;
    m1_t5 v1_5 = Q_AMPERSAND MethodTab::display_on_structure;
    m1_t6 v1_6 = Q_AMPERSAND MethodTab::export_data;
    m1_t7 v1_7 = Q_AMPERSAND MethodTab::update_graph;
    m1_t8 v1_8 = Q_AMPERSAND MethodTab::showTip;
    QMetaData *slot_tbl = QMetaObject::new_metadata(9);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(9);
    slot_tbl[0].name = "clear()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Public;
    slot_tbl[1].name = "calculate()";
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl_access[1] = QMetaData::Protected;
    slot_tbl[2].name = "bootstrap()";
    slot_tbl[2].ptr = *((QMember*)&v1_2);
    slot_tbl_access[2] = QMetaData::Protected;
    slot_tbl[3].name = "bootstrap(int)";
    slot_tbl[3].ptr = *((QMember*)&v1_3);
    slot_tbl_access[3] = QMetaData::Protected;
    slot_tbl[4].name = "display_on_alignment()";
    slot_tbl[4].ptr = *((QMember*)&v1_4);
    slot_tbl_access[4] = QMetaData::Protected;
    slot_tbl[5].name = "display_on_structure()";
    slot_tbl[5].ptr = *((QMember*)&v1_5);
    slot_tbl_access[5] = QMetaData::Protected;
    slot_tbl[6].name = "export_data()";
    slot_tbl[6].ptr = *((QMember*)&v1_6);
    slot_tbl_access[6] = QMetaData::Protected;
    slot_tbl[7].name = "update_graph()";
    slot_tbl[7].ptr = *((QMember*)&v1_7);
    slot_tbl_access[7] = QMetaData::Protected;
    slot_tbl[8].name = "showTip()";
    slot_tbl[8].ptr = *((QMember*)&v1_8);
    slot_tbl_access[8] = QMetaData::Protected;
    typedef void(MethodTab::*m2_t0)();
    m2_t0 v2_0 = Q_AMPERSAND MethodTab::resultsChanged;
    QMetaData *signal_tbl = QMetaObject::new_metadata(1);
    signal_tbl[0].name = "resultsChanged()";
    signal_tbl[0].ptr = *((QMember*)&v2_0);
    metaObj = QMetaObject::new_metaobject(
	"MethodTab", "QWidget",
	slot_tbl, 9,
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
void MethodTab::resultsChanged()
{
    activate_signal( "resultsChanged()" );
}
