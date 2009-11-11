#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ObjectListModel_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ObjectListModel.cpp
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
* \brief        Model managing the collection of objects.
* \ingroup      Control
*
*/

#include "ObjectListModel.h"
#include "WoolzObject.h"

#include "WoolzFileObject.h"
#include "WoolzDynWarpedObject.h"
#include "WoolzDynThresholdedObj.h"
#include "WoolzDynMeshObject.h"
#include "WoolzDynContourISO.h"
#include "ObjectViewer.h"

#include "Commands.h"
#include <QtXml/QXmlStreamWriter>

#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtXml/QDomText>

const char* ObjectListModel::xmlTag = "Objects";

ObjectListModel::ObjectListModel (LandmarkModel *landmarkModel, QUndoStack *undoStack, QObject * parent):  ObjectListModelAbstract(parent), m_meshObject(NULL), m_undoStack(undoStack), m_landmarkModel(landmarkModel) {
}

int ObjectListModel::rowCount(const QModelIndex & parent) const {
    if (!parent.isValid())                 //this is the root node
      return 3;
    else if (getStore(parent) == -1 )      //this is a store
      return objects[parent.row()].size();
    else 
      return 0;
}

int ObjectListModel::columnCount(const QModelIndex & /*parent*/) const {
    return 4;
}

int ObjectListModel::getStore(const QModelIndex & index) const {
    if (index.isValid()) {
      long long longvalue=reinterpret_cast<long long>(index.internalPointer());
      return static_cast<int>(longvalue);
      //return (int)(index.internalPointer());
    }
    return -1;
}

WoolzObject *ObjectListModel::getObject(const QModelIndex & index) const {
   if (index.isValid()) {
        int store = getStore(index);
        if (store >= 0 && store < 3)
          return objects[store].at(index.row());
    }
    return NULL;
}

QVariant ObjectListModel::data(const QModelIndex & index, int role ) const {
  if (!index.isValid())
    return QVariant();
  if (role == Qt::DisplayRole) {
    WoolzObject *object = getObject(index);
    if (object ) {//this is an object
      switch (index.column()) {
          case 0:
            return object->name();
          case 1:
            return QVariant();
          case 2:
            return object->qColour();
          case 3:
            return object->notes();
      }
    } else {
        if (index.column() == 0) {
          switch (index.row()) {
            case 0:
              return tr("Source");
            case 1:
              return tr("Target");
            case 2:
              return tr("Warped");
        }
      } //column
    }
  }  else if (role == Qt::CheckStateRole) {
    WoolzObject *object = getObject(index);
    if (object) {//this is an object
      switch (index.column()) {
          case 1:
            return object->visible() ? Qt::Checked : Qt::Unchecked;
      }
    }
    return QVariant();
  } else if (role == Qt::EditRole) {
      WoolzObject *object = getObject(index);
      if (object) {//this is an object
        switch (index.column()) {
            case 0:
              return object->name();
            case 1:
              return object->visible();
            case 2:
              return object->qColour();
        }
      }
  } // role
  return QVariant();
}

QVariant ObjectListModel::headerData(int section, Qt::Orientation orientation, int role ) const {
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
    switch (section) {
      case 0:
        return QString(tr("Object"));
      case 1:
        return QString(tr("Visible"));
      case 2:
        return QString(tr("Colour"));
      case 3:
        return QString(tr("Properties"));
    }
  }
  return QVariant();
}

ObjectListModel::~ObjectListModel ( ) {
  removeRows(0,3);
  int i, store;
  for (store=0; store<3; store++) {
    for (i=0; i< objects[store].size(); i++)
      delete objects[store].at(i);
  }
}

Qt::ItemFlags ObjectListModel::flags(const QModelIndex &index) const {
  Qt::ItemFlags flags = QAbstractItemModel::flags(index);
  if (getStore(index)>=0) { //if an object allow to edit name and colour
    if (index.column()==0 || index.column()==1 || index.column()==2)
      flags |= Qt::ItemIsEditable;
    if (index.column()==1)
       flags |= Qt::ItemIsUserCheckable ;
  }
  return flags;
}

bool ObjectListModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if (!index.isValid())
      return false;

  if (role == Qt::EditRole) {
    WoolzObject *object = getObject(index);
    if (object ) {//this is an object
      switch (index.column()) {
          case 0:
                addCommand(new RenameObject(object, value.toString()));
            break;
          case 2:
             if (!qVariantCanConvert<QColor>(value)) {
                return false;
             } else {
                QColor colour = qVariantValue<QColor>(value);
                addCommand(new ColourObject(object, colour));
             }
            break;
        default:
           return false;
      }
      emit dataChanged(index, index);
      return true;
    }
  } else if (role == Qt::CheckStateRole) {
      if (index.column()==1) {
        WoolzObject *object = getObject(index);
        if (object) {
          addCommand(new WoolzObjectChangeVisibility(object, value.toBool()));
          emit dataChanged(index, index);
          return true;
      }
    }
  }
  return false;
}

QModelIndex ObjectListModel::index(int row, int column, const QModelIndex &parent ) const {
  if (row<0 || column <0)
    return QModelIndex();
  if (!parent.isValid()) {                        // store
      return createIndex(row, column, (void*)-1);
  } else {                                        // objects
      if (parent.row()>=0 && parent.row()<3)
        return createIndex(row, column, parent.row());
      Q_ASSERT(false);
  }
  return QModelIndex();
}

QModelIndex ObjectListModel::parent(const QModelIndex &child) const {
  if (child.isValid()) {
      int store = getStore(child);
      if (store>=0 && store<3) {
          return createIndex(store, 0, -1 );
      }
  }
  return QModelIndex();
}

void ObjectListModel::addObject(WoolzObject * object) {
  Q_ASSERT(object);
  if (!object->isWarped()) {
    if (object->type() & WoolzObject::source) {
      const int index = 0;
      objects[index].append(object);
      int row = objects[index].size()-1;
      beginInsertRows(createIndex(index, 0, (void*)-1), row, row);
      endInsertRows();
    }
    if (object->type() & WoolzObject::target) {
      const int index = 1;
      objects[index].append(object);
      int row = objects[index].size()-1;
      beginInsertRows(createIndex(index, 0, (void*)-1), row, row);
      endInsertRows();
    }
  } else {
    const int index = 2;
    objects[index].append(object);
    int row = objects[index].size()-1;
    beginInsertRows(createIndex(index, 0, (void*)-1), row, row);
    endInsertRows();
  }

  if (object->ID() < 0) {
      object->setID(++m_objectIDCounter);
      object->setupConnections(this);
  }
  emit addObjectSignal(object);
}

void ObjectListModel::addAllObjectsTo(WoolzObject::WoolzObjectType /*type*/, ObjectViewer *viewer) {
  Q_ASSERT(viewer);
  int i;
  int size;

  size  = objects[0].size();
  for (i=0; i<size; i++)
    viewer->addObject(objects[0].at(i), false);

  size= objects[1].size();
  for (i=0; i<size; i++)
    viewer->addObject(objects[1].at(i), false);

  size= objects[2].size();
  for (i=0; i<size; i++)
    viewer->addObject(objects[2].at(i), false);
}

void ObjectListModel::addAllObjects(ObjectViewer *viewer) {
  Q_ASSERT(viewer);
  int i;
  int size;

  size  = objects[0].size();
  for (i=0; i<size; i++)
    viewer->addObject(objects[0].at(i), false);

  size= objects[1].size();
  for (i=0; i<size; i++)
    viewer->addObject(objects[1].at(i), false);

  size= objects[2].size();
  for (i=0; i<size; i++)
    viewer->addObject(objects[2].at(i), false);
}


void ObjectListModel::removeObjectFromStore(int store, WoolzObject * object) {
      int row ;
      row = objects[store].indexOf(object);
      if (row>=0) {
         beginRemoveRows(createIndex(store, 0, (void*)-1), row, row ); 
         endRemoveRows();
         objects[store].removeAt(row);
      }
 }

void ObjectListModel::removeObject(WoolzObject * object) {
  if (object) {
    removeSelected(object);
    emit removedObjectSignal(object);  //has to be emited before removed from lists or object is closed
    removeObjectFromStore(0, object);
    removeObjectFromStore(1, object);
    removeObjectFromStore(2, object);
    object->close();
    delete object;
  }
}

void ObjectListModel::removeObjectNoDelete(WoolzObject * object) {
  if (object) {
    removeSelected(object);
    removeObjectFromStore(0, object);
    removeObjectFromStore(1, object);
    removeObjectFromStore(2, object);
    emit removedObjectSignal(object);
    object->close();
  }
}

QList <WoolzObject*> ObjectListModel::getObjects(bool sources, bool targets, bool valueOnly) {
  QList <WoolzObject*> list;
  if (sources) {
    int i;
    int size = objects[0].size();
    for (i=0; i<size; i++)
      if (!valueOnly || objects[0].at(i)->isValue())
        list.append(objects[0].at(i));
  }
  if (targets) {
    int i;
    int size= objects[1].size();
    for (i=0; i<size; i++)
      if (!valueOnly || objects[1].at(i)->isValue())
        list.append(objects[1].at(i));
  }
  return list;
}


WoolzObject* ObjectListModel::getMeshObject() {
  return m_meshObject;
}

void ObjectListModel::addMeshObject(WoolzObject* meshObject) {
  if (m_meshObject) {
    removeObject(m_meshObject);
    m_meshObject->disconnect(m_meshObject, 0, 0, 0);
  }
  addObject(meshObject);
  m_meshObject = meshObject;
  emit replaceWarpMesh(meshObject);
}

void ObjectListModel::addMeshObjectNoDelete(WoolzObject* meshObject) {
  if (m_meshObject) {
    removeObjectNoDelete(m_meshObject);
    m_meshObject->disconnect(m_meshObject, 0, 0, 0);
  }
  if (meshObject)
    addObject(meshObject);
  m_meshObject = meshObject;
  emit replaceWarpMesh(meshObject);
}

WoolzObject* ObjectListModel::getFirstWarped() {
  int size = objects[2].size();
  WoolzObject* res=NULL;
  for (int i=0; i<size; i++) {
    if ((res=objects[2].at(i))->type() & WoolzObject::target)  return res;
  }
  return NULL;
}

void ObjectListModel::objectChanged() {
  WoolzObject* obj = qobject_cast<WoolzObject*>(sender());
  if (obj) {
    int row=-1;
    int store=0;
    for (;store<3;store++) {
      row = objects[store].indexOf(obj);
      if (row>=0)
        changePersistentIndex(createIndex(row, 0, store), createIndex(row, columnCount(QModelIndex())-1, store)); //update changed columns
        emit layoutChanged();
    }
  }
}

QModelIndex ObjectListModel::getObjIndex(WoolzObject *object) const {
  int row;
  for (int store =0 ; store<3; store++) {
   row = objects[store].indexOf(object);
   if (row>=0)
      return createIndex(row, 1, store);
  }
  return QModelIndex();
}

void ObjectListModel::objectTypeChanged() {
  WoolzObject* object = qobject_cast<WoolzObject*>(sender());
  if (!object)
    return;

  int row = 0;
  emit removedObjectSignal(object);  //has to be emited before removed from lists
  for (int store =0 ; store<3; store++) {
   row = objects[store].indexOf(object);
   if (row>=0)
      removeObjectFromStore(store, object);
  }
  //remove connections which addObject will recreate
  disconnect(object, SIGNAL(objectPropertyChanged()), this, SLOT(objectChanged()));
  disconnect(object, SIGNAL(objectTypeChanged()),  this, SLOT(objectTypeChanged()));
  disconnect(object, SIGNAL(statusChange(QString, int)),  this, SIGNAL(statusChanged(QString, int)));
  addObject(object);
}

void ObjectListModel::updateAll() {
  emit updateAllSignal();
}

void ObjectListModel::updateAllWarped() {
  emit updateAllWarpedSignal();
}

void ObjectListModel::loadAll() {
  emit loadAllSignal();
}

void ObjectListModel::addCommand(QUndoCommand *command) {
   m_undoStack->push(command);
 }

WoolzObject* ObjectListModel::getObject(int ID) {
  int store, i;
  for (store=0; store<3; store++) {
    for (i=0; i< objects[store].size(); i++)
      if (objects[store].at(i)->ID() == ID)
          return objects[store].at(i);
  }
  return NULL;
}

bool ObjectListModel::saveAsXml(QXmlStreamWriter *xmlWriter) {
  Q_ASSERT(xmlWriter);
  xmlWriter->writeStartElement(xmlTag);
  int store, i;
  for (store=0; store<3; store++) {
      switch (store) {
          case 0:
            xmlWriter->writeStartElement("Source");
            break;
          case 1:
            xmlWriter->writeStartElement("Target");
            break;
          case 2:
            xmlWriter->writeStartElement("Warped");
            break;
          default:
            Q_ASSERT(false);
      }
      for (i=0; i< objects[store].size(); i++)
        if (!objects[store].at(i)->saveAsXml(xmlWriter))
            return false;
    xmlWriter->writeEndElement();
  }
  xmlWriter->writeTextElement("ObjectIDCounter", QString("%1").arg(m_objectIDCounter));
  if (m_meshObject)
     xmlWriter->writeTextElement("MeshObjectID", QString("%1").arg(m_meshObject->ID()));
  if (m_selectedObject)
    xmlWriter->writeTextElement("SelectedObjectID", QString("%1").arg(m_selectedObject->ID()));
  xmlWriter->writeEndElement();
  return true;
}

bool ObjectListModel::parseDOM(const QDomElement &element) {
  if (element.tagName() != xmlTag)
      return false;
  readStoreFromXml(0, element.firstChildElement("Source"));
  readStoreFromXml(1, element.firstChildElement("Target"));
  readStoreFromXml(2, element.firstChildElement("Warped"));

  QDomElement temp;
  temp = element.firstChildElement("MeshObjectID");
  if (temp.isNull())
      m_meshObject = NULL;
   else {
      const int meshObjID = temp.text().toInt();
      m_meshObject = getObject(meshObjID);
  }

  temp = element.firstChildElement("SelectedObjectID");
  if (temp.isNull())
      setSelectObject(NULL);
   else {
      const int selObjectID = temp.text().toInt();
      setSelectObject(getObject(selObjectID));
  }

  temp = element.firstChildElement("ObjectIDCounter");
  if (temp.isNull())
       m_objectIDCounter = 0;
   else {
      m_objectIDCounter = temp.text().toInt();
  }

  return true;
}

WoolzObject * ObjectListModel::createObject(QString type) {
    if (type == WoolzFileObject::xmlTag)
        return new WoolzFileObject();
    else if (type == WoolzDynWarpedObject::xmlTag)
        return new WoolzDynWarpedObject(this, m_landmarkModel);
    else if (type == WoolzDynThresholdedObj::xmlTag)
        return new WoolzDynThresholdedObj(this);
    else if (type == WoolzDynMeshObject::xmlTag)
        return new WoolzDynMeshObject(this);
    else if (type == WoolzDynContourISO::xmlTag)
        return new WoolzDynContourISO(this);
    return  NULL;
}

bool ObjectListModel::readStoreFromXml(int store, const QDomElement &element) {
  QDomNode child = element.firstChild();
  WoolzObject * obj = NULL;
  while (!child.isNull()) {
        obj = createObject(child.toElement().tagName());
        if (obj) {
            obj->setupConnections(this);
            obj->parseDOM(child.toElement());
            objects[store].append(obj);
            emit addObjectSignal(obj);
        }
        child = child.nextSibling();
  }
  return true;
}
