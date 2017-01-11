/****************************************************************************
** AlignmentView meta object code from reading C++ file 'alignment_view.h'
**
** Created: Tue Jan 10 16:33:53 2017
**      by: The Qt MOC ($Id: //depot/qt/main/src/moc/moc.y#178 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_AlignmentView
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "alignment_view.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *AlignmentView::className() const
{
    return "AlignmentView";
}

QMetaObject *AlignmentView::metaObj = 0;

void AlignmentView::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QWidget::className(), "QWidget") != 0 )
	badSuperclassWarning("AlignmentView","QWidget");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString AlignmentView::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("AlignmentView",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* AlignmentView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QWidget::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(AlignmentView::*m1_t0)(int,int);
    typedef void(AlignmentView::*m1_t1)(int,int);
    typedef void(AlignmentView::*m1_t2)(int,int);
    m1_t0 v1_0 = Q_AMPERSAND AlignmentView::alignmentViewMoving;
    m1_t1 v1_1 = Q_AMPERSAND AlignmentView::labelViewMoving;
    m1_t2 v1_2 = Q_AMPERSAND AlignmentView::siteViewMoving;
    QMetaData *slot_tbl = QMetaObject::new_metadata(3);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(3);
    slot_tbl[0].name = "alignmentViewMoving(int,int)";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Protected;
    slot_tbl[1].name = "labelViewMoving(int,int)";
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl_access[1] = QMetaData::Protected;
    slot_tbl[2].name = "siteViewMoving(int,int)";
    slot_tbl[2].ptr = *((QMember*)&v1_2);
    slot_tbl_access[2] = QMetaData::Protected;
    metaObj = QMetaObject::new_metaobject(
	"AlignmentView", "QWidget",
	slot_tbl, 3,
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
