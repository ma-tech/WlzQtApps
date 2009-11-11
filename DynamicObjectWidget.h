#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _DynamicObjectWidget_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         DynamicObjectWidget.h
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
* \brief        Segmented object generation dialog
* \ingroup      UI
*
*/
#ifndef DynamicObjectWidget_H
#define DynamicObjectWidget_H

#include <QDockWidget>

class ObjectListModel;
class WoolzObject;
/*!
* \brief	Controler class for setting mesh properties
* \ingroup      UI
*/
class DynamicObjectWidget : public QDockWidget
{
Q_OBJECT
public:
 /*!
  * \ingroup      UI
  * \brief        Constructor
  * \param        parent parent widget
  * \par      Source:
  *                DynamicObjectWidget.cpp
  */
  DynamicObjectWidget(QWidget *parent, ObjectListModel *objectListModel);

  ~DynamicObjectWidget();

//  WoolzObject * getResult();
//  void free();

private slots:
 void recomputeObject();


 /*!
  * \ingroup      UI
  * \brief        Processes the signal of landmark selection change
  * \param        current new selection
  * \param        previous previous selection
  * \par      Source:
  *                DynamicObjectWidget.cpp
  */

 /*!
  */

 /*!
  * \ingroup      UI
  * \brief        Create new segmented object
  * \par      Source:
  *                DynamicObjectWidget.cpp
  */
  void createNew();


 void sourceObjectChanged(int);

 void goBaseGo();

public slots:
  void objectSelected(WoolzObject* object);
  void removedObjectSignal(WoolzObject* obj);

  void addObjectSignal(WoolzObject* obj);
  void objectPropertyChanged();
  void setAutoUpdate(bool enabled);
  void update();

protected:
   WoolzDynObject *m_objt;
   WoolzObject *m_sourceObject;
   ObjectListModel* m_objectListModel;
   virtual QSize sizeHint () const;
};

#endif  //DynamicObjectWidget_H
