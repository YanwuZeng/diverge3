/****************************************************************************
** AncestralSeqTab meta object code from reading C++ file 'ancestral_seq_tab.h'
**
** Created: Tue Jan 10 16:33:53 2017
**      by: The Qt MOC ($Id: //depot/qt/main/src/moc/moc.y#178 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_AncestralSeqTab
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "ancestral_seq_tab.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *AncestralSeqTab::className() const
{
    return "AncestralSeqTab";
}

QMetaObject *AncestralSeqTab::metaObj = 0;

void AncestralSeqTab::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QWidget::className(), "QWidget") != 0 )
	badSuperclassWarning("AncestralSeqTab","QWidget");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString AncestralSeqTab::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("AncestralSeqTab",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* AncestralSeqTab::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QWidget::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(AncestralSeqTab::*m1_t0)();
    typedef void(AncestralSeqTab::*m1_t1)();
    typedef void(AncestralSeqTab::*m1_t2)();
    typedef void(AncestralSeqTab::*m1_t3)();
    typedef void(AncestralSeqTab::*m1_t4)(const node_t*);
    typedef void(AncestralSeqTab::*m1_t5)();
    m1_t0 v1_0 = Q_AMPERSAND AncestralSeqTab::clear;
    m1_t1 v1_1 = Q_AMPERSAND AncestralSeqTab::update_alignment;
    m1_t2 v1_2 = Q_AMPERSAND AncestralSeqTab::load_tree;
    m1_t3 v1_3 = Q_AMPERSAND AncestralSeqTab::nj_cluster;
    m1_t4 v1_4 = Q_AMPERSAND AncestralSeqTab::node_selected;
    m1_t5 v1_5 = Q_AMPERSAND AncestralSeqTab::go;
    QMetaData *slot_tbl = QMetaObject::new_metadata(6);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(6);
    slot_tbl[0].name = "clear()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Public;
    slot_tbl[1].name = "update_alignment()";
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl_access[1] = QMetaData::Public;
    slot_tbl[2].name = "load_tree()";
    slot_tbl[2].ptr = *((QMember*)&v1_2);
    slot_tbl_access[2] = QMetaData::Private;
    slot_tbl[3].name = "nj_cluster()";
    slot_tbl[3].ptr = *((QMember*)&v1_3);
    slot_tbl_access[3] = QMetaData::Private;
    slot_tbl[4].name = "node_selected(const node_t*)";
    slot_tbl[4].ptr = *((QMember*)&v1_4);
    slot_tbl_access[4] = QMetaData::Private;
    slot_tbl[5].name = "go()";
    slot_tbl[5].ptr = *((QMember*)&v1_5);
    slot_tbl_access[5] = QMetaData::Private;
    metaObj = QMetaObject::new_metaobject(
	"AncestralSeqTab", "QWidget",
	slot_tbl, 6,
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
