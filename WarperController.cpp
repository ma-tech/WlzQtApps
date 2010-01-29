#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _WarperController_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         WarperController.cpp
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
* \brief        Control of the warping interface.
* \ingroup      Control
*
*/

#include "WarperController.h"

//Qt includes
#include <QMessageBox>
#include <QFileDialog>
#include <QDialog>
#include <QActionGroup>
#include <QMdiSubWindow>
#include <QSize>
#include <QtXml/QXmlStreamWriter>
#include <QUndoStack>
#include <QSettings>

//project inlcudes
#include "MainWindow.h"
#include "WarperSourceViewer.h"
#include "WarperTargetViewer.h"
#include "WarperResultViewer.h"
#include "WoolzObject.h"
#include "WoolzDynWarpedObject.h"
#include "ObjectListModel.h"
#include "WarperConfig.h"
#include "ObjectViewerController.h"
#include "ObjectViewerModel.h"
#include "LandmarkModel.h"
#include "LandmarkView.h"
#include "WoolzFileObject.h"
#include "WoolzDynMeshObject.h"
#include "LandmarkController.h"
#include "ObjectViewerController.h"
#include "WoolzTransform.h"

//dialogs
#include "MeshDialog.h"
#include "PreferencesDialog.h"

//widgets
#include "ObjectPropertyWidget.h"
#include "TransformWidget.h"
#include "WarpingWidget.h"
#include "SegmentationWidget.h"
#include "ContourISOSurfaceWidget.h"
#include "TransferFunctionWidget.h"
#include "ProjectPropertiesDialog.h"
#include "ProjectProperties.h"

#include "ObjectToolWidget.h"
#include "ViewToolWidget.h"
#include "LandmarkWidget.h"

//Commands
#include "Commands.h"

// Woolz includes
#include <WlzExtFF.h>
#include "WarperConfig.h"

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>

const char* WarperController::xmlTag2D = "Warping2DProject";
const char* WarperController::xmlTag3D = "Warping3DProject";

WarperConfig config;

WarperController::WarperController ( MainWindow * mainwindow, ProjectProperties *projectProperties): m_projectProperties(projectProperties) {
  mainWindow = mainwindow;
  Q_ASSERT(mainwindow != NULL);
  segmentWidget    = NULL;
  transformWidget  = NULL;
  warpingWidget    = NULL;
  objectPropertyWidget = NULL;
  contourWidget    = NULL;
  transferFunctionWidget = NULL;
  objectToolWidget = NULL;
  viewToolWidget = NULL;
  landmarkWidget = NULL;
  undoStack = NULL;
  actionUndo = NULL;
  actionRedo = NULL;
  m_woolzTransform = NULL;

  project3D = true;
  projectInitialised = false;

  menuMeshSelectionActionGroup = new QActionGroup(this);
  Q_ASSERT(menuMeshSelectionActionGroup);
  menuMeshSelectionActionGroup->addAction(mainwindow->actionTargetMesh);
  menuMeshSelectionActionGroup->addAction(mainwindow->actionSourceMesh);
  mainwindow->actionTargetMesh->setChecked(true);

//  mainWindow->actionAutoWarp->setChecked(config.globalAutoUpdate());
  undoStack = new QUndoStack();
  Q_ASSERT(undoStack);

  actionUndo = undoStack->createUndoAction(mainWindow->toolBarOperations);
  actionRedo = undoStack->createRedoAction(mainWindow->toolBarOperations);

  //create landmark model
  landmarkModel = new LandmarkModel();
  Q_ASSERT(landmarkModel);

  // create transform object
  m_woolzTransform = new WoolzTransform(landmarkModel, mainwindow);
  Q_ASSERT(m_woolzTransform);

  // create object and view managers
  objectListModel = new ObjectListModel(m_woolzTransform, undoStack);
  Q_ASSERT(objectListModel);
  connect(objectListModel, SIGNAL(replaceWarpMesh(WoolzObject *)), m_woolzTransform , SLOT(setMeshObject(WoolzObject *)));


  Q_ASSERT(objectListModel);
  connect(&config, SIGNAL(updateAll()), objectListModel , SLOT(updateAll()));
  connect(&config, SIGNAL(updateAllWarped()), objectListModel , SLOT(updateAllWarped()));

  connect(objectListModel , SIGNAL(statusChanged(QString, int)), mainWindow, SLOT(statusChanged(QString, int)));

  //connect(landmarkModel, SIGNAL(warpingChanged()), objectListModel , SLOT(updateAllWarped()));

  //objectListModel->statusChanged("Controller",0);

  //create landmark manager
  landmarkController = new LandmarkController(undoStack, landmarkModel);

  m_objectViewerController = new ObjectViewerController(mainWindow, landmarkController);
  Q_ASSERT(m_objectViewerController);


  // create docking widgets
  int widgetPos =0 ;
  QSize sizeMain = mainWindow->size();

  if (!viewToolWidget) {
     viewToolWidget = new ViewToolWidget(m_objectViewerController->model(), mainWindow);
     Q_ASSERT(viewToolWidget);
     viewToolWidget->setFloating(true);
     viewToolWidget->hide();

     mainWindow->addDockWidget(Qt::LeftDockWidgetArea, viewToolWidget );
     QSize sizeWidget = viewToolWidget ->size();
     viewToolWidget ->move(widgetPos,
          sizeMain.rheight()-sizeWidget.rheight());
     //widgetPos += sizeWidget.rwidth();
  }

  if (!objectToolWidget) {
     objectToolWidget = new ObjectToolWidget(objectListModel, mainWindow);
     Q_ASSERT(objectToolWidget);
     objectToolWidget->setFloating(true);
     objectToolWidget->hide();

     mainWindow->addDockWidget(Qt::LeftDockWidgetArea, objectToolWidget );
     QSize sizeWidget = objectToolWidget ->size();
     objectToolWidget ->move(widgetPos,
          sizeMain.rheight()-sizeWidget.rheight());
     //widgetPos += sizeWidget.rwidth();
  }

  if (!landmarkWidget) {
     landmarkWidget = new LandmarkWidget(landmarkController, mainWindow);
     Q_ASSERT(landmarkWidget);
     landmarkWidget->setFloating(true);
     landmarkWidget->hide();

     mainWindow->addDockWidget(Qt::LeftDockWidgetArea, landmarkWidget );
     QSize sizeWidget = landmarkWidget ->size();
     landmarkWidget ->move(widgetPos,
          sizeMain.rheight()-sizeWidget.rheight());
     //widgetPos += sizeWidget.rwidth();
  }

  if (!objectPropertyWidget) {
     objectPropertyWidget = new ObjectPropertyWidget( mainWindow,
           objectListModel);
     Q_ASSERT(objectPropertyWidget);
     objectPropertyWidget->setFloating(true);

     objectPropertyWidget->hide();
     mainWindow->addDockWidget(Qt::LeftDockWidgetArea, objectPropertyWidget);
     QSize sizeWidget = objectPropertyWidget->size();
     objectPropertyWidget->move(widgetPos,
        sizeMain.rheight()-sizeWidget.rheight());
     widgetPos += sizeWidget.rwidth();

 }
  if (!segmentWidget) {
     segmentWidget = new SegmentationWidget( mainWindow, objectListModel);
     Q_ASSERT(segmentWidget);
     segmentWidget->setFloating(true);
     segmentWidget->hide();

     mainWindow->addDockWidget(Qt::LeftDockWidgetArea, segmentWidget);
     QSize sizeWidget = segmentWidget->size();
     segmentWidget->move(widgetPos,
          sizeMain.rheight()-sizeWidget.rheight());
     widgetPos += sizeWidget.rwidth();
  }

  if (!transformWidget) {
     transformWidget = new TransformWidget( mainWindow, objectListModel, m_woolzTransform);
     Q_ASSERT(transformWidget);
     transformWidget->setFloating(true);
     transformWidget->hide();

     mainWindow->addDockWidget(Qt::LeftDockWidgetArea, transformWidget);
     QSize sizeWidget = transformWidget->size();
     transformWidget->move(widgetPos,
          sizeMain.rheight()-sizeWidget.rheight());
     widgetPos += sizeWidget.rwidth();
  }

  if (!contourWidget) {
     contourWidget = new ContourISOSurfaceWidget( mainWindow, objectListModel);
     Q_ASSERT(contourWidget);
     contourWidget->setFloating(true);
     contourWidget->hide();

     mainWindow->addDockWidget(Qt::LeftDockWidgetArea, contourWidget);
     QSize sizeWidget = contourWidget->size();
     contourWidget->move(widgetPos,
          sizeMain.rheight()-sizeWidget.rheight());
     widgetPos += sizeWidget.rwidth();
 }

  if (!transferFunctionWidget) {
     transferFunctionWidget= new TransferFunctionWidget( mainWindow, objectListModel);
     Q_ASSERT(transferFunctionWidget);
     transferFunctionWidget->setFloating(true);
     transferFunctionWidget->hide();

     mainWindow->addDockWidget(Qt::LeftDockWidgetArea, transferFunctionWidget);
     QSize sizeWidget = transferFunctionWidget->size();
     transferFunctionWidget->move(widgetPos,
          sizeMain.rheight()-sizeWidget.rheight());
     widgetPos += sizeWidget.rwidth();
  }

  if (!warpingWidget) {
     warpingWidget = new WarpingWidget( mainWindow, landmarkController);
     Q_ASSERT(warpingWidget);
     warpingWidget->setFloating(true);
     warpingWidget->hide();

     mainWindow->addDockWidget(Qt::LeftDockWidgetArea, warpingWidget);
     QSize sizeWidget = warpingWidget->size();
     warpingWidget->move(widgetPos,
          sizeMain.rheight()-sizeWidget.rheight());
     widgetPos += sizeWidget.rwidth();
  }

  mainWindow->actionAddLandmark->setChecked(true);
  mainWindow->actionDeleteLandmark->setChecked(false);
  mainWindow->actionMoveLandmark->setChecked(false);
  mainWindow->actionShowLandmarks->setChecked(true);

  //set up connection between models
  connect(objectListModel, SIGNAL(addObjectSignal(WoolzObject*)), m_objectViewerController, SIGNAL(addObjectSignal(WoolzObject*)));
  connect(objectListModel, SIGNAL(removedObjectSignal(WoolzObject*)), m_objectViewerController, SIGNAL(removedObjectSignal(WoolzObject*)));
  connect(objectListModel, SIGNAL(updatePossibleChange(bool)), this, SLOT(updatePossibleChanged(bool)));

  connect( mainWindow->actionAllEditing, SIGNAL( triggered() ), m_objectViewerController, SIGNAL(setEditingMode() ) );
  connect( mainWindow->actionAllViewing, SIGNAL( triggered() ), m_objectViewerController, SIGNAL(setViewingMode() ) );
  connect( mainWindow->actionSwitchModes, SIGNAL( triggered() ), m_objectViewerController, SIGNAL(switchModes() ) );

  connect( m_objectViewerController, SIGNAL( signalRequestAllObjects(ObjectViewer *) ), objectListModel, SLOT(addAllObjects(ObjectViewer *) ) );

  connect( mainWindow->actionShowLandmarks, SIGNAL( toggled (bool) ), this, SLOT( landmarkVisibilityToggled(bool) ) );
  connect( mainWindow->actionAutoWarp, SIGNAL(toggled(bool)), this, SLOT(globalAutoUpdate(bool)));
  connect( mainWindow->actionShowMesh, SIGNAL(toggled(bool)), this, SLOT(warpingMeshUpdate(bool)));
  mainWindow->actionEnableAlpha->setChecked(false);
}

void WarperController::setupToolbars() {
   {
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/undo.png")), QIcon::Normal, QIcon::Off);
    actionUndo->setIcon(icon);
    mainWindow->toolBarGeneral->addAction(actionUndo);
    mainWindow->menuEdit->addAction(actionUndo);
  }
  {
    actionRedo = undoStack->createRedoAction(mainWindow->toolBarOperations);
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/redo.png")), QIcon::Normal, QIcon::Off);
    actionRedo->setIcon(icon);
    mainWindow->toolBarGeneral->addAction(actionRedo);
    mainWindow->menuEdit->addAction(actionRedo);
  }

  if (objectToolWidget) {
       QAction *toggleAction = objectToolWidget->toggleViewAction();
       QIcon icon;
       icon.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/objects.png")), QIcon::Normal, QIcon::Off);
       toggleAction->setIcon(icon);
       mainWindow->toolBarOperations->addAction(toggleAction);
       mainWindow->menuView->insertAction(mainWindow->menuToolbars->menuAction(), toggleAction);
  }
  if (viewToolWidget) {
       QAction *toggleAction = viewToolWidget->toggleViewAction();
       QIcon icon;
       icon.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/views.png")), QIcon::Normal, QIcon::Off);
       toggleAction->setIcon(icon);
       mainWindow->toolBarOperations->addAction(toggleAction);
       mainWindow->menuView->insertAction(mainWindow->menuToolbars->menuAction(), toggleAction);
  }

  if (landmarkWidget) {
       QAction *toggleAction = landmarkWidget->toggleViewAction();
       /*       QIcon icon;
       icon.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/landmarks.png")), QIcon::Normal, QIcon::Off);
       toggleAction->setIcon(icon);*/
       //mainWindow->toolBarOperations->addAction(toggleAction);
       mainWindow->menuView->insertAction(mainWindow->menuToolbars->menuAction(), toggleAction);
  }
  mainWindow->menuView->insertSeparator(mainWindow->menuToolbars->menuAction());

  if (objectPropertyWidget) {
     QAction *toggleAction = objectPropertyWidget->toggleViewAction();
     QIcon icon;
     icon.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/properties.png")), QIcon::Normal, QIcon::Off);
     toggleAction->setIcon(icon);
     mainWindow->toolBarOperations->addAction(toggleAction );
     mainWindow->menuActions->addAction(toggleAction );
  }

  if (segmentWidget) {
     QAction *toggleAction = segmentWidget->toggleViewAction();
     QIcon icon;
     icon.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/segmentation.png")), QIcon::Normal, QIcon::Off);
     toggleAction->setIcon(icon);
     mainWindow->toolBarOperations->addAction(toggleAction );
     mainWindow->menuActions->addAction(toggleAction );
  }

  if (transformWidget) {
     QAction *toggleAction = transformWidget->toggleViewAction();
     QIcon icon;
     icon.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/transformation.png")), QIcon::Normal, QIcon::Off);
     toggleAction->setIcon(icon);
     mainWindow->toolBarOperations->addAction(toggleAction );
     mainWindow->menuActions->addAction(toggleAction );
  }

    if (warpingWidget) {
     QAction *toggleAction = warpingWidget->toggleViewAction();
     QIcon icon;
     icon.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/warpingparams.png")), QIcon::Normal, QIcon::Off);
     toggleAction->setIcon(icon);
     mainWindow->toolBarOperations->addAction(toggleAction );
     mainWindow->menuActions->addAction(toggleAction );
  }

  if (contourWidget) {
     QAction *toggleAction = contourWidget->toggleViewAction();
     QIcon icon;
     icon.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/contour.png")), QIcon::Normal, QIcon::Off);
     toggleAction->setIcon(icon);
     toggleAction->setText(project3D ? "ISO surface":"Contour");
     mainWindow->toolBarOperations->addAction(toggleAction );
     mainWindow->menuActions->addAction(toggleAction );
  }

  if (transferFunctionWidget && projectInitialised && project3D) {
     QAction *toggleAction = transferFunctionWidget->toggleViewAction();
     QIcon icon;
     icon.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/transferfunction.png")), QIcon::Normal, QIcon::Off);
     toggleAction->setIcon(icon);
     mainWindow->toolBarOperations->addAction(toggleAction );
     mainWindow->menuActions->addAction(toggleAction );
  }
  mainWindow->toolBarModes->setVisible(true);
  mainWindow->toolBarWarpingMode->setVisible(true);
  mainWindow->toolBarOperations->setVisible(true);
}

WarperController::~WarperController ( ) {
  // remove toolbars
  mainWindow->toolBarModes->setVisible(false);
  mainWindow->toolBarWarpingMode->setVisible(false);
  mainWindow->toolBarOperations->setVisible(false);

  if (viewToolWidget)
      delete viewToolWidget;

  if (objectToolWidget)
      delete objectToolWidget;

  if (landmarkWidget)
      delete landmarkWidget;

  if (segmentWidget)
      delete segmentWidget;

  if (transformWidget)
      delete transformWidget;

  if (warpingWidget)
      delete warpingWidget;

  if (contourWidget)
      delete contourWidget;

  if (objectPropertyWidget)
      delete objectPropertyWidget;

  if (transferFunctionWidget)
      delete transferFunctionWidget;

  mainWindow->getWorkspace()->closeAllSubWindows();
  if (objectListModel) { //first delete object and their views
      delete objectListModel;
   }
  if (m_objectViewerController) //then delete viewers displaying the views
      delete m_objectViewerController;
  if (landmarkController )
      delete landmarkController;
  if (landmarkModel)
      delete landmarkModel;
  if (actionRedo)
    delete actionRedo;
  if (actionUndo)
    delete actionUndo;
  if (undoStack)
    delete undoStack;
  if (menuMeshSelectionActionGroup)
    delete menuMeshSelectionActionGroup;

  mainWindow->actionSaveProject->setEnabled(false);
}


bool WarperController::openValueTarget () {
    QString filename = QFileDialog::getOpenFileName(mainWindow, tr("Open target value object"),
                                                getLastPath(),
                                                WoolzFileObject::getValueFormats());
    if (filename.isEmpty())
      return false;
    bool ok=false;
    WoolzObject *newVolumeObject;
    if ((newVolumeObject = loadValue(filename, WoolzObject::target)))  {
        if (!is2D3Dcompatibile(newVolumeObject)) {
          ok = false;
        } else {
                QApplication::setOverrideCursor(Qt::WaitCursor);
                m_objectViewerController->addTargetViewer();
                undoStack->push(new CreateWoolzObject(objectListModel, newVolumeObject));
                setLastPath(filename);
                QApplication::restoreOverrideCursor();
                ok=true;
         }
    }
    m_objectViewerController->checkToSetupDefault();
    return ok;
}

bool WarperController::openValueSource () {
    QString filename = QFileDialog::getOpenFileName(mainWindow, tr("Open source value object"),
                                                getLastPath(), WoolzFileObject::getValueFormats());
    if (filename.isEmpty())
      return false;
    bool ok=false;
    WoolzObject *newVolumeObject;
    if ((newVolumeObject = loadValue(filename, WoolzObject::source)))  {
        if (!is2D3Dcompatibile(newVolumeObject)) {
          ok = false;
        } else {
                 QApplication::setOverrideCursor(Qt::WaitCursor);
                 m_objectViewerController->addSourceViewer();
                 undoStack->push(new CreateWoolzObject(objectListModel, newVolumeObject));

                 if (!objectListModel->getFirstWarped()) {
                     WoolzDynWarpedObject * warpedObject = new WoolzDynWarpedObject (objectListModel, m_woolzTransform);
                     warpedObject->setSourceObj(newVolumeObject);
                     warpedObject->setName("Warped "+ newVolumeObject->name());
                     warpedObject->generateNewColour();
                     warpedObject->setRemovable(true);
                     undoStack->push(new CreateWoolzObject(objectListModel, warpedObject));
                 }
                 setLastPath(filename);
                 QApplication::restoreOverrideCursor();
                 ok=true;
        }
    }
    m_objectViewerController->checkToSetupDefault();
    return ok;
}


bool WarperController::openMeshTarget () {
    QString filename = QFileDialog::getOpenFileName(mainWindow, tr("Open Woolz Mesh"),
                                                getLastPath(),
                                                tr("Woolz object (*.wlz)"));
    if (filename.isEmpty())
      return false;
    bool ok=false;
    WoolzObject *newMeshObject;
    if ((newMeshObject = loadMesh(filename, WoolzObject::target))) {
        if (!is2D3Dcompatibile(newMeshObject)) {
          ok = false;
        } else {
                QApplication::setOverrideCursor(Qt::WaitCursor);

                m_objectViewerController->addTargetViewer();
                undoStack->push(new CreateMeshObject(objectListModel, landmarkModel, newMeshObject));
                setLastPath(filename);
                QApplication::restoreOverrideCursor();
                ok=true;
         }
    }
    m_objectViewerController->checkToSetupDefault();
    return ok;
}

bool WarperController::openMeshSource () {
    QString filename = QFileDialog::getOpenFileName(mainWindow, tr("Open Woolz Mesh"),
                                                getLastPath(),
                                                tr("Woolz object (*.wlz)"));
    if (filename.isEmpty())
      return false;
    bool ok=false;
    WoolzObject *newMeshObject;
    if ((newMeshObject = loadMesh(filename, WoolzObject::source))) {
        if (!is2D3Dcompatibile(newMeshObject)) {
          ok = false;
        } else {
                QApplication::setOverrideCursor(Qt::WaitCursor);
                m_objectViewerController->addSourceViewer();
                undoStack->push(new CreateMeshObject(objectListModel, landmarkModel, newMeshObject));
                setLastPath(filename);
                QApplication::restoreOverrideCursor();
                ok=true;
         }
    }
    m_objectViewerController->checkToSetupDefault();
    return ok;
}


bool WarperController::openSurfaceTarget () {
    QString filename = QFileDialog::getOpenFileName(mainWindow, tr("Open Woolz Surface"),
                                                getLastPath(),
                                                tr("Woolz object (*.wlz)"));
    if (filename.isEmpty())
      return false;
    WoolzObject * surfaceObject;
    if (!(surfaceObject=loadSurface(filename, WoolzObject::target)))
      return false;
    if (!is2D3Dcompatibile(surfaceObject))
      return false;
    if (!addToTargetViewer(surfaceObject))
      return false;
    m_objectViewerController->checkToSetupDefault();
    return true;
}

bool WarperController::openCompositeTarget () {
    QMessageBox::information(NULL, "Information", "This feature is not yet implemented.\nSorry...");
    return true;
}

bool WarperController::openSurfaceSource () {
    QString filename = QFileDialog::getOpenFileName(mainWindow, tr("Open Woolz Surface"),
                                                getLastPath(),
                                                tr("Woolz object (*.wlz)"));
    if (filename.isEmpty())
      return false;
    WoolzObject * surfaceObject;
    if (!(surfaceObject=loadSurface(filename, WoolzObject::source)))
      return false;
    if (!is2D3Dcompatibile(surfaceObject))
      return false;
    if (!addToSourceViewer(surfaceObject))
      return false;
    m_objectViewerController->checkToSetupDefault();
    return true;
}

void  WarperController::connectSignals () {
    setupToolbars();
    connect( mainWindow->actionSaveProjectAs, SIGNAL(triggered()), this, SLOT(saveProjectAs()));
    connect( mainWindow->actionSaveProject, SIGNAL(triggered()), this, SLOT(saveProject()));
    connect( mainWindow->actionOpenValueTarget, SIGNAL( triggered() ), this, SLOT( openValueTarget() ) );
    connect( mainWindow->actionOpenMeshTarget, SIGNAL( triggered() ), this, SLOT( openMeshTarget( ) ) );
    connect( mainWindow->actionOpenSurfaceTarget, SIGNAL( triggered() ), this, SLOT( openSurfaceTarget() ) );
    connect( mainWindow->actionOpenCompositeTarget, SIGNAL( triggered() ), this, SLOT( openCompositeTarget() ) );
    connect( mainWindow->actionOpenValueSource, SIGNAL( triggered() ), this, SLOT( openValueSource() ) );
    connect( mainWindow->actionOpenMeshSource, SIGNAL( triggered() ), this, SLOT( openMeshSource( ) ) );
    connect( mainWindow->actionOpenSurfaceSource, SIGNAL( triggered() ), this, SLOT( openSurfaceSource() ) );
    connect( mainWindow->actionExportWarpedObject, SIGNAL( triggered() ), this, SLOT( saveWarpedObject() ) );
    connect( mainWindow->actionExportWarpTransform, SIGNAL( triggered() ), this, SLOT( saveWarpingTransform() ) );

    connect( mainWindow->actionSaveLandmarks, SIGNAL( triggered() ), this, SLOT( saveAsLandmarks() ) );
    connect( mainWindow->actionLoadLandmarks, SIGNAL( triggered() ), this, SLOT( loadLandmarks() ) );
    connect( mainWindow->actionGenerateMesh, SIGNAL( triggered() ), this, SLOT( generateMesh() ) );
    connect( mainWindow->actionSourceView, SIGNAL( triggered() ), this, SLOT( addSourceViewer() ) );
    connect( mainWindow->actionTargetView, SIGNAL( triggered() ), this, SLOT( addTargetViewer() ) );
    connect( mainWindow->actionResultView, SIGNAL( triggered() ), this, SLOT( addResultViewer() ) );
    connect( mainWindow->actionMinimize, SIGNAL( triggered() ), this, SLOT( minimizeSubWindow() ) );
    connect( mainWindow->actionMaximize, SIGNAL( triggered() ), this, SLOT( maximizeSubWindow() ) );
    connect( mainWindow->actionRestore, SIGNAL( triggered() ), this, SLOT( restoreSubWindow() ) );
    connect( mainWindow->actionDefaultLayout, SIGNAL( triggered() ), m_objectViewerController, SLOT( defaultLayout() ) );
    connect( mainWindow->actionDefaultViewLinks, SIGNAL( triggered() ), m_objectViewerController, SLOT( defaultViewLinks() ) );
    connect( mainWindow->getWorkspace(), SIGNAL( subWindowActivated(QMdiSubWindow*) ), m_objectViewerController, SLOT( updateActions() ) );
    connect( mainWindow->actionAddLandmark, SIGNAL( toggled (bool) ), this, SLOT( addToggled(bool) ) );
    connect( mainWindow->actionDeleteLandmark, SIGNAL( toggled (bool) ), this, SLOT( deleteToggled(bool) ) );
    connect( mainWindow->actionMoveLandmark, SIGNAL( toggled (bool) ), this, SLOT( moveToggled(bool) ) );
    connect( mainWindow->actionDeleteLandmark, SIGNAL( toggled (bool) ), landmarkController, SLOT( deleteHighlightedLandmark() ) );
    connect( mainWindow->actionRemoveAllLandmarks, SIGNAL(triggered()), landmarkController, SLOT(removeAllLandmarks()));
    connect( objectListModel, SIGNAL(replaceWarpMesh(WoolzObject *)), this, SLOT(updateShowMesh(WoolzObject*)));
    connect( mainWindow->actionOptions, SIGNAL(triggered()), this, SLOT(options()));
    connect( mainWindow->actionProjectProperties, SIGNAL(triggered()), this, SLOT(setProjectProperties()));

    connect( mainWindow->actionUpdate, SIGNAL(triggered()), this, SLOT(updateNow()));
    mainWindow->actionShowMesh->setEnabled(objectListModel->getMeshObject()!=NULL);
}

WoolzObject* WarperController::loadMesh(QString filename, WoolzObject::WoolzObjectType type) {
    WoolzFileObject* meshObject;
    meshObject = new WoolzFileObject();
    if (meshObject) {
       QApplication::setOverrideCursor(Qt::WaitCursor);
       meshObject->readType(filename);
       if (!meshObject->isMeshPreRead())  {
          QApplication::restoreOverrideCursor();
          QMessageBox::warning(NULL, "Mesh open", "Woolz file has no mesh.") ;
          delete meshObject;
          meshObject = NULL;
       } else {
         meshObject->open(filename, type);
         QApplication::restoreOverrideCursor();
         if (meshObject->isEmpty())  {
            QMessageBox::warning(NULL, "Mesh open", "File " + filename + "\n is not a valid Woolz file.") ;
            delete meshObject;
            meshObject = NULL;
         } else if (!meshObject->isMesh())  {
            QMessageBox::warning(NULL, "Mesh open", "Woolz file has no mesh.") ;
            delete meshObject;
            meshObject = NULL;
         } else {
            meshObject->generateNewColour();
            meshObject->setRemovable(false);        // it cannot be removed from object tool dialog
         }
       }
    }
    return meshObject;
}

WoolzObject *WarperController::loadValue(QString filename , WoolzObject::WoolzObjectType type) {
    WoolzFileObject *valueObject  = new WoolzFileObject();
    if (valueObject) {
       QApplication::setOverrideCursor(Qt::WaitCursor);
       valueObject->readType(filename);
       if (!valueObject->isValuePreRead())  {
          QApplication::restoreOverrideCursor();
            QMessageBox::warning(NULL, "Value object open", "Woolz file has no value object.") ;
          delete valueObject;
          valueObject = NULL;
       } else {
         valueObject->open(filename, type);
         if (valueObject->is3D() && valueObject->isValueSet()) {  // check if type is supported
             WlzGreyType greyType = valueObject->getWoolzGreyType();
             if (greyType != WLZ_GREY_UBYTE && greyType != WLZ_GREY_SHORT && greyType != WLZ_GREY_INT) {
                 QMessageBox::warning(NULL, "Value object open", "Only UBYTE, SHORT and INT objects are supported.") ;
                 delete valueObject;
                 return  NULL;
             }
         }

         QApplication::restoreOverrideCursor();
         if (valueObject->isEmpty()) {
            QMessageBox::warning(NULL, "Value object open", "File " + filename + "\n cannot be read.") ;
            delete valueObject;
            valueObject = NULL;
/*         } else if (!valueObject->isValue()) {
            QMessageBox::warning(NULL, "Value object open", "Woolz file has no value object.") ;
            delete valueObject;
            valueObject = NULL;*/
         } else {
            valueObject->generateNewColour();
            valueObject->setRemovable(true);        // it cannot be removed from object tool dialog
         }
       }
    }
    return valueObject;
}

WoolzObject *WarperController::loadSurface(QString filename , WoolzObject::WoolzObjectType type) {
    WoolzFileObject *surfaceObject = new WoolzFileObject();
    if (surfaceObject) {
       QApplication::setOverrideCursor(Qt::WaitCursor);
       surfaceObject->readType(filename);
       if (!surfaceObject->isContourPreRead())  {
          QApplication::restoreOverrideCursor();
          QMessageBox::warning(NULL, "Contour open", "Woolz file has no contour object.") ;
          delete surfaceObject;
          surfaceObject = NULL;
       } else {
         surfaceObject->open(filename, type);
         QApplication::restoreOverrideCursor();
         if (surfaceObject->isEmpty()) {
            QMessageBox::warning(NULL, "Surface open", "File " + filename + "\n is not a valid Woolz file.") ;
            delete surfaceObject;
            surfaceObject = NULL;
         } else if (!surfaceObject->isContour()) {
            QMessageBox::warning(NULL, "Contour open", "Woolz file has no contour object.") ;
            delete surfaceObject;
            surfaceObject = NULL;
         } else {
            surfaceObject->generateNewColour();
            surfaceObject->setRemovable(true);           // it cannot be removed from object tool dialog
         }
       }
    }
    return surfaceObject;
}

bool WarperController::addToSourceViewer(WoolzObject * obj) {
  m_objectViewerController->addSourceViewer();
  undoStack->push(new CreateWoolzObject(objectListModel, obj));
  return true;
}

bool WarperController::addToTargetViewer (WoolzObject * obj) {
  m_objectViewerController->addTargetViewer();
  undoStack->push(new CreateWoolzObject(objectListModel, obj));
  return true;
}

void WarperController::addSourceViewer ()
{
    m_objectViewerController->addSourceViewer(true);
    return ;
}

void WarperController::addTargetViewer ()
{
    m_objectViewerController->addTargetViewer(true);
    return ;
}

void WarperController::addResultViewer ()
{
    m_objectViewerController->addResultViewer(true);
    return ;
}

void WarperController::init() {
  connectSignals();
}

void WarperController::minimizeSubWindow() {
  QMdiSubWindow *subWindow =  mainWindow->getWorkspace()->activeSubWindow();
  if (subWindow)
    subWindow->showMinimized();
}

void WarperController::maximizeSubWindow() {
  QMdiSubWindow *subWindow =  mainWindow->getWorkspace()->activeSubWindow();
  if (subWindow)
    subWindow->showMaximized();
}

void WarperController::restoreSubWindow() {
  QMdiSubWindow *subWindow =  mainWindow->getWorkspace()->activeSubWindow();
  if (subWindow)
    subWindow->showNormal();
}

bool WarperController::is2D3Dcompatibile(WoolzObject *object) {
  bool is3D = object->is3D();
  if (projectInitialised) {
    if (is3D != project3D) { // object is not the same dimensionality as the previous object loaded
        QMessageBox::warning(NULL, tr("Load mesh error"),
        tr("Object cannot be loaded into this project: it is %1 while objects of the project are %2").arg(is3D?"3D":"2D").arg(is3D?"2D":"3D") );
        return false;
     }
  } else set3D(is3D);
  return true;
}

void WarperController::addToggled(bool checked) {
  if (checked)
    mainWindow->actionDeleteLandmark->setChecked(false);
  else
    landmarkController->cancelIncompleteLandmarks();
}

void WarperController::landmarkVisibilityToggled(bool checked) {
  landmarkController->getLandmarkView(LandmarkModel::sourceV)->setVisibility(checked);
  landmarkController->getLandmarkView(LandmarkModel::targetV)->setVisibility(checked);
}

void WarperController::deleteToggled(bool checked) {
  if (checked) {
    mainWindow->actionAddLandmark->setChecked(false);
    mainWindow->actionMoveLandmark->setChecked(false);
  }
}

void WarperController::moveToggled(bool checked) {
  if (checked)
    mainWindow->actionDeleteLandmark->setChecked(false);
  landmarkController->setMove(checked);
}

void WarperController::saveWarpedObject () {
  WoolzObject *warpedObject = objectListModel->getFirstWarped();
   if (!warpedObject || warpedObject->isEmpty()) {
      QMessageBox::warning(NULL, "Save warped object", "No available warped object.");
      return;
  }
  QString filename = QFileDialog::getSaveFileName(mainWindow, tr("Save warped object"),
                                                getLastPath(),
                                                tr("Woolz object (*.wlz)"));
  if (!filename.isEmpty())
    if (!warpedObject->saveAs(filename))
        QMessageBox::warning(NULL, "Save warped object", "Cannot save object to file " + filename + ".") ;
    else
        setLastPath(filename);

}

void WarperController::saveWarpingTransform  () {
  QString filename = QFileDialog::getSaveFileName(mainWindow, tr("Save warping transform"),
                                                getLastPath(),
                                                tr("Warping transform (*.wlz)"));
  if (!filename.isEmpty()) {
      QString ext = QFileInfo(filename).suffix().toUpper();
      if (ext.isEmpty()) {
          filename += ".wlz";
          if (!saveWarpingTransformToFile(filename))
            QMessageBox::warning(NULL, "Save warping transform", "Canot save transform to file.") ;
      } else
            setLastPath(filename);
  }
}

bool WarperController::saveWarpingTransformToFile(QString filename) {
    WlzErrorNum errNum = WLZ_ERR_NONE;

    mainWindow->statusChanged("Generating warping transform.", 0);

    //check for errors
    QString errorMsg;
    if (!m_woolzTransform->isReadyForWarp(errorMsg)) {
       mainWindow->statusChanged("Transform generation failed: " + errorMsg + ".", 0);
       return false;
    }

  WlzObject *transformObj = m_woolzTransform->getTransformObj(&errNum);

  if (errNum == WLZ_ERR_NONE) {
    FILE *fp;
    fp=fopen(filename.toAscii(),"wb");

    if (!fp)
       return false;
    errNum = WlzWriteObj( fp, transformObj);

    if (fp)
        fclose(fp);
    mainWindow->statusChanged(errNum == WLZ_ERR_NONE ? "Saving transform succeded.":"Saving transform failed.", 0);
  }
  return (errNum == WLZ_ERR_NONE);
}



void WarperController::saveAsLandmarks() {
  QString selectedFilter;
  enum {lmk, txt} type;
  QString filename = QFileDialog::getSaveFileName(mainWindow, tr("Save landmarks"),
                                                getLastPath(),
                                                tr("Landmarks (*.lmk);; Displacement (*.txt)"), &selectedFilter);
  if (filename.isEmpty())
     return;
  if (selectedFilter.contains("txt"))
      type = txt;
  else
      type = lmk;

  QString ext = QFileInfo(filename).suffix().toUpper();
  if (ext.isEmpty()) 
    filename += (type == lmk) ? ".lmk" : ".txt";

  QFile file(filename);
  if (!file.open(QFile::WriteOnly | QFile::Text)) {
    QMessageBox::warning(NULL, tr("Save warping"), tr("Cannot create warping file."));
    return;
  }
  if (type == txt) {
    landmarkModel->saveAsText(&file);
  } else {
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    landmarkModel->saveAsXml(&xmlWriter);
    if (file.error()) {
      QMessageBox::warning(NULL, tr("Save warping"), tr("Error writing file."));
      file.remove();
      return;
    }
  }
  setLastPath(filename);
}

void WarperController::loadLandmarks() {
  QString filename = QFileDialog::getOpenFileName(mainWindow, tr("Load landmarks"),
                                                getLastPath(),
                                                tr("Landmarks (*.wrp *.lmk)"));
  if (filename.isEmpty())
     return;
  QString ext = QFileInfo(filename).suffix().toUpper();
  if (ext.isEmpty())
    filename += ".lmk";
  landmarkController->loadLandmarks(filename);
  setLastPath(filename);
}

void WarperController::generateMesh  () {
  bool useSource = mainWindow->actionSourceMesh->isChecked();
  MeshDialog *dialog = new MeshDialog( mainWindow, objectListModel, useSource, project3D);
  dialog->colorpickerObject->setCurrentColor(QColor(255,255,255));
  if (dialog->exec() == QDialog::Accepted) {
    if (dialog->comboSourceObject->currentIndex() != -1) {
      QApplication::setOverrideCursor(Qt::WaitCursor);

      WoolzDynMeshObject *newMeshObject;
      newMeshObject = new WoolzDynMeshObject(objectListModel);
      Q_ASSERT(newMeshObject);

      WoolzObject * sourceObject = dialog->getSelected();
      Q_ASSERT(sourceObject);

      newMeshObject->setMinDist(dialog->minSpinBox->value());
      newMeshObject->setMaxDist(dialog->maxSpinBox->value());
      newMeshObject->setSourceObj(sourceObject);
      QApplication::restoreOverrideCursor();

      if (newMeshObject && (!objectListModel->getMeshObject() ||
          (QMessageBox::question(NULL, tr("Replace mesh"),
          tr("This operation will replace your current mesh. \n Are you sure to continue?"),
          QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes))) {
              QApplication::setOverrideCursor(Qt::WaitCursor);
              if (newMeshObject) {
                  newMeshObject->setName(dialog->lineEditName->text());
                  newMeshObject->setRemovable(false);
                  newMeshObject->setVisible(mainWindow->actionShowMesh->isChecked());
                  newMeshObject->setQColour(dialog->colorpickerObject->currentColor());
              }
              undoStack->push(new CreateMeshObject(objectListModel, landmarkModel, newMeshObject));

              if (newMeshObject) {
                  newMeshObject->setVisible(mainWindow->actionShowMesh->isChecked());
              }
              QApplication::restoreOverrideCursor();
       }
    }
  }
  delete dialog;
  return;
}

void WarperController::globalAutoUpdate(bool checked) {
  config.setGlobalAutoUpdate(checked);
}

void WarperController::updateShowMesh(WoolzObject* mesh) {
   mainWindow->actionShowMesh->setEnabled(mesh!=NULL);
    if (mesh)
      connect(mesh, SIGNAL(objectVisibilityChanged()), this, SLOT(warpingMeshUpdateAction()));
}

void WarperController::warpingMeshUpdate(bool checked) {
   if (objectListModel->getMeshObject())
       objectListModel->getMeshObject()->setVisible(checked);
}

void WarperController::warpingMeshUpdateAction() {
  WoolzObject* mesh = qobject_cast<WoolzObject*>(sender());
  if (mesh)
     mainWindow->actionShowMesh->setChecked(mesh->visible());
}

void WarperController::options() {
  PreferencesDialog *dialog=new PreferencesDialog(NULL);
  connect(dialog, SIGNAL(configurationChanged()), m_objectViewerController->model(), SLOT(configurationChanged()));
  connect(dialog, SIGNAL(configurationChanged()), landmarkController, SLOT(configurationChanged()));
  dialog->exec();
  delete dialog;
}

void WarperController::setLastPath(QString filename) {
    QString dir = QFileInfo(filename).absolutePath();
    QSettings settings;
    settings.setValue("lastFileDir", dir);
}

QString WarperController::getLastPath() {
   QSettings settings;
   return settings.value("lastFileDir", QString("")).value<QString>();
}

bool WarperController::saveXmlToolbars(QXmlStreamWriter *xmlWriter) {
  xmlWriter->writeStartElement("Toolbars");
/*  Refresh state is not saved by the request of EO
  xmlWriter->writeTextElement("Refresh", mainWindow->actionAutoWarp->isChecked() ? "Yes" : "No");*/
  xmlWriter->writeTextElement("Mesh", mainWindow->actionShowMesh->isChecked() ? "Yes" : "No");
  xmlWriter->writeTextElement("Add", mainWindow->actionAddLandmark->isChecked() ? "Yes" : "No");
  xmlWriter->writeTextElement("Delete", mainWindow->actionDeleteLandmark->isChecked() ? "Yes" : "No");
  xmlWriter->writeTextElement("Move", mainWindow->actionMoveLandmark->isChecked() ? "Yes" : "No");
  xmlWriter->writeTextElement("Landmarks", mainWindow->actionShowLandmarks->isChecked() ? "Yes" : "No");
  xmlWriter->writeEndElement();
  return true;
}

bool WarperController::parseToolbarsDOM(const QDomElement &element) {
  if (element.tagName() != "Toolbars")
        return false;
  QDomNode child = element.firstChild();
  while (!child.isNull()) {
/*  Refresh state is not saved by the request of EO
       if (child.toElement().tagName() == "Refresh") {
         QString str = child.toElement().text().toUpper();
         mainWindow->actionAutoWarp->setChecked(str=="YES");
      } else*/ if (child.toElement().tagName() == "Mesh") {
         QString str = child.toElement().text().toUpper();
         mainWindow->actionShowMesh->setChecked(str=="YES");
      } else if (child.toElement().tagName() == "Add") {
         QString str = child.toElement().text().toUpper();
         mainWindow->actionAddLandmark->setChecked(str=="YES");
      } else if (child.toElement().tagName() == "Delete") {
         QString str = child.toElement().text().toUpper();
         mainWindow->actionDeleteLandmark->setChecked(str=="YES");
      } else if (child.toElement().tagName() == "Move") {
         QString str = child.toElement().text().toUpper();
         mainWindow->actionMoveLandmark->setChecked(str=="YES");
         landmarkController->setMove(str=="YES");
      } else if (child.toElement().tagName() == "Landmarks") {
         QString str = child.toElement().text().toUpper();
         mainWindow->actionShowLandmarks->setChecked(str=="YES");
      }
      child = child.nextSibling();
  }
  return true;
}


void WarperController::saveProject() {
  if (m_projectProperties->m_fileName=="") {
     saveProjectAs();
     return;
  }

  if (!m_projectProperties->m_isNameSet){
       setProjectProperties();
  }
  QFile file(m_projectProperties->m_fileName);
  if (!file.open(QFile::WriteOnly | QFile::Text)) {
    QMessageBox::warning(NULL, tr("Save project"), tr("Cannot create project file."));
    return;
  }
  QXmlStreamWriter xmlWriter(&file);
  xmlWriter.setAutoFormatting(true);
  xmlWriter.writeStartDocument();

  if (project3D)
     xmlWriter.writeStartElement(xmlTag3D);
  else
     xmlWriter.writeStartElement(xmlTag2D);

  if (m_projectProperties->m_isNameSet)
     xmlWriter.writeTextElement("Name", m_projectProperties->m_projectName);

  xmlWriter.writeTextElement("Version", "1.0");

  objectListModel->saveAsXml(&xmlWriter);

  m_objectViewerController->saveAsXml(&xmlWriter);

  landmarkModel->saveAsXml(&xmlWriter);
  landmarkModel->saveWarpingAsXml(&xmlWriter);
  saveXmlToolbars(&xmlWriter);
  xmlWriter.writeTextElement("UI", mainWindow->saveState(0).toHex());

  xmlWriter.writeEndElement();  // WarpingXXProject

  if (file.error()) {
    QMessageBox::warning(NULL, tr("Save project"), tr("Error writing file."));
    file.remove();
    return;
  }
}

void WarperController::saveProjectAs() {
  QString filename = QFileDialog::getSaveFileName(mainWindow, tr("Save project"),
                                                getLastPath(),
                                                tr("Warping project(*.wpr)"));
  if (filename.isEmpty())
     return;
  QString ext = QFileInfo(filename).suffix().toUpper();
  if (ext.isEmpty())
     filename += ".wpr";
  m_projectProperties->m_fileName = filename;
  mainWindow->actionSaveProject->setEnabled(true);

  setLastPath(filename);
  saveProject();
}


void WarperController::setProjectProperties() {
    ProjectPropertiesDialog *dialog = new ProjectPropertiesDialog(m_projectProperties);
    dialog->exec();
    mainWindow->setTitle(m_projectProperties->m_projectName);
}

bool WarperController::parseProject(const QDomElement &element) {
  {
    QDomElement child = element.firstChildElement("Name").toElement();
    if (!child.isNull()) {
        m_projectProperties->m_projectName = child.text();
        m_projectProperties->m_isNameSet = true;
    } else
        m_projectProperties->m_isNameSet = false;
  }
  undoStack->clear();
  config.setGlobalAutoUpdate(false);
  objectListModel->parseDOM(element.firstChildElement(ObjectListModel::xmlTag).toElement());
  m_objectViewerController->parseDOM(element.firstChildElement(ObjectViewerController::xmlTag));
  landmarkModel->parseDOM(element.firstChildElement(LandmarkModel::xmlTag));
  landmarkModel->parseWarpingDOM(element.firstChildElement(LandmarkModel::xmlTagWarping));
  landmarkModel->setMeshData(objectListModel->getMeshObject()); // syncronise meshes
  parseToolbarsDOM(element.firstChildElement("Toolbars"));

  mainWindow->setTitle(m_projectProperties->m_projectName);
  objectListModel->loadAll();
  config.setGlobalAutoUpdate(mainWindow->actionAutoWarp->isChecked());
  warpingWidget->update();

  // reads UI state
  {
    QDomElement child = element.firstChildElement("UI").toElement();
    if (!child.isNull()) {
        mainWindow->restoreState(QByteArray(QByteArray::fromHex(child.text().toAscii())));
    }
  }
  return true;
}

void WarperController::set3D(bool is3D) {
  project3D = is3D;
  projectInitialised = true;
  landmarkModel->set3D(project3D);
  mainWindow->actionEnableAlpha->setVisible(is3D);
  m_objectViewerController->set3D(project3D);
}

void WarperController::updateNow() {
  objectListModel->updateAll(true);
}

void WarperController::updatePossibleChanged(bool possible) {
  mainWindow->actionUpdate->setEnabled(possible);
}
