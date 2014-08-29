#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _WarperSourceViewer_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         WarperSourceViewer.cpp
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

#include "WarperSourceViewer.h"

#include "WoolzObject.h"
#include "Viewer2D3D.h"
#include <QSettings>

const char* WarperSourceViewer::xmlTag = "SourceViewer";

WarperSourceViewer::
WarperSourceViewer(
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

QColor WarperSourceViewer::
getBackgroundColour()
{
  QSettings settings;
  return(settings.value("viewer/source/color",
	                QColor(0, 77, 51)).value<QColor>());
}

void WarperSourceViewer::
init()
{
  setupLandmarkView(LandmarkModel::sourceV);
  setBackgroundColour();
}

bool WarperSourceViewer::
accepting(
  WoolzObject * object)
{
  //accept source objects only
  return(object && (object->type() & WoolzObject::source));
}

void WarperSourceViewer::
addLandmark(
  const WlzDVertex3 point)
{
  m_landmarkController->addSourceLandmark(point);
}

bool WarperSourceViewer::
saveAsXml(
  QXmlStreamWriter *xmlWriter)
{
  Q_ASSERT(xmlWriter);
  xmlWriter->writeStartElement(xmlTag);
  WarperViewer::saveAsXml(xmlWriter);
  xmlWriter->writeEndElement();
  return(true);
}

