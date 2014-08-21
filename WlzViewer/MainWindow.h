#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _MainWindow_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         MainWindow.h
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
* \brief	Application main window
* \ingroup	UI
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
 * \ingroup	UI
 */
class MainWindow : public QMainWindow, public Ui::MainWindow
{
  Q_OBJECT
  public:
   /*!
    * \ingroup	UI
    * \brief	Constructor
    */
    MainWindow();

   /*!
    * \ingroup	UI
    * \brief	Destructor
    */
    ~MainWindow();

   /*!
    * \ingroup	UI
    * \brief	Adds an MDI window
    * \param	window			MDI child window
    */
    void addMDIWindows(QWidget * window);

   /*!
    * \return	MDI workspace
    * \ingroup	UI
    * \brief	Returns the current MDI workspace.
    */
    QMdiArea * getWorkspace() {return mdiArea;}

  protected:
   /*!
    * \return	string of formats for QFileDialog
    * \ingroup	Control
    * \brief	Returns the types and extensions of value formats to be
    * 		used as QFileDialog format parameter
    */
    QString getValueFormats();

   /*!
    * \return	a newly created WoolzObject
    * \ingroup	Control
    * \brief	Loads value obect from a file
    * \param	filename		from which mesh is read
    * \param	type			object type (source or target)
    */
    WoolzObject* load(QString filename , WoolzObject::WoolzObjectType type);

   /*!
    * \ingroup	Control
    * \brief	Creates a new viewer
    * \return	void
    */
    void addViewer ();

  public slots:
   /*!
    * \ingroup	UI
    * \brief	Shows the about dialog of the application
    *           Changes project mode to warping, updates main menu.
    */
    void about();

   /*!
    * \ingroup	UI
    * \brief	Opens a value target object
    *           Changes project mode to warping, updates main menu.
    * \param	filename		path to file.
    */
    void openObject(QString filename);

   /*!
    * \ingroup	UI
    * \brief	Opens a value target object
    *           Changes project mode to warping, updates main menu.
    */
    void openObjectFromDialog();

   /*!
    * \ingroup	Control
    * \brief	Opens the object tool dialog
    */
    void openObjectDialog ();

   /*!
    * \ingroup	UI
    * \brief	Sets interface options
    */
    void options();

   /*!
    * \ingroup	Control
    * \brief	Minimizes the current window
    */
    void minimizeSubWindow();

   /*!
    * \ingroup	Control
    * \brief	Maximizes the current window
    */
    void maximizeSubWindow();

   /*!
    * \ingroup	Control
    * \brief	Restores the size of the current window
    */
    void restoreSubWindow();

  protected:
     ObjectSimpleViewer *m_viewer;
     ObjectToolDialog *m_objectToolDialog;	/*!< object tool dialog */
     ObjectListModelAbstract *m_objectListModel;  /*!< object container */
     TransferFunctionWidget* m_transferFunctionWidget; /*!< transfer function
						       editor widget*/
     bool m_project3D;
};
#endif
