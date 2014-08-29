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
     * \ingroup	UI
     * \brief	Returns the current MDI workspace.
     * \return	MDI workspace
     */
    QMdiArea * getWorkspace() {return(mdiArea);}

    /*!
     * \ingroup	UI
     * \brief	Sets main window title
     * \param	additionalTitle		part title to add into the title
     */
    void setTitle(QString additionalTitle);

    /*!
     * \ingroup	UI
     * \brief	Adds a file to the recent files list
     * \param	filename		path of the file to be added
     */
    void setCurrentFile(const QString &fileName);

  public slots:
    /*!
     * \ingroup	UI
     * \brief	Shows the about dialog of the application.
     *
     *  	Changes project mode to warping, updates main menu.
     */
    void about();

    /*!
     * \ingroup	UI
     * \brief	Opens a value target object
     *
     *          Changes project mode to warping, updates main menu.
     */
    void openValueTarget();

    /*!
     * \ingroup	UI
     * \brief	Opens a mesh target object
     *
     *          Changes project mode to warping, updates main menu.
     */
    void openMeshTarget();

    /*!
     * \ingroup	UI
     * \brief	Opens a surface target object
     *
     *          Changes project mode to warping, updates main menu.
     */
    void openSurfaceTarget();

    /*!
     * \ingroup	UI
     * \brief	Opens a composite target object
     *
     *          Changes project mode to warping, updates main menu.
     */
    void openCompositeTarget();

    /*!
     * \ingroup	UI
     * \brief	Opens a value source object
     *
     *          Changes project mode to warping, updates main menu.
     */
    void openValueSource();

    /*!
     * \ingroup	UI
     * \brief	Opens a surface source object
     *
     *          Changes project mode to warping, updates main menu.
     */
    void openSurfaceSource();

    /*!
     * \ingroup	UI
     * \brief	Opens a mesh source object
     *
     *          Changes project mode to warping, updates main menu.
     */
    void openMeshSource();


    /*!
     * \ingroup	UI
     * \brief	Closes the project.
     *
     *          Changes project mode to none. Removes additional menu options.
     */
    void closeProject();

    /*!
     * \ingroup	UI
     * \brief	Sets interface options
     */
    void options();

    /*!
     * \ingroup	UI
     * \brief	Opens an existing project
     */
    void openProject ();

    /*!
     * \ingroup	UI
     * \brief	Loads project from a file
     * \param	filename		file to load
     */
    void loadProject(QString filename);

    /*!
     * \ingroup	UI
     * \brief	Slot processing status update
     * \param	message			new message
     * \param	timeout			timeout of the new message
     */
    void statusChanged(QString message, int timeout);

  private:
    /*!
     * \ingroup	UI
     * \brief	Switches tghe project type.
     *
     *          Changes the menu and other project dependent settings.
     */
    void switchProjectType(ProjectProperties::ProjectTypes newType);

    /*!
     * \ingroup	UI
     * \brief	Sets up the default signal and slots.
     */
    void setDefaultConnections();

    /*!
     * \ingroup	UI
     * \brief	Removes the default signal and slots.
     */
    void removeDefaultConnections();

    /*!
     * \ingroup	UI
     * \brief	If result is not 0, performs the project change to the new
     * 		project type.
     * \param	result			condition to change, if not 0 change
     * 					is performed, otherwise controller
     * 					is removed
     * \param	type			new project tyoe
     */
    void checkAndSwitch(int result, ProjectProperties::ProjectTypes type);

  private slots:
    /*!
     * \ingroup	UI
     * \brief	Reopens a file in the recent items list
     */
    void openRecentFile();

  private:
    /*!
     * \ingroup	UI
     * \brief	Updates recent file item in the menu
     */
    void updateRecentFileActions();

    /*!
     * \ingroup	UI
     * \brief	Strips the full path of a file and returns only the file name
     * \param	fullFileName			full file name to be stripped
     * \return	stripped file name
     */
    QString strippedName(const QString &fullFileName);

  protected:
    enum { MaxRecentFiles = 5 };              /*!< number of recent projects*/
    ProjectProperties projectSettings;        /*!< project settings */
    WarperController *controler;              /*!< current controller (warping
						   or mesh editing) */
    QAction *recentFileActs[MaxRecentFiles];  /*!< recent projects actions*/
    QAction *separatorAct;                    /*!< separator of the recent
						   projects*/
};
#endif
