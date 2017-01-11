/****************************************************************************
** SequencesTab meta object code from reading C++ file 'sequences_tab.h'
**
** Created: Tue Jan 10 16:33:53 2017
**      by: The Qt MOC ($Id: //depot/qt/main/src/moc/moc.y#178 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_SequencesTab
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "sequences_tab.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *SequencesTab::className() const
{
    return "SequencesTab";
}

QMetaObject *SequencesTab::metaObj = 0;

void SequencesTab::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QWidget::className(), "QWidget") != 0 )
	badSuperclassWarning("SequencesTab","QWidget");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString SequencesTab::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("SequencesTab",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* SequencesTab::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QWidget::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(SequencesTab::*m1_t0)();
    typedef void(SequencesTab::*m1_t1)();
    m1_t0 v1_0 = Q_AMPERSAND SequencesTab::load_alignment;
    m1_t1 v1_1 = Q_AMPERSAND SequencesTab::load_pdb;
    QMetaData *slot_tbl = QMetaObject::new_metadata(2);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(2);
    slot_tbl[0].name = "load_alignment()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Private;
    slot_tbl[1].name = "load_pdb()";
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl_access[1] = QMetaData::Private;
    typedef void(SequencesTab::*m2_t0)();
    m2_t0 v2_0 = Q_AMPERSAND SequencesTab::alignmentChanged;
    QMetaData *signal_tbl = QMetaObject::new_metadata(1);
    signal_tbl[0].name = "alignmentChanged()";
    signal_tbl[0].ptr = *((QMember*)&v2_0);
    metaObj = QMetaObject::new_metaobject(
	"SequencesTab", "QWidget",
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

// SIGNAL alignmentChanged
void SequencesTab::alignmentChanged()
{
    activate_signal( "alignmentChanged()" );
}
