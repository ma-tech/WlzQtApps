#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _WoolzDynMeshObject_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         WoolzDynMeshObject.cpp
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
* \brief        Dynamicaly generated mesh Woolz objects
* \ingroup      Control
*/

#include "WoolzDynMeshObject.h"
#include <QtXml/QDomElement>
#include <QtXml/QXmlStreamWriter>

const char* WoolzDynMeshObject::xmlTag = "MeshObject";

WoolzDynMeshObject::
WoolzDynMeshObject(
  ObjectListModel * objectListModel):
WoolzDynObject(objectListModel)
{
  m_meshMinDist = 0;
  m_meshMaxDist = 0;
  m_meshMinDistUsed = -1;
  m_meshMaxDistUsed = -1;
}

WoolzDynMeshObject::
~WoolzDynMeshObject()
{
}

void WoolzDynMeshObject::
doUpdate()
{
  WoolzObject *obj = sourceObj();

  if(!obj)
  {
    return ;
  }
  statusChange("Computing mesh " + m_name + "...", 0);

  WlzMeshGenMethod meshGenMth = WLZ_MESH_GENMETHOD_CONFORM;
  WlzErrorNum	errNum = WLZ_ERR_NONE;

  WlzObject * meshObj = NULL;
  if(m_obj)
  {
    WlzFreeObj(m_obj);
  }
  meshObj  = WlzCMeshTransformFromObj(obj->getObj(),
                                      meshGenMth, m_meshMinDist, m_meshMaxDist,
                                      NULL, 1, &errNum);
  if(errNum != WLZ_ERR_NONE && meshObj)
  {
    WlzFreeObj(meshObj);
    meshObj = NULL;
  }
  if(meshObj != NULL)
  {
    WlzValues     val;
    val.core = NULL;
    m_obj = WlzMakeMain((obj->is3D())?
	                WLZ_CMESH_3D: WLZ_CMESH_2D,
			meshObj->domain, val, NULL, NULL, NULL);
    m_obj = WlzAssignObject(m_obj, NULL);
    m_name = "Mesh";
    m_type = obj->type();
    saveUsedParameters();
    statusChange("Mesh " + m_name + " finished.", 0);
    emit objectChanged();
  }
  else
  {
    statusChange("Mesh " + m_name + " failed.", 0);
  }
  if(meshObj)
  {
    WlzFreeObj(meshObj);  // Valgrind reports an error
                          // Possible leak here!
    meshObj = NULL;
  }
  return;
}

bool WoolzDynMeshObject::
saveAsXml(
  QXmlStreamWriter *xmlWriter)
{
  xmlWriter->writeStartElement(xmlTag);
  saveAsXmlProperties(xmlWriter);
  xmlWriter->writeEndElement();
  return(true);
}

bool WoolzDynMeshObject::
saveAsXmlProperties(
  QXmlStreamWriter *xmlWriter)
{
  WoolzDynObject::saveAsXmlProperties(xmlWriter);
  xmlWriter->writeTextElement("MinDistance", QString("%1").arg(m_meshMinDist));
  xmlWriter->writeTextElement("MaxDistance", QString("%1").arg(m_meshMaxDist));
  return(true);
}

bool WoolzDynMeshObject::
parseDOMLine(
  const QDomElement &element)
{
  if(element.tagName()  == "MaxDistance")
  {
    m_meshMaxDist = element.text().toInt();
    return(true);
  }
  else if(element.tagName() == "MinDistance")
  {
    m_meshMinDist = element.text().toInt();
    return(true);
  }
  else
  {
    return(WoolzDynObject::parseDOMLine(element));
  }
  return(false);
}

bool WoolzDynMeshObject::
needsUpdate()
{
  return((m_meshMinDist != m_meshMinDistUsed) ||
         (m_meshMaxDist != m_meshMaxDistUsed) ||
	 (WoolzDynObject::needsUpdate()));
}

void WoolzDynMeshObject::
saveUsedParameters()
{
  m_meshMinDistUsed = m_meshMinDist;
  m_meshMaxDistUsed = m_meshMaxDist;
  WoolzDynObject::saveUsedParameters();
}

void WoolzDynMeshObject::
setupConnections(
  QObject *target)
{
  WoolzDynObject::setupConnections(target);
  connect(target, SIGNAL(loadAllSignal()), this, SLOT(update()));
}
