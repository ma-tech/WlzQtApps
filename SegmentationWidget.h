#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _SegmentationWidget_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         SegmentationWidget.h
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
* \brief        Widget for manipulating segmented Woolz Object properties (see WoolzDynThresholdedObj)
* \ingroup      UI
*
*/
#ifndef SEGMENTATIONWIDGET_H
#define SEGMENTATIONWIDGET_H

#include <QDockWidget>
#include <ui_SegmentationWidget.h>

class ObjectListModel;
class WoolzObject;
class WoolzDynThresholdedObj;
class QObject;

/*!
 * \brief	 Widget class for manipulating segmented Woolz Object (WoolzDynThresholdedObj) common properties
 * \ingroup      UI
 */
class SegmentationWidget : public QDockWidget, public Ui::SegmentationWidget
{
Q_OBJECT
public:
 /*!
  * \ingroup      UI
  * \brief        Constructor
  * \param        parent parent widget
  * \param        objectListModel to manage objects
  * \par      Source:
  *                SegmentationWidget.cpp
  */
  SegmentationWidget(QWidget *parent, ObjectListModel *objectListModel);

 /*!
  * \ingroup      UI
  * \brief        Destructor
  * \par      Source:
  *                SegmentationWidget.cpp
  */
  ~SegmentationWidget();

private slots:
 /*!
  * \ingroup      UI
  * \brief        Processes low channel selection change
  * \par      Source:
  *                SegmentationWidget.cpp
  */
  void channelChanged();

 /*!
  * \ingroup      UI
  * \brief        Processes low threshold change
  * \par      Source:
  *                SegmentationWidget.cpp
  */
  void lowThresholdChanged() ;

   /*!
  * \ingroup      UI
  * \brief        Processes low threshold change
  * \par      Source:
  *                SegmentationWidget.cpp
  */
  void highThresholdChanged() ;

 /*!
  * \ingroup      UI
  * \brief        Forces update of the object views
  * \par      Source:
  *                SegmentationWidget.cpp
  */
  void update();

 /*!
  * \ingroup      UI
  * \brief        Selects the base object of the current object
  * \par      Source:
  *                SegmentationWidget.cpp
  */
  void goBaseGo();

 /*!
  * \ingroup      UI
  * \brief        Creates new segmented object
  * \par      Source:
  *                SegmentationWidget.cpp
  */
  void createNew();

 /*!
  * \ingroup      UI
  * \brief        Processes source object change.
  * \param        index new selection 
  * \par      Source:
  *                SegmentationWidget.cpp
  */
  void sourceObjectChanged(int index);

 /*!
  * \ingroup      UI
  * \brief        Processes object removal.
  * \return       void
  * \par      Source:
  *                SegmentationWidget.cpp
  */
  void removedObjectSignal(WoolzObject* obj);

 /*!
  * \ingroup      UI
  * \brief        Processes object addition.
  * \return       void
  * \par      Source:
  *                SegmentationWidget.cpp
  */
  void addObjectSignal(WoolzObject* obj);

 /*!
  * \ingroup      UI
  * \brief        Processes external object selection.
  * \param        object new selected object
  * \return       void
  * \par      Source:
  *                SegmentationWidget.cpp
  */
  void objectSelected(WoolzObject* object);

 /*!
  * \ingroup      UI
  * \brief        Processes external object property change.
  * \return       void
  * \par      Source:
  *                SegmentationWidget.cpp
  */
  void objectPropertyChanged();

 /*!
  * \ingroup      UI
  * \brief        Processes auto update state change
  * \param        enabled auto update state
  * \return       void
  * \par      Source:
  *                SegmentationWidget.cpp
  */
 void setAutoUpdate(bool enabled);

private:
 /*!
  * \ingroup      UI
  * \brief        Sets a new source object, if current has no source then adds a *removed* item
  * \param        source source object
  * \return       void
  * \par      Source:
  *                SegmentationWidget.cpp
  */
  void selectSource(WoolzObject * source);

 /*!
  * \ingroup      UI
  * \brief        Fills widget with the values of the object.
  * \param        object object to be loaded
  * \return       void
  * \par      Source:
  *                ObjectPropertyWidget.cpp
  */
  void loadProperties(WoolzDynThresholdedObj* object);

protected:
  ObjectListModel* m_objectListModel;    /*!< the object list model*/
  WoolzDynThresholdedObj * m_object;     /*!< current object*/
  WoolzObject *m_sourceObject;           /*!< the source (parent) object*/
};
#endif  //SEGMENTATIONWIDGET_H
