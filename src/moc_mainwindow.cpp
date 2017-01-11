/****************************************************************************
** MainWindow meta object code from reading C++ file 'mainwindow.h'
**
** Created: Tue Jan 10 16:33:53 2017
**      by: The Qt MOC ($Id: //depot/qt/main/src/moc/moc.y#178 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_MainWindow
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "mainwindow.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *MainWindow::className() const
{
    return "MainWindow";
}

QMetaObject *MainWindow::metaObj = 0;

void MainWindow::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QMainWindow::className(), "QMainWindow") != 0 )
	badSuperclassWarning("MainWindow","QMainWindow");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString MainWindow::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("MainWindow",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* MainWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QMainWindow::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(MainWindow::*m1_t0)();
    typedef void(MainWindow::*m1_t1)();
    typedef void(MainWindow::*m1_t2)();
    typedef void(MainWindow::*m1_t3)();
    typedef void(MainWindow::*m1_t4)();
    typedef void(MainWindow::*m1_t5)();
    typedef void(MainWindow::*m1_t6)();
    typedef void(MainWindow::*m1_t7)();
    m1_t0 v1_0 = Q_AMPERSAND MainWindow::fileNew;
    m1_t1 v1_1 = Q_AMPERSAND MainWindow::fileOpen;
    m1_t2 v1_2 = Q_AMPERSAND MainWindow::fileSave;
    m1_t3 v1_3 = Q_AMPERSAND MainWindow::fileSaveAs;
    m1_t4 v1_4 = Q_AMPERSAND MainWindow::filePrint;
    m1_t5 v1_5 = Q_AMPERSAND MainWindow::closeWindow;
    m1_t6 v1_6 = Q_AMPERSAND MainWindow::helpPaper;
    m1_t7 v1_7 = Q_AMPERSAND MainWindow::helpAbout;
    QMetaData *slot_tbl = QMetaObject::new_metadata(8);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(8);
    slot_tbl[0].name = "fileNew()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Protected;
    slot_tbl[1].name = "fileOpen()";
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl_access[1] = QMetaData::Protected;
    slot_tbl[2].name = "fileSave()";
    slot_tbl[2].ptr = *((QMember*)&v1_2);
    slot_tbl_access[2] = QMetaData::Protected;
    slot_tbl[3].name = "fileSaveAs()";
    slot_tbl[3].ptr = *((QMember*)&v1_3);
    slot_tbl_access[3] = QMetaData::Protected;
    slot_tbl[4].name = "filePrint()";
    slot_tbl[4].ptr = *((QMember*)&v1_4);
    slot_tbl_access[4] = QMetaData::Protected;
    slot_tbl[5].name = "closeWindow()";
    slot_tbl[5].ptr = *((QMember*)&v1_5);
    slot_tbl_access[5] = QMetaData::Protected;
    slot_tbl[6].name = "helpPaper()";
    slot_tbl[6].ptr = *((QMember*)&v1_6);
    slot_tbl_access[6] = QMetaData::Protected;
    slot_tbl[7].name = "helpAbout()";
    slot_tbl[7].ptr = *((QMember*)&v1_7);
    slot_tbl_access[7] = QMetaData::Protected;
    metaObj = QMetaObject::new_metaobject(
	"MainWindow", "QMainWindow",
	slot_tbl, 8,
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
