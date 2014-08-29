#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _WarperViewer_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         WarperViewer.cpp
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
* \brief        Viewer displaying source objects
* \ingroup      UI
*/

//Inventor includes
#include <Inventor/SoPickedPoint.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/SoPath.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <VolumeViz/nodes/SoVolumeRender.h>
#include <VolumeViz/nodes/SoObliqueSlice.h>
#include <Inventor/manips/SoClipPlaneManip.h>
#include <Inventor/nodes/SoPickStyle.h>

//Qt includes
#include <QAction>
#include <QSlider>

//specific
#include "CrossHairManip.h"
#include "WarperViewer.h"
#include "Viewer2D3D.h"
#include "LandmarkController.h"
#include "LandmarkView.h"
#include "ObjectView.h"
#include "WoolzObject.h"

// Constructors/Destructors

WarperViewer::
WarperViewer(
  ObjectViewerModel *objectViewerModel,
  bool is3D,
  LandmarkController* landmarkController,
  QAction * AddAction,
  QAction * DeleteAction,
  QAction * MoveAction,
  QAction * RemovelElemAction) :
ObjectViewer(
  objectViewerModel,
  is3D,
  true),
landmarkView(NULL),
m_landmarkController(landmarkController),
addAction(AddAction),
deleteAction(DeleteAction),
moveAction(MoveAction),
removeMeshElementAction(RemovelElemAction),
m_pickStyle(NULL),
m_clipPlane(NULL),
m_clipOn(true)
{
  setEditingMode();  // switch to editing mode
  if(m_clipLandmarkButton)
  {
    m_clipLandmarkButton->setVisible(true);
    connect(m_clipLandmarkButton, SIGNAL(clicked(bool)),
	    this, SLOT(buttonClipLandmarkClicked(bool)));
  }

  m_pickStyle = new SoPickStyle;
  m_pickStyle->style = SoPickStyle::SHAPE;
  m_pickStyle->ref();
  root->insertChild(m_pickStyle, 0);

  connect(addAction, SIGNAL(triggered()), this, SLOT(modeUpdated()));
  connect(deleteAction, SIGNAL(triggered()), this, SLOT(modeUpdated()));
}

WarperViewer::
~WarperViewer()
{
  if(m_clipPlane)
  {
    m_clipPlane->unref();
    m_clipPlane = NULL;
  }
  if(m_pickStyle)
  {
    m_pickStyle->unref();
    m_pickStyle = NULL;
  }
}

WlzDVertex3 WarperViewer::
covertSbVec3f2WlzDVertex3(
  const SbVec3f point)
{
  WlzDVertex3 v;
  v.vtX = point[0];
  v.vtY = point[1];
  v.vtZ = point[2];
  return(v);
}

void WarperViewer::
notifyMouseEvent(
  SoEventCallback * event)
{
  const SoMouseButtonEvent * mbe = (SoMouseButtonEvent *)event->getEvent();
  if(mbe->getButton() == SoMouseButtonEvent::BUTTON1 &&
      mbe->getState() == SoButtonEvent::DOWN)
  {

    SoRayPickAction rp(m_viewer->getViewportRegion());

    rp.setPoint(mbe->getPosition());
    // add mode?
    if(addAction->isChecked())
    {
      rp.apply(root);
      SoPickedPoint * point = rp.getPickedPoint();
      if(point == NULL)
      {
        return;
      }
      SoPath *path = point ->getPath();

      if(!path ->getTail()->isOfType(SoIndexedFaceSet::getClassTypeId()) &&
          !path ->getTail()->isOfType(SoFaceSet::getClassTypeId()) &&
          !path ->getTail()->isOfType(SoVolumeRender::getClassTypeId()) &&
          !path ->getTail()->isOfType(SoObliqueSlice::getClassTypeId())
        )
      {
        return;
      }
      addLandmark(covertSbVec3f2WlzDVertex3(point->getPoint()));
      event->setHandled();
    }
    // Remove element
    if(removeMeshElementAction->isChecked())
    {
      rp.apply(root);
      SoPickedPoint * point = rp.getPickedPoint();
      if(point == NULL)
      {
        return;
      }
      SoPath *path = point ->getPath();

      if(!path ->getTail()->isOfType(SoIndexedFaceSet::getClassTypeId()) || (0))
        return;
      emit removeMeshElementSignal(covertSbVec3f2WlzDVertex3(point->getPoint()));
      event->setHandled();
    }
    // Delete mode?
    if(deleteAction->isChecked())
    {
      rp.apply(root);
      SoPickedPoint * point = rp.getPickedPoint();
      if(point == NULL)
      {
        return;
      }
      SoPath *path = point ->getPath();

      if(!path ->getTail()->isOfType(LandmarkManip::getClassTypeId()))
      {
        return;
      }
      m_landmarkController->removeLandmark(
	  ((LandmarkManip *) path ->getTail())->pointPair);
      event->setHandled();
    }
  }
}

void WarperViewer::
setupLandmarkView(
  LandmarkModel::IndexType indexType)
{
  landmarkView = m_landmarkController->getLandmarkView(indexType);
  Q_ASSERT(landmarkView);
  // root
  //   - clip plane
  //   - pick style
  //   - landmarks

  SoSeparator * m_landmarks = new SoSeparator;
  m_landmarks ->ref();
  m_clipPlane = new SoClipPlane;
  m_clipPlane->ref();
  m_clipPlane->on = false;
  root->addChild(m_clipPlane);
  m_landmarks ->addChild(landmarkView->getSceneGraph());
  SoPickStyle *ps = new SoPickStyle;
  ps->style = SoPickStyle::SHAPE;
  root->addChild(ps);
  root->addChild(m_landmarks);
  connect(this, SIGNAL(addedClipPlane(SoClipPlane *)),
          this, SLOT(addedClipPlaneSlot(SoClipPlane *)));
}

void WarperViewer::
addedClipPlaneSlot(
  SoClipPlane * plane)
{
  if(!plane)
  {
    m_clipPlane->plane.disconnect();
    m_clipPlane->on = false;
  }
  else
  {
    m_clipPlane->plane.connectFrom(&plane->plane);
    m_clipPlane->on = m_clipOn;
  }
}

void WarperViewer::
buttonClipLandmarkClicked(
  bool on)
{
  m_clipPlane->on = on;
  m_clipOn = on;
}

void WarperViewer::
transparencyChanged(
  int transparency)
{
  for(int i = 0; i < views.size(); ++i)
  {
    views[i]->updateTransparency(transparency);
  }
}

void WarperViewer::
modeUpdated()
{
  m_pickStyle->style = (addAction->isChecked() || moveAction->isChecked())?
                       (SoPickStyle::SHAPE): (SoPickStyle::UNPICKABLE);
  m_pickStyle->style = (addAction->isChecked())?
                       (SoPickStyle::SHAPE): (SoPickStyle::UNPICKABLE);
}

void WarperViewer::
addObject(
  WoolzObject * object,
  bool doViewAll,
  ObjectView *previousView)
{
  ObjectViewer::addObject(object, doViewAll, previousView);
  if(accepting(object))
  {
    if(object->isMeshPreRead())
    {
      connect(this, SIGNAL(removeMeshElementSignal(const WlzDVertex3)),
	      object, SLOT(removeMeshElement(const WlzDVertex3)));
    }
  }
}
