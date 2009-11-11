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
#include "ObjectListModelAbstract.h"

class QUndoStack;
class QUndoCommand;
class ObjectSimpleViewer;

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
  * \param        parent parent object
  * \param        parent parent object
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  ObjectListModel ( QObject * parent = 0 );

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
  virtual void addObject(WoolzObject * object);

 /*!
  * \ingroup      Control
  * \brief        Adds all object from a store to the viewer
  * \param        type object store type
  * \param        viewer to add objects to
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void addAllObjectsTo(ObjectSimpleViewer *viewer);

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
  * \brief        Removes an object
  * \param        index the object index to be removed
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void removeObject(const QModelIndex &index);

 /*!
  * \ingroup      Control
  * \brief        Check if an object is removable
  * \param        index an object index
  * \return       true if object is removable, false if object can only be replaced, but not removed
  * \par      Source:
  *                ObjectListModel.cpp
  */
  bool removable(const QModelIndex & index) const;

 /*!
  * \ingroup      Control
  * \brief        Called to propagete new object selection.
  * \param        object the object to be selected
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void setSelectObject(WoolzObject * object);

 /*!
  * \ingroup      Control
  * \brief        Returns the index of the object.
  * \param        object query object
  * \return       index of the requested obect
  * \par      Source:
  *                ObjectListModel.cpp
  */
  QModelIndex getObjIndex(WoolzObject *object) const;

protected:
 /*!
  * \ingroup      Control
  * \brief        Removes an object from a store
  * \param        object the object to be removed
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void removeObjectFromStore(WoolzObject * object);

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
  * \brief        Signals the selection of an object.
  * \param        object the object to be selected
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void objectSelected(WoolzObject * object);

 /*!
  * \ingroup      Control
  * \brief        Signals the update request of updatable objects
  * \param        oldtype old object type
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void updateAllSignal();

  /*!
  * \ingroup      Control
  * \brief        Signals the update request of updatable objects
  * \param        oldtype old object type
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void updateAllWarpedSignal();

 /*!
  * \ingroup      Control
  * \brief        Signals changed mesh for warping
  * \param        mesh new mesh used 
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void replaceWarpMesh(WoolzObject *mesh);

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

 /*!
  * \ingroup      Control
  * \brief        Selection of an object
  * \param        oldtype old object type
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void selectAnObject();

public slots:

protected:
  QList <WoolzObject*> objects;  /*!< object stores: 0 - source, 1-target, 2-warped */
};

#endif // OBJECTLIST_H
