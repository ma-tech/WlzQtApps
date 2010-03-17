#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _SectioningPlaneWidget_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         SectioningPlaneWidget.h
* \author       Zsolt Husz
* \date         March 2010
* \version      $Id$
* \par
* Address:
*               MRC Human Genetics Unit,
*               Western General Hospital,
*               Edinburgh, EH4 2XU, UK.
* \par
* Copyright (C) 2010 Medical research Council, UK.
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
* \brief        Widget for manipulating cutting plane in viewers
* \ingroup      UI
*
*/
#ifndef SECTIONINGPLANEWIDGET_H
#define SECTIONINGPLANEWIDGET_H

#include <QDockWidget>
#include <ui_SectioningPlaneWidget.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/SbLinear.h>

class QObject;

class SoFieldSensor;
class SoSensor;

class ObjectViewerController;
class ObjectViewer;

/*!
 * \brief	Widget class for manipulating transfer functions of volume objects
 * \ingroup      UI
 */
class SectioningPlaneWidget : public QDockWidget, public Ui::SectioningPlaneWidget
{
Q_OBJECT
public:
 /*!
  * \ingroup      UI
  * \brief        Constructor
  * \param        parent parent widget
  * \param        objectViewerController tge manager of viewers
  * \par      Source:
  *                SectioningPlaneWidget.cpp
  */
  SectioningPlaneWidget(QWidget *parent, ObjectViewerController *objectViewerController);

 /*!
  * \ingroup      UI
  * \brief        Destructor
  * \par      Source:
  *                SectioningPlaneWidget.cpp
  */
  ~SectioningPlaneWidget();

private slots:
 /*!
  * \ingroup      UI
  * \brief        Processes linked combo box selection
  * \par      Source:
  *                SectioningPlaneWidget.cpp
  */
  void changedLink() ;

 /*!
  * \ingroup      UI
  * \brief        Processes viewer selection changes
  * \return       void
  * \par      Source:
  *                SectioningPlaneWidget.cpp
  */
  void viewerSelected();

 /*!
  * \ingroup      Controls
  * \brief        Field sensor callback to capture plane move
  * Called when sensor signals change of the field value.
  * \return       void
  * \par      Source:
  *                SectioningPlaneWidget.cpp
  */
  static void fieldSensorCB(void *, SoSensor *);

public slots:
 /*!
  * \ingroup      UI
  * \brief        Processes plane change
  * \return       void
  * \par      Source:
  *                SectioningPlaneWidget.cpp
  */
  void planePropertyChanged();

 /*!
  * \ingroup      UI
  * \brief        Clip plane added
  * \return       void
  * \par      Source:
  *                SectioningPlaneWidget.cpp
  */
  void addedClipPlane();

 /*!
  * \ingroup      UI
  * \brief        Apply new settings to the sectioning plane
  * \return       void
  * \par      Source:
  *                SectioningPlaneWidget.cpp
  */
  void apply();

private:
 /*!
  * \ingroup      UI
  * \brief        Fills widget with the values the transfer function of the object.
  * \param        object object to be loaded
  * \return       void
  * \par      Source:
  *                SectioningPlaneWidget.cpp
  */
  void loadProperties(ObjectViewer* viewer);

protected:
  ObjectViewerController* m_objectViewerController;    /*!< object viewer controller*/
  ObjectViewer * m_viewer;                             /*!< current viewer*/
  SoFieldSensor *m_fieldSensor;                        /*!< sensor signalling plane changes*/
};

#endif  //SECTIONINGPLANEWIDGET_H
