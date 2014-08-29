#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _ObjectViewerController_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         ObjectViewerController.cpp
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
* \brief        Model managing the viewers and views.
* \ingroup      Control
*/

//project includes
#include "ObjectViewerController.h"
#include "ObjectViewerModel.h"
#include "WoolzObject.h"
#include "ObjectView.h"
//#include "View.h"
//#include "Mesh3DView.h"
#include "ObjectViewer.h"
#include "MainWindow.h"
#include "WarperSourceViewer.h"
#include "WarperTargetViewer.h"
#include "WarperResultViewer.h"
#include "ObjectViewerModel.h"

//Inventor includes
#include <Inventor/nodes/SoSeparator.h>

//Qt includes
#include <QAction>
#include <QMdiSubWindow>
#include <QMdiArea>
#include <QtXml/QXmlStreamWriter>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtXml/QDomText>

const char* ObjectViewerController::xmlTag = "Viewers";

ObjectViewerController::
ObjectViewerController(
  MainWindow * mainWindow,
  LandmarkController *landmarkController,
  QObject * parent):
QObject(parent),
m_mainWindow(mainWindow),
m_landmarkController(landmarkController),
lastSourceViewer(NULL),
lastTargetViewer(NULL),
lastResultViewer(NULL),
sourceViewerCounter(0),
targetViewerCounter(0),
resultViewerCounter(0)
{
  m_objectViewerModel = new ObjectViewerModel(mainWindow->getWorkspace());
  Q_ASSERT(m_objectViewerModel);
  m_menuWindowActionGroup = new QActionGroup(this);
  Q_ASSERT(m_menuWindowActionGroup);
}


ObjectViewerController::
~ObjectViewerController()
{
  if(m_menuWindowActionGroup)
  {
    delete m_menuWindowActionGroup;
  }
  if(m_objectViewerModel)
  {
    delete m_objectViewerModel;
  }
}

bool ObjectViewerController::
saveAsXml(
  QXmlStreamWriter *xmlWriter)
{
  Q_ASSERT(xmlWriter);
  model()->prepareIDs();
  xmlWriter->writeStartElement(xmlTag);
  QList<QMdiSubWindow *> windows = 
      m_mainWindow->getWorkspace()->subWindowList();

  for(int i = 0; i < windows.size(); i++)
  {
    QMdiSubWindow *window = windows.at(i);
    if(window)
    {
      ObjectViewer *viewer = qobject_cast<ObjectViewer*>(window->widget());
      if(viewer)
      {
        viewer->saveAsXml(xmlWriter);
      }
      else
      {
        Q_ASSERT(false);
        return(false);
      }
    }
    else
    {
      Q_ASSERT(false);
      return(false);
    }
  }
  xmlWriter->writeEndElement();
  return(true);
}

bool ObjectViewerController::
parseDOM(
  const QDomElement &element)
{
  if(element.tagName() != ObjectViewerController::xmlTag)
  {
    return false;
  }
  QDomNode child = element.firstChild();
  while(!child.isNull())
  {
    QString tag = child.toElement().tagName();
    if(tag == WarperSourceViewer::xmlTag)
    {
      addSourceViewer(true);
      lastSourceViewer ->parseDOM(child.toElement());
    }
    else if(tag == WarperTargetViewer::xmlTag)
    {
      addTargetViewer(true);
      lastTargetViewer->parseDOM(child.toElement());
    }
    else if(tag == WarperResultViewer::xmlTag)
    {
      addResultViewer(true);
      lastResultViewer->parseDOM(child.toElement());
    };
    child = child.nextSibling();
  }
  model()->restoreLinks();
  return(true);
}

void ObjectViewerController::
addSourceViewer(
  bool force)
{
  if(!force && lastSourceViewer)
  {
    return;
  }

  WarperSourceViewer *sourceViewer =
      new WarperSourceViewer(m_objectViewerModel, m_is3D,
	  		     m_landmarkController,
      			     m_mainWindow->actionAddLandmark,
			     m_mainWindow->actionDeleteLandmark,
			     m_mainWindow->actionMoveLandmark,
			     m_mainWindow->actionRemoveMeshElement);

  Q_ASSERT(sourceViewer);
  sourceViewer->init();

  connect(m_mainWindow->actionEnableAlpha, SIGNAL(toggled(bool)),
          sourceViewer, SLOT(alphaChanged(bool)));

  //set title and minum size
  sourceViewerCounter++;
  if(sourceViewerCounter > 1)
  {
    sourceViewer->setWindowTitle(QString("Source view - %1").
	arg(sourceViewerCounter));
  }
  else
  {
    sourceViewer->setWindowTitle(QString("Source view"));
  }

  setupObjectViewer(sourceViewer); 	//set up viewer

  lastSourceViewer = sourceViewer; 	// from now on this is the last
  					// access source viewer
  m_objectViewerModel->addedViewer(sourceViewer); //remove to be signal
}


void ObjectViewerController::
addTargetViewer(
  bool force)
{
  if(!force && lastTargetViewer)
  {
    return;
  }

  WarperTargetViewer *targetViewer = new
      WarperTargetViewer(m_objectViewerModel, m_is3D,
	  		 m_landmarkController,
			 m_mainWindow->actionAddLandmark,
			 m_mainWindow->actionDeleteLandmark,
			 m_mainWindow->actionMoveLandmark,
			 m_mainWindow->actionRemoveMeshElement);

  Q_ASSERT(targetViewer);
  targetViewer->init();
  connect(m_mainWindow->actionEnableAlpha, SIGNAL(toggled(bool)),
          targetViewer, SLOT(alphaChanged(bool)));

  //set title and minum size
  targetViewerCounter++;
  if(targetViewerCounter > 1)
  {
    targetViewer->setWindowTitle(QString("Target view - %1").
	arg(targetViewerCounter));
  }
  else
  {
    targetViewer->setWindowTitle(QString("Target view"));
  }

  setupObjectViewer(targetViewer); 	//set up viewer

  lastTargetViewer = targetViewer; 	// from now on this is the last
  					// access source viewer
  // emit addedViewer(targetViewer);	//signal that new viewer was added
}

void ObjectViewerController::
addResultViewer(
  bool force)
{
  if(!force && lastResultViewer)
  {
    return;
  }

  WarperResultViewer *resultViewer =
      new WarperResultViewer(m_objectViewerModel, m_is3D);

  Q_ASSERT(resultViewer);
  resultViewer->init();

  connect(m_mainWindow->actionEnableAlpha, SIGNAL(toggled(bool)),
          resultViewer, SLOT(alphaChanged(bool)));

  //set title and minum size
  resultViewerCounter++;
  if(resultViewerCounter > 1)
  {
    resultViewer->setWindowTitle(QString("Result view - %1").
	arg(resultViewerCounter));
  }
  else
  {
    resultViewer->setWindowTitle(QString("Result view"));
  }

  setupObjectViewer(resultViewer); 	// set up viewer
  lastResultViewer = resultViewer; 	// from now on this is the last
  					// access source viewer
  m_objectViewerModel->addedViewer(resultViewer); //remove to be signal
}

void ObjectViewerController::
setupObjectViewer(
  ObjectViewer *viewer)
{
  viewer->setMinimumSize(300, 300);

  m_menuWindowActionGroup->addAction(viewer->activateAction());
  m_mainWindow->menuWindow->addAction(viewer->activateAction());
  m_mainWindow->addMDIWindows(viewer);

  connect(this, SIGNAL(addObjectSignal(WoolzObject*)),
          viewer, SLOT(addObject(WoolzObject*)));
  connect(this, SIGNAL(removedObjectSignal(WoolzObject*)),
          viewer, SLOT(removedObject(WoolzObject*)));

  //connect listening to object changes
  connect(viewer, SIGNAL(changedObjectView(ObjectView *)),
          m_objectViewerModel, SLOT(changedObjectView(ObjectView *)));
  connect(viewer, SIGNAL(removedViewStart(ObjectView *)),
          m_objectViewerModel, SLOT(removedViewStart(ObjectView *)));
  connect(viewer, SIGNAL(removedViewFinish(ObjectView *)),
          m_objectViewerModel, SLOT(removedViewFinish(ObjectView *)));

  // connect model switching actions
  connect(this, SIGNAL(setEditingMode()), viewer, SLOT(setEditingMode()));
  connect(this, SIGNAL(setViewingMode()), viewer, SLOT(setViewingMode()));
  connect(this, SIGNAL(switchModes()), viewer, SLOT(switchModes()));

  signalRequestAllObjects(viewer);
}


ObjectViewer* ObjectViewerController::
activeWindow()
{
  QMdiSubWindow *subWindow =
      m_mainWindow->getWorkspace()->activeSubWindow();
  if(subWindow)
  {
    return(qobject_cast <ObjectViewer*> (subWindow->widget()));
  }
  return(NULL);
}

void ObjectViewerController::
updateActions()
{
  ObjectViewer* aw = activeWindow();
  if(aw)
  {
    activeWindow()->activateAction()->setChecked(true);
    if(qobject_cast <WarperSourceViewer*> (aw) != NULL)
    {
      lastSourceViewer = qobject_cast <WarperSourceViewer*> (aw);
    }
    if(qobject_cast <WarperTargetViewer*> (aw) != NULL)
    {
      lastTargetViewer = qobject_cast <WarperTargetViewer*> (aw);
    }
    if(qobject_cast <WarperResultViewer*> (aw) != NULL)
    {
      lastResultViewer = qobject_cast <WarperResultViewer*> (aw);
    }
  }
}

void ObjectViewerController::
defaultLayout()
{
  QSize size = m_mainWindow->getWorkspace()->size();
  size.setWidth(size.width() / 3);
  if(lastSourceViewer)
  {
    // MDI window, not the viewer
    QWidget *window = (QWidget *)lastSourceViewer->parent();
    window->showNormal();
    window->move(0, 0);
    window->resize(size);
  }
  if(lastTargetViewer)
  {
    // MDI window, not the viewer
    QWidget *window = (QWidget *)lastTargetViewer->parent();
    window->showNormal();
    window->move(size.width(), 0);
    window->resize(size);
  }
  if(lastResultViewer)
  {
    // MDI window, not the viewer
    QWidget *window = (QWidget *)lastResultViewer->parent();
    window->showNormal();
    window->move(size.width() * 2, 0);
    window->resize(size);
  }
}

void ObjectViewerController::
defaultViewLinks()
{
  if(lastSourceViewer && lastTargetViewer)
  {
    lastSourceViewer->setLinkedTo(lastTargetViewer);
    lastTargetViewer->setLinkedTo(lastSourceViewer);
  }
  if(lastSourceViewer && lastResultViewer)
  {
    lastResultViewer->setLinkedTo(lastSourceViewer);
  }
  else if(lastTargetViewer && lastResultViewer)
  {
    lastResultViewer->setLinkedTo(lastTargetViewer);
  }
}

void ObjectViewerController::
checkToSetupDefault()
{
  if((resultViewerCounter == 0) && (sourceViewerCounter == 1) &&
     (targetViewerCounter == 1))
  {
    addResultViewer(true);
    defaultLayout();
  }
}
