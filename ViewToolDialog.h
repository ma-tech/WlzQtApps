#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ViewToolDialog_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ViewToolDialog.h
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
* \brief        Viewer and view tool dialog
* \ingroup      UI
*
*/
#ifndef VIEWTOOLDIALOG_H
#define VIEWTOOLDIALOG_H

#include <QDialog>
#include <ui_ViewToolDialog.h>

class ObjectViewerModel;
class MainWindow;

/*!
 * \brief	Controler class for view tool dialog
 * \ingroup      UI
 */
class ViewToolDialog : public QDialog, public Ui::ViewToolDialog
{
Q_OBJECT
public:

 /*!
  * \ingroup      UI
  * \brief        Constructor
  * \param        model object viewer model
  * \param        parent parent widget
  * \par      Source:
  *                ViewToolDialog.cpp
  */
  ViewToolDialog(ObjectViewerModel *objectViewerModel, MainWindow *parent);

 /*!
  * \ingroup      UI
  * \brief        Destructor
  * \par      Source:
  *                ViewToolDialog.cpp
  */
  ~ViewToolDialog();

private slots:
 /*!
  * \ingroup      UI
  * \brief        Processes the signal of edit button hit
  * \par      Source:
  *                ViewToolDialog.cpp
  */
  void EditView();

 /*!
  * \ingroup      UI
  * \brief        Processes the signal of object selection change

  *               It also changes the focus in the MDI window
  * \param        current new selection
  * \param        previous previous selection
  * \par      Source:
  *                ViewToolDialog.cpp
  */
  void currentChanged ( const QModelIndex & current, const QModelIndex & previous );
};

#endif
