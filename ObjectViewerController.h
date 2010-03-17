#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ObjectViewerController_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ObjectViewerController.h
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

#ifndef OBJECTVIEWERCONTROLER_H
#define OBJECTVIEWERCONTROLER_H

#include <QObject>
#include <QtXml/QXmlStreamWriter>

//project objects
class MainWindow;
class ObjectViewer;
class WoolzObject;
class ObjectViewerModel;
class LandmarkController;

//Qt objects
class QActionGroup;
class QDomElement;

/*!
 * \brief	Model of viewers. Manages the object viewers and views
 *
 *               The list of viewers is stored and partly managed by the QMDIArea Qt object.
 *               Each viewer has its own list views contained.
 * \ingroup      Control
 */
class ObjectViewerController : public QObject
{
  Q_OBJECT
public:

 /*!
  * \ingroup      Control
  * \brief        Constructor
  * \param        parent parent object
  * \return       void
  * \par      Source:
  *                ObjectViewerController.cpp
  */
  ObjectViewerController (MainWindow * mainWindow, LandmarkController *landmarkController, QObject * parent = 0 );

 /*!
  * \ingroup      Control
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *                ObjectViewerController.cpp
  */
  virtual ~ObjectViewerController ( );

 /*!
  * \ingroup      Control
  * \brief        Saves model in xml format.
  * \param        xmlWriter output xml stream
  * \return       true if succeded, false if not
  * \par      Source:
  *                ObjectViewerController.cpp
  */
  virtual bool saveAsXml(QXmlStreamWriter *xmlWriter);

 /*!
  * \ingroup      Control
  * \brief        Reads viewers from DOM tree
  * \param        element current element of the DOM tree
  * \return       true if succeded, false if not
  * \par      Source:
  *                ObjectViewerController.cpp
  */
  bool parseDOM(const QDomElement &element);

 /*!
  * \ingroup      Control
  * \brief        Set 3D flag of the model. Should be used only after the model is created.
  * \param        b true if model is 3D, false if model is 2D
  * \return       void
  * \par      Source:
  *                ObjectViewerController.cpp
  */
  void set3D(bool b) {m_is3D = b;}

 /*!
  * \ingroup      Control
  * \brief        Returns the 3D flag of the model.
  * \return       true if model is 3D, false if model is 2D
  * \par      Source:
  *                ObjectViewerController.cpp
  */
  virtual bool get3D() {return m_is3D;}

 /*!
  * \ingroup      Control
  * \brief        Creates result viewer and sets up default viewer
  *               if just the first source and target viewer have been created
  * \return       void
  * \par      Source:
  *                ObjectViewerController.cpp
  */
  void checkToSetupDefault();

 /*!
  * \ingroup      Control
  * \brief        Returns the managed model
  * \return       objet model
  * \par      Source:
  *                ObjectViewerController.cpp
  */
  ObjectViewerModel * model() { return m_objectViewerModel;}

public slots:
 /*!
  * \ingroup      Control
  * \brief        Processes MDI window focus change
  * \return       void
  * \par      Source:
  *                ObjectViewerController.cpp
  */
  void updateActions ();

 /*!
  * \ingroup      Control
  * \brief        Sets up the default layout
  *
  *               with source viewer on the left, target viewer middle, result viewer on the right.
  *               If multiple source/target/result viewers are, the first is used.
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void defaultLayout();

 /*!
  * \ingroup      Control
  * \brief        Sets up the default camera links
  *
  *               with result view linked to both source and targert views.
  * \return       void
  * \par      Source:
  *                WarperController.cpp
  */
  void defaultViewLinks();

 /*!
  * \ingroup      Control
  * \brief        Adds a new source viewer
  * \param        force, if false the new viewer will be created is none exists
  * \return       void
  * \par      Source:
  *                ObjectViewerController.cpp
  */
  void addSourceViewer (bool force = false);

 /*!
  * \ingroup      Control
  * \brief        Adds a new target viewer
  * \param        force, if false the new viewer will be created is none exists
  * \return       void
  * \par      Source:
  *                ObjectViewerController.cpp
  */
  void addTargetViewer (bool force = false);

  /*!
  * \ingroup      Control
  * \brief        Adds a new result viewer
  * \param        force, if false the new viewer will be created is none exists
  * \return       void
  * \par      Source:
  *                ObjectViewerController.cpp
  */
  void addResultViewer (bool force = false);

 /*!
  * \ingroup      Control
  * \brief        Retruns the active viewer
  * \return       active viewer or NULL if there is no active viewer
  * \par      Source:
  *                ObjectViewerController.cpp
  */
  ObjectViewer* activeWindow();

signals:
 /*!
  * \ingroup      Control
  * \brief        Signals the addition of an object. Viewers should process this signal
  * \param        object the object to be added
  * \return       void
  * \par      Source:
  *                ObjectViewerController.cpp
  */
  void addObjectSignal(WoolzObject * object);

 /*!
  * \ingroup      Control
  * \brief        Signals the removal of an object. Viewers should process this signal.
  * \param        object the object to be removed
  * \return       void
  * \par      Source:
  *                ObjectViewerController.cpp
  */
  void removedObjectSignal(WoolzObject * object);

  /*!
  * \ingroup      Control
  * \brief        Requesting objects.
  * \return       void
  * \par      Source:
  *                ObjectViewerController.cpp
  */
  void signalRequestAllSignals();


 /*!
  * \ingroup      Control
  * \brief        Signals mode change to editing.
  * \return       void
  * \par      Source:
  *                ObjectViewerController.cpp
  */
  void setEditingMode();

 /*!
  * \ingroup      Control
  * \brief        Signals mode change to viewing.
  * \return       void
  * \par      Source:
  *                ObjectViewerController.cpp
  */
  void setViewingMode();

 /*!
  * \ingroup      Control
  * \brief        Signals mode switch.
  * \return       void
  * \par      Source:
  *                ObjectViewerController.cpp
  */
  void switchModes();

 /*!
  * \ingroup      Control
  * \brief        Signals request to load all compatible objects.
  * \return       void
  * \par      Source:
  *                ObjectViewerController.cpp
  */
  void signalRequestAllObjects(ObjectViewer *);

private:
 /*!
  * \ingroup      Control
  * \brief        Sets up UI connections to a new viewer
  * \param        viewer new viewer to be set up
  * \return       void
  * \par      Source:
  *                ObjectViewerController.cpp
  */
  void setupObjectViewer(ObjectViewer *viewer);

protected:
  ObjectViewerModel *m_objectViewerModel;    /*!< viewer model */

  MainWindow *m_mainWindow;                  /*!< MDI area managing viewers */
  QActionGroup *m_menuWindowActionGroup;     /*!< action group for window selection */
  LandmarkController *m_landmarkController;  /*!< landmark controller */

  bool m_is3D;                               /*!< if this is a 3D or 2D landmark model */
  ObjectViewer* lastSourceViewer;            /*!< the last source viewer */
  ObjectViewer* lastTargetViewer;            /*!< the last target viewer */
  ObjectViewer* lastResultViewer;            /*!< the last result viewer */

  unsigned sourceViewerCounter;              /*!< number of source viewers */
  unsigned targetViewerCounter;              /*!< number of target viewers */
  unsigned resultViewerCounter;              /*!< number of result viewers */

public:
  static const char * xmlTag;                /*!< xml section tag string */
};

#endif // OBJECTVIEWERCONTROLER_H
