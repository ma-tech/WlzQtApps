#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ObjectPropertyWidget_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ObjectPropertyWidget.h
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
* \brief        Widget for selecting and manipulating Woolz Object common properties
* \ingroup      UI
*
*/
#ifndef OBJECTPROPERTYWIDGET_H
#define OBJECTPROPERTYWIDGET_H

#include <QDockWidget>
#include <ui_ObjectPropertyWidget.h>

class ObjectListModel;
class WoolzObject;

/*!
 * \brief	Widget class for secting and manipulating Woolz Object common properties
 * \ingroup      UI
 */
class ObjectPropertyWidget : public QDockWidget, public Ui::ObjectPropertyWidget
{
Q_OBJECT
public:
 /*!
  * \ingroup      UI
  * \brief        Constructor
  * \param        parent parent widget
  * \param        objectListModel to manage objects
  * \par      Source:
  *                ObjectPropertyWidget.cpp
  */
  ObjectPropertyWidget(QWidget *parent, ObjectListModel *objectListModel);

 /*!
  * \ingroup      UI
  * \brief        Destructor
  * \par      Source:
  *                ObjectPropertyWidget.cpp
  */
  ~ObjectPropertyWidget();

private slots:
 /*!
  * \ingroup      UI
  * \brief        Processes object colour update.
  *
  * \return       void
  * \par      Source:
  *                ObjectPropertyWidget.cpp
  */
  void colourChanged();

 /*!
  * \ingroup      UI
  * \brief        Processes object addition.
  * \return       void
  * \par      Source:
  *                ObjectPropertyWidget.cpp
  */
  void addObjectSignal(WoolzObject* obj);

 /*!
  * \ingroup      UI
  * \brief        Processes object removal.
  * \return       void
  * \par      Source:
  *                ObjectPropertyWidget.cpp
  */
  void removedObjectSignal(WoolzObject* obj);

 /*!
  * \ingroup      UI
  * \brief        Processes object selection in the combobox.
  * \return       void
  * \par      Source:
  *                ObjectPropertyWidget.cpp
  */
  void objectChanged(int index);

 /*!
  * \ingroup      UI
  * \brief        Processes object name change in the combobox.
  * \return       void
  * \par      Source:
  *                ObjectPropertyWidget.cpp
  */
  void nameChanged(const QString &str);

 /*!
  * \ingroup      UI
  * \brief        Processes external object property change.
  * \return       void
  * \par      Source:
  *                ObjectPropertyWidget.cpp
  */
  void objectPropertyChanged();

 /*!
  * \ingroup      UI
  * \brief        Processes removal button event.
  * \return       void
  * \par      Source:
  *                ObjectPropertyWidget.cpp
  */
  void removeObject();

 /*!
  * \ingroup      UI
  * \brief        Processes visibility checkbox change.
  * \return       void
  * \par      Source:
  *                ObjectPropertyWidget.cpp
  */
  void visibleChanged(bool checked);

public slots:
 /*!
  * \ingroup      UI
  * \brief        Processes external object selection.
  * \param        object new selected object
  * \return       void
  * \par      Source:
  *                ObjectPropertyWidget.cpp
  */
  void objectSelected(WoolzObject* object);

private:
 /*!
  * \ingroup      UI
  * \brief        Fills widget with the values of the object.
  * \param        object object to be loaded
  * \return       void
  * \par      Source:
  *                ObjectPropertyWidget.cpp
  */
  void loadProperties(WoolzObject* object);

protected:
  ObjectListModel* m_objectListModel;    /*!< the object list model*/
  WoolzObject * m_object;                /*!< current object*/
};

#endif  //OBJECTPROPERTYWIDGET
