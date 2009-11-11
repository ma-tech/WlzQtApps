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
#include "ProjectProperties.h"

class WarperController;
class QAction;
class QMdiArea;

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

 /*!
  * \ingroup      UI
  * \brief        Sets main window title
  * \param        additionalTitle part title to add into the title
  * \return       void
   * \par      Source:
  *                MainWindow.cpp
  */
  void setTitle(QString additionalTitle);

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
  * \par      Source:
  *                MainWindow.cpp
  */
  void openValueTarget();

 /*!
  * \ingroup      UI
  * \brief        Opens a mesh target object
  *
  *               Changes project mode to warping, updates main menu.
  * \return       void
  * \par      Source:
  *                MainWindow.cpp
  */
  void openMeshTarget();

 /*!
  * \ingroup      UI
  * \brief        Opens a surface target object
  *
  *               Changes project mode to warping, updates main menu.
  * \return       void
  * \par      Source:
  *                MainWindow.cpp
  */
  void openSurfaceTarget();

 /*!
  * \ingroup      UI
  * \brief        Opens a composite target object
  *
  *               Changes project mode to warping, updates main menu.
  * \return       void
  * \par      Source:
  *                MainWindow.cpp
  */
  void openCompositeTarget();

 /*!
  * \ingroup      UI
  * \brief        Opens a value source object
  *
  *               Changes project mode to warping, updates main menu.
  * \return       void
  * \par      Source:
  *                MainWindow.cpp
  */
  void openValueSource();

 /*!
  * \ingroup      UI
  * \brief        Opens a surface source object
  *
  *               Changes project mode to warping, updates main menu.
  * \return       void
  * \par      Source:
  *                MainWindow.cpp
  */
  void openSurfaceSource();

 /*!
  * \ingroup      UI
  * \brief        Opens a mesh source object
  *
  *               Changes project mode to warping, updates main menu.
  * \return       void
  * \par      Source:
  *                MainWindow.cpp
  */
  void openMeshSource();


 /*!
  * \ingroup      UI
  * \brief        Closes the project.
  *
  *               Changes project mode to none. Removes additional menu options.
  * \return       void
  * \par      Source:
  *                MainWindow.cpp
  */
  void closeProject();

 /*!
  * \ingroup      UI
  * \brief        Sets interface options
  * \return       void
  * \par      Source:
  *                MainWindow.cpp
  */
  void options();

 /*!
  * \ingroup      UI
  * \brief        Opens an existing project
  * \return       void
  * \par      Source:
  *                MainWindow.cpp
  */
  void openProject ();

 /*!
  * \ingroup      UI
  * \brief        Loads project from a file
  * \param        filename file to load
  * \return       void
  * \par      Source:
  *                MainWindow.cpp
  */
  void loadProject(QString filename);

  void statusChanged(QString message, int timeout);

protected:
    ProjectProperties projectSettings;   /*!< project settings */

    WarperController *controler;       /*!< current controller (warping or mesh editing)*/
private:
 /*!
  * \ingroup      UI
  * \brief        Switches tghe project type.
  *
  *               Changes the menu and other project dependent settings.
  * \return       void
  * \par      Source:
  *                MainWindow.cpp
  */
  void switchProjectType(ProjectProperties::ProjectTypes newType);

 /*!
  * \ingroup      UI
  * \brief        Sets up the default signal and slots.
  * \return       void
  * \par      Source:
  *                MainWindow.cpp
  */
  void setDefaultConnections();

 /*!
  * \ingroup      UI
  * \brief        Removes the default signal and slots.
  * \return       void
  * \par      Source:
  *                MainWindow.cpp
  */
  void removeDefaultConnections();

 /*!
  * \ingroup      UI
  * \brief        If result is not 0, performs the project change to the new project type.
  * \param        result condition to change, if not 0 change is performed, otherwise controller is removed
  * \param        type new project tyoe
  * \return       void
  * \par      Source:
  *                MainWindow.cpp
  */
  void checkAndSwitch(int result, ProjectProperties::ProjectTypes type);
};
#endif
