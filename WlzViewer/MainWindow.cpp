#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _MainWindow_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         MainWindow.cpp
* \author       Zsolt Husz
* \date         October 2008
* \version      $Id$
* \par
* Address:
*               MRC Human Genetics Unit,
*               Western General Hospital,
*               Edinburgh, EH4 2XU, UK.
* \par
* Copyright (C) 2008 Medical research Council, UK.
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be
* useful but WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
* PURPOSE.  See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public
* License along with this program; if not, write to the Free
* Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA  02110-1301, USA.
* \brief        Application main window
* \ingroup      UI
*
*/
#include <iostream>

// Qt4 includes
#include <QtGui>
#include <QTextEdit>
#include <QFileDialog>
#include <QString>
#include <QMdiArea>

// application includes
#include "MainWindow.h"

//SoQt includes
#include <Inventor/Qt/SoQt.h>
#include <VolumeViz/nodes/SoVolumeRendering.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>

#include <WoolzObject.h>
#include <ObjectSimpleViewer.h>
#include <WoolzFileObject.h>


#include "ObjectListModel.h"
#include <ObjectSimpleViewer.h>

//dialogs
#include "ObjectToolDialog.h"

//widgets
#include "TransferFunctionWidget.h"
MainWindow::MainWindow() : m_objectToolDialog(NULL), m_objectListModel(NULL), m_transferFunctionWidget(NULL) {
    cout << "SHIT" << endl;
    ;
}
MainWindow::MainWindow(int fileNo,char *files[]) : m_objectToolDialog(NULL), m_objectListModel(NULL), m_transferFunctionWidget(NULL) {
  cout << ":::" << fileNo << endl;

  SoQt::init(this);
  SoVolumeRendering::init();

  setupUi( this ); // this sets up GUI
  setCentralWidget(mdiArea);
  showMaximized();
  /* BUG FIX by workaround (Zsolt Husz)

     For future coin / qt versions the following line might nto needed.

     If all SoQt windows are closed, then probably SoQt frees up some local areas. 
     This causes segmentation fault after a new SoQt m_viewer is opened.
     To avoid this problem, here a new examiner m_viewer is created that is not freed unless
     during the whole lifespan of the application, until the application is closed.

     Is not know if this a Qt or Coid3D bug, probably the later.
  */
  new SoQtExaminerViewer;

  //Menu global menu signals
  connect( actionAbout, SIGNAL( triggered() ), this, SLOT( about() ) ); 
  connect( actionExit, SIGNAL( triggered() ), this, SLOT( close() ) ); 
  //for(int i = 1; i < fileNo ; i++){
      //connect( actionOpenObject, SIGNAL( triggered() ), this, SLOT( openObject() ) );
      connect( actionObjects, SIGNAL( triggered() ), this, SLOT( openObjectDialog() ) );

      connect( actionMinimize, SIGNAL( triggered() ), this, SLOT( minimizeSubWindow() ) );
      connect( actionMaximize, SIGNAL( triggered() ), this, SLOT( maximizeSubWindow() ) );
      connect( actionRestore, SIGNAL( triggered() ), this, SLOT( restoreSubWindow() ) );

      m_viewer=NULL;
      m_objectListModel = new ObjectListModel();
      Q_ASSERT(m_objectListModel );
      m_project3D = true;  ///TODO
    //if I iterate for each file in here, only the last .wlz is being loaded.

      if (!m_transferFunctionWidget) {
          m_transferFunctionWidget= new TransferFunctionWidget( this, m_objectListModel);
          Q_ASSERT(m_transferFunctionWidget);
          m_transferFunctionWidget->setFloating(true);
          m_transferFunctionWidget->hide();

          QSize sizeWidget = m_transferFunctionWidget->size();
          QSize sizeMain = size();

          m_transferFunctionWidget->move(0,
                                         sizeMain.rheight()-sizeWidget.rheight());
          addDockWidget(Qt::LeftDockWidgetArea, m_transferFunctionWidget);
          QAction *toggleAction = m_transferFunctionWidget->toggleViewAction();
          QIcon icon;
          icon.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/transferfunction.png")), QIcon::Normal, QIcon::Off);
          toggleAction->setIcon(icon);
          toolBar->addAction(toggleAction);
      }
      toolBar->setVisible(true);
      menuWindow->addAction(toolBar->toggleViewAction());
  if (fileNo == 1)
    connect( actionOpenObject, SIGNAL( triggered() ), this, SLOT( openObject() ) );
    //  ;
  else{

          openObject(fileNo, files) ;
      //}
      return;
  }
  /*
  connect( actionObjects, SIGNAL( triggered() ), this, SLOT( openObjectDialog() ) );

  connect( actionMinimize, SIGNAL( triggered() ), this, SLOT( minimizeSubWindow() ) );
  connect( actionMaximize, SIGNAL( triggered() ), this, SLOT( maximizeSubWindow() ) );
  connect( actionRestore, SIGNAL( triggered() ), this, SLOT( restoreSubWindow() ) );

  m_viewer=NULL;
  m_objectListModel = new ObjectListModel();
  Q_ASSERT(m_objectListModel );
  m_project3D = true;  ///TODO

  if (!m_transferFunctionWidget) {
     m_transferFunctionWidget= new TransferFunctionWidget( this, m_objectListModel);
     Q_ASSERT(m_transferFunctionWidget);
     m_transferFunctionWidget->setFloating(true);
     m_transferFunctionWidget->hide();

     QSize sizeWidget = m_transferFunctionWidget->size();
     QSize sizeMain = size();

     m_transferFunctionWidget->move(0,
          sizeMain.rheight()-sizeWidget.rheight());
     addDockWidget(Qt::LeftDockWidgetArea, m_transferFunctionWidget);
     QAction *toggleAction = m_transferFunctionWidget->toggleViewAction();
     QIcon icon;
     icon.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/transferfunction.png")), QIcon::Normal, QIcon::Off);
     toggleAction->setIcon(icon);
     toolBar->addAction(toggleAction);
  }
  toolBar->setVisible(true);
  menuWindow->addAction(toolBar->toggleViewAction());
  */
}

//      connect( actionOpenMeshTarget, SIGNAL( triggered() ), this, SLOT( openMeshTarget( ) ) );

void MainWindow::openObject() {
    cout << "hehe" << endl;
    QString filename = QFileDialog::getOpenFileName(this, tr("Open object"),
                                                ".",
                                                WoolzFileObject::getValueFormats());
    //printf("hehe %s",filename.constData());
    if (filename.isEmpty())
      return;
    WoolzObject *newVolumeObject;
    if ((newVolumeObject = load(filename, WoolzObject::target)))  {
                QApplication::setOverrideCursor(Qt::WaitCursor);
                addViewer();   // will be default show mesh object and value (if loaded) objects
                newVolumeObject->update(true);
                if (m_objectListModel == NULL)
                    cout << "NULL HERE" << endl;
                m_objectListModel->addObject(newVolumeObject);
                QApplication::restoreOverrideCursor();
    }
}

void MainWindow::openObject(int fileNo, char* files[]) {
    cout << "hehe22" << endl;
    for(int i = 1; i < fileNo ; i++){
        cout << ">>>>>>argv[" << i << "] = " << files[i] << endl;
        QString filename = files[i];
        if (filename.isEmpty())
            return;
        WoolzObject *newVolumeObject;
        if ((newVolumeObject = load(filename, WoolzObject::target)))  {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            addViewer();   // will be default show mesh object and value (if loaded) objects
            newVolumeObject->update(true);
            m_objectListModel->addObject(newVolumeObject);
            QApplication::restoreOverrideCursor();
        }
    }
}


void MainWindow::about()  {
      QMessageBox::about(this, "About Woolz viewing",
                               "<h3>Woolz viewing Interface</h3>"
                               "<p>Copyright (C) 2009 MRC Human Genetcs Unit<p>");
}


void MainWindow::addMDIWindows(QWidget * window) {
///\BUG: QT bug :http://trolltech.com/developer/task-tracker/index_html?method=entry&id=194717
/// the description is not exactly the same.
/// however getActiveSubWindow returns nil

  mdiArea->setActiveSubWindow(0);    //should be removed

  QMdiSubWindow *subWindow=mdiArea->addSubWindow(window);
  Q_ASSERT(subWindow);
  if (subWindow) {
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->showMaximized();
    mdiArea->setActiveSubWindow(subWindow);
  }
}

MainWindow::~MainWindow() {
  if (m_transferFunctionWidget)
      delete m_transferFunctionWidget;

    if (m_objectToolDialog )
      delete m_objectToolDialog;

  mdiArea->closeAllSubWindows ();
}

void MainWindow::options() {
 }

void MainWindow::addViewer ()
{
     // create m_viewer using this window (window)
    if (m_viewer)
        return;
    m_viewer = new ObjectSimpleViewer(m_project3D);

    Q_ASSERT(m_viewer);
    m_viewer ->init();

    //set title and minum size
    m_viewer->setWindowTitle("Wlz 3D object viewer");

    addMDIWindows(m_viewer);

    connect(m_objectListModel, SIGNAL(addObjectSignal(WoolzObject*)), m_viewer, SLOT(addObject(WoolzObject*)));
    connect(m_objectListModel, SIGNAL(removedObjectSignal(WoolzObject*)), m_viewer, SLOT(removedObject(WoolzObject*)));
}

WoolzObject *MainWindow::load(QString filename , WoolzObject::WoolzObjectType type) {
    WoolzFileObject *object  = new WoolzFileObject();
    if (object) {
       object->readType(filename);
         object->open(filename, type);
         QApplication::restoreOverrideCursor();
         if (object->isEmpty()) {
            QMessageBox::warning(NULL, "Value object open", "File " + filename + "\n cannot be read.") ;
            delete object;
            object = NULL;
         } else {
            object->generateNewColour();
            object->setRemovable(true);
         }
    }
    return object;
}

void MainWindow::openObjectDialog () {
  if (!m_objectToolDialog) {
    m_objectToolDialog = new ObjectToolDialog( m_objectListModel, this);
  }
  Q_ASSERT(m_objectToolDialog);
  m_objectToolDialog->show();
  m_objectToolDialog->raise();
  m_objectToolDialog->activateWindow();
  return;
}

void MainWindow::minimizeSubWindow() {
  QMdiSubWindow *subWindow =  getWorkspace()->activeSubWindow();
  if (subWindow)
    subWindow->showMinimized();
}

void MainWindow::maximizeSubWindow() {
  QMdiSubWindow *subWindow =  getWorkspace()->activeSubWindow();
  if (subWindow)
    subWindow->showMaximized();
}

void MainWindow::restoreSubWindow() {
  QMdiSubWindow *subWindow =  getWorkspace()->activeSubWindow();
  if (subWindow)
    subWindow->showNormal();
}
