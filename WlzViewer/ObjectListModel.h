#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _ObjectListModel_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         ObjectListModel.h
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
* \brief	Model managing the collection of objects.
* \ingroup	Control
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
* \ingroup	Control
*/
class ObjectListModel : public ObjectListModelAbstract 
{
  Q_OBJECT
  public:
   /*!
    * \ingroup	Control
    * \brief	Constructor
    * \param	parent			parent object
    */
    ObjectListModel( QObject * parent = 0 );

   /*!
    * \ingroup	Control
    * \brief	Destructor
    * \return	void
    */
    virtual ~ObjectListModel();

   /*!
    * \return	3 for the root index, otherwise the number of object for
    * 		source/target/warped types.
    * \ingroup	Control
    * \brief	Returns the number of rows.
    * \param	parent			parent index
    */
    int rowCount(const QModelIndex & parent) const;

   /*!
    * \return	3
    * \ingroup	Control
    * \brief	Returns the number of columns.
    * \param	parent			parent index
    */
    int columnCount(const QModelIndex & parent) const;

   /*!
    * \return	data requested
    * \ingroup	Control
    * \brief	Returns the data stored under the given role for the item
    * 		referred to by the index.
    * \param	index			index of requested data
    * \param	role			requested role
    */
    QVariant data(const QModelIndex & index, int role) const;

   /*!
    * \return	data requested
    * \ingroup	Control
    * \brief	Returns the data for the given role and section in the header
    * 		with the specified orientation.
    * \param	section			requested section
    * \param	orientation		requested orientation 
    * \param	role			requested role
    */
    QVariant headerData(int section, Qt::Orientation orientation,
    		        int role) const;

   /*!
    * \return	index flags
    * \ingroup	Control
    * \brief	Returns the item flags for the given index.
    * \param	index			of requested data
    */
    Qt::ItemFlags flags(const QModelIndex &index) const;

   /*!
    * \return	index of the requested item
    * \ingroup	Control
    * \brief	Returns the index of the item in the model specified by the
    * 		given row, column and parent index.
    * \param	row			row number
    * \param	column			column number
    * \param	parent			index of the parent item
    */
    QModelIndex index(int row, int column,
		      const QModelIndex &parent) const ;

   /*!
    * \return	index of the parent item
    * \ingroup	Control
    * \brief	Returns the parent of the model index, or QModelIndex() if it
    * 		has no parent.
    * \param	child			index of the child
    */
    QModelIndex parent(const QModelIndex &child) const ;

   /*!
    * \return	true if success, false if failed
    * \ingroup	Control
    * \brief	Sets the role data for the item at index to value. Returns
    * 		true if successful; otherwise returns false.
    * \param	index			to be set
    * \param	value			new value
    * \param	role			to be set
    */
    bool setData(const QModelIndex &index, const QVariant &value, int role);

   /*!
    * \return	requested WoolzObject
    * \ingroup	Control
    * \brief	Returns an object of the model
    * \param	index			index of the requested object
    */
    WoolzObject *getObject(const QModelIndex & index) const;

   /*!
    * \ingroup	Control
    * \brief	Adds object to the store
    * \param	object			the object to be added
    */
    virtual void addObject(WoolzObject * object);

   /*!
    * \ingroup	Control
    * \brief	Adds all object from a store to the viewer
    * \param	type			object store type
    * \param	viewer			to add objects to
    */
    void addAllObjectsTo(ObjectSimpleViewer *viewer);

   /*!
    * \ingroup	Control
    * \brief	Removes an object and frees all allocated memmory
    * \param	object			the object to be removed
    */
    void removeObject(WoolzObject * object);

   /*!
    * \ingroup	Control
    * \brief	Removes an object representation data but does not delete
    * 		it. Therefore can be restored.
    * \param	object			the object to be removed
    */
    void removeObjectNoDelete(WoolzObject * object);

   /*!
    * \ingroup	Control
    * \brief	Removes an object
    * \param	index			the object index to be removed
    */
    void removeObject(const QModelIndex &index);

   /*!
    * \return	true if object is removable, false if object can only be
    * 		replaced, but not removed
    * \ingroup	Control
    * \brief	Check if an object is removable
    * \param	index			an object index
    */
    bool removable(const QModelIndex & index) const;

   /*!
    * \ingroup	Control
    * \brief	Called to propagete new object selection.
    * \param	object			the object to be selected
    * \return	void
    */
    void setSelectObject(WoolzObject * object);

   /*!
    * \return	index of the requested obect
    * \ingroup	Control
    * \brief	Returns the index of the object.
    * \param	object			query object
    */
    QModelIndex getObjIndex(WoolzObject *object) const;

  protected:
   /*!
    * \ingroup	Control
    * \brief	Removes an object from a store
    * \param	object			the object to be removed
    */
    void removeObjectFromStore(WoolzObject * object);

  signals:
   /*!
    * \ingroup	Control
    * \brief	Signals the addition of an object. Viewers should process
    * 		this signal
    * \param	object			the object to be added
    */
    void addObjectSignal (WoolzObject * object);

   /*!
    * \ingroup	Control
    * \brief	Signals the removal of an object. Viewers should process
    * 		this signal.
    * \param	object			the object to be removed
    */
    void removedObjectSignal (WoolzObject * object);

   /*!
    * \ingroup	Control
    * \brief	Signals the selection of an object.
    * \param	object			the object to be selected
    */
    void objectSelected(WoolzObject * object);

   /*!
    * \ingroup	Control
    * \brief	Signals the update request of updatable objects
    * \param	oldtype			old object type
    */
    void updateAllSignal(bool);

    /*!
    * \ingroup	Control
    * \brief	Signals the update request of updatable objects
    * \param	oldtype			old object type
    */
    void updateAllWarpedSignal(bool);

   /*!
    * \ingroup	Control
    * \brief	Signals changed mesh for warping
    * \param	mesh			new mesh used 
    */
    void replaceWarpMesh(WoolzObject *mesh);

  protected slots:
   /*!
    * \ingroup	Control
    * \brief	Removal of an object. Viewers should process this signal.
    * \param	object			the object to be removed
    */
    void objectChanged();

   /*!
    * \ingroup	Control
    * \brief	Change of the object type
    * \param	oldtype			old object type
    */
    void objectTypeChanged();

   /*!
    * \ingroup	Control
    * \brief	Selection of an object
    * \param	oldtype			old object type
    */
    void selectAnObject();

  public slots:

  protected:
    QList <WoolzObject*> objects;  /*!< object stores: 0 - source, 1-target,
    					2-warped */
};

#endif // OBJECTLIST_H
