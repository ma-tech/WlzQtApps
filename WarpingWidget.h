#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _WarpingWidgett_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         WarpingWidget.h
* \author       Zsolt Husz
* \date         June 2009
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
* \brief        Widget for manipulating warped woolz objects (see WoolzDynWarpedObject )
* \ingroup      UI
*
*/

#ifndef WARPINGWIDGET_H
#define WARPINGWIDGET_H

#include <QDockWidget>
#include <ui_WarpingWidget.h>

class QObject;
class LandmarkController;

/*!
 * \brief	Widget class for manipulating segmented Woolz Object (WoolzDynThresholdedObj) common properties 
 * \ingroup      UI
 */
class WarpingWidget : public QDockWidget, public Ui::WarpingWidget
{
Q_OBJECT
public:
 /*!
  * \ingroup      UI
  * \brief        Constructor
  * \param        parent parent widget
  * \param        objectListModel to manage objects
  * \param        landmarkModel to manage landmarks
  * \par      Source:
  *                WarpingWidget.cpp
  */
  WarpingWidget(QWidget *parent, LandmarkController *landmarkController);

 /*!
  * \ingroup      UI
  * \brief        Destructor
  * \par      Source:
  *                WarpingWidget.cpp
  */
  ~WarpingWidget();

private slots:

 /*!
  * \ingroup      UI
  * \brief        Processes auto update state change
  * \param        enabled auto update state
  * \return       void
  * \par      Source:
  *                WarpingWidget.cpp
  */
  void setAutoUpdate(bool enabled);

 /*!
  * \ingroup      UI
  * \brief        Processes delta change
  * \return       void
  * \par      Source:
  *                WarpingWidget.cpp
  */
  void deltaChanged();

 /*!
  * \ingroup      UI
  * \brief        Processes basis function change
  * \return       void
  * \par      Source:
  *                WarpingWidget.cpp
  */
  void basisFnChanged();

public slots:
 /*!
  * \ingroup      UI
  * \brief        Reloads parameters externaly changed
  * \return       void
  * \par      Source:
  *                WarpingWidget.cpp
  */
  void update();

protected:
  LandmarkController *m_landmarkController;        /*!< the landmark model specifying the warping*/
  bool m_enableUpdate;
};

#endif  //WARPINGWIDGET
