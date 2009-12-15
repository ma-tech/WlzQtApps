#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ObjectViewer_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ObjectViewer.cpp
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
* \brief        Woolz object viewer.
* \ingroup      UI
*
*/

// project includes
#include "ObjectViewer.h"
#include "WoolzObject.h"
#include "ObjectView.h"
#include "ObjectView.h"
#include "ObjectViewerModel.h"
#include "Viewer3D.h"
#include "Viewer2D.h"
#include "Viewer2D3D.h"
#include "Camera2D.h"
#include "ClipPlaneButton.h"
#include "Mesh3DView.h"
#include "Contour3DView.h"
#include "VolumeView.h"
#include "Mesh2DView.h"
#include "Contour2DView.h"
#include "ImageView.h"
#include "EmptyView.h"

// Inventor includes
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/manips/SoClipPlaneManip.h>
#include <VolumeViz/nodes/SoTransferFunction.h>
#include <Inventor/nodes/SoClipPlane.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>

// Qt includes
#include <QAction>
#include <QMdiArea>
#include <QSizePolicy>
#include <QMdiSubWindow>
#include <QList>
#include <QCheckBox>
#include <QSlider>

#include <QtXml/QXmlStreamWriter>
#include <QtXml/QDomElement>

ObjectViewer::ObjectViewer (ObjectViewerModel *objectViewerModel, bool is3D, bool isBlending) :
        ObjectSimpleViewer(is3D, isBlending), m_camera(NULL), m_sensor(NULL), m_isLinked(false), m_linkedTo(NULL) {
    if (is3D)
       m_camera = new SoPerspectiveCamera;
    else
       m_camera = new Camera2D;

    // set camera viewpoint to match Woolz viewing point
    m_camera->position = SbVec3f(0,0,-1);
    m_camera->pointAt(SbVec3f(0,0,0), SbVec3f(0,-1,0));
    m_camera->ref();

    root->insertChild(m_camera, 1);// has to be added before view root
    m_viewer->setCamera(m_camera);

   if (m_sensor) {  //make sure no sensor was allocated
     m_sensor->detach();
     delete m_sensor;
     m_sensor = NULL;
    }
    m_sensor = new SoNodeSensor(cameraCB, this);
    m_sensor->attach(m_camera);

    //this has to go in the Editor
    connect(this, SIGNAL(removedViewerStart()), objectViewerModel, SLOT(removedViewerStart()));
    connect(this, SIGNAL(removedViewerFinished()), objectViewerModel, SLOT(removedViewerFinished()));
    connect(this, SIGNAL(addedView(ObjectView *)), objectViewerModel, SLOT(addedView(ObjectView *)));
    connect(this, SIGNAL(changedViewer()), objectViewerModel, SLOT(changedViewer()));

    connect(objectViewerModel, SIGNAL(setBackgroundColour()), this, SLOT(setBackgroundColour()));
}

ObjectViewer::~ObjectViewer ( ) { 
  if (m_sensor)
     delete m_sensor;
  if (m_camera)
    m_camera->unref(); // remove camera and sensor;
}

void ObjectViewer::setLinkedTo ( ObjectViewer * new_var )   {
   if (new_var != m_linkedTo) {
     if (m_linkedTo)
        disconnect( m_linkedTo, SIGNAL( cameraChanged() ), this, SLOT( cameraChangedIn() ) );

     m_linkedTo = new_var;
     if (m_linkedTo) {
        connect( m_linkedTo, SIGNAL( cameraChanged() ), this, SLOT( cameraChangedIn() ) );
        updateCameraLink();
        emit changedViewer();
     }
   }
}

void ObjectViewer::updateCameraLink() {
  Q_ASSERT(m_linkedTo != this);
  Q_ASSERT(m_linkedTo);
  if (m_linkedTo) {
    m_viewer->changeCameraValues(m_linkedTo->m_camera);
  }
}

void ObjectViewer::cameraCB(void *data, SoSensor *) {
   Q_ASSERT(data);
   if (data)
     ((ObjectViewer*)data)->notifyCameraChanged();
}

void ObjectViewer::notifyCameraChanged() {
  emit cameraChanged();
}

void ObjectViewer::cameraChangedIn() {
  ObjectViewer * senderView = qobject_cast<ObjectViewer*>(sender());
  if (senderView) 
  {  if (!(m_camera->fieldsAreEqual (senderView->m_camera)))
       m_viewer->changeCameraValues(senderView->m_camera);
  }
}

void ObjectViewer::replacedCamera ( SoCamera *cam ) {
   if (m_camera) {
     m_camera->unref();
   }
   m_camera = cam;

   if (m_sensor) {
     m_sensor->detach();
     delete m_sensor;
     m_sensor = NULL;
    }

   if (m_camera) {
     m_camera->ref();
     m_sensor = new SoNodeSensor(cameraCB, this);
     m_sensor->attach(m_camera);
   }
}

bool ObjectViewer::saveAsXml(QXmlStreamWriter *xmlWriter) {
  Q_ASSERT(xmlWriter);
  xmlWriter->writeTextElement("Linked", m_isLinked ? "Yes":"No");
  /*if (m_isLinked)
    xmlWriter->writeTextElement("LinkedTo", "NA");*/

  ObjectSimpleViewer::saveAsXml(xmlWriter);
  return true;
}

bool ObjectViewer::parseDOMLine(const QDomElement &element) {
    if (element.tagName() == "Linked") {
         QString str = element.text().toUpper();
         m_isLinked = str == "YES";
        return true;
    } else return ObjectSimpleViewer::parseDOMLine(element);
    return false;
}

void ObjectViewer::alphaChanged(bool alpha) {
  if (!m_viewer)
      return;

   if (alpha)
       m_viewer->setTransparencyType(SoGLRenderAction::SORTED_LAYERS_BLEND);
   else
       m_viewer->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_BLEND);

   m_viewer->setAlphaChannel(alpha);
}
