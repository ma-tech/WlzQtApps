#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ObjectView_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ObjectView.cpp
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
* \brief        View for generic Woolz object
* \ingroup      Views
*
*/

// project includes
#include "WoolzObject.h"
#include "ObjectView.h"


//Qt includes
#include <QtXml/QXmlStreamWriter>

//inventor includes
#include <Inventor/nodes/SoSeparator.h>

// Constructors/Destructors
//  
ObjectView::ObjectView (QObject * parent, WoolzObject *object) : View (parent), m_parent (parent)  {
  obj = object;
  connect(obj, SIGNAL(objectPropertyChanged()), this, SLOT(objectPropertyChanged()));
  connect(obj, SIGNAL(objectVisualisationChange()), this, SLOT(objectPropertyChanged()));
  connect(obj, SIGNAL(objectChanged() ), this, SLOT( objectChanged()) );
  connect(obj, SIGNAL(objectVisibilityChanged() ), this, SLOT( objectVisibilityChanged()) );
  m_maskVisibility = m_visible;
}

ObjectView::~ObjectView ( ) {
}

//
// Methods
QColor ObjectView::getQColour ( ) {
  return obj->qColour();
}

QString ObjectView::getTitle ( ) {
    return obj->name();
}

QString ObjectView::getProperties ( )  {
  return obj->notes();
}

void ObjectView::objectVisibilityChanged ( )  {
  if (obj->visible()) {
     View::setVisibility ( m_maskVisibility );  // use the view visibility
  } else
     View::setVisibility ( false  );  // disable view
}

void ObjectView::objectChanged() {
  if (compatible())
    getSceneGraph(true);
  else
    emit regerateView();
}

void ObjectView::objectPropertyChanged( ) {
    emit viewPropertyChanged();
}

void ObjectView::updateTransparency(int transparency) {
  if (obj->isWarped())
        setTransparency(transparency);
  else
        setTransparency(100-transparency);
}

void ObjectView::selectView() {
  obj->selectObject();
}

void ObjectView::setVisibility ( bool visibility ) {
  m_maskVisibility = visibility;
  if (obj->visible()) {
     View::setVisibility ( visibility );// use the view visibility
  } else
     View::setVisibility ( false ); //disable view
}


bool ObjectView::saveAsXmlProperties(QXmlStreamWriter *xmlWriter) {
  xmlWriter->writeTextElement("ObjectID", QString("%1").arg(obj->ID()));
  View::saveAsXmlProperties(xmlWriter);
  return true;
}

bool ObjectView::isUsing ( int testobjID ) {
    return obj && obj->ID()==testobjID;
}
