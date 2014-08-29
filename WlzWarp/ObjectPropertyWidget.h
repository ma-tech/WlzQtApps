#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _ObjectPropertyWidget_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         ObjectPropertyWidget.h
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
* \brief	Widget for selecting and manipulating Woolz Object
* 		common properties
* \ingroup	UI
*/

#ifndef OBJECTPROPERTYWIDGET_H
#define OBJECTPROPERTYWIDGET_H

#include <QDockWidget>
#include <ui_ObjectPropertyWidget.h>

class ObjectListModel;
class WoolzObject;

/*!
 * \brief	Widget class for secting and manipulating Woolz Object
 * 		common properties
 * \ingroup	UI
 */
class ObjectPropertyWidget:
public QDockWidget, public Ui::
ObjectPropertyWidget
{
  Q_OBJECT
  public:
    /*!
     * \ingroup	UI
     * \brief	Constructor
     * \param	parent			parent widget
     * \param	objectListModel			to manage objects
     */
    ObjectPropertyWidget(QWidget *parent, ObjectListModel *objectListModel);

    /*!
     * \ingroup	UI
     * \brief	Destructor
     */
    ~ObjectPropertyWidget();

  private slots:
    /*!
     * \ingroup	UI
     * \brief	Processes object colour update.
     *
     */
    void colourChanged();

    /*!
     * \ingroup	UI
     * \brief	Processes object addition.
     */
    void addObjectSignal(WoolzObject* obj);

    /*!
     * \ingroup	UI
     * \brief	Processes object removal.
     */
    void removedObjectSignal(WoolzObject* obj);

    /*!
     * \ingroup	UI
     * \brief	Processes object selection in the combobox.
     */
    void objectChanged(int index);

    /*!
     * \ingroup	UI
     * \brief	Processes object name change in the combobox.
     */
    void nameChanged(const QString &str);

    /*!
     * \ingroup	UI
     * \brief	Processes external object property change.
     */
    void objectPropertyChanged();

    /*!
     * \ingroup	UI
     * \brief	Processes removal button event.
     */
    void removeObject();

    /*!
     * \ingroup	UI
     * \brief	Processes visibility checkbox change.
     */
    void visibleChanged(bool checked);

    public slots:
      /*!
       * \ingroup	UI
       * \brief	Processes external object selection.
       * \param	object			new selected object
       */
      void objectSelected(WoolzObject* object);

  private:
    /*!
     * \ingroup	UI
     * \brief	Fills widget with the values of the object.
     * \param	object			object to be loaded
     */
    void loadProperties(WoolzObject* object);

  protected:
    ObjectListModel* m_objectListModel;    /*!< the object list model */
    WoolzObject * m_object;                /*!< current object */
};

#endif  //OBJECTPROPERTYWIDGET
