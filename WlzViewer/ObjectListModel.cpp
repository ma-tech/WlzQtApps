#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _ObjectListModel_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         ObjectListModel.cpp
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

#include "ObjectListModel.h"
#include "WoolzObject.h"
#include "ObjectSimpleViewer.h"

ObjectListModel::
ObjectListModel(
  QObject * parent):
ObjectListModelAbstract(
  parent)
{
}

int ObjectListModel::
rowCount(
  const QModelIndex & parent) const
{
  int	s = 0;

  if(!parent.isValid())                 //this is the root node
  {
    s = objects.size();
  }
  return(s);
}

int ObjectListModel::
columnCount(
  const QModelIndex & /*parent*/) const
{
  return(4);
}

WoolzObject *ObjectListModel::
getObject(
  const QModelIndex & index) const
{
  if(index.isValid())
  {
    return(objects.at(index.row()));
  }
  return(NULL);
}

QVariant ObjectListModel::
data(
  const QModelIndex & index,
  int role) const
{
  if(!index.isValid())
  {
    return QVariant();
  }
  if(role == Qt::DisplayRole)
  {
    WoolzObject *object = getObject(index);
    if(object)
    {
      switch(index.column())
      {
	case 0:
	  return(object->name());
	case 1:
	  return(QVariant());
	case 2:
	  return(object->qColour());
	case 3:
	  return(object->notes());
      }
    }
    else
    {
      if(index.column() == 0)
      {
	switch(index.row())
	{
	  case 0:
	    return(tr("Source"));
	  case 1:
	    return(tr("Target"));
	  case 2:
	    return(tr("Warped"));
	}
      }
    }
  }
  else if(role == Qt::CheckStateRole)
  {
    WoolzObject *object = getObject(index);
    if(object)
    {
      switch(index.column())
      {
	case 1:
	  return(object->visible() ? Qt::Checked : Qt::Unchecked);
      }
    }
    return(QVariant());
  }
  else if(role == Qt::EditRole)
  {
    WoolzObject *object = getObject(index);
    if(object)
    {
      switch(index.column())
      {
	case 0:
	  return(object->name());
	case 1:
	  return(object->visible());
	case 2:
	  return(object->qColour());
      }
    }
  }
  return(QVariant());
}

QVariant ObjectListModel::
headerData(
  int section,
  Qt::Orientation orientation,
  int role) const
{
  if((role == Qt::DisplayRole) && (orientation == Qt::Horizontal))
  {
    switch(section)
    {
      case 0:
	return(QString(tr("Object")));
      case 1:
	return(QString(tr("Visible")));
      case 2:
	return(QString(tr("Colour")));
      case 3:
	return(QString(tr("Properties")));
    }
  }
  return(QVariant());
}

ObjectListModel::
~ObjectListModel()
{
  removeRows(0,objects.size());
  for(int i = 0; i < objects.size(); ++i)
  {
    delete objects.at(i);
  }
}

Qt::ItemFlags ObjectListModel::
flags(
  const QModelIndex &index) const
{
  Qt::ItemFlags flags = QAbstractItemModel::flags(index);
  if(index.column() == 1)
  {
    flags |= Qt::ItemIsUserCheckable;
  }
  return(flags);
}

bool ObjectListModel::
setData(
  const QModelIndex &index,
  const QVariant &value,
  int role)
{
  if(!index.isValid())
  {
    return(false);
  }

  if(role == Qt::EditRole)
  {
    WoolzObject *object = getObject(index);
    if(object )
    {
      switch(index.column())
      {
	case 0:
	  object->setName(value.toString());
	  break;
	case 2:
	  if(!qVariantCanConvert<QColor>(value))
	  {
	    return(false);
	  }
	  else
	  {
	    QColor colour = qVariantValue<QColor>(value);
	    object->setQColour(colour);
	  }
	  break;
	default:
	  return(false);
      }
      emit dataChanged(index, index);
      return(true);
    }
  }
  else if(role == Qt::CheckStateRole)
  {
    if(index.column() == 1)
    {
      WoolzObject *object = getObject(index);
      if(object)
      {
	object->setVisible(value.toBool());
	emit dataChanged(index, index);
	return(true);
      }
    }
  }
  return(false);
}

QModelIndex ObjectListModel::
index(
  int row,
  int column,
  const QModelIndex &parent) const
{
  if((row < 0) || (column < 0))
  {
    return(QModelIndex());
  }
  if(!parent.isValid())
  {
    return(createIndex(row, column, (void*)-1));
  }
  else
  {
    if((parent.row() >= 0) && (parent.row() < 3))
    {
      return(createIndex(row, column, parent.row()));
    }
    Q_ASSERT(false);
  }
  return(QModelIndex());
}

QModelIndex ObjectListModel::
parent(
  const QModelIndex &/*child*/) const
{
  return(QModelIndex());
}


void ObjectListModel::
addObject(
  WoolzObject * object)
{
  Q_ASSERT(object);
  objects.append(object);
  int row = objects.size() - 1;
  beginInsertRows(QModelIndex(), row, row);
  endInsertRows();

  if(object->ID() < 0)
  {
    object->setID(++m_objectIDCounter);
    object->setupConnections(this);
  }
  emit addObjectSignal(object);
  objectSelected(object);
}

void ObjectListModel::
addAllObjectsTo(
  ObjectSimpleViewer *viewer)
{
  Q_ASSERT(viewer);
  QListIterator<WoolzObject *> i(objects);
  while(i.hasNext())
  {
    viewer->addObject(i.next(), false);
  }
}

void ObjectListModel::
removeObjectFromStore(
  WoolzObject * object)
{
  int row = objects.indexOf(object);
  if(row >= 0)
  {
    beginRemoveRows(QModelIndex(), row, row);
    endRemoveRows();
    objects.removeAt(row);
  }
}

void ObjectListModel::
removeObject(
  WoolzObject * object)
{
  if(object)
  {
    removeSelected(object);
    emit removedObjectSignal(object);  // this has to be emited before removed
                                       // from lists or object is closed
    removeObjectFromStore(object);
    object->close();
    delete object;
  }
}

void ObjectListModel::
removeObjectNoDelete(
  WoolzObject * object)
{
  if(object)
  {
    removeObjectFromStore(object);
    emit removedObjectSignal(object);
    object->close();
  }
}

void ObjectListModel::
removeObject(
  const QModelIndex &index)
{
  if(index.isValid())
  {
    removeObject(getObject(index));
  }
}

bool ObjectListModel::
removable(
  const QModelIndex & index) const
{
  WoolzObject *object = getObject(index);
  return(object && object->removable());
}

void ObjectListModel::
objectChanged()
{
  WoolzObject* obj = qobject_cast<WoolzObject*>(sender());
  if(obj)
  {
    int row=-1;
    row = objects.indexOf(obj);
    if(row >= 0)
    {
      changePersistentIndex(QModelIndex(),
          createIndex(row,
	      columnCount(QModelIndex())-1, 0));    //update changed columns
    }
    emit layoutChanged();
  }
}

void ObjectListModel::
setSelectObject(
  WoolzObject *object)
{
  emit objectSelected(object);
}

void ObjectListModel::
selectAnObject()
{
  WoolzObject* object = qobject_cast<WoolzObject*>(sender());
  setSelectObject(object);
}

QModelIndex ObjectListModel::
getObjIndex(WoolzObject *object) const
{
  int row;
  for(int store = 0 ; store < 3; ++store)
  {
   row = objects.indexOf(object);
   if(row>=0)
   {
      return(createIndex(row, 1, store));
   }
  }
  return(QModelIndex());
}

void ObjectListModel::
objectTypeChanged()
{
  WoolzObject * object = qobject_cast<WoolzObject*>(sender());
  if(!object)
  {
    return;
  }

  int row = 0;
  emit removedObjectSignal(object); 	// object has to be emited before
  					// removed from lists
  for(int store = 0 ; store < 3; ++store)
  {
    row = objects.indexOf(object);
    if(row >= 0)
    {
      removeObjectFromStore(object);
    }
  }
  
  //remove connections which addObject will recreate
  disconnect(object, SIGNAL(objectPropertyChanged()),
             this, SLOT(objectChanged()));
  disconnect(object, SIGNAL(objectTypeChanged()),
             this, SLOT(objectTypeChanged()));

  addObject(object);
}
