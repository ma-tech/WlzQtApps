#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _ObjectViewer_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         ObjectViewer.h
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
* \brief	Woolz object viewer.
* \ingroup	UI
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
 * \brief	Woolz object viewer class.
 *
 *               For earch object viewed manages an view that cares of the
 *               actual visualisation of the object by generating an Inventor
 *               SoSeparator node
 *
 *               Uses a scene graph:
 *               \verbatim
                   - root
                       - event callback (mouse events)
                       - camera
                       - views_root (see ObjectSimpleViewer)
                 \endverbatim
 * \ingroup	UI
 */
class ObjectViewer : public ObjectSimpleViewer
{
  Q_OBJECT
  public:
    /*!
     * \ingroup	UI
     * \brief	Constructor
     * \param	objectViewerModel	model managing the viewer
     * \param	is3D			if viewer is for 3D objects
     */
    ObjectViewer(ObjectViewerModel *objectViewerModel, bool is3D,
        bool isBlending = false);

    /*!
     * \ingroup	UI
     * \brief	Destructor
     */
    virtual ~ObjectViewer();

    /*!
     * \ingroup	UI
     * \brief	Returns view list
     * \return	view list
     */
    virtual QList <ObjectView*> getViews() {return(views);}

    /*!
     * \ingroup	UI
     * \brief	Callback function to notify in the camera was replaced.
     * \param	cam			new camera
     */
    virtual void replacedCamera( SoCamera *cam);

    /*!
     * \ingroup	UI
     * \brief	Sets a new viewer ID
     * \param	ID			new id
     */
    void setID(int ID) {m_ID = ID;}

    /*!
     * \ingroup	UI
     * \brief	Rerturns the viewer ID
     * \return	true if viewer is linked, false otherwise
     */
    int ID() {return(m_ID);}

    /*!
     * \ingroup	UI
     * \brief	Rerturns the status of linking
     * \return	true if viewer is linked, false otherwise
     */
    void setLinkedIDs()
    {
      m_linkedViewerID = m_linkedTo ? m_linkedTo->m_ID : -1;
      m_linkedPlaneID = m_linkedPlaneTo ? m_linkedPlaneTo->m_ID : -1;
    }

    /*!
     * \ingroup	UI
     * \brief	Rerturns the linked viewer ID
     * \return	linked viewerID
     */
    int getLinkedID() {return(m_linkedViewerID);}

    /*!
     * \ingroup	UI
     * \brief	Rerturns the linked viewer ID
     * \return	linked viewerID
     */
    int getLinkedPlaneID()  {return(m_linkedPlaneID);}

    /*!
     * \ingroup	UI
     * \brief	Sets the viewer that imposes the current viewer camera view
     * \param	viewer			Viewer to whitch the current viewer
     * 				        will be linked to
     */
    void setLinkedTo(ObjectViewer * viewer);

    /*!
     * \ingroup	UI
     * \brief	Rerturns linked viewer
     * \return	linked viewer
     */
    ObjectViewer * getLinkedTo() {return(m_linkedTo);}

    /*!
     * \ingroup	Control
     * \brief	Saves model in xml format.
     * \param	xmlWriter			output xml stream
     * \return	true if succeded, false if not
     */
    virtual bool saveAsXml(QXmlStreamWriter *xmlWriter);

    /*!
     * \ingroup	Control
     * \brief	Reads current token if known
     * \param	element			current element of the DOM tree
     * \return	true if succeded, false if not
     */
    bool parseDOMLine(const QDomElement &element);

    /*!
     * \ingroup	UI
     * \brief	Sets the viewer that imposes plane copied in this viewer
     * \param	viewer			Viewer to whitch the current viewer
     * 					will be linked to
     */
    void setLinkedPlaneTo( ObjectViewer * viewer );

    /*!
     * \ingroup	UI
     * \brief	Rerturns linked plane viewer
     * \return	linked viewer
     */
    ObjectViewer * getLinkedPlaneTo() {return(m_linkedPlaneTo);}

  public slots:
    /*!
     * \ingroup	UI
     * \brief	Processes camera changes in other viewers.
     */
    void cameraChangedIn();

    /*!
     * \ingroup	UI
     * \brief	Sets alpha
     *
     */
    void alphaChanged(bool alpha);

  protected:
    /*!
     * \ingroup	UI
     * \brief	Returns the default object transparency.
     * \param	object			the new object
     * \return	100 is transparent, 0 if not
     */
    virtual int initialTransparency(WoolzObject * object ) = 0;

    /*!
     * \ingroup	UI
     * \brief	Processes camara change.
     *
     *          Emits cameraChanged() signal.
     */
    virtual void notifyCameraChanged();

  private:
    /*!
     * \ingroup	UI
     * \brief	Update Inventor's viewer camera
     */
    void updateCameraLink();

    /*!
     * \ingroup	UI
     * \brief	Static event handler for camera changes.
     *
     *          Calls notifyCameraChanged().
     */
    static void cameraCB(void *data, SoSensor  *);

  signals:
    /*!
     * \ingroup	UI
     * \brief	Notifies changed camera
     */
    void cameraChanged();

  protected:
    // protected attributes
    SoCamera *m_camera;               /*!< current camera */
    SoNodeSensor *m_sensor;           /*!< camera sensor */
    ObjectViewer * m_linkedTo;        /*!< viewer linked to */
    ObjectViewer * m_linkedPlaneTo;   /*!< sectioning plane linked to */
    int m_ID;                         /*!< viewer ID to save and recoved
    					   links */
    int m_linkedViewerID;             /*!< linked viewer ID */
    int m_linkedPlaneID;              /*!< linked section plane ID */
};
#endif // OBJECTVIEWER_H
