#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _WarperViewer_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         WarperViewer.h
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
* \brief        Viewer displaying source objects
* \ingroup      UI
*
*/

#ifndef WARPERVIEWER_H
#define WARPERVIEWER_H

#include <ObjectViewer.h>
#include "LandmarkController.h"

class LandmarkView;
class SoPickStyle;

/*!
* \brief        Superclass for warping source and target classes
*
*               Viewer class displaying source or target objects. It manages feature point views.
*
*                 - root (see ObjectViewer for details)
*                     - pickstyle (for views)
*                     - event callback (mouse events)
*                     - camera
*                     - views_root (see ObjectSimpleViewer)
*                     - clip plane
*                     - pick style (SHAPE)
*                     - landmark
*
* \ingroup      UI
*/
class WarperViewer:  public ObjectViewer
{
  Q_OBJECT

public:
 /*!
  * \ingroup      UI
  * \brief        Constructor
  * \param        objectViewerModel model managing the viewer
  * \param        is3D if viewer is for 3D objects
  * \param        fPointModel landmark model
  * \param        AddAction pointer referring add landmark action Qt event
  * \param        DeleteAction pointer referring delete landmark action Qt event
  * \param        MoveAction pointer referring move landmark action Qt event
  *
  * \return       void
  * \par      Source:
  *                WarperViewer.cpp
  */
  WarperViewer (ObjectViewerModel *objectViewerModel, bool is3D,
      LandmarkController* landmarkController, QAction * AddAction, QAction * DeleteAction, QAction * MoveAction);

 /*!
  * \ingroup      Views
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *                WarperViewer.cpp
  */
  virtual ~WarperViewer ( );

 /*!
  * \ingroup      UI
  * \brief        Processes mouse event
  *
  *               It allows landmark addition and deletion only
  *               if an apropriate mode is selected
  *               (move is checked by the dragger itself)
  *
  * \return       void
  * \par      Source:
  *                WarperViewer.cpp
  */
  virtual void notifyMouseEvent(SoEventCallback * event);

 /*!
  * \ingroup      UI
  * \brief        Configures the view
  *
  * \return       void
  * \par      Source:
  *                WarperViewer.cpp
  */
  virtual void init() = 0;

 /*!
  * \ingroup      UI
  * \brief        Processes the signal of landmark addition
  *
  *                It forwards the signal to the appropiate source or target model.
  * \param        point point to add
  * \return       void
  * \par      Source:
  *                WarperViewer.cpp
  */
  virtual void addLandmark(const WlzDVertex3 point) = 0;

   /*!
  * \ingroup      UI
  * \brief        Activates and deactivates linked camera view
  * \param        isLinked true of viewer view is set to be linked
  *
  * \return       void
  * \par      Source:
  *                WarperViewer.cpp
  */
  void setIsLinked ( bool isLinked )   {
      m_isLinked = isLinked;
  }

 /*!
  * \ingroup      UI
  * \brief        Rerturns the status of linking
  *
  * \return       true if viewer is linked, false otherwise
  * \par      Source:
  *                WarperViewer.cpp
  */
  bool getIsLinked ( )   { return m_isLinked; }

public slots:
 /*!
  * \ingroup      Visualisation
  * \brief        Processes update in landmark addition mode
  * \return       void.
  * \par      Source:
  *                WarperViewer.cpp
  */
  void modeUpdated ( );


private slots:
 /*!
  * \ingroup      Visualisation
  * \brief        Sets up or deletes clip plane
  * \param        plane new plane to use
  * \return       void.
  * \par      Source:
  *                WarperViewer.cpp
  */
  void addedClipPlaneSlot(SoClipPlane * plane);

 /*!
  * \ingroup      Visualisation
  * \brief        Manages clip plane on / off
  * \param        on turn plane on / off
  * \return       void.
  * \par      Source:
  *                WarperViewer.cpp
  */
  void buttonClipLandmarkClicked(bool on);

 /*!
  * \ingroup      UI
  * \brief        Processes transparency slider change if slider is present
  * \param        transparency new transparency
  *
  *               Reimplemented form ObjectViewer
  * \return       void
  * \par      Source:
  *                WarperViewer.cpp
  */
  virtual void transparencyChanged(int transparency);

protected:
 /*!
  * \ingroup      UI
  * \brief        Converts Inventor coordinate stucture to Woolz stucture
  *
  * \param        point Inventor coordinate
  * \return       Woolz coordinate
  * \par      Source:
  *                WarperViewer.cpp
  */
  WlzDVertex3 covertSbVec3f2WlzDVertex3(const SbVec3f point);

 /*!
  * \ingroup      UI
  * \brief        Creates landmark view
  *
  * \param        indexType landmark view type
  * \return       void
  * \par      Source:
  *                WarperViewer.cpp
  */
  void setupLandmarkView(LandmarkModel::IndexType indexType);


private:
 /*!
  * \ingroup      UI
  * \brief        Update Inventor's viewer camera
  *
  * \return       void
  * \par      Source:
  *                WarperViewer.cpp
  */
  void updateCameraLink();


protected:
  LandmarkView * landmarkView;              /*!< landmark view */
  LandmarkController* m_landmarkController; /*!< landmark model */

  QAction * addAction;                      /*!< action allowing landmark addition model */
  QAction * deleteAction;                   /*!< action allowing landmark deletion model */
  QAction * moveAction;                     /*!< action allowing landmark move model */

  SoPickStyle *m_pickStyle;                 /*!< specifies if views object are pickable or not*/
  SoClipPlane *m_clipPlane;                 /*!< clip plane */
  bool m_clipOn;                            /*!< if clip plane is on*/
};
#endif // WARPERVIEWER_H
