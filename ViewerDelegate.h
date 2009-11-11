#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ViewerDelegate_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ViewerDelegate.h
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
* \brief        Delegate for viewer tool tree view
* \ingroup      UI
*
*/
#ifndef VIEWERDELEGATE_H
#define VIEWERDELEGATE_H

#include <QStyledItemDelegate>
#include <QModelIndex>

class QPainter;
class QMdiArea;

/*!
 * \brief	Provides display and editing facilities in viewer tool tree view
 * \ingroup      UI
 */
class ViewerDelegate : public QStyledItemDelegate
{
Q_OBJECT
public:
 /*!
  * \ingroup      UI
  * \brief        Constructor
  * \param        mdiArea area containing the viewers
  * \param        parent parent object
  * \par      Source:
  *                ViewerDelegate.cpp
  */
  ViewerDelegate(QMdiArea * mdiArea, QObject *parent = 0);

 /*!
  * \ingroup      UI
  * \brief        Renders the delegate
  *               using the given painter and style option for
  *               the item specified by index.
  * \return      void
  * \par      Source:
  *                ViewerDelegate.cpp
  */
  void paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
 /*!
  * \ingroup      UI
  * \brief        Returns the widget used to edit
  *
  *  the item specified by index for editing.
  * \param        parent parent widget 
  * \param        option style option are used to control how the editor widget appears
  * \param        index index to edit
  * \return       editor widget
  * \par      Source:
  *                ViewerDelegate.cpp
  */
  QWidget * createEditor ( QWidget * parent, const QStyleOptionViewItem & option, 
       const QModelIndex & index ) const;

 /*!
  * \ingroup      UI
  * \brief        Sets the data to be displayed and edited
  * \param        editor editor doing the job
  * \param        index data index in the model to be edited
  * \return       void
  * \par      Source:
  *                ViewerDelegate.cpp
  */
  void setEditorData(QWidget *editor, const QModelIndex &index) const;

 /*!
  * \ingroup      UI
  * \brief        Gets data drom the editor widget
  *               and stores it in the the model at the item index
  * \param        editor editor doing the job
  * \param        model  model to be set
  * \param        index  data index in the model to be set
  * \return       void
  * \par      Source:
  *                ViewerDelegate.cpp
  */
  void setModelData(QWidget *editor, QAbstractItemModel *model, 
         const QModelIndex &index) const;
};
#endif
