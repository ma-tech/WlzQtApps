#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _ObjectListModelAbstract_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         ObjectListModelAbstract.cpp
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
* \brief        Model managing the collection of objects.
* \ingroup      Control
*/

#include "ObjectListModelAbstract.h"
#include "WoolzObject.h"

ObjectListModelAbstract::
ObjectListModelAbstract(
  QObject * parent):
QAbstractItemModel(
  parent),
m_objectIDCounter(0),
m_selectedObject(NULL)
{
}

ObjectListModelAbstract::
~ObjectListModelAbstract()
{
}

void ObjectListModelAbstract::
removeObject(
  const QModelIndex &index)
{
  if(index.isValid())
  {
    removeObject(getObject(index));
  }
}

bool ObjectListModelAbstract::
removable(
  const QModelIndex & index) const 
{
  WoolzObject *object = getObject(index);
  return(object && object->removable());
}

void ObjectListModelAbstract::
setSelectObject(
  WoolzObject *object)
{
  m_selectedObject = object;
  emit objectSelected(object);
}

void ObjectListModelAbstract::
selectAnObject()
{
  WoolzObject* object = qobject_cast<WoolzObject*>(sender());
  setSelectObject(object);
}

void ObjectListModelAbstract::
removeSelected(
  WoolzObject * object)
{
  if(object == m_selectedObject)
  {
    m_selectedObject =  NULL;
  }
}
