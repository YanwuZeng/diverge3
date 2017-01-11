/****************************************************************************
** PDBView meta object code from reading C++ file 'pdb_viewer.h'
**
** Created: Tue Jan 10 16:33:53 2017
**      by: The Qt MOC ($Id: //depot/qt/main/src/moc/moc.y#178 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_PDBView
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "pdb_viewer.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *PDBView::className() const
{
    return "PDBView";
}

QMetaObject *PDBView::metaObj = 0;

void PDBView::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QGLWidget::className(), "QGLWidget") != 0 )
	badSuperclassWarning("PDBView","QGLWidget");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString PDBView::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("PDBView",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* PDBView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QGLWidget::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(PDBView::*m1_t0)();
    typedef void(PDBView::*m1_t1)();
    typedef void(PDBView::*m1_t2)();
    typedef void(PDBView::*m1_t3)();
    typedef void(PDBView::*m1_t4)();
    typedef void(PDBView::*m1_t5)(QEvent*);
    typedef void(PDBView::*m1_t6)(QEvent*);
    m1_t0 v1_0 = Q_AMPERSAND PDBView::reset_camera;
    m1_t1 v1_1 = Q_AMPERSAND PDBView::force_redraw;
    m1_t2 v1_2 = Q_AMPERSAND PDBView::rotateCamera;
    m1_t3 v1_3 = Q_AMPERSAND PDBView::panCamera;
    m1_t4 v1_4 = Q_AMPERSAND PDBView::zoomCamera;
    m1_t5 v1_5 = Q_AMPERSAND PDBView::enterEvent;
    m1_t6 v1_6 = Q_AMPERSAND PDBView::leaveEvent;
    QMetaData *slot_tbl = QMetaObject::new_metadata(7);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(7);
    slot_tbl[0].name = "reset_camera()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Public;
    slot_tbl[1].name = "force_redraw()";
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl_access[1] = QMetaData::Public;
    slot_tbl[2].name = "rotateCamera()";
    slot_tbl[2].ptr = *((QMember*)&v1_2);
    slot_tbl_access[2] = QMetaData::Public;
    slot_tbl[3].name = "panCamera()";
    slot_tbl[3].ptr = *((QMember*)&v1_3);
    slot_tbl_access[3] = QMetaData::Public;
    slot_tbl[4].name = "zoomCamera()";
    slot_tbl[4].ptr = *((QMember*)&v1_4);
    slot_tbl_access[4] = QMetaData::Public;
    slot_tbl[5].name = "enterEvent(QEvent*)";
    slot_tbl[5].ptr = *((QMember*)&v1_5);
    slot_tbl_access[5] = QMetaData::Public;
    slot_tbl[6].name = "leaveEvent(QEvent*)";
    slot_tbl[6].ptr = *((QMember*)&v1_6);
    slot_tbl_access[6] = QMetaData::Public;
    metaObj = QMetaObject::new_metaobject(
	"PDBView", "QGLWidget",
	slot_tbl, 7,
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


const char *PDBViewer::className() const
{
    return "PDBViewer";
}

QMetaObject *PDBViewer::metaObj = 0;

void PDBViewer::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QDialog::className(), "QDialog") != 0 )
	badSuperclassWarning("PDBViewer","QDialog");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString PDBViewer::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("PDBViewer",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* PDBViewer::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QDialog::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    QMetaData::Access *slot_tbl_access = 0;
    metaObj = QMetaObject::new_metaobject(
	"PDBViewer", "QDialog",
	0, 0,
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
