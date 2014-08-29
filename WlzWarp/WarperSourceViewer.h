#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _WarperSourceViewer_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         WarperSourceViewer.h
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
* \brief	Viewer displaying source objects
* \ingroup	UI
*/

#ifndef WARPERSOURCEVIEWER_H

#define WARPERSOURCEVIEWER_H
#include "WarperViewer.h"

class QXmlStreamWriter;

/*!
 * \brief	Warping source object viewer class.
 *              Viewer class displaying source objects. It has feature
 *              point views.
 *
 * \ingroup	UI
 */
class WarperSourceViewer : public WarperViewer
{
  Q_OBJECT

  public:
    /*!
     * \ingroup	UI
     * \brief	Constructor
     * \param	objectViewerModel	model managing the viewer
     * \param	is3D			if viewer is for 3D objects
     * \param	fPointModel		landmark model
     * \param	AddAction		pointer referring add landmark action
     * 					Qt event
     * \param	DeleteAction		pointer referring delete landmark
     * 					action Qt event
     * \param	MoveAction		pointer referring move landmark action
     * 					Qt event
     * \param	RemovelElemAction	pointer referring remove element
     * 					action Qt event
     *
     */
    WarperSourceViewer (ObjectViewerModel *objectViewerModel, bool is3D,
        LandmarkController* fPointModel, QAction * AddAction,
	QAction * DeleteAction, QAction * MoveAction,
	QAction * RemovelElemAction);

    /*!
     * \ingroup	UI
     * \brief	Checks if viewer accepts object.
     * \return	true if viewer object is a source object
     */
    virtual bool accepting(WoolzObject * object );

    /*!
     * \ingroup	UI
     * \brief	Returns the default object transparency.
     * \return	0, all source objects are visible
     */
    virtual int initialTransparency(WoolzObject * ) {return 0;}

    /*!
     * \ingroup	UI
     * \brief	Configures the view
     *
     */
    virtual void init();

    /*!
     * \ingroup	Control
     * \brief	Saves model in xml format.
     * \param	xmlWriter			output xml stream
     * \param	header			true of header should be written
     * \return	true if succeded, false if not
     */
    virtual bool saveAsXml(QXmlStreamWriter *xmlWriter);

  protected:
    /*!
     * \ingroup	UI
     * \brief	Returns the background colour of the viewer
     *          Reimplemented form ObjectViewer
     * \return	colour
     */
    virtual QColor getBackgroundColour();

    /*!
     * \ingroup	UI
     * \brief	Processes the signal of landmark addition
     *          It forwards the signal to the appropiate source model.
     * \param	point			point to add
     */
    virtual void addLandmark(const WlzDVertex3 point);

  public:
    static const char * xmlTag;        /*!< xml section tag string */
};

#endif // WARPERSOURCEVIEWER_H
