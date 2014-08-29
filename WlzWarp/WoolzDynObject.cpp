#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _WoolzDynObject_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         WoolzDynObject.cpp
* \author       Zsolt Husz
* \date         March 2009
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
* \brief        Container class for WlzObject type
* \ingroup      Control
*/

#include "WoolzDynObject.h"
#include "WarperConfig.h"
#include "ObjectListModel.h"

#include <QtXml/QDomElement>
#include <QtXml/QXmlStreamWriter>

WoolzDynObject::
WoolzDynObject(
  ObjectListModel * objectListModel):
WoolzObject(),
m_objectListModel(objectListModel)
{
  m_autoUpdate = false;
  m_srcObjID = -1;
  m_srcObjIDUsed = 0;
  connect(this, SIGNAL(updated(bool)),
          m_objectListModel, SLOT(objectUpdated(bool)));
  emit updated(false);
}

WoolzDynObject::
~WoolzDynObject()
{
}

void WoolzDynObject::
sourceChanged()
{
  if((m_autoUpdate && config.globalAutoUpdate()))
  {
    update(true);
  }
}

void WoolzDynObject::
update(bool force)
{
  if(force || (m_autoUpdate && config.globalAutoUpdate() && needsUpdate()))
  {
    removeCachedVisualisation();
    doUpdate();
  }
}

void WoolzDynObject::
setAutoUpdate(bool newAutoupdate)
{
  m_autoUpdate = newAutoupdate;
}

void WoolzDynObject::
removedSourceObject()
{
  WoolzObject* obj = qobject_cast<WoolzObject*>(sender());
  if(sourceObj() == obj)
  {
    emit objectPropertyChanged();
  }
}

void WoolzDynObject::
setSourceObj(
  WoolzObject *src)
{
  WoolzObject *currentSrc = sourceObj();
  if(currentSrc  == src || !isAllowedSource(src))
  {
    return;
  }
  if(currentSrc)
  {
    disconnect(currentSrc, 0, this, 0);
  }
  if(src)
  {
    m_srcObjID = src->ID();
    WoolzDynObject* dynobj = qobject_cast<WoolzDynObject*>(src);
    if(dynobj)
      connect(dynobj, SIGNAL(objectChanged()),
              this, SLOT(sourceChanged()));
    setTypeFromSource();
  }
  else
  {
    m_srcObjID = -1;
  }
  emit updated(false);
}

void WoolzDynObject::
setSourceObj(
  int srcID)
{
  if(srcID == m_srcObjID)
  {
    return;
  }
  if(m_srcObjID >= 0)
  {
    disconnect(sourceObj(), 0, this, 0);
  }
  m_srcObjID = srcID;
  WoolzDynObject* dynobj = qobject_cast<WoolzDynObject*>(sourceObj());
  if(dynobj)
  {
    connect(dynobj, SIGNAL(objectChanged()), this, SLOT(sourceChanged()));
  }
  setTypeFromSource();
  emit updated(false);
}


bool WoolzDynObject::
isAllowedSource(
  WoolzObject *src)
{
  if(src == this)   // no self reference is allowed
  {
    return(false);
  }
  WoolzDynObject* dynobj = qobject_cast<WoolzDynObject*>(src);
  if(!dynobj)
  {
    return(true);  // if not dynamic, then can't be circular, so is OK
  }
  return(isAllowedSource(dynobj->sourceObj()));
}

void WoolzDynObject::
setTypeFromSource()
{
  WoolzObject *srcObj = sourceObj();
  if(srcObj)
  {
    changeWoolzObjectType(srcObj->type());
  }
}

WoolzObject* WoolzDynObject::
sourceObj()
{
  return(m_objectListModel->getObject(m_srcObjID));
}

void WoolzDynObject::
setupConnections(
  QObject *target)
{
  WoolzObject::setupConnections(target);
}

bool WoolzDynObject::
saveAsXmlProperties(
  QXmlStreamWriter *xmlWriter)
{
  WoolzObject::saveAsXmlProperties(xmlWriter);
  xmlWriter->writeTextElement("SourceObjID", QString("%1").arg(m_srcObjID));
  xmlWriter->writeTextElement("AutoUpdate", m_autoUpdate ? "YES" : "NO");
  return(true);
}

bool WoolzDynObject::
parseDOMLine(
  const QDomElement &element)
{
  if(element.tagName()  == "SourceObjID")
  {
    setSourceObj(element.text().toInt());
    return(true);
  }
  else if(element.tagName() == "AutoUpdate")
  {
    setAutoUpdate(element.text().toUpper() == "YES");
    return(true);
  }
  else
  {
    return(WoolzObject::parseDOMLine(element));
  }
  return(false);
}

bool  WoolzDynObject::
needsUpdate()
{
  return(m_srcObjID != m_srcObjIDUsed);
}

void WoolzDynObject::
saveUsedParameters()
{
  m_srcObjIDUsed = m_srcObjID;
  emit updated(true);
}
