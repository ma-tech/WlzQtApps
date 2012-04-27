#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _WarperResultViewer_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         WarperResultViewer.cpp
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
* \brief        Viewer displaying result (warped) objects
* \ingroup      UI
*
*/

#include "WarperResultViewer.h"
#include "WoolzObject.h"
#include "Viewer2D3D.h"
#include "ObjectView.h"
#include "LandmarkModel.h"
#include "LandmarkView.h"
#include <QSettings>

const char* WarperResultViewer::xmlTag = "ResultViewer";

WarperResultViewer::WarperResultViewer (ObjectViewerModel *objectViewerModel, bool is3D) : ObjectViewer (objectViewerModel, is3D, true) {
}

QColor WarperResultViewer::getBackgroundColour() {
  QSettings settings;
  return settings.value("viewer/result/color", QColor(77, 0, 0)).value<QColor>();
}


void WarperResultViewer::init() {
  setBackgroundColour();
}


bool WarperResultViewer::accepting(WoolzObject * object ) {
   return (object && (object->type()& WoolzObject::target)); //accept source objects only
}

int WarperResultViewer::initialTransparency(WoolzObject * object) {
  if (!object->isValue())
     return 100 ;         // meshes are not shown in result view by default

  if (object->isWarped()) 
     return 0;
  else
     return 20;           // other loaded objects are shown transparent
}

void WarperResultViewer::transparencyChanged(int transparency) {
  for (int i = 0; i < views.size(); ++i) {
    views[i]->updateTransparency(transparency);
  }
}

bool WarperResultViewer::saveAsXml(QXmlStreamWriter *xmlWriter) {
  Q_ASSERT(xmlWriter);
  xmlWriter->writeStartElement(xmlTag);
  ObjectViewer::saveAsXml(xmlWriter);
  xmlWriter->writeEndElement();
  return true;
}
