#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ObjectListModelGeneal_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ObjectListModelGeneral.h
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

#ifndef OBJECTLISTMODELGENERAL_H
#define OBJECTLISTMODELGENERAL_H

#include "WoolzObject.h"
#include <QWidget>
#include <QAbstractItemModel>

class ObjectViewer;

/*!
* \brief	Model to manage the collection of objects
* \ingroup      Control
*/
class ObjectListModelAbstract : public QAbstractItemModel
{
  Q_OBJECT
public:
 /*!
  * \ingroup      Control
  * \brief        Constructor
  * \param        parent parent object
  * \return       void
  * \par      Source:
  *                ObjectListModelAbstract.cpp
  */
  ObjectListModelAbstract ( QObject * parent = 0 );

 /*!
  * \ingroup      Control
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *                ObjectListModelAbstract.cpp
  */
  virtual ~ObjectListModelAbstract ( );

 /*!
  * \ingroup      Control
  * \brief        Returns the number of rows.
  * \param        parent parent index
  * \return       3 for the root index, otherwise the number of object for source/target/warped types.
  * \par      Source:
  *                ObjectListModelAbstract.cpp
  */
//  int rowCount(const QModelIndex & parent) const;

 /*!
  * \ingroup      Control
  * \brief        Returns the number of columns.
  * \param        parent parent index
  * \return       3
  * \par      Source:
  *                ObjectListModelAbstract.cpp
  */
//  int columnCount(const QModelIndex & parent) const;

 /*!
  * \ingroup      Control
  * \brief        Returns the data stored under the given role for the item referred to by the index.
  * \param        index of requested data
  * \param        role requested role
  * \return       data requested
  * \par      Source:
  *                ObjectListModelAbstract.cpp
  */
//  QVariant data(const QModelIndex & index, int role ) const;

 /*!
  * \ingroup      Control
  * \brief        Returns the data for the given role and section in the header with the specified orientation.
  * \param        section requested section
  * \param        orientation requested orientation 
  * \param        role requested role
  * \return       data requested
  * \par      Source:
  *                ObjectListModelAbstract.cpp
  */
//  QVariant headerData(int section, Qt::Orientation orientation, int role ) const;

 /*!
  * \ingroup      Control
  * \brief        Returns the item flags for the given index.
  * \param        index of requested data
  * \return       index flags
  * \par      Source:
  *                ObjectListModelAbstract.cpp
  */
  //Qt::ItemFlags flags(const QModelIndex &index) const;

 /*!
  * \ingroup      Control
  * \brief        Returns the index of the item in the model specified by the given row, column and parent index.
  * \param        row row number
  * \param        column column number
  * \param        parent index of the parent item
  * \return       index of the requested item
  * \par      Source:
  *                ObjectListModelAbstract.cpp
  */
  //QModelIndex index(int row, int column,
    //                          const QModelIndex &parent) const ;

 /*!
  * \ingroup      Control
  * \brief        Returns the parent of the model index, or QModelIndex() if it has no parent.
  * \param        child index of the child
  * \return       index of the parent item
  * \par      Source:
  *                ObjectListModelAbstract.cpp
  */
//  QModelIndex parent(const QModelIndex &child) const ;

 /*!
  * \ingroup      Control
  * \brief        Sets the role data for the item at index to value. Returns true if successful; otherwise returns false.
  * \param        index to be set
  * \param        value new value
  * \param        role to be set
  * \return       true if success, false if failed
  * \par      Source:
  *                ObjectListModelAbstract.cpp
  */
//  bool setData(const QModelIndex &index, const QVariant &value, int role);

 /*!
  * \ingroup      Control
  * \brief        Returns an object of the model
  * \param        index index of the requested object
  * \return       requested WoolzObject
  * \par      Source:
  *                ObjectListModelAbstract.cpp
  */
  virtual WoolzObject *getObject(const QModelIndex & index) const = 0;

 /*!
  * \ingroup      Control
  * \brief        Adds object to the store
  * \param        object the object to be added
  * \return       void 
  * \par      Source:
  *                ObjectListModelAbstract.cpp
  */
  virtual void addObject(WoolzObject * object) = 0;

  /*!
  * \ingroup      Control
  * \brief        Removes an object and frees all allocated memmory
  * \param        object the object to be removed
  * \return       void
  * \par      Source:
  *                ObjectListModelAbstract.cpp
  */
  virtual void removeObject(WoolzObject * object) =0;

 /*!
  * \ingroup      Control
  * \brief        Removes an object
  * \param        index the object index to be removed
  * \return       void
  * \par      Source:
  *                ObjectListModelAbstract.cpp
  */
  virtual void removeObject(const QModelIndex &index);

 /*!
  * \ingroup      Control
  * \brief        Check if an object is removable
  * \param        index an object index
  * \return       true if object is removable, false if object can only be replaced, but not removed
  * \par      Source:
  *                ObjectListModelAbstract.cpp
  */
  virtual bool removable(const QModelIndex & index) const;

  /*!
  * \ingroup      Control
  * \brief        Called to propagete new object selection.
  * \param        object the object to be selected
  * \return       void
  * \par      Source:
  *                ObjectListModelAbstract.cpp
  */
  virtual void setSelectObject(WoolzObject * object);

 /*!
  * \ingroup      Control
  * \brief        Returns the index of the object.
  * \param        object query object
  * \return       index of the requested obect
  * \par      Source:
  *                ObjectListModelAbstract.cpp
  */
  virtual QModelIndex getObjIndex(WoolzObject *object) const =0;

signals:
 /*!
  * \ingroup      Control
  * \brief        Signals the addition of an object. Viewers should process this signal
  * \param        object the object to be added
  * \return       void
  * \par      Source:
  *                ObjectListModelAbstract.cpp
  */
  virtual void addObjectSignal (WoolzObject * object);

 /*!
  * \ingroup      Control
  * \brief        Signals the removal of an object. Viewers should process this signal.
  * \param        object the object to be removed
  * \return       void
  * \par      Source:
  *                ObjectListModelAbstract.cpp
  */
  virtual void removedObjectSignal (WoolzObject * object);

 /*!
  * \ingroup      Control
  * \brief        Signals the selection of an object.
  * \param        object the object to be selected
  * \return       void
  * \par      Source:
  *                ObjectListModelAbstract.cpp
  */
  virtual void objectSelected(WoolzObject * object);

 /*!
  * \ingroup      Control
  * \brief        Signals the update request of updatable objects
  * \param        oldtype old object type
  * \return       void
  * \par      Source:
  *                ObjectListModelAbstract.cpp
  */
//  void updateAllSignal();

protected slots:

 /*!
  * \ingroup      Control
  * \brief        Removal of an object. Viewers should process this signal.
  * \param        object the object to be removed
  * \return       void
  * \par      Source:
  *                ObjectListModelAbstract.cpp
  */
  virtual void objectChanged() =0;

 /*!
  * \ingroup      Control
  * \brief        Selection of an object
  * \param        oldtype old object type
  * \return       void
  * \par      Source:
  *                ObjectListModelAbstract.cpp
  */
  virtual void selectAnObject();
protected:

 /*!
  * \ingroup      Control
  * \brief        Removes an object
  * \param        object object to be removed
  * \return       void
  * \par      Source:
  *                ObjectListModelAbstract.cpp
  */
  void removeSelected(WoolzObject * object);

protected:
  int m_objectIDCounter;            /*!< ID object counter generating individual object IDs */
  WoolzObject *m_selectedObject;    /*!< selected object */
};

#endif // OBJECTLISTMODELGENERAL_H
