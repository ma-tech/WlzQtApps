#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ObjectViewerModel_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ObjectViewerModel.h
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
* \brief        Model managing the viewers and views.
* \ingroup      Control
*
*/

#ifndef OBJECTVIEWERMODEL_H
#define OBJECTVIEWERMODEL_H

#include <QAbstractItemModel>

class QMdiArea ;
class ObjectView; 
class ObjectViewer;
class WoolzObject;

/*!
 * \brief	Model of viewers. Manages the object viewers and views
 *
 *               The list of viewers is stored and partly managed by the QMDIArea Qt object.
 *               Each viewer has its own list views contained.
 * \ingroup      Control
 */
class ObjectViewerModel : public QAbstractItemModel 
{
  Q_OBJECT
public:

 /*!
  * \ingroup      Control
  * \brief        Constructor
  * \param        parent parent object
  * \return       void
  * \par      Source:
  *                ObjectViewerModel.cpp
  */
  ObjectViewerModel (QMdiArea *mdiArea, QObject * parent = 0 );

 /*!
  * \ingroup      Control
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *                ObjectViewerModel.cpp
  */
  virtual ~ObjectViewerModel ( );

 /*!
  * \ingroup      Control
  * \brief        Returns the number of rows.

  * \param        parent parent index
  * \return       the number of viewers/views
  * \par      Source:
  *                ObjectViewerModel.cpp
  */
  int rowCount(const QModelIndex & parent) const;

 /*!
  * \ingroup      Control
  * \brief        Returns the number of columns.
  * \param        parent parent index
  * \return       7
  * \par      Source:
  *                ObjectViewerModel.cpp
  */
  int columnCount(const QModelIndex & parent) const;

 /*!
  * \ingroup      Control
  * \brief        Returns the data stored under the given role for the item referred to by the index.
  * \param        index of requested data
  * \param        role requested role
  * \return       data requested
  * \par      Source:
  *                ObjectViewerModel.cpp
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
  *                ObjectViewerModel.cpp
  */
  QVariant headerData(int section, Qt::Orientation orientation, int role ) const;

 /*!
  * \ingroup      Control
  * \brief        Returns the item flags for the given index.
  * \param        index of requested data
  * \return       index flags
  * \par      Source:
  *                ObjectViewerModel.cpp
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
  *                ObjectViewerModel.cpp
  */
  QModelIndex index(int row, int column,
                              const QModelIndex &parent) const ;

 /*!
  * \ingroup      Control
  * \brief        Returns the parent of the model index, or QModelIndex() if it has no parent.
  * \param        child index of the child
  * \return       index of the parent item
  * \par      Source:
  *                ObjectViewerModel.cpp
  */
  QModelIndex parent(const QModelIndex &child) const ;

 /*!
  * \ingroup      Control
  * \brief        Returns the view at an index.
  * \param        index of the requested view
  * \return       view or NULL if the indexed item is not a view
  * \par      Source:
  *                ObjectViewerModel.cpp
  */
  ObjectView *getView(const QModelIndex & index) const;

 /*!
  * \ingroup      Control
  * \brief        Returns the viewer at an index.
  * \param        index of the requested viewer
  * \return       viewer or NULL if the indexed item is not a viewer
  * \par      Source:
  *                ObjectViewerModel.cpp
  */
  ObjectViewer *getViewer(const QModelIndex & index) const;

 /*!
  * \ingroup      Control
  * \brief        Returns the viewer of the viewer at an index.
  * \param        index of the view
  * \return       the viewer of the view indexed or the viewer indexed, or if neither then NULL
  * \par      Source:
  *                ObjectViewerModel.cpp
  */
  ObjectViewer *getViewerOf(const QModelIndex & index) const;

 /*!
  * \ingroup      Control
  * \brief        Sets the role data for the item at index to value. Returns true if successful; otherwise returns false.
  * \param        index to be set
  * \param        value new value
  * \param        role to be set
  * \return       true if success, false if failed
  * \par      Source:
  *                ObjectViewerModel.cpp
  */
  bool setData(const QModelIndex &index, const QVariant &value, int role);

 /*!
  * \ingroup      Control
  * \brief        Checks if the item at an index is a view
  * \param        index of the requested view
  * \return       true if item is a view, false if not
  * \par      Source:
  *                ObjectViewerModel.cpp
  */
  bool isView(const QModelIndex & index) const;

 /*!
  * \ingroup      Control
  * \brief        Checks if the item at an index is a viewer
  * \param        index of the requested viewer
  * \return       true if item is a viewer, false if not
  * \par      Source:
  *                ObjectViewerModel.cpp
  */
  bool isViewer(const QModelIndex & index) const;

 /*!
  * \ingroup      Control
  * \brief        Connects view viewPropertyChanged signal to the viewer
  * \param        view view to be connected
  * \return       void
  * \par      Source:
  *                ObjectViewerModel.cpp
  */
  void listenToObject(ObjectView *view);

public slots:
  /*!
  * \ingroup      Control
  * \brief        Processes configuration changes
  * \return       void
  * \par      Source:
  *                ObjectViewerModel.cpp
  */
  void configurationChanged() {
      emit setBackgroundColour();
  }

 /*!
  * \ingroup      Control
  * \brief        Processes viewer add
  *
  *               This must be called *after* mdiArea is updated.
  * \param        viewer added
  * \return       void
  * \par      Source:
  *                ObjectViewerModel.cpp
  */
  void addedViewer(ObjectViewer * viewer);

 /*!
  * \ingroup      Control
  * \brief        Prepares for viewer removal
  * \return       void
  * \par      Source:
  *                ObjectViewerModel.cpp
  */
  void removedViewerStart();

 /*!
  * \ingroup      Control
  * \brief        Finishes viewer removal
  * \return       void
  * \par      Source:
  *                ObjectViewerModel.cpp
  */
  void removedViewerFinished();

 /*!
  * \ingroup      Control
  * \brief        Updates selection focus when viewer focus changes
  * \return       void
  * \par      Source:
  *                ObjectViewerModel.cpp
  */
  void changedViewer();

 /*!
  * \ingroup      Control
  * \brief        Updates selection focus when viewer focus changes
  * \return       void
  * \par      Source:
  *                ObjectViewerModel.cpp
  */
  void changedObjectView(ObjectView *);

 /*!
  * \ingroup      Control
  * \brief        Processes view add
  * \param        view added
  * \return       void
  * \par      Source:
  *                ObjectViewerModel.cpp
  */
  void addedView(ObjectView *view);

 /*!
  * \ingroup      Control
  * \brief        Processes view remove begining
  * \param        view removed
  * \return       void
  * \par      Source:
  *                ObjectViewerModel.cpp
  */
  void removedViewStart(ObjectView *view);

 /*!
  * \ingroup      Control
  * \brief        Finishes the view removal
  * \param        view removed
  * \return       void
  * \par      Source:
  *                ObjectViewerModel.cpp
  */
  void removedViewFinish(ObjectView *view);

signals:
 /*!
  * \ingroup      UI
  * \brief        Signals background colour change
  * \return       void
  * \par      Source:
  *                ObjectViewer.cpp
  */
  void setBackgroundColour();

  /*!
  * \ingroup      Control
  * \brief        Requesting objects.
  * \return       void
  * \par      Source:
  *                ObjectListModel.cpp
  */
  void signalRequestAllSignals();

protected:
 /*!
  * \ingroup      Control
  * \brief        Returns the row number of a viewer
  * \param        viewer queried
  * \return       row number
  * \par      Source:
  *                ObjectViewerModel.cpp
  */
  int getRowOfViewer(const ObjectViewer * viewer) const;

protected:
  QMdiArea *m_mdiArea;           /*!< MDI area managing viewers */
private:
  bool deleteInProgress;       /*!< deletion in progress flag */
};

#endif // OBJECTVIEWERMODEL_H
