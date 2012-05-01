#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _MainWindow_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         MainWindow.h
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
* \brief        Application main window
* \ingroup      UI
*
*/

#ifndef MYQTAPP_H
#define MYQTAPP_H

// Qt4 includes
#include <QMdiArea>

// UI includes
#include "ui_MainWindow.h"

//own includes
#include "MainWindow.h"
#include <WoolzObject.h>

class QAction;
class QMdiArea;

class ObjectListModelAbstract ;
class ObjectSimpleViewer;
class ObjectToolDialog;
class TransferFunctionWidget;

/*!
 * \brief	Application main window
 * \ingroup      UI
 */
class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT
public:
 /*!
  * \ingroup      UI
  * \brief        Constructor
  * \return       void
  * \par      Source:
  *                MainWindow.cpp
  */
  MainWindow();

 /*!
  * \ingroup      UI
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *                MainWindow.cpp
  */
  ~MainWindow();

 /*!
  * \ingroup      UI
  * \brief        Adds an MDI window
  * \param        window MDI child window
  * \return       void
  * \par      Source:
  *                MainWindow.cpp
  */
  void addMDIWindows(QWidget * window);

 /*!
  * \ingroup      UI
  * \brief        Returns the current MDI workspace.
  * \return       MDI workspace
  * \par      Source:
  *                MainWindow.cpp
  */
  QMdiArea * getWorkspace() {return mdiArea;}

protected:
 /*!
  * \ingroup      Control
  * \brief        Returns the types and extensions of value formats to be used as QFileDialog format parameter
  * \return       string of formats for QFileDialog
  * \par      Source:
  *                WarperController.cpp
  */
  QString getValueFormats();

 /*!
  * \ingroup      Control
  * \brief        Loads value obect from a file
  * \param        filename from which mesh is read
  * \param        type object type (source or target)
  * \return       a newly created WoolzObject
  * \par      Source:
  *                WarperController.cpp
  */
  WoolzObject* load(QString filename , WoolzObject::WoolzObjectType type);

 /*!
  * \ingroup      Control
  * \brief        Creates a new viewer
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void addViewer ();

public slots:
 /*!
  * \ingroup      UI
  * \brief        Shows the about dialog of the application
  *
  *               Changes project mode to warping, updates main menu.
  * \return       void
  * \par      Source:
  *                MainWindow.cpp
  */
  void about();

 /*!
  * \ingroup      UI
  * \brief        Opens a value target object
  *
  *               Changes project mode to warping, updates main menu.
  * \return       void
  * \param	  filename path to file.
  * \par      Source:
  *                MainWindow.cpp
  */
  void openObject(QString filename);

 /*!
  * \ingroup      UI
  * \brief        Opens a value target object
  *
  *               Changes project mode to warping, updates main menu.
  * \return       void
  * \par      Source:
  *                MainWindow.cpp
  */
  void openObjectFromDialog();

 /*!
  * \ingroup      Control
  * \brief        Opens the object tool dialog
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void openObjectDialog ();

 /*!
  * \ingroup      UI
  * \brief        Sets interface options
  * \return       void
  * \par      Source:
  *                MainWindow.cpp
  */
  void options();

 /*!
  * \ingroup      Control
  * \brief        Minimizes the current window
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void minimizeSubWindow();

 /*!
  * \ingroup      Control
  * \brief        Maximizes the current window
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void maximizeSubWindow();

 /*!
  * \ingroup      Control
  * \brief        Restores the size of the current window
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void restoreSubWindow();

protected:
   ObjectSimpleViewer *m_viewer;
   ObjectToolDialog *m_objectToolDialog;              /*!< object tool dialog */
   ObjectListModelAbstract *m_objectListModel;                /*!< object container */
   TransferFunctionWidget* m_transferFunctionWidget;  /*!< transfer function editor widget*/
   bool m_project3D;
};
#endif
