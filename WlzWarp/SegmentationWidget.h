#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _SegmentationWidget_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         SegmentationWidget.h
* \author       Zsolt Husz
* \date         June 2009
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
* \brief	Widget for manipulating segmented Woolz Object properties
* 		(see WoolzDynThresholdedObj())
* \ingroup	UI
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
 * \brief	Widget class for manipulating segmented Woolz Object
 * 		(WoolzDynThresholdedObj) common properties
 * \ingroup	UI
 */
class SegmentationWidget: public QDockWidget, public Ui::SegmentationWidget
{
  Q_OBJECT
  public:
    /*!
     * \ingroup	UI
     * \brief	Constructor
     * \param	parent			parent widget
     * \param	objectListModel		to manage objects
     */
    SegmentationWidget(QWidget *parent, ObjectListModel *objectListModel);

    /*!
     * \ingroup	UI
     * \brief	Destructor
     */
    ~SegmentationWidget();

  private slots:
    /*!
     * \ingroup	UI
     * \brief	Processes low channel selection change
     */
    void channelChanged();

    /*!
     * \ingroup	UI
     * \brief	Processes low threshold change
     */
    void lowThresholdChanged() ;

    /*!
     * \ingroup	UI
     * \brief	Processes low threshold change
     */
    void highThresholdChanged() ;

    /*!
     * \ingroup	UI
     * \brief	Forces update of the object views
     */
    void update();

    /*!
     * \ingroup	UI
     * \brief	Selects the base object of the current object
     */
    void goBaseGo();

    /*!
     * \ingroup	UI
     * \brief	Creates new segmented object
     */
    void createNew();

    /*!
     * \ingroup	UI
     * \brief	Processes source object change.
     * \param	index			new selection 
     */
    void sourceObjectChanged(int index);

    /*!
     * \ingroup	UI
     * \brief	Processes object removal.
     */
    void removedObjectSignal(WoolzObject* obj);

    /*!
     * \ingroup	UI
     * \brief	Processes object addition.
     */
    void addObjectSignal(WoolzObject* obj);

    /*!
     * \ingroup	UI
     * \brief	Processes external object selection.
     * \param	object			new selected object
     */
    void objectSelected(WoolzObject* object);

    /*!
     * \ingroup	UI
     * \brief	Processes external object property change.
     */
    void objectPropertyChanged();

    /*!
     * \ingroup	UI
     * \brief	Processes auto update state change
     * \param	enabled			auto update state
     */
    void setAutoUpdate(bool enabled);

    /*!
     * \ingroup	UI
     * \brief	Processes object update state change
     * \param	enabled			auto update
     */
    void enableUpdate(bool enabled);

  private:
    /*!
     * \ingroup	UI
     * \brief	Sets a new source object, if current has no source then
     * 		adds a *removed* item
     * \param	source			source object
     */
    void selectSource(WoolzObject * source);

    /*!
     * \ingroup	UI
     * \brief	Fills widget with the values of the object.
     * \param	object			object to be loaded
     */
    void loadProperties(WoolzDynThresholdedObj* object);

  protected:
    ObjectListModel* m_objectListModel;    /*!< the object list model */
    WoolzDynThresholdedObj * m_object;     /*!< current object */
    WoolzObject *m_sourceObject;           /*!< the source (parent) object */
};
#endif  //SEGMENTATIONWIDGET_H
