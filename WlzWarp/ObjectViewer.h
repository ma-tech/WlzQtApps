#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ObjectViewer_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ObjectViewer.h
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

#ifndef OBJECTVIEWER_H
#define OBJECTVIEWER_H

#include <QWidget>
#include "ClipPlaneButton.h"

//project objects
class WoolzObject;
class Viewer2D3D;
class ObjectView;
class WarperController;
class ObjectViewerModel;

//Inventor objects
class SoEventCallback;
class SoSeparator;
class SoCamera;
class SoNodeSensor;
class SoSFPlane;
class SoClipPlaneManip;
class SoClipPlane;

//Qt objects
class QCloseEvent;
class QXmlStreamWriter;
class QXmlStreamReader;

#include <ObjectSimpleViewer.h>

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
*                     - camera
*                     - views_root (see ObjectSimpleViewer)
* \ingroup      UI
*/
class ObjectViewer : public ObjectSimpleViewer
{
  Q_OBJECT
public:
 /*!
  * \ingroup      UI
  * \brief        Constructor
  * \param        objectViewerModel model managing the viewer
  * \param        is3D if viewer is for 3D objects
  * \return       void
  * \par      Source:
  *                ObjectViewer.cpp
  */
 ObjectViewer (ObjectViewerModel *objectViewerModel, bool is3D, bool isBlending = false);

 /*!
  * \ingroup      UI
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *                Viewer3D.cpp
  */
  virtual ~ObjectViewer ( );

 /*!
  * \ingroup      UI
  * \brief        Returns view list
  * \return       view list
  * \par      Source:
  *                ObjectViewer.cpp
  */
  virtual QList <ObjectView*> getViews ( ) {return views;}

 /*!
  * \ingroup      UI
  * \brief        Callback function to notify in the camera was replaced.
  * \param        cam new camera
  * \return       void
  * \par      Source:
  *                ObjectViewer.cpp
  */
  virtual void replacedCamera  ( SoCamera *cam );

 /*!
  * \ingroup      UI
  * \brief        Sets a new viewer ID
  * \param        ID new id
  * \return       void
  * \par      Source:
  *                ObjectViewer.cpp
  */
  void setID( int ID ) {m_ID = ID;}

 /*!
  * \ingroup      UI
  * \brief        Rerturns the viewer ID
  * \return       true if viewer is linked, false otherwise
  * \par      Source:
  *                ObjectViewer.cpp
  */
  int ID( )   { return m_ID; }

 /*!
  * \ingroup      UI
  * \brief        Rerturns the status of linking
  * \return       true if viewer is linked, false otherwise
  * \par      Source:
  *                ObjectViewer.cpp
  */
  void setLinkedIDs ( )   {
      m_linkedViewerID = m_linkedTo ? m_linkedTo->m_ID : -1;
      m_linkedPlaneID = m_linkedPlaneTo ? m_linkedPlaneTo->m_ID : -1;
  }

 /*!
  * \ingroup      UI
  * \brief        Rerturns the linked viewer ID
  * \return       linked viewerID
  * \par      Source:
  *                ObjectViewer.cpp
  */
  int getLinkedID( )   { return m_linkedViewerID; }

 /*!
  * \ingroup      UI
  * \brief        Rerturns the linked viewer ID
  * \return       linked viewerID
  * \par      Source:
  *                ObjectViewer.cpp
  */
  int getLinkedPlaneID( )   { return m_linkedPlaneID; }

 /*!
  * \ingroup      UI
  * \brief        Sets the viewer that imposes the current viewer camera view
  * \param        viewer Viewer to whitch the current viewer will be linked to
  * \return       void
  * \par      Source:
  *                ObjectViewer.cpp
  */
  void setLinkedTo ( ObjectViewer * viewer );

 /*!
  * \ingroup      UI
  * \brief        Rerturns linked viewer
  * \return       linked viewer
  * \par      Source:
  *                ObjectViewer.cpp
  */
  ObjectViewer * getLinkedTo ( )   { return m_linkedTo;}

 /*!
  * \ingroup      Control
  * \brief        Saves model in xml format.
  * \param        xmlWriter output xml stream
  * \return       true if succeded, false if not
  * \par      Source:
  *                ObjectViewer.cpp
  */
  virtual bool saveAsXml(QXmlStreamWriter *xmlWriter);

 /*!
  * \ingroup      Control
  * \brief        Reads current token if known
  * \param        element current element of the DOM tree
  * \return       true if succeded, false if not
  * \par      Source:
  *                WarperController.cpp
  */
  bool parseDOMLine(const QDomElement &element);

 /*!
  * \ingroup      UI
  * \brief        Sets the viewer that imposes plane copied in this viewer
  * \param        viewer Viewer to whitch the current viewer will be linked to
  * \return       void
  * \par      Source:
  *                ObjectViewer.cpp
  */
  void setLinkedPlaneTo ( ObjectViewer * viewer );

 /*!
  * \ingroup      UI
  * \brief        Rerturns linked plane viewer
  * \return       linked viewer
  * \par      Source:
  *                ObjectViewer.cpp
  */
  ObjectViewer * getLinkedPlaneTo ( )   { return m_linkedPlaneTo;}

public slots:
 /*!
  * \ingroup      UI
  * \brief        Processes camera changes in other viewers.
  * \return       void
  * \par      Source:
  *                ObjectViewer.cpp
  */
  void cameraChangedIn();

  /*!
   * \ingroup      UI
   * \brief        Sets alpha
   *
   * \return       void
   * \par      Source:
   *                ObjectViewer.cpp
   */
   void alphaChanged(bool alpha);

protected:
 /*!
  * \ingroup      UI
  * \brief        Returns the default object transparency.
  * \param        object the new object
  * \return       100 is transparent, 0 if not
  * \par      Source:
  *                ObjectViewer.cpp
  */
  virtual int initialTransparency(WoolzObject * object ) = 0;

 /*!
  * \ingroup      UI
  * \brief        Processes camara change.
  *
  *               Emits cameraChanged() signal.
  *
  * \return       void
  * \par      Source:
  *                ObjectViewer.cpp
  */
  virtual void notifyCameraChanged();

private:
 /*!
  * \ingroup      UI
  * \brief        Update Inventor's viewer camera
  * \return       void
  * \par      Source:
  *                ObjectViewer.cpp
  */
  void updateCameraLink();

 /*!
  * \ingroup      UI
  * \brief        Static event handler for camera changes.
  *
  *               Calls notifyCameraChanged().
  *
  * \return       void
  * \par      Source:
  *                ObjectViewer.cpp
  */
  static void cameraCB(void *data, SoSensor  *);

signals:
 /*!
  * \ingroup      UI
  * \brief        Notifies changed camera
  * \return       void
  * \par      Source:
  *                ObjectViewer.cpp
  */
  void cameraChanged();

protected:
  // protected attributes
  SoCamera *m_camera;               /*!< current camera */
  SoNodeSensor *m_sensor;           /*!< camera sensor*/
  ObjectViewer * m_linkedTo;        /*!< viewer linked to */
  ObjectViewer * m_linkedPlaneTo;   /*!< sectioning plane linked to*/
  int m_ID;                         /*!< viewer ID to save and recoved links*/
  int m_linkedViewerID;             /*!< linked viewer ID*/
  int m_linkedPlaneID;              /*!< linked section plane ID*/
};
#endif // OBJECTVIEWER_H
