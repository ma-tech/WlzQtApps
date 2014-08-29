#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _MainWindow_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         MainWindow.cpp
* \author       Zsolt Husz
* \date         October 2008
* \version      $Id$
* \par
* Address:
*               MRC Human Genetics Unit,
*               MRC Institute of Genetics and Molecular Medicine,
*               University of Edinburgh,
*               Western General Hospital,
*               Edinburgh, EH4 2XU, UK.
* \par
* Copyright (C), [2014],
* The University Court of the University of Edinburgh,
* Old College, Edinburgh, UK.
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
*/

// Qt4 includes
#include <QtGui>
#include <QTextEdit>
#include <QFileDialog>
#include <QString>
#include <QMdiArea>
#include <QtXml/QDomDocument>

// application includes
#include "MainWindow.h"
#include "WoolzObject.h"
#include "WarperController.h"
#include "TranslateRadialManip.h"
#include "CrossHairManip.h"
#include "SnapSurfaceManip.h"
#include "PreferencesDialog.h"
#include "WarperConfig.h"

//SoQt includes
#include <Inventor/Qt/SoQt.h>
#include <VolumeViz/nodes/SoVolumeRendering.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>

MainWindow::
MainWindow():
controler(NULL),
separatorAct(NULL)
{
  SoQt::init(this);
  SoVolumeRendering::init();

  for(int i = 0; i < MaxRecentFiles; ++i)
  {
    recentFileActs[i] = NULL;
  }

  SnapSurfaceManip::initClass();
  TranslateRadialManip::initClass();
  CrossHairManip::initClass();

  setupUi(this); // this sets up GUI
  switchProjectType(ProjectProperties::noneProject);
  setCentralWidget(mdiArea);
  showMaximized();

  /*
   * If all SoQt windows are closed, then probably SoQt frees up some
   * local areas. This causes a segmentation fault after a new SoQt
   * viewer is opened. To avoid this problem, here a new examiner viewer
   * is created that is not freed during the whole lifespan of the
   * application, until the application is closed.
  */
  new SoQtExaminerViewer;

  //Menu global menu signals
  connect(actionAbout, SIGNAL(triggered()),
          this, SLOT(about())); 
  connect(actionCloseProject, SIGNAL(triggered()),
          this, SLOT(closeProject())); 
  connect(actionExit, SIGNAL(triggered()),
          this, SLOT(close())); 

  setDefaultConnections();

  //set up toolbars in menus
  menuToolbars->addAction(toolBarWarpingMode->toggleViewAction());
  menuToolbars->addAction(toolBarGeneral->toggleViewAction());
  menuToolbars->addAction(toolBarModes->toggleViewAction());
  menuToolbars->addAction(toolBarOperations->toggleViewAction());

  toolBarModes->setVisible(true);
  toolBarModes->setVisible(false);
  toolBarWarpingMode->setVisible(false);
  toolBarOperations->setVisible(false);

  menuFile->removeAction(menuOpen_project->menuAction());
  statusbar->showMessage("Application started", 5000);

  for(int i = 0; i < MaxRecentFiles; ++i)
  {
    recentFileActs[i] = new QAction(this);
    recentFileActs[i]->setVisible(false);
    connect(recentFileActs[i], SIGNAL(triggered()),
	    this, SLOT(openRecentFile()));
  }
  for(int i = 0; i < MaxRecentFiles; ++i)
  {
    menuFile->insertAction(actionExit,recentFileActs[i]);
  }
  separatorAct = menuFile->insertSeparator(actionExit);
  updateRecentFileActions();
}

void MainWindow::
setDefaultConnections()
{
  connect(actionOpenValueTarget, SIGNAL(triggered()),
          this, SLOT(openValueTarget()));
  connect(actionOpenMeshTarget, SIGNAL(triggered()),
          this, SLOT(openMeshTarget()));
  connect(actionOpenSurfaceTarget, SIGNAL(triggered()),
          this, SLOT(openSurfaceTarget()));
  connect(actionOpenCompositeTarget, SIGNAL(triggered()),
          this, SLOT(openCompositeTarget()));
  connect(actionOpenValueSource, SIGNAL(triggered()),
          this, SLOT(openValueSource()));
  connect(actionOpenSurfaceSource, SIGNAL(triggered()),
          this, SLOT(openSurfaceSource()));
  connect(actionOpenMeshSource, SIGNAL(triggered()),
          this, SLOT(openMeshSource()));

  connect(actionTile, SIGNAL(triggered()),
          mdiArea, SLOT(tileSubWindows())); 
  connect(actionCascade, SIGNAL(triggered()),
          mdiArea, SLOT(cascadeSubWindows())); 

  connect(actionOptions, SIGNAL(triggered()),
          this, SLOT(options()));
  connect(actionOpenProject, SIGNAL(triggered()),
          this, SLOT(openProject()));
}

void MainWindow::
removeDefaultConnections()
{
  disconnect(actionOpenValueTarget, SIGNAL(triggered()),
             this, SLOT(openValueTarget())); 
  disconnect(actionOpenMeshTarget, SIGNAL(triggered()),
             this, SLOT(openMeshTarget()));
  disconnect(actionOpenSurfaceTarget, SIGNAL(triggered()),
             this, SLOT(openSurfaceTarget()));
  disconnect(actionOpenCompositeTarget, SIGNAL(triggered()),
             this, SLOT(openCompositeTarget()));
  disconnect(actionOpenValueSource, SIGNAL(triggered()),
             this, SLOT(openValueSource()));
  disconnect(actionOpenSurfaceSource, SIGNAL(triggered()),
             this, SLOT(openSurfaceSource()));
  disconnect(actionOpenMeshSource, SIGNAL(triggered()),
             this, SLOT(openMeshSource()));
  disconnect(actionOptions, SIGNAL(triggered()),
             this, SLOT(options()));
  disconnect(actionOpenProject, SIGNAL(triggered()),
             this, SLOT(openProject()));
}

void MainWindow::
openValueTarget()
{
  controler = new WarperController(this, &projectSettings);
  checkAndSwitch(controler->openValueTarget(),
                 ProjectProperties::warpingProject);
}

void MainWindow::
openMeshTarget()
{
  controler = new WarperController(this, &projectSettings);
  checkAndSwitch(controler->openMeshTarget(),
                 ProjectProperties::warpingProject);
}

void MainWindow::
openSurfaceTarget()
{
  controler = new WarperController(this, &projectSettings);
  checkAndSwitch(controler->openSurfaceTarget(), ProjectProperties::warpingProject);
}

void MainWindow::
openCompositeTarget()
{
  controler = new WarperController(this, &projectSettings);
  checkAndSwitch(controler->openCompositeTarget(),
                 ProjectProperties::warpingProject);
}

void MainWindow::
openValueSource()
{
  controler = new WarperController(this, &projectSettings);
  checkAndSwitch(controler->openValueSource(),
                 ProjectProperties::warpingProject);
}

void MainWindow::
openMeshSource()
{
  controler = new WarperController(this, &projectSettings);
  checkAndSwitch(controler->openMeshSource(),
                 ProjectProperties::warpingProject);
}


void MainWindow::
openSurfaceSource()
{
  controler = new WarperController(this, &projectSettings);
  checkAndSwitch(controler->openSurfaceSource(),
                 ProjectProperties::warpingProject);
}


void MainWindow::
checkAndSwitch(
  int result,
  ProjectProperties::ProjectTypes type) 
{
  Q_ASSERT(controler);
  if(controler)
  {
    if(result)
    {
      // if success
      controler->init();
      removeDefaultConnections();
      switchProjectType(type);
    }
    else
    { 
      // if fail
      delete controler;
      controler = NULL;
    }
  }
}

// Is this node of a type that is influenced by transforms?
void MainWindow::
switchProjectType(
  ProjectProperties::ProjectTypes newType)
{
  projectSettings.projectType = newType;
  switch (newType)
  {
    case ProjectProperties::noneProject:
      // File menu
      actionSaveLandmarks->setVisible(false);
      actionExportWarpedObject->setVisible(false);
      menuExport->menuAction()->setVisible(false);
      actionProjectProperties->setVisible(false);
      actionSeparator->setVisible(false);
      actionCloseProject->setVisible(false);
      actionLoadLandmarks->setVisible(false);
      actionOpenProject->setVisible(true);
      actionSaveProject->setVisible(false);
      actionSaveProjectAs->setVisible(false);
      // Export menu
      actionExportWarpTransform->setVisible(false);
      actionExportWarpedObject->setVisible(false);
      actionSaveLandmarks->setVisible(false);
      actionExportMesh->setVisible(false);
      // Edit menu
      actionGenerateMesh->setVisible(false);
      // Actions menu
      actionMove_vetex->setVisible(false);
      actionAdd_vertex->setVisible(false);
      actionDelete_vertex->setVisible(false);
      actionAdd_element->setVisible(false);
      actionDelete_element->setVisible(false);
      actionCut_mesh->setVisible(false);
      actionAddLandmark->setVisible(false);
      actionDeleteLandmark->setVisible(false);
      actionMoveLandmark->setVisible(false);
      actionShowLandmarks->setVisible(false);
      actionUpdate->setVisible(false);
      // View menu
      actionSourceView->setVisible(false);
      actionTargetView->setVisible(false);
      actionResultView->setVisible(false);
      actionSectionView->setVisible(false);
      menuToolbars->setVisible(false);
      //Settings menu  
      actionAutoWarp->setVisible(true);
      actionShowMesh->setVisible(false);
      // Memu bar
      menubar->clear();
      menubar->addAction(menuFile->menuAction());
      menubar->addAction(menuHelp->menuAction());
      break;
    case ProjectProperties::mesheditProject:
      // File menu
      actionOpenProject->setVisible(false);
      actionSaveLandmarks->setVisible(false);
      actionExportWarpedObject->setVisible(false);
      menuExport->menuAction()->setVisible(false);
      actionProjectProperties->setVisible(true);
      actionSeparator->setVisible(true);
      actionSeparator->setSeparator(true);
      actionCloseProject->setVisible(true);
      actionLoadLandmarks->setVisible(false);
      actionSaveProject->setVisible(true);
      actionSaveProjectAs->setVisible(true);
      // Export menu
      actionExportWarpTransform->setVisible(false);
      actionExportWarpedObject->setVisible(false);
      actionSaveLandmarks->setVisible(false);
      actionExportMesh->setVisible(true);
      // Edit menu
      actionGenerateMesh->setVisible(true);
      // Actions menu
      actionMove_vetex->setVisible(true);
      actionAdd_vertex->setVisible(true);
      actionDelete_vertex->setVisible(true);
      actionAdd_element->setVisible(true);
      actionDelete_element->setVisible(true);
      actionCut_mesh->setVisible(true);
      actionAddLandmark->setVisible(false);
      actionDeleteLandmark->setVisible(false);
      actionMoveLandmark->setVisible(false);
      actionShowLandmarks->setVisible(false);
      actionUpdate->setVisible(false);
      // View menu
      actionSourceView->setVisible(false);
      actionTargetView->setVisible(false);
      actionResultView->setVisible(false);
      actionSectionView->setVisible(false);
      //Settings menu  
      actionAutoWarp->setVisible(true);
      actionShowMesh->setVisible(false);
      // Memu bar
      menubar->clear();
      menuActions->setVisible(false);
      menubar->addAction(menuFile->menuAction());
      menubar->addAction(menuEdit->menuAction());
      menubar->addAction(menuActions->menuAction());
      menubar->addAction(menuView->menuAction());
      menubar->addAction(menuWindow->menuAction());
      menubar->addAction(menuHelp->menuAction());
      break;
    case ProjectProperties::warpingProject:
      // File menu
      actionSaveLandmarks->setVisible(true);
      actionExportWarpedObject->setVisible(true);
      menuExport->menuAction()->setVisible(true);
      actionProjectProperties->setVisible(true);
      actionLoadLandmarks->setVisible(true);
      actionOpenProject->setVisible(false);
      actionSeparator->setVisible(true);
      actionSeparator->setSeparator(true);
      actionCloseProject->setVisible(true);
      actionSaveProject->setVisible(true);
      actionSaveProjectAs->setVisible(true);
      // Export menu
      actionExportWarpTransform->setVisible(true);
      actionExportWarpedObject->setVisible(true);
      actionSaveLandmarks->setVisible(true);
      actionExportMesh->setVisible(false);
      // Edit menu
      actionGenerateMesh->setVisible(true);
      // Actions menu
      actionMove_vetex->setVisible(false);
      actionAdd_vertex->setVisible(false);
      actionDelete_vertex->setVisible(false);
      actionAdd_element->setVisible(false);
      actionDelete_element->setVisible(false);
      actionCut_mesh->setVisible(false);
      actionAddLandmark->setVisible(true);
      actionDeleteLandmark->setVisible(true);
      actionMoveLandmark->setVisible(true);
      actionShowLandmarks->setVisible(true);
      actionUpdate->setVisible(true);
      // View menu
      actionSourceView->setVisible(true);
      actionTargetView->setVisible(true);
      actionResultView->setVisible(true);
      actionSectionView->setVisible(false);
      //Settings menu  
      actionAutoWarp->setVisible(true);
      actionShowMesh->setVisible(true);
      // Memu bar
      menubar->clear();
      menuActions->setVisible(false);
      menubar->addAction(menuFile->menuAction());
      menubar->addAction(menuEdit->menuAction());
      menubar->addAction(menuActions->menuAction());
      menubar->addAction(menuView->menuAction());
      menubar->addAction(menuWindow->menuAction());
      menubar->addAction(menuHelp->menuAction());
      break;
    default:
      Q_ASSERT(FALSE);
      break;
  }
  updateRecentFileActions();
}

void MainWindow::
about()
{
  QMessageBox::about(this, "About Woolz Warp",
      "<h3>Woolz Warp Interface</h3>"
      "<p>ma-tech@hgu.mrc.ac.uk<p>"
      "<p>Copyright (C) 2014 University of Edinburgh<p>");
}

void MainWindow::
closeProject()
{
  if(controler)
  {
    delete controler;
    controler = NULL;
  }
  switchProjectType(ProjectProperties::noneProject);
  setDefaultConnections();
  projectSettings.reset();
  setTitle("");
}

void MainWindow::
addMDIWindows(
  QWidget * window)
{
  // mdiArea->setActiveSubWindow(0);    // this used to be required as a bug
  					// fix to avoid getActiveSubWindow
					// returning nil

  QMdiSubWindow *subWindow=mdiArea->addSubWindow(window);
  Q_ASSERT(subWindow);
  if (subWindow) {
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    QSize size = subWindow->size();
    size.setHeight(mdiArea->height());
    subWindow->resize(size);
    subWindow->show();

    mdiArea->setActiveSubWindow(subWindow);
  }
}

MainWindow::
~MainWindow()
{
  mdiArea->closeAllSubWindows();
  if(controler)
  {
    delete controler;
  }
}

void MainWindow::
options()
{
  PreferencesDialog *dialog=new PreferencesDialog(NULL);
  dialog->exec();
  delete dialog;
}


void MainWindow::
openProject()
{
  QString filename = QFileDialog::getOpenFileName(this, tr("Open project"),
      "", tr("Warping project(*.wpr)"));
  if(filename.isEmpty())
  {
    return;
  }
  QString ext = QFileInfo(filename).suffix().toUpper();
  if(ext.isEmpty())
  {
    filename += ".wpr";
  }
  loadProject(filename);
}

void MainWindow::
loadProject(
  QString filename)
{
  setCurrentFile(filename);
  QApplication::setOverrideCursor(Qt::WaitCursor);
  QFile file(filename);
  if(!file.open(QFile::ReadOnly | QFile::Text))
  {
    QMessageBox::warning(NULL, "Load project", "Cannot open project file.");
    QApplication::restoreOverrideCursor();
    return;
  }
  config.setProjectDirFromFile(filename);

  QDomDocument doc;
  QString errorStr;
  int errorLine, errorColumn;
  if(!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
  {
    QMessageBox::warning(NULL, "Load project",
        QString("Parse error at line %1 column %2 :").arg(errorLine).
	        arg(errorColumn) + errorStr);
    QApplication::restoreOverrideCursor();
    return;
  }
  QDomElement root = doc.documentElement();
  if(root.tagName() == WarperController::xmlTag2D)
  {
    controler = new WarperController(this, &projectSettings);
    controler->set3D(false);
    bool success=controler->parseProject(root);
    projectSettings.m_fileName = filename;
    actionSaveProject->setEnabled(true);
    checkAndSwitch(success, ProjectProperties::warpingProject);
  }
  else if(root.tagName() == WarperController::xmlTag3D)
  {
    controler = new WarperController(this, &projectSettings);
    controler->set3D(true);
    bool success=controler->parseProject(root);
    projectSettings.m_fileName = filename;
    actionSaveProject->setEnabled(true);
    checkAndSwitch(success, ProjectProperties::warpingProject);
  }
  else
  {
    QMessageBox::warning(NULL, "Load project",
                         QString("Not a warping project"));
  }
  QApplication::restoreOverrideCursor();
  file.close();
  return;
}

void MainWindow::
setTitle(
  QString additionalTitle)
{
  if(additionalTitle!="")
  {
    additionalTitle = " - "+additionalTitle;
  }
  setWindowTitle("Woolz Warping" + additionalTitle);
}

void MainWindow::
statusChanged(
  QString message,
  int timeout)
{
  statusBar()->showMessage(message, timeout);
  statusBar()->repaint();
}

void MainWindow::
openRecentFile()
{
  QAction *action = qobject_cast<QAction *>(sender());
  if(action)
  {
    loadProject(action->data().toString());
  }
}

void MainWindow::
setCurrentFile(
  const QString &fileName)
{
  QSettings settings;
  QStringList files = settings.value("recentFileList").toStringList();
  files.removeAll(fileName);
  files.prepend(fileName);
  while(files.size() > MaxRecentFiles)
  {
    files.removeLast();
  }

  settings.setValue("recentFileList", files);

  foreach(QWidget *widget, QApplication::topLevelWidgets())
  {
    MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
    if(mainWin)
    {
      mainWin->updateRecentFileActions();
    }
  }
}

void MainWindow::
updateRecentFileActions()
{
  QSettings settings;
  QStringList files = settings.value("recentFileList").toStringList();

  int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

  if(projectSettings.projectType != ProjectProperties::noneProject)
  {
    numRecentFiles  = 0;
  }
  for(int i = 0; i < numRecentFiles; ++i)
  {
    QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
    if(recentFileActs[i])
    {
      recentFileActs[i]->setText(text);
      recentFileActs[i]->setData(files[i]);
      recentFileActs[i]->setVisible(true);
    }
  }

  for(int i = numRecentFiles; i < MaxRecentFiles; ++i)
  {
    if(recentFileActs[i])
    {
      recentFileActs[i]->setVisible(false);
    }
  }

  if(separatorAct)
  {
    separatorAct->setVisible(numRecentFiles > 0);
  }
}

QString MainWindow::
strippedName(
  const QString &fullFileName)
{
  return(QFileInfo(fullFileName).fileName());
}
