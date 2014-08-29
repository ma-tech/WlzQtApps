#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _WarperTargetViewer_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         WarperTargetViewer.cpp
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
* \brief        Viewer displaying targer objects
* \ingroup      UI
*/

#include "WarperTargetViewer.h"
#include "Viewer2D3D.h"
#include "WoolzObject.h"
#include <QSettings>

const char* WarperTargetViewer::xmlTag = "TargetViewer";

WarperTargetViewer::
WarperTargetViewer(
  ObjectViewerModel *objectViewerModel,
  bool is3D,
  LandmarkController* landmarkController,
  QAction * AddAction,
  QAction * DeleteAction,
  QAction * MoveAction,
  QAction * RemovelElemAction):
WarperViewer(
  objectViewerModel,
  is3D,
  landmarkController,
  AddAction,
  DeleteAction,
  MoveAction,
  RemovelElemAction)
{
}

QColor WarperTargetViewer::
getBackgroundColour()
{
  QSettings settings;
  return settings.value("viewer/target/color",
                        QColor(0, 51, 77)).value<QColor>();
}

void WarperTargetViewer::
init()
{
  setupLandmarkView(LandmarkModel::targetV);
  setBackgroundColour();
}

bool WarperTargetViewer::
accepting(
  WoolzObject * object)
{
  // accept target objects only
  return(object && (object->type()& WoolzObject::target));
}

void WarperTargetViewer::
addLandmark(
  const WlzDVertex3 point)
{
  m_landmarkController->addTargetLandmark(point);
}

int WarperTargetViewer::
initialTransparency(
  WoolzObject * object)
{
  if(object->isWarped() && !object->isMesh())
  {
    return(100);
  }
  else
  {
    return(0);
  }
}

bool WarperTargetViewer::
saveAsXml(
  QXmlStreamWriter *xmlWriter)
{
  Q_ASSERT(xmlWriter);
  xmlWriter->writeStartElement(xmlTag);
  WarperViewer::saveAsXml(xmlWriter);
  xmlWriter->writeEndElement();
  return(true);
}
