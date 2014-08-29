#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _ObjectViewerModel_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         ObjectViewerModel.h
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
* \brief	Model managing the viewers and views.
* \ingroup	Control
*/

#ifndef OBJECTVIEWERMODEL_H
#define OBJECTVIEWERMODEL_H

#include <QAbstractItemModel>
#include <QList>

class QMdiArea ;
class ObjectView; 
class ObjectViewer;
class WoolzObject;

/*!
 * \brief	Model of viewers. Manages the object viewers and views
 *
 *             	The list of viewers is stored and partly managed by the
 *             	QMDIArea Qt object.
 *              Each viewer has its own list views contained.
 * \ingroup	Control
 */
class ObjectViewerModel : public QAbstractItemModel 
{
  Q_OBJECT
  public:

    /*!
     * \ingroup	Control
     * \brief	Constructor
     * \param	parent			parent object
     */
    ObjectViewerModel(QMdiArea *mdiArea, QObject * parent = 0);

    /*!
     * \ingroup	Control
     * \brief	Destructor
     */
    virtual ~ObjectViewerModel();

    /*!
     * \ingroup	Control
     * \brief	Returns the number of rows.

     * \param	parent			parent index
     * \return	the number of viewers/views
     */
    int rowCount(const QModelIndex & parent) const;

    /*!
     * \ingroup	Control
     * \brief	Returns the number of columns.
     * \param	parent			parent index
     * \return	7
     */
    int columnCount(const QModelIndex & parent) const;

    /*!
     * \ingroup	Control
     * \brief	Returns the data stored under the given role for the item
     * 		referred to by the index.
     * \param	index			of requested data
     * \param	role			requested role
     * \return	data requested
     */
    QVariant data(const QModelIndex & index, int role ) const;

    /*!
     * \ingroup	Control
     * \brief	Returns the data for the given role and section in the
     * 		header with the specified orientation.
     * \param	section			requested section
     * \param	orientation			requested orientation 
     * \param	role			requested role
     * \return	data requested
     */
    QVariant headerData(int section, Qt::Orientation orientation,
        int role ) const;

    /*!
     * \ingroup	Control
     * \brief	Returns the item flags for the given index.
     * \param	index			of requested data
     * \return	index flags
     */
    Qt::ItemFlags flags(const QModelIndex &index) const;

    /*!
     * \ingroup	Control
     * \brief	Returns the index of the item in the model specified by the
     * 		given row, column and parent index.
     * \param	row			row number
     * \param	column			column number
     * \param	parent			index of the parent item
     * \return	index of the requested item
     */
    QModelIndex index(int row, int column,
	const QModelIndex &parent) const ;

    /*!
     * \ingroup	Control
     * \brief	Returns the parent of the model index, or QModelIndex()
     * 		if it has no parent.
     * \param	child			index of the child
     * \return	index of the parent item
     */
    QModelIndex parent(const QModelIndex &child) const ;

    /*!
     * \ingroup	Control
     * \brief	Returns the view at an index.
     * \param	index			of the requested view
     * \return	view or NULL if the indexed item is not a view
     */
    ObjectView *getView(const QModelIndex & index) const;

    /*!
     * \ingroup	Control
     * \brief	Returns the viewer at an index.
     * \param	index			of the requested viewer
     * \return	viewer or NULL if the indexed item is not a viewer
     */
    ObjectViewer *getViewer(const QModelIndex & index) const;

    /*!
     * \ingroup	Control
     * \brief	Returns the viewer of the viewer at an index.
     * \param	index			of the view
     * \return	the viewer of the view indexed or the viewer indexed, or
     * 		if neither then NULL
     */
    ObjectViewer *getViewerOf(const QModelIndex & index) const;

    /*!
     * \ingroup	Control
     * \brief	Sets the role data for the item at index to value.
     * 		Returns true if successful; otherwise returns false.
     * \param	index			to be set
     * \param	value			new value
     * \param	role			to be set
     * \return	true if success, false if failed
     */
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    /*!
     * \ingroup	Control
     * \brief	Checks if the item at an index is a view
     * \param	index			of the requested view
     * \return	true if item is a view, false if not
     */
    bool isView(const QModelIndex & index) const;

    /*!
     * \ingroup	Control
     * \brief	Checks if the item at an index is a viewer
     * \param	index			of the requested viewer
     * \return	true if item is a viewer, false if not
     */
    bool isViewer(const QModelIndex & index) const;

    /*!
     * \ingroup	Control
     * \brief	Connects view viewPropertyChanged signal to the viewer
     * \param	view			view to be connected
     */
    void listenToObject(ObjectView *view);

    /*!
     * \ingroup	Control
     * \brief	Prepares viewer IDs for saving
     */
    void prepareIDs();

    /*!
     * \ingroup	Control
     * \brief	Restores links after viewer are read from disk
     */
    void restoreLinks();

    /*!
     * \ingroup	Control
     * \brief	Provides the list of ObjectViewers
     * \return	list of object viewers
     */
    QList<ObjectViewer *>  getViewers() const;

  protected:
    /*!
     * \ingroup	Control
     * \brief	Provides the ObjectViewer with the given ID
     * \param	ID			ID of the object to be searched
     * \return	object with the given ID or NULL if such does not exists
     */
    ObjectViewer* findViewerByID(int ID);

  public slots:
    /*!
     * \ingroup	Control
     * \brief	Processes configuration changes
     */
    void configurationChanged()
    {
      emit setBackgroundColour();
    }

    /*!
     * \ingroup	Control
     * \brief	Processes viewer add
     *
     *               This must be called *after* mdiArea is updated.
     * \param	viewer			added
     */
    void addedViewer(ObjectViewer * viewer);

    /*!
     * \ingroup	Control
     * \brief	Prepares for viewer removal
     */
    void removedViewerStart();

    /*!
     * \ingroup	Control
     * \brief	Finishes viewer removal
     */
    void removedViewerFinished();

    /*!
     * \ingroup	Control
     * \brief	Updates selection focus when viewer focus changes
     */
    void changedViewer();

    /*!
     * \ingroup	Control
     * \brief	Updates selection focus when viewer focus changes
     */
    void changedObjectView(ObjectView *);

    /*!
     * \ingroup	Control
     * \brief	Processes view add
     * \param	view			added
     */
    void addedView(ObjectView *view);

    /*!
     * \ingroup	Control
     * \brief	Processes view remove begining
     * \param	view			removed
     */
    void removedViewStart(ObjectView *view);

    /*!
     * \ingroup	Control
     * \brief	Finishes the view removal
     * \param	view			removed
     */
    void removedViewFinish(ObjectView *view);

  signals:
    /*!
     * \ingroup	UI
     * \brief	Signals background colour change
     */
    void setBackgroundColour();

    /*!
     * \ingroup	Control
     * \brief	Requesting objects.
     */
    void signalRequestAllSignals();

  protected:
    /*!
     * \ingroup	Control
     * \brief	Returns the row number of a viewer
     * \param	viewer			queried
     * \return	row number
     */
    int getRowOfViewer(const ObjectViewer * viewer) const;

  protected:
    QMdiArea *m_mdiArea;           /*!< MDI area managing viewers */
  private:
    bool deleteInProgress;         /*!< deletion in progress flag */
};

#endif // OBJECTVIEWERMODEL_H
