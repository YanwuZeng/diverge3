/****************************************************************************
** ClusteringTab meta object code from reading C++ file 'clustering_tab.h'
**
** Created: Tue Jan 10 16:33:53 2017
**      by: The Qt MOC ($Id: //depot/qt/main/src/moc/moc.y#178 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_ClusteringTab
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "clustering_tab.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *ClusteringTab::className() const
{
    return "ClusteringTab";
}

QMetaObject *ClusteringTab::metaObj = 0;

void ClusteringTab::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QWidget::className(), "QWidget") != 0 )
	badSuperclassWarning("ClusteringTab","QWidget");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString ClusteringTab::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("ClusteringTab",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* ClusteringTab::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QWidget::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(ClusteringTab::*m1_t0)();
    typedef void(ClusteringTab::*m1_t1)();
    typedef void(ClusteringTab::*m1_t2)();
    typedef void(ClusteringTab::*m1_t3)();
    typedef void(ClusteringTab::*m1_t4)();
    typedef void(ClusteringTab::*m1_t5)(QListViewItem*);
    typedef void(ClusteringTab::*m1_t6)(QListViewItem*);
    typedef void(ClusteringTab::*m1_t7)(const node_t*);
    m1_t0 v1_0 = Q_AMPERSAND ClusteringTab::clear;
    m1_t1 v1_1 = Q_AMPERSAND ClusteringTab::load_tree;
    m1_t2 v1_2 = Q_AMPERSAND ClusteringTab::nj_cluster;
    m1_t3 v1_3 = Q_AMPERSAND ClusteringTab::add_cluster;
    m1_t4 v1_4 = Q_AMPERSAND ClusteringTab::delete_cluster;
    m1_t5 v1_5 = Q_AMPERSAND ClusteringTab::view_cluster;
    m1_t6 v1_6 = Q_AMPERSAND ClusteringTab::sc_cluster;
    m1_t7 v1_7 = Q_AMPERSAND ClusteringTab::node_selected;
    QMetaData *slot_tbl = QMetaObject::new_metadata(8);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(8);
    slot_tbl[0].name = "clear()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Public;
    slot_tbl[1].name = "load_tree()";
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl_access[1] = QMetaData::Private;
    slot_tbl[2].name = "nj_cluster()";
    slot_tbl[2].ptr = *((QMember*)&v1_2);
    slot_tbl_access[2] = QMetaData::Private;
    slot_tbl[3].name = "add_cluster()";
    slot_tbl[3].ptr = *((QMember*)&v1_3);
    slot_tbl_access[3] = QMetaData::Private;
    slot_tbl[4].name = "delete_cluster()";
    slot_tbl[4].ptr = *((QMember*)&v1_4);
    slot_tbl_access[4] = QMetaData::Private;
    slot_tbl[5].name = "view_cluster(QListViewItem*)";
    slot_tbl[5].ptr = *((QMember*)&v1_5);
    slot_tbl_access[5] = QMetaData::Private;
    slot_tbl[6].name = "sc_cluster(QListViewItem*)";
    slot_tbl[6].ptr = *((QMember*)&v1_6);
    slot_tbl_access[6] = QMetaData::Private;
    slot_tbl[7].name = "node_selected(const node_t*)";
    slot_tbl[7].ptr = *((QMember*)&v1_7);
    slot_tbl_access[7] = QMetaData::Private;
    typedef void(ClusteringTab::*m2_t0)();
    m2_t0 v2_0 = Q_AMPERSAND ClusteringTab::clustersChanged;
    QMetaData *signal_tbl = QMetaObject::new_metadata(1);
    signal_tbl[0].name = "clustersChanged()";
    signal_tbl[0].ptr = *((QMember*)&v2_0);
    metaObj = QMetaObject::new_metaobject(
	"ClusteringTab", "QWidget",
	slot_tbl, 8,
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

// SIGNAL clustersChanged
void ClusteringTab::clustersChanged()
{
    activate_signal( "clustersChanged()" );
}
