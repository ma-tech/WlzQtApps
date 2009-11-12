#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ObjectSimpleViewer_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ObjectSimpleViewer.h
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
* \brief        Woolz object viewer.
* \ingroup      UI
*
*/

#ifndef OBJECTSIMPLEVIEWER_H
#define OBJECTSIMPLEVIEWER_H

#include <QWidget>
#include "ClipPlaneButton.h"

//project objects
class WoolzObject;
class Viewer2D3D;
class ObjectView;
class WarperController;

//Inventor objects
class SoEventCallback;
class SoSeparator;
class SoCamera;
class SoSensor;
class SoNodeSensor;
class SoSFPlane;
class SoClipPlaneManip;
class SoClipPlane;

//Qt objects
class QCloseEvent;
class QDomElement;
class QSlider;

#include <QtXml/QXmlStreamWriter>

/*!
* \brief        Woolz object viewer class.
*
*               For earch object viewed manages an view that cares of the
*               actual visualisation of the object by generating an Inventor
*               SoSeparator node
*
*               Uses a scene graph:
*                 - root
*                     - event callback (mouse events)
*                     - views_root
*                        - view 0
*                        .....
*                        - view n
* \ingroup      UI
*/
class ObjectSimpleViewer : public QWidget
{
  Q_OBJECT
public:
 /*!
  * \ingroup      UI
  * \brief        Constructor
  * \param        is3D if viewer is for 3D objects
  * \param        isBlending if mixing control to be generated
  *
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
 ObjectSimpleViewer (bool is3D, bool isBlending = false);

 /*!
  * \ingroup      UI
  * \brief        Destructor
  *
  * \return       void
  * \par      Source:
  *                Viewer3D.cpp
  */
  virtual ~ObjectSimpleViewer ( );

 /*!
  * \ingroup      UI
  * \brief        Sets viewer title.
  * \param        title viewer title
  *
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  void setWindowTitle(QString title);

 /*!
  * \ingroup      UI
  * \brief        Returns view list
  *
  * \return       view list
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  virtual QList <ObjectView*> getViews ( ) {return views;}

 /*!
  * \ingroup      UI
  * \brief        Activates window
  *
  *               Rises window of the viewer and brings it in focus
  *
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  void activate();

 /*!
  * \ingroup      UI
  * \brief        Callback function to notify in the camera was replaced.
  * \param        cam new camera
  *
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  virtual void replacedCamera  ( SoCamera */*cam*/ ) {};

 /*!
  * \ingroup      Control
  * \brief        Returns the blending value
  * \return       blending value between 0 to 100
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
//  virtual int getBlending();

  /*!
  * \ingroup      Control
  * \brief        Saves model in xml format.
  * \param        xmlWriter output xml stream
  * \return       true if succeded, false if not
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  virtual bool saveAsXml(QXmlStreamWriter *xmlWriter);

 /*!
  * \ingroup      Control
  * \brief        Reads viewer from DOM tree
  * \param        element current element of the DOM tree
  * \return       true if succeded, false if not
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  virtual bool parseDOM(const QDomElement& element);

 /*!
  * \ingroup      Control
  * \brief        Reads window geometry from DOM node
  * \param        element current element of the DOM tree
  * \return       true if succeded, false if not
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  bool parseGeometry(const QDomElement &element);

  /*!
  * \ingroup      Control
  * \brief        Reads views from DOM node
  * \param        element current element of the DOM tree
  * \return       true if succeded, false if not
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  bool parseViews(const QDomElement &element);

 /*!
  * \ingroup      Control
  * \brief        Reads current token if known
  * \param        element current element of the DOM tree
  * \return       true if succeded, false if not
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  virtual bool parseDOMLine(const QDomElement &/*element*/);

public slots:
 /*!
  * \ingroup      UI
  * \brief        Adds new object to the viewer.
  *
  *               Adds a new object to the viewer. The object is made visible
  *               in the viewer only if the the viewer is the default viewer of the object
  *               (i.e. this == viewer) 
  *
  * \param        object object to be added
  * \param        doViewAll if true and no view all was done in the viewer then it also viewer will be adjusted to include all objects
  * \param        previousView if not NULL, copies transparencu and visibility values to the new view
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  void addObject (WoolzObject * object, bool doViewAll = true, ObjectView *previousView = NULL);

 /*!
  * \ingroup      UI
  * \brief        Removes all views of an object.
  *
  *
  * \param        object object to be removed
  *
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  void removedObject (WoolzObject * object );

 /*!
  * \ingroup      UI
  * \brief        Forwards object property change signals.
  *
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  void viewPropertyChanged();

 /*!
  * \ingroup      UI
  * \brief        Sets viewer in editing mode.
  *
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  void setEditingMode();

 /*!
  * \ingroup      UI
  * \brief        Sets viewer in viwing mode.
  *
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  void setViewingMode();

 /*!
  * \ingroup      UI
  * \brief        Switches the viewer editing/viewing modes.
  *
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  void switchModes();

 /*!
  * \ingroup      UI
  * \brief        Processes transparency slider change if slider is present
  * \param        transparency new transparency
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  virtual void transparencyChanged(int /*transparency*/) {}

 /*!
  * \ingroup      UI
  * \brief        Processes view type changes. Called by a view when the view canot handle its object
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  virtual void regerateView();

 /*!
  * \ingroup      UI
  * \brief        Slot processing status update
  * \param        message new message
  * \param        timeout timeout of the new message
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  virtual void stateChanged(ClipPlaneButton::statetype state);

 /*!
  * \ingroup      UI
  * \brief        Sets the background colour of the viewer
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  virtual void setBackgroundColour();

 /*!
  * \ingroup      UI
  * \brief        Returns action for activating viewer
  *
  * \return       activation action
  * \par      Source:
  *                ObjectSimpleViewer.cp
  */
  QAction * activateAction() const {return m_activateAction;}

protected:
 /*!
  * \ingroup      UI
  * \brief        Handler for viewer closing.
  *
  *               Frees all viewes and additional allocated memory.
  *
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  void closeEvent(QCloseEvent */*event*/);

 /*!
  * \ingroup      UI
  * \brief        Returns the default object transparency.
  * \param        object the new object
  * \return       100 is transparent, 0 if not
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  virtual int initialTransparency(WoolzObject * /*object*/ ) { return  0;}

 /*!
  * \ingroup      UI
  * \brief        Static event handler for camera mouse events.
  *
  *               Callback for mouse events .
  *
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  virtual void notifyMouseEvent(SoEventCallback * event);

 /*!
  * \ingroup      Views
  * \brief        Method generating the subclass coresponding to a Woolz object
  * \param        parent parent viewer
  * \param        object Woolz object to be visualised
  * \return       new view created
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  ObjectView * Factory(QObject * parent, WoolzObject *object);

 /*!
  * \ingroup      UI
  * \brief        Checks if viewer accepts object.
  *
  * \return       true if viewer accepts object, false otherwise
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  virtual bool accepting(WoolzObject * object) {return object!=NULL;}

private:
 /*!
  * \ingroup      UI
  * \brief        Static event handler for camera mouse events.
  *
  *               Calls notifyCameraChanged().
  *
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  static void mouseEventCB(void *data, SoEventCallback * event);

signals:
 /*!
  * \ingroup      UI
  * \brief        Signals peparation of removing the viewer.
  *
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  void removedViewerStart();

 /*!
  * \ingroup      UI
  * \brief        Signals finished viewer removal.
  *
  *               Called from destructor.
  *
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  void removedViewerFinished();

 /*!
  * \ingroup      UI
  * \brief        Signals changed viewer
  *
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  void changedViewer();

 /*!
  * \ingroup      UI
  * \brief        Notifies changed viewer
  * \param        viewer the change viewer
  *
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  void changedObjectView(ObjectView * viewer);

 /*!
  * \ingroup      UI
  * \brief        Notifies added view
  * \param        view added view
  *
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  void addedView(ObjectView * view);

 /*!
  * \ingroup      UI
  * \brief        Notifies a view removal start
  * \param        view removed view
  *
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  void removedViewStart(ObjectView * view);

 /*!
  * \ingroup      UI
  * \brief        Notifies a view removal finish
  * \param        view removed view
  *
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  void removedViewFinish(ObjectView * view);

 /*!
  * \ingroup      UI
  * \brief        Notifies changed camera
  *
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  void cameraChanged();

 /*!
  * \ingroup      UI
  * \brief        Notifies cretion of clip plane
  *
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  void addedClipPlane(SoClipPlane * plane);

protected:

  // protected attributes
  Viewer2D3D * m_viewer;            /*!< Inventor viewer that performs the viewing */
  SoSeparator *root;                /*!< secene grapg root*/
  SoSeparator *views_root;          /*!< views root */
  QList <ObjectView *> views;       /*!< list of views*/

  QAction * m_activateAction ;

  SoClipPlaneManip *m_clipPlaneManip;          /*!< clip plane manipulator*/
  SoClipPlane *m_clipPlane;                    /*!< clip plane */
  ClipPlaneButton *m_clipManipulatorButton;    /*!< clip manipulator button */
  QPushButton * m_obliqueSliceButton;          /*!< oblique slice button */
  QPushButton * m_clipLandmarkButton;          /*!< landmark clipping button */  //TODO: move from child class
  QSlider *m_mixSlider;                        /*!< mixing slider */
  bool m_viewAll;                              /*!< if true, after an object is inserted viewAll() is
                                                    called to change to view all object in the viewer*/
public:

 /*!
  * \ingroup      UI
  * \brief        Configures the view
  *
  * \return       void
  * \par      Source:
  *                WarperSourceViewer.cpp
  */
  virtual void init();

protected:
 /*!
  * \ingroup      UI
  * \brief        Returns the background colour of the viewer
  *
  *               Reimplemented form ObjectViewer
  * \return       colour
  * \return       void
  * \par      Source:
  *                ObjectSimpleViewer.cpp
  */
  virtual QColor getBackgroundColour();
};
#endif // OBJECTSIMPLEVIEWER_H
