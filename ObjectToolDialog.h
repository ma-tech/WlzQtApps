#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ObjectToolDialog_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ObjectToolDialog.h
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
* \brief        Object tool dialog
* \ingroup      UI
*
*/
#ifndef OBJECTTOOLDIALOG_H
#define OBJECTTOOLDIALOG_H

#include <QDialog>
#include <ui_ObjectToolDialog.h>

class ObjectListModel;
class WoolzObject;
/*!
* \brief	Controler class for object tool dialog
* \ingroup      UI
*/
class ObjectToolDialog : public QDialog, public Ui::ObjectToolDialog
{
Q_OBJECT
public:
 /*!
  * \ingroup      UI
  * \brief        Constructor
  * \param        model object model
  * \param        parent parent widget
  * \par      Source:
  *                ObjectToolDialog.cpp
  */
  ObjectToolDialog(ObjectListModel * model, QWidget *parent = 0);

 /*!
  * \ingroup      UI
  * \brief        Destructor
  * \par      Source:
  *                ObjectToolDialog.cpp
  */
  ~ObjectToolDialog();

private slots:
 /*!
  * \ingroup      UI
  * \brief        Processes the signal of delete button press
  * \par      Source:
  *                ObjectToolDialog.cpp
  */
  void removeObject();

/*!
  * \ingroup      UI
  * \brief        Processes the signal of export button press
  * \par      Source:
  *                ObjectToolDialog.cpp
  */
  void exportObject();

 /*!
  * \ingroup      UI
  * \brief        Processes the internal signal of object selection change from the internal widgets
  * \param        current new selection
  * \param        previous previous selection
  * \par      Source:
  *                ObjectToolDialog.cpp
  */
  void currentRowChanged ( const QModelIndex & current, 
       const QModelIndex & /*previous */);

 /*!
  * \ingroup      UI
  * \brief        Processes the external signal of object selection change
  * \param        current new selection
  * \param        previous previous selection
  * \par      Source:
  *                ObjectToolDialog.cpp
  */
  void objectSelected(WoolzObject *obj);
};
#endif
