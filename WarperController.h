#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _WarperController_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         WarperController.h
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
* \brief        Control of the warping interface.
* \ingroup      Control
*
*/

#ifndef WARPERCONTROLLER_H
#define WARPERCONTROLLER_H

#include <QObject>
#include <QString>
#include <QColor>
#include "WoolzObject.h"

class MainWindow;
class WarperSourceViewer;
class WarperTargetViewer;
class WarperResultViewer;
//class ViewToolDialog;
//class ObjectToolDialog;
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
class WarpingWidget;
class ContourISOSurfaceWidget;
class TransferFunctionWidget;
class LandmarkController;
class ProjectProperties;
class WoolzDirectTransform;
class QUndoStack;
class QAction;
class QDomElement;
class QXmlStreamWriter;
class QActionGroup;

/*!
* \brief	Class controls interface of the warping mode
* \ingroup      Control
*/
class WarperController : public QObject {
  Q_OBJECT
public:
 /*!
  * \ingroup      Control
  * \brief        Editing modes
  */
  typedef enum { noMode=0, addLandmark = 1, deleteLandmark} EditingMode;

  static const char * xmlTag2D;        /*!< xml section tag string for 2D projects*/
  static const char * xmlTag3D;        /*!< xml section tag string for 3D projects*/

 /*!
  * \ingroup      Control
  * \brief        Constructor
  * \param        mainwindow main window widget
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  WarperController ( MainWindow * mainwindow, ProjectProperties *projectProperties ) ;

 /*!
  * \ingroup      Control
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  virtual ~WarperController ( );

 /*!
  * \ingroup      Control
  * \brief        Intialises the class
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void init();

 /*!
  * \ingroup      Control
  * \brief        Adds an object to the current source viewer
  * \param        obj object to add
  * \return       true if succeded, false otherwise
  * \par      Source:
  *                WarperController.cpp
  */
  bool addToSourceViewer (WoolzObject * obj) ;

 /*!
  * \ingroup      Control
  * \brief        Adds an object to the current target viewer
  * \param        obj object to add
  * \return       true if succeded, false otherwise
  * \par      Source:
  *                WarperController.cpp
  */
  bool addToTargetViewer (WoolzObject * obj) ;

 /*!
  * \ingroup      Control
  * \brief        Saves toolbar status in xml format.
  * \param        xmlWriter output xml stream
  * \return       true if succeded, false if not
  * \par      Source:
  *                WoolzDynThresholdedObj.cpp
  */
  bool saveXmlToolbars(QXmlStreamWriter *xmlWriter);

 /*!
  * \ingroup      Control
  * \brief        Reads project from DOM tree
  * \param        element current element of the DOM tree
  * \return       true if succeded, false if not
  * \par      Source:
  *                WarperController.cpp
  */
  bool parseProject(const QDomElement &element);

 /*!
  * \ingroup      Control
  * \brief        Reads toolbar configuration from DOM tree
  * \param        element current element of the DOM tree
  * \return       true if succeded, false if not
  * \par      Source:
  *                WarperController.cpp
  */
  bool parseToolbarsDOM(const QDomElement &element);

   /*!
  * \ingroup      Control
  * \brief        Sets 3D or 2D project
  * \param        is3D is project 3D
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void set3D(bool is3D);

public slots:
 /*!
  * \ingroup      Control
  * \brief        Opens a target value object
  * \return       true on success, false otherwise
  * \par      Source:
  *                WarperController.cpp
  */
  bool openValueTarget () ;

 /*!
  * \ingroup      Control
  * \brief        Opens a target mesh object
  * \return       true on success, false otherwise
  * \par      Source:
  *                WarperController.cpp
  */
  bool openMeshTarget () ;

 /*!
  * \ingroup      Control
  * \brief        Opens a target surface object
  * \return       true on success, false otherwise
  * \par      Source:
  *                WarperController.cpp
  */
  bool openSurfaceTarget () ;

 /*!
  * \ingroup      Control
  * \brief        Opens a target composite (mesh and value)
  * \return       true on success, false otherwise
  * \par      Source:
  *                WarperController.cpp
  */
  bool openCompositeTarget () ;

 /*!
  * \ingroup      Control
  * \brief        Opens a source value object
  * \return       true on success, false otherwise
  * \par      Source:
  *                WarperController.cpp
  */
  bool openValueSource () ;

 /*!
  * \ingroup      Control
  * \brief        Opens a source surface object
  * \return       true on success, false otherwise
  * \par      Source:
  *                WarperController.cpp
  */
  bool openSurfaceSource () ;

 /*!
  * \ingroup      Control
  * \brief        Opens a source mesh object
  * \return       true on success, false otherwise
  * \par      Source:
  *                WarperController.cpp
  */
  bool openMeshSource () ;

 /*!
  * \ingroup      Control
  * \brief        Opens the view tool dialog
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  //void openViewDialog ();

 /*!
  * \ingroup      Control
  * \brief        Opens the object tool dialog
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  //void openObjectDialog ();

 /*!
  * \ingroup      Control
  * \brief        Opens the landmark dialog
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  //void openLandmarkDialog ();

 /*!
  * \ingroup      Control
  * \brief        Saves the warped object
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void saveWarpedObject ();

 /*!
  * \ingroup      Control
  * \brief        Saves the warping transform
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
   void saveWarpingTransform ();

 /*!
  * \ingroup      Control
  * \brief        Saves the warping transform into the specified file
  * \param        filename the name of the file where transform is to be saved
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
   bool saveWarpingTransformToFile(QString filename);

 /*!
  * \ingroup      Control
  * \brief        Saves the warped object with the current filename
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
   void saveProject();

 /*!
  * \ingroup      Control
  * \brief        Saves the warped project with a new filename
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void saveProjectAs();

 /*!
  * \ingroup      Control
  * \brief        Generates mesh
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void generateMesh ();

 /*!
  * \ingroup      Control
  * \brief        Saves  landmarks
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void saveAsLandmarks ();

  /*!
  * \ingroup      Control
  * \brief        Sets project properties
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void setProjectProperties();

 /*!
  * \ingroup      Control
  * \brief        Loads the landmarks
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void loadLandmarks (); // temporarly?! should be removed when saving/loading is sorted out

 /*!
  * \ingroup      Control
  * \brief        Adds a new source view
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void addSourceViewer ();

 /*!
  * \ingroup      Control
  * \brief        Adds a new target view
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void addTargetViewer ();

 /*!
  * \ingroup      Control
  * \brief        Adds a new result view
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void addResultViewer ();

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

 /*!
  * \ingroup      Control
  * \brief        Processes add mode selection
  *
  *               Add and delete modes are exclusive.
  * \param        checked the checked state
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void addToggled(bool checked);

 /*!
  * \ingroup      Control
  * \brief        Processes landmark on/off switch
  * \param        checked the state
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void landmarkVisibilityToggled(bool checked);

 /*!
  * \ingroup      Control
  * \brief        Processes delete mode selection
  *
  *               Add/move and delete modes are exclusive.
  * \param        checked the checked state
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void deleteToggled(bool checked);

 /*!
  * \ingroup      Control
  * \brief        Processes move mode selection
  *
  *               Move and delete modes are exclusive.
  * \param        checked the checked state
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void moveToggled(bool checked);

 /*!
  * \ingroup      Control
  * \brief        Processes auto Update selection
  *
  * \param        checked the checked state
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void globalAutoUpdate(bool checked);

 /*!
  * \ingroup      Control
  * \brief        Shows or hides mesh on/off action
  * \param        checked the checked state
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void updateShowMesh(WoolzObject* mesh);

 /*!
  * \ingroup      Control
  * \brief        Processes warping mesh on/off
  * \param        checked the checked state
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void warpingMeshUpdate(bool on);

  /*!
   * \ingroup      Control
   * \brief        Processes update request for all objects requireing update
   * \return       void
   * \par      Source:
   *                WarperController.cpp
   */
   void updateNow();

  /*!
  * \ingroup      Control
  * \brief        Updates warping mesh on/off action enable/disabled C
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void warpingMeshUpdateAction();

 /*!
  * \ingroup      Control
  * \brief        Opens option window
  * \par      Source:
  *                WarperController.cpp
  */
  void options();

  /*!
   * \ingroup      Control
   * \brief        Processes update possible signal
   * \param        possible update is possible
   * \return       void
   * \par      Source:
   *                WarperController.cpp
   */
   void updatePossibleChanged(bool possible);

protected:

 /*!
  * \ingroup      Control
  * \brief        Sets up toolbars
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void setupToolbars();

private:
 /*!
  * \ingroup      Control
  * \brief        Conects menu bar signals to this controler object.
  *
  *               The menu bar signals will be processed by this controler.
  *               When controler is deleted, signals will be removed automaticaly.
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void connectSignals ();

 /*!
  * \ingroup      Control
  * \brief        Loads mesh from a file
  * \param        filename from which mesh is read
  * \return       a newly created WoolzObject
  * \par      Source:
  *                WarperController.cpp
  */
  WoolzObject* loadMesh(QString filename, WoolzObject::WoolzObjectType type);

 /*!
  * \ingroup      Control
  * \brief        Loads surface obect from a file
  * \param        filename from which mesh is read
  * \param        type object type (source or target)
  * \return       a newly created WoolzObject
  * \par      Source:
  *                WarperController.cpp
  */
  WoolzObject* loadSurface(QString filename, WoolzObject::WoolzObjectType type);

 /*!
  * \ingroup      Control
  * \brief        Loads value obect from a file
  * \param        filename from which mesh is read
  * \param        type object type (source or target)
  * \return       a newly created WoolzObject
  * \par      Source:
  *                WarperController.cpp
  */
  WoolzObject* loadValue(QString filename , WoolzObject::WoolzObjectType type);


 /*!
  * \ingroup      Control
  * \brief        Retruns the segmentation widget. It creates if did not exists.
  * \return       view tool dialog
  * \par      Source:
  *                WarperController.cpp
  */
  SegmentationWidget* getSegmentWidget(void);


 /*!
  * \ingroup      Control
  * \brief        Retruns the object tool dialog. It creates if did not exists.
  * \return       object tool dialog
  * \par      Source:
  *                WarperController.cpp
  */
  //ObjectToolDialog* getObjectToolDialog(void);

 /*!
  * \ingroup      Control
  * \brief        Retruns the landmark dialog. It creates if did not exists.
  * \return       landmark dialog
  * \par      Source:
  *                WarperController.cpp
  */
//  LandmarkDialog* getLandmarkDialog(void);

 /*!
  * \ingroup      Control
  * \brief        Check if project is 3D or 2D, the object is same, 3D or 2D
  * \return       true if project and object have the same dimensionality, false otherwise
  * \par      Source:
  *                WarperController.cpp
  */
  bool is2D3Dcompatibile(WoolzObject *object);


 /*!
  * \ingroup      Control
  * \brief        Saves permanenty the last used directory when opening or saving a file.
  * \param        filename last filename
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void setLastPath(QString filename);

 /*!
  * \ingroup      Control
  * \brief        Returns the last used directory.
  * \return       directory last used
  * \par      Source:
  *                WarperController.cpp
  */
  QString getLastPath();

signals:
 /*!
  * \ingroup      Control
  * \brief        Signals start of viewer removal (viewer is still present).
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void removedViewerStart(ObjectViewer *);

 /*!
  * \ingroup      Control
  * \brief        Signals viewer addition.
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  //void addedViewer(ObjectViewer *);

 /*!
  * \ingroup      Control
  * \brief        Signals object change
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void changedObject();

private:
  QActionGroup *menuMeshSelectionActionGroup;      /*!< action group for mesh selection */
  bool project3D;                                  /*!< true if 3D object */
  bool projectInitialised;                         /*!< true if project loaded or intialised */
  //ViewToolDialog *viewToolDialog;                  /*!< view tool dialog */
  //ObjectToolDialog *objectToolDialog;              /*!< object tool dialog */
  //LandmarkDialog *landmarkDialog;                  /*!< landmark dialog */
  SegmentationWidget *segmentWidget;               /*!< segmentation widget */
  ObjectPropertyWidget *objectPropertyWidget;      /*!< general object property widget */
  TransformWidget* transformWidget;                /*!< transform widget */
  WarpingWidget* warpingWidget;                    /*!< object warping widget */
  ContourISOSurfaceWidget* contourWidget;          /*!< contour / ISO surface widget */
  TransferFunctionWidget* transferFunctionWidget;  /*!< transfer function editor widget */
  ObjectToolWidget * objectToolWidget;             /*!< tree list of objects */
  ViewToolWidget * viewToolWidget;                 /*!< view tool widget */
  LandmarkWidget * landmarkWidget;                 /*!< landmark widget */
  ObjectListModel *objectListModel;                /*!< object container */
  ObjectViewerController *m_objectViewerController;/*!< viewer controller */
  LandmarkModel *landmarkModel;                    /*!< landmark container */
  LandmarkController *landmarkController;          /*!< landmark controller */
  MainWindow * mainWindow;                         /*!< the main window object */
  QUndoStack *undoStack;                           /*!< stack for undoable commands */
  QAction *actionUndo;                             /*!< action for Undo */
  QAction *actionRedo;                             /*!< action for Redo */
  ProjectProperties *m_projectProperties;          /*!< pointer to project properties */
  WoolzDirectTransform *m_woolzTransform;          /*!< tranformation object */
};
#endif // WARPERCONTROLLER_H
