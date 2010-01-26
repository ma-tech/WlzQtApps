#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ObjectListModel_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ObjectListModel.h
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

#ifndef OBJECTLISTMODEL_H
#define OBJECTLISTMODEL_H

#include "WoolzObject.h"
#include <QWidget>
#include <ObjectListModelAbstract.h>

#include <QString>

class QUndoStack;
class QUndoCommand;
class ObjectViewer;
class QXmlStreamWriter;
class WoolzTransform;
class QDomElement;

/*!
 * \brief	Model to manage the collection of objects
 * \ingroup      Control
 */
class ObjectListModel : public ObjectListModelAbstract
{
  Q_OBJECT
public:
 /*!
  * \ingroup      Control
  * \brief        Constructor
  * \param        woolzTransform transformation manager
  * \param        undoStack undo stack
  * \param        parent parent object
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  ObjectListModel ( WoolzTransform *woolzTransform, QUndoStack *undoStack, QObject * parent = 0 );

 /*!
  * \ingroup      Control
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  virtual ~ObjectListModel ( );

 /*!
  * \ingroup      Control
  * \brief        Returns the number of rows.
  * \param        parent parent index
  * \return       3 for the root index, otherwise the number of object for source/target/warped types.
  * \par      Source:
  *                ObjectListModel.cpp
  */
  int rowCount(const QModelIndex & parent) const;

 /*!
  * \ingroup      Control
  * \brief        Returns the number of columns.
  * \param        parent parent index
  * \return       3
  * \par      Source:
  *                ObjectListModel.cpp
  */
  int columnCount(const QModelIndex & parent) const;

 /*!
  * \ingroup      Control
  * \brief        Returns the data stored under the given role for the item referred to by the index.
  * \param        index of requested data
  * \param        role requested role
  * \return       data requested
  * \par      Source:
  *                ObjectListModel.cpp
  */
  QVariant data(const QModelIndex & index, int role ) const;

 /*!
  * \ingroup      Control
  * \brief        Returns the data for the given role and section in the header with the specified orientation.
  * \param        section requested section
  * \param        orientation requested orientation 
  * \param        role requested role
  * \return       data requested
  * \par      Source:
  *                ObjectListModel.cpp
  */
  QVariant headerData(int section, Qt::Orientation orientation, int role ) const;

 /*!
  * \ingroup      Control
  * \brief        Returns the item flags for the given index.
  * \param        index of requested data
  * \return       index flags
  * \par      Source:
  *                ObjectListModel.cpp
  */
  Qt::ItemFlags flags(const QModelIndex &index) const;

 /*!
  * \ingroup      Control
  * \brief        Returns the index of the item in the model specified by the given row, column and parent index.
  * \param        row row number
  * \param        column column number
  * \param        parent index of the parent item
  * \return       index of the requested item
  * \par      Source:
  *                ObjectListModel.cpp
  */
  QModelIndex index(int row, int column,
                              const QModelIndex &parent) const ;

 /*!
  * \ingroup      Control
  * \brief        Returns the parent of the model index, or QModelIndex() if it has no parent.
  * \param        child index of the child
  * \return       index of the parent item
  * \par      Source:
  *                ObjectListModel.cpp
  */
  QModelIndex parent(const QModelIndex &child) const ;

 /*!
  * \ingroup      Control
  * \brief        Sets the role data for the item at index to value. Returns true if successful; otherwise returns false.
  * \param        index to be set
  * \param        value new value
  * \param        role to be set
  * \return       true if success, false if failed
  * \par      Source:
  *                ObjectListModel.cpp
  */
  bool setData(const QModelIndex &index, const QVariant &value, int role);

 /*!
  * \ingroup      Control
  * \brief        Returns an object of the model
  * \param        index index of the requested object
  * \return       requested WoolzObject
  * \par      Source:
  *                ObjectListModel.cpp
  */
  WoolzObject *getObject(const QModelIndex & index) const;

 /*!
  * \ingroup      Control
  * \brief        Adds object to the store
  * \param        object the object to be added
  * \return       void 
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void addObject(WoolzObject * object);

 /*!
  * \ingroup      Control
  * \brief        Adds all object from a store to the viewer
  * \param        type object store type
  * \param        viewer to add objects to
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void addAllObjectsTo(WoolzObject::WoolzObjectType type, ObjectViewer *viewer);

 /*!
  * \ingroup      Control
  * \brief        Removes an object and frees all allocated memmory
  * \param        object the object to be removed
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void removeObject(WoolzObject * object);

 /*!
  * \ingroup      Control
  * \brief        Removes an object representation data but does not deletes it. Therefore can be restored.
  * \param        object the object to be removed
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void removeObjectNoDelete(WoolzObject * object);

 /*!
  * \ingroup      Control
  * \brief        Returns a list of source and target objects.
  *
  * \param        sources if true, list includes source objects
  * \param        targets if true, list includes target objects
  * \param        valueOnly include value objects only
  * \return       list of objects
  * \par      Source:
  *                ObjectListModel.cpp
  */
  QList <WoolzObject*> getObjects(bool sources, bool targets, bool valueOnly=false);

 /*!
  * \ingroup      Control
  * \brief        Returns the first warped object.
  * \return       the first warped object
  * \par      Source:
  *                ObjectListModel.cpp
  */
  WoolzObject* getFirstWarped();

 /*!
  * \ingroup      Control
  * \brief        Returns the object with the given ID.
  * \return       pointer to the object of NULL if can't be found.
  * \par      Source:
  *                ObjectListModel.cpp
  */
  WoolzObject* getObject(int ID);

 /*!
  * \ingroup      Control
  * \brief        Returns the mesh object.
  * \return       the mesh object
  * \par      Source:
  *                ObjectListModel.cpp
  */
  WoolzObject* getMeshObject();

 /*!
  * \ingroup      Control
  * \brief        Adds or replaces the match object.
  * \param        meshObject new mesh object
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void addMeshObject(WoolzObject* meshObject);

 /*!
  * \ingroup      Control
  * \brief        Adds or replaces the match object, but the old object is not freed from memory.
  * \param        meshObject new mesh object
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void addMeshObjectNoDelete(WoolzObject* meshObject);

 /*!
  * \ingroup      Control
  * \brief        Returns the index of the object.
  * \param        object query object
  * \return       index of the requested obect
  * \par      Source:
  *                ObjectListModel.cpp
  */
  QModelIndex getObjIndex(WoolzObject *object) const;

 /*!
  * \ingroup      Control
  * \brief        Adds command to the command (undo) stack.
  * \param        command command to add
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void addCommand(QUndoCommand *command);

 /*!
  * \ingroup      Control
  * \brief        Saves model in xml format.
  * \param        xmlWriter output xml stream
  * \return       true if succeded, false if not
  * \par      Source:
  *                ObjectListModel.cpp
  */
  virtual bool saveAsXml(QXmlStreamWriter *xmlWriter);

 /*!
  * \ingroup      Control
  * \brief        Reads object model from DOM
  * \param        element current element of the DOM tree
  * \return       true if succeded, false if not
  * \par      Source:
  *                ObjectListModel.cpp
  */
  bool parseDOM(const QDomElement &element);

 /*!
  * \ingroup      Control
  * \brief        Load objects
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void loadAll();

protected:
 /*!
  * \ingroup      Control
  * \brief        Returns the store (source/target/warped) of the index.
  * \param        index of an item
  * \return       0 for source, 1 for target, 2 for warped, -1 for not in model
  * \par      Source:
  *                ObjectListModel.cpp
  */
  int getStore(const QModelIndex & index) const;

 /*!
  * \ingroup      Control
  * \brief        Removes an object from a store
  * \param        store the store number
  * \param        object the object to be removed
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void removeObjectFromStore(int store, WoolzObject * object);

 /*!
  * \ingroup      Control
  * \brief        Create WoolzObject for the given xml tag
  * \param        xmlTag xml tag of the saved Woolz Object
  * \return       new object or NULL for invalid tag
  * \par      Source:
  *                ObjectListModel.cpp
  */
  WoolzObject * createObject(QString xmlTag);

 /*!
  * \ingroup      Control
  * \brief        Reads object list from xml format in a store
  * \param        store the store number
  * \param        xmlReader input xml stream
  * \return       true if succeded, false if not
  * \par      Source:
  *                ObjectListModel.cpp
  */
  bool readStoreFromXml(int store, const QDomElement &element);

signals:
 /*!
  * \ingroup      Control
  * \brief        Signals the addition of an object. Viewers should process this signal
  * \param        object the object to be added
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void addObjectSignal (WoolzObject * object);

 /*!
  * \ingroup      Control
  * \brief        Signals the removal of an object. Viewers should process this signal.
  * \param        object the object to be removed
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void removedObjectSignal (WoolzObject * object);

 /*!
  * \ingroup      Control
  * \brief        Signals the update request of updatable objects
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void updateAllSignal(bool);

 /*!
  * \ingroup      Control
  * \brief        Signals the update request of updatable objects
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void updateAllWarpedSignal(bool);

 /*!
  * \ingroup      Control
  * \brief        Signals changed mesh for warping
  * \param        mesh new mesh used 
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void replaceWarpMesh(WoolzObject *mesh);

  /*!
   * \ingroup      Control
   * \brief        Signals that an object was changed and needs update
   * \return       void
   * \par      Source:
   *                WarperController.cpp
   */
  void updatePossibleChange(bool possible);

protected slots:
/*!
  * \ingroup      Control
  * \brief        Removal of an object. Viewers should process this signal.
  * \param        object the object to be removed
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void objectChanged();

 /*!
  * \ingroup      Control
  * \brief        Change of the object type
  * \param        oldtype old object type
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void objectTypeChanged();


public slots:
 /*!
  * \ingroup      Control
  * \brief        Updates all dynamic objects
  * \param        force if true updates are forced
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void updateAll(bool force = false);

 /*!
  * \ingroup      Control
  * \brief        Updates all warped dynamic objects
  * \param        force if true updates are forced
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void updateAllWarped(bool force = false);

 /*!
  * \ingroup      Control
  * \brief        Adds all object from a store to the sender viewer
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void addAllObjects(ObjectViewer *viewer);

  /*!
   * \ingroup      Control
   * \brief        Processes if an object is updated
   * \return       void
   * \par      Source:
   *                WarperController.cpp
   */
   void objectUpdated(bool updated);

protected:
  QList <WoolzObject*> objects[3];  /*!< object stores: 0 - source, 1-target, 2-warped */
  WoolzObject *m_meshObject;        /*!< mesh object for direct access */
  QUndoStack *m_undoStack;          /*!< undo stack */
  WoolzTransform *m_woolzTransform; /*!< transformation object */

  bool m_allObjectsUpdated;         /*!< true if all objects are updated */
public:
  static const char * xmlTag;                /*!< xml section tag string */
};

#endif // OBJECTLIST_H
