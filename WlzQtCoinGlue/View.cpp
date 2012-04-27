#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _View_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         View.cpp
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
* \brief        General interface for view based on Open Inventor.
* \ingroup      Views
*
*/

#include "View.h"
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>

#include <QtXml/QXmlStreamWriter>
#include <QtXml/QDomElement>

const char* View::xmlTag = "View";

// Constructors/Destructors
View::View (QObject *parent) : QObject(parent) {
  switchroot = new SoSwitch;
  Q_ASSERT(switchroot);
  switchroot->ref();

  switchroot->addChild(new SoSeparator);  //empty child, ie view not visible
  root = new SoSeparator;
  switchroot->addChild(root);

  Q_ASSERT(root);
  m_visible = true;
  switchroot->whichChild = m_visible ? 1 : 0;
}

View::~View ( ) { 
  if (switchroot) 
    switchroot->unref();
}

// Methods
SoNode* View::getSceneGraph ( bool bForce) {
  if (bForce) {
    root->removeAllChildren();

    /*  Add children. 
        Do not force, individual objects don't have to regenerated */
    generateSceneGraph(bForce);//false->bForce changed 24-03, since if mesh changes, needs to be updated
   }
  return switchroot;
}

void View::setVisibility ( bool visibility ) {
  if (m_visible != visibility) {
     m_visible = visibility;
     switchroot->whichChild = m_visible ? 1 : 0;
  }
}

bool View::saveAsXml(QXmlStreamWriter *xmlWriter) {
  Q_ASSERT(xmlWriter);
  xmlWriter->writeStartElement(xmlTag);
  saveAsXmlProperties(xmlWriter);
  xmlWriter->writeEndElement();
  return true;
}

bool View::saveAsXmlProperties(QXmlStreamWriter *xmlWriter) {
  xmlWriter->writeTextElement("Visible", getVisibility() ? "Yes":"No");
  xmlWriter->writeTextElement("Transparency", QString("%1").arg(m_transparency));
  return true;
}

bool View::parseDOMLine(const QDomElement &element) {
    if (element.tagName() == "Transparency") {
       setTransparency(element.text().toInt());
       return true;
    } else if (element.tagName() == "Visible") {
       setVisibility(element.text().toUpper() == "YES");
       return true;
   }
  return false;
}

bool View::parseDOM(const QDomElement &element) {
  QDomNode child = element.firstChild();
  while (!child.isNull()) {
        parseDOMLine(child.toElement());
        child = child.nextSibling();
  }
  return true;
}


