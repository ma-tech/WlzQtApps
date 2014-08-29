#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _WarperController_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         WarperController.h
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
* \brief	Control of the warping interface.
* \ingroup	Control
*/

#ifndef WARPERCONTROLLER_H
#define WARPERCONTROLLER_H

#include <QObject>
#include <QString>
#include <QColor>
#include "WoolzObject.h"

//Project objects
class MainWindow;
class WarperSourceViewer;
class WarperTargetViewer;
class WarperResultViewer;
class ViewToolWidget;
class ObjectToolWidget;
class LandmarkWidget;
class ObjectViewer;
class ObjectListModel;
class ObjectViewerController;
class ObjectView;
class LandmarkModel;
class WoolzDynWarpedObject ;
class SegmentationWidget;
class ObjectPropertyWidget;
class TransformWidget;
class SectioningPlaneWidget;
class WarpingWidget;
class ContourISOSurfaceWidget;
class TransferFunctionWidget;
class LandmarkController;
class ProjectProperties;
class WoolzTransform;

//Qt objects
class QUndoStack;
class QAction;
class QDomElement;
class QXmlStreamWriter;
class QActionGroup;

/*!
 * \brief	Class controls interface of the warping mode
 * \ingroup	Control
 */
class WarperController : public QObject
{
  Q_OBJECT
  public:
    /*!
     * \ingroup	Control
     * \brief	Editing modes
     */
    typedef enum { noMode=0, addLandmark = 1, deleteLandmark} EditingMode;

    static const char * xmlTag2D;	/*!< xml section tag string for 2D
    					     projects */
    static const char * xmlTag3D;       /*!< xml section tag string for 3D
    					     projects */

    /*!
     * \ingroup	Control
     * \brief	Constructor
     * \param	mainwindow			main window widget
     */
    WarperController(MainWindow * mainwindow,
        ProjectProperties *projectProperties);

    /*!
     * \ingroup	Control
     * \brief	Destructor
     */
    virtual ~WarperController();

    /*!
     * \ingroup	Control
     * \brief	Intialises the class
     */
    void init();

    /*!
     * \ingroup	Control
     * \brief	Adds an object to the current source viewer
     * \param	obj			object to add
     * \return	true if succeded, false otherwise
     */
    bool addToSourceViewer(WoolzObject * obj) ;

    /*!
     * \ingroup	Control
     * \brief	Adds an object to the current target viewer
     * \param	obj			object to add
     * \return	true if succeded, false otherwise
     */
    bool addToTargetViewer(WoolzObject * obj) ;

    /*!
     * \ingroup	Control
     * \brief	Saves toolbar status in xml format.
     * \param	xmlWriter			output xml stream
     * \return	true if succeded, false if not
     */
    bool saveXmlToolbars(QXmlStreamWriter *xmlWriter);

    /*!
     * \ingroup	Control
     * \brief	Reads project from DOM tree
     * \param	element			current element of the DOM tree
     * \return	true if succeded, false if not
     */
    bool parseProject(const QDomElement &element);

    /*!
     * \ingroup	Control
     * \brief	Reads toolbar configuration from DOM tree
     * \param	element			current element of the DOM tree
     * \return	true if succeded, false if not
     */
    bool parseToolbarsDOM(const QDomElement &element);

    /*!
     * \ingroup	Control
     * \brief	Sets 3D or 2D project
     * \param	is3D			is project 3D
     */
    void set3D(bool is3D);

  public slots:
    /*!
     * \ingroup	Control
     * \brief	Opens a target value object
     * \return	true on success, false otherwise
     */
    bool openValueTarget() ;

    /*!
     * \ingroup	Control
     * \brief	Opens a target mesh object
     * \return	true on success, false otherwise
     */
    bool openMeshTarget() ;

    /*!
     * \ingroup	Control
     * \brief	Opens a target surface object
     * \return	true on success, false otherwise
     */
    bool openSurfaceTarget() ;

    /*!
     * \ingroup	Control
     * \brief	Opens a target composite (mesh and value)
     * \return	true on success, false otherwise
     */
    bool openCompositeTarget() ;

    /*!
     * \ingroup	Control
     * \brief	Opens a source value object
     * \return	true on success, false otherwise
     */
    bool openValueSource() ;

    /*!
     * \ingroup	Control
     * \brief	Opens a source surface object
     * \return	true on success, false otherwise
     */
    bool openSurfaceSource() ;

    /*!
     * \ingroup	Control
     * \brief	Opens a source mesh object
     * \return	true on success, false otherwise
     */
    bool openMeshSource() ;

    /*!
     * \ingroup	Control
     * \brief	Saves the warped object
     */
    void saveWarpedObject();

    /*!
     * \ingroup	Control
     * \brief	Saves the warping transform
     */
    void saveWarpingTransform();

    /*!
     * \ingroup	Control
     * \brief	Saves the warping transform into the specified file
     * \param	filename		the name of the file where transform
     * 					is to be saved
     */
    bool saveWarpingTransformToFile(QString filename);

    /*!
     * \ingroup	Control
     * \brief	Saves the warped object with the current filename
     */
    void saveProject();

    /*!
     * \ingroup	Control
     * \brief	Saves the warped project with a new filename
     */
    void saveProjectAs();

    /*!
     * \ingroup	Control
     * \brief	Generates mesh
     */
    void generateMesh();

    /*!
     * \ingroup	Control
     * \brief	Saves  landmarks
     */
    void saveAsLandmarks();

    /*!
     * \ingroup	Control
     * \brief	Sets project properties
     */
    void setProjectProperties();

    /*!
     * \ingroup	Control
     * \brief	Loads the landmarks
     */
    void loadLandmarks();

    /*!
     * \ingroup	Control
     * \brief	Adds a new source view
     */
    void addSourceViewer();

    /*!
     * \ingroup	Control
     * \brief	Adds a new target view
     */
    void addTargetViewer();

    /*!
     * \ingroup	Control
     * \brief	Adds a new result view
     */
    void addResultViewer();

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

    /*!
     * \ingroup	Control
     * \brief	Processes remove mesh mode selection
     *
     *          Add, delete and mesh remove elements are exclusive.
     * \param	checked			the checked state
     */
    void removeElementToggled(bool checked);

    /*!
     * \ingroup	Control
     * \brief	Processes add mode selection
     *
     *          Add and delete modes are exclusive.
     * \param	checked			the checked state
     */
    void addToggled(bool checked);

    /*!
     * \ingroup	Control
     * \brief	Processes landmark on/off switch
     * \param	checked			the state
     */
    void landmarkVisibilityToggled(bool checked);

    /*!
     * \ingroup	Control
     * \brief	Processes delete mode selection
     *
     *          Add/move and delete modes are exclusive.
     * \param	checked			the checked state
     */
    void deleteToggled(bool checked);

    /*!
     * \ingroup	Control
     * \brief	Processes move mode selection
     *
     *          Move and delete modes are exclusive.
     * \param	checked			the checked state
     */
    void moveToggled(bool checked);

    /*!
     * \ingroup	Control
     * \brief	Processes auto Update selection
     *
     * \param	checked			the checked state
     */
    void globalAutoUpdate(bool checked);

    /*!
     * \ingroup	Control
     * \brief	Shows or hides mesh on/off action
     * \param	checked			the checked state
     */
    void updateShowMesh(WoolzObject* mesh);

    /*!
     * \ingroup	Control
     * \brief	Processes warping mesh on/off
     * \param	checked			the checked state
     */
    void warpingMeshUpdate(bool on);

    /*!
     * \ingroup	Control
     * \brief	Processes update request for all objects requireing update
     */
    void updateNow();

    /*!
     * \ingroup	Control
     * \brief	Updates warping mesh on/off action enable/disabled C
     */
    void warpingMeshUpdateAction();

    /*!
     * \ingroup	Control
     * \brief	Opens option window
     */
    void options();

    /*!
     * \ingroup	Control
     * \brief	Processes update possible signal
     * \param	possible			update is possible
     */
    void updatePossibleChanged(bool possible);

  protected:
    /*!
     * \ingroup	Control
     * \brief	Sets up toolbars
     */
    void setupToolbars();

  private:
    /*!
     * \ingroup	Control
     * \brief	Conects menu bar signals to this controler object.
     *
     *          The menu bar signals will be processed by this controler.
     *          When controler is deleted, signals will be removed
     *          automaticaly.
     */
    void connectSignals();

    /*!
     * \ingroup	Control
     * \brief	Loads mesh from a file
     * \param	filename			from which mesh is read
     * \return	a newly created WoolzObject
     */
    WoolzObject* loadMesh(QString filename, WoolzObject::WoolzObjectType type);

    /*!
     * \ingroup	Control
     * \brief	Loads surface obect from a file
     * \param	filename		from which mesh is read
     * \param	type			object type (source or target)
     * \return	a newly created WoolzObject
     */
    WoolzObject* loadSurface(QString filename,
        WoolzObject::WoolzObjectType type);

    /*!
     * \ingroup	Control
     * \brief	Loads value obect from a file
     * \param	filename			from which mesh is read
     * \param	type			object type (source or target)
     * \return	a newly created WoolzObject
     */
    WoolzObject* loadValue(QString filename,
        WoolzObject::WoolzObjectType type);

    /*!
     * \ingroup	Control
     * \brief	Retruns the segmentation widget. It creates if did not exists.
     * \return	view tool dialog
     */
    SegmentationWidget* getSegmentWidget(void);

    /*!
     * \ingroup	Control
     * \brief	Check if project is 3D or 2D, the object is same, 3D or 2D
     * \return	true if project and object have the same dimensionality,
     * 		false otherwise
     */
    bool is2D3Dcompatibile(WoolzObject *object);

    /*!
     * \ingroup	Control
     * \brief	Saves permanenty the last used directory when opening or
     * 		saving a file.
     * \param	filename			last filename
     */
    void setLastPath(QString filename);

    /*!
     * \ingroup	Control
     * \brief	Returns the last used directory.
     * \return	directory last used
     */
    QString getLastPath();

  signals:
    /*!
     * \ingroup	Control
     * \brief	Signals start of viewer removal (viewer is still present).
     */
    void removedViewerStart(ObjectViewer *);

    /*!
     * \ingroup	Control
     * \brief	Signals object change
     */
    void changedObject();

  private:
    QActionGroup *menuMeshSelectionActionGroup;	/*!< action group for mesh
    						     selection */
    bool project3D;                             /*!< true if 3D object */
    bool projectInitialised;                    /*!< true if project loaded or
    						     intialised */
    SegmentationWidget *segmentWidget;          /*!< segmentation widget */
    ObjectPropertyWidget *objectPropertyWidget; /*!< general object property 
    						     widget */
    TransformWidget* transformWidget;           /*!< transform widget */
    WarpingWidget* warpingWidget;               /*!< object warping widget */
    ContourISOSurfaceWidget* contourWidget;     /*!< contour / ISO surface 
    						     widget */
    TransferFunctionWidget* transferFunctionWidget; /*!< transfer function
    					             editor widget */
    SectioningPlaneWidget* sectioningPlaneWidget; /*!< sectioning plane editor
    						     widget */
    ObjectToolWidget * objectToolWidget;        /*!< tree list of objects */
    ViewToolWidget * viewToolWidget;            /*!< view tool widget */
    LandmarkWidget * landmarkWidget;            /*!< landmark widget */
    ObjectListModel *objectListModel;           /*!< object container */
    ObjectViewerController *m_objectViewerController; /*!< viewer controller */
    LandmarkModel *landmarkModel;               /*!< landmark container */
    LandmarkController *landmarkController;     /*!< landmark controller */
    MainWindow * mainWindow;                    /*!< the main window object */
    QUndoStack *undoStack;                      /*!< stack for undoable
    						     commands */
    QAction *actionUndo;                        /*!< action for Undo */
    QAction *actionRedo;                        /*!< action for Redo */
    ProjectProperties *m_projectProperties;     /*!< pointer to project
    						     properties */
    WoolzTransform *m_woolzTransform;           /*!< tranformation object */
};
#endif // WARPERCONTROLLER_H
