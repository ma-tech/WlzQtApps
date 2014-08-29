#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _WarperViewer_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         WarperViewer.h
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

#ifndef WARPERVIEWER_H
#define WARPERVIEWER_H

#include "ObjectViewer.h"
#include "LandmarkController.h"

class LandmarkView;
class SoPickStyle;

/*!
 * \brief	Superclass for warping source and target classes
 *
 *              Viewer class displaying source or target objects.
 *              It manages feature point views.
 *                 - root (see ObjectViewer for details)
 *                     - pickstyle (for views)
 *                     - event callback (mouse events)
 *                     - camera
 *                     - views_root (see ObjectSimpleViewer)
 *                     - clip plane
 *                     - pick style (SHAPE)
 *                     - landmark
 * \ingroup	UI
 */
class WarperViewer:  public ObjectViewer
{
  Q_OBJECT

  public:
    /*!
     * \ingroup	UI
     * \brief	Constructor
     * \param	objectViewerModel	model managing the viewer
     * \param	is3D			if viewer is for 3D objects
     * \param	fPointModel		landmark model
     * \param	AddAction		pointer referring add landmark
     * 					action Qt event
     * \param	DeleteAction		pointer referring delete landmark 
     * 					action Qt event
     * \param	MoveAction		pointer referring move landmark 
     * 					action Qt event
     * \param	RemovelElemAction	pointer referring remove element 
     * 					action Qt event
     */
    WarperViewer(ObjectViewerModel *objectViewerModel, bool is3D,
	LandmarkController* landmarkController, QAction * AddAction,
	QAction * DeleteAction, QAction * MoveAction,
	QAction * RemovelElemAction);

    /*!
     * \ingroup	Views
     * \brief	Destructor
     */
    virtual ~WarperViewer ( );

    /*!
     * \ingroup	UI
     * \brief	Processes mouse event
     *
     * 		It allows landmark addition and deletion only if an
     * 		apropriate mode is selected (move is checked by the
     * 		dragger itself)
     *
     */
    virtual void notifyMouseEvent(SoEventCallback * event);

    /*!
     * \ingroup	UI
     * \brief	Configures the view
     *
     */
    virtual void init() = 0;

    /*!
     * \ingroup	UI
     * \brief	Processes the signal of landmark addition
     *
     *          It forwards the signal to the appropiate source or
     *          target model.
     * \param	point			point to add
     */
    virtual void addLandmark(const WlzDVertex3 point) = 0;

    /*!
     * \ingroup	UI
     * \brief	Adds new object to the viewer.
     * \param	object			object to be added
     * \param	doViewAll		if true and no view all was done in
     * 					the viewer then it also viewer will
     * 					be adjusted to include all objects
     * \param	previousView		if not NULL, copies transparencu and
     * 					visibility values to the new view
     */
    virtual void addObject(WoolzObject * object, bool doViewAll = true,
        ObjectView *previousView = NULL);

  public slots:
    /*!
     * \ingroup	Visualisation
     * \brief	Processes update in landmark addition mode
     */
    void modeUpdated();

  private slots:
    /*!
     * \ingroup	Visualisation
     * \brief	Sets up or deletes clip plane
     * \param	plane			new plane to use
     */
    void addedClipPlaneSlot(SoClipPlane * plane);

    /*!
     * \ingroup	Visualisation
     * \brief	Manages clip plane on / off
     * \param	on			turn plane on / off
     */
    void buttonClipLandmarkClicked(bool on);

    /*!
     * \ingroup	UI
     * \brief	Processes transparency slider change if slider is present.
     *          Reimplemented form ObjectViewer
     * \param	transparency			new transparency
     *
     */
    virtual void transparencyChanged(int transparency);

  protected:
    /*!
     * \ingroup	UI
     * \brief	Converts Inventor coordinate stucture to Woolz stucture
     * \param	point			Inventor coordinate
     * \return	Woolz coordinate
     */
    WlzDVertex3 covertSbVec3f2WlzDVertex3(const SbVec3f point);

    /*!
     * \ingroup	UI
     * \brief	Creates landmark view
     * \param	indexType			landmark view type
     */
    void setupLandmarkView(LandmarkModel::IndexType indexType);

  signals:
    /*!
     * \ingroup	UI
     * \brief	Signals request to remove an element
     * \param	point			to be removed
     */
    void removeMeshElementSignal(const WlzDVertex3 point);

  private:
    /*!
     * \ingroup	UI
     * \brief	Update Inventor's viewer camera
     *
     */
    void updateCameraLink();

  protected:
    LandmarkView * landmarkView;	/*!< landmark view */
    LandmarkController* m_landmarkController; /*!< landmark model */

    QAction * addAction;               /*!< action allowing landmark addition
    					    model */
    QAction * deleteAction;            /*!< action allowing landmark deletion
    					    model */
    QAction * moveAction;              /*!< action allowing landmark move
    					    model */
    QAction * removeMeshElementAction; /*!< action allowing mesh element
    					    removal */

    SoPickStyle *m_pickStyle;          /*!< specifies if views object are
    					    pickable or not */
    SoClipPlane *m_clipPlane;          /*!< clip plane */
    bool m_clipOn;                     /*!< if clip plane is on */
};
#endif // WARPERVIEWER_H
