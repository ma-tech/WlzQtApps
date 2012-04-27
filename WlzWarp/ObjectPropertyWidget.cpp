#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ObjectPropertyWidget_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ObjectPropertyWidget.cpp
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

//project includes
#include "ObjectPropertyWidget.h"
#include "ObjectListModel.h"
#include "WoolzObject.h"
#include "WoolzDynThresholdedObj.h"
#include "Commands.h"

ObjectPropertyWidget::ObjectPropertyWidget(QWidget *parent,
   ObjectListModel *objectListModel): QDockWidget(parent), m_objectListModel(objectListModel) {
  setupUi( this );
  setMinimumSize(350,0);
  setMaximumSize(500,200);
  resize(350,1);
  colorpickerObject->setStandardColors();

  connect( colorpickerObject, SIGNAL(colorChanged(const QColor &)),
           this, SLOT(colourChanged()));
  connect( comboObjectName, SIGNAL(editTextChanged (const QString &)),
           this, SLOT(nameChanged(const QString &)));
  connect( comboObjectName, SIGNAL(currentIndexChanged(int)),
           this, SLOT(objectChanged(int)));
  connect( objectListModel, SIGNAL(addObjectSignal(WoolzObject*)),
           this, SLOT(addObjectSignal(WoolzObject*)));
  connect( objectListModel, SIGNAL(removedObjectSignal(WoolzObject*)),
           this, SLOT(removedObjectSignal(WoolzObject*)));
  connect( objectListModel, SIGNAL(objectSelected(WoolzObject*)),
           this, SLOT(objectSelected(WoolzObject*)));
  connect(pushButtonRemove, SIGNAL(clicked(bool)),
           this, SLOT(removeObject()));
  connect(checkBoxVisible, SIGNAL(clicked(bool)),
           this, SLOT(visibleChanged(bool)));
  m_object = NULL;
}

ObjectPropertyWidget::~ObjectPropertyWidget() {
}

void ObjectPropertyWidget::colourChanged() {
  if (m_object)
     m_objectListModel->addCommand(new ColourObject(m_object, colorpickerObject->currentColor()));
}

void ObjectPropertyWidget::addObjectSignal(WoolzObject* obj) {
  comboObjectName->addItem(obj->name(),qVariantFromValue<QObject*>(obj));

  int index = comboObjectName->findData ( qVariantFromValue<QObject*>(obj), Qt::UserRole, Qt::MatchExactly);

  if (m_object)
    disconnect( m_object , 0, this, 0);

  if (index>=0) {
     m_object = NULL;
     comboObjectName->setCurrentIndex(index);
     loadProperties(obj);
  }
  connect( obj, SIGNAL(objectPropertyChanged()), this, SLOT(objectPropertyChanged()));
  connect( obj, SIGNAL(objectVisibilityChanged()), this, SLOT(objectPropertyChanged())); 
}

void ObjectPropertyWidget::removedObjectSignal(WoolzObject* obj) {
  int index = comboObjectName->findData ( qVariantFromValue<QObject*>(obj), Qt::UserRole, Qt::MatchExactly);
  if (index>=0) 
     comboObjectName->removeItem(index);
  disconnect( obj, 0, this, 0);

  index = comboObjectName->currentIndex();
  loadProperties(qobject_cast<WoolzObject*>(qVariantValue<QObject*>(comboObjectName->itemData(index))));
}

void ObjectPropertyWidget::objectChanged(int index) {
  if (index<0) 
    return;
  if (!qVariantCanConvert<QObject*>(comboObjectName->itemData(index)))
    return;
  WoolzObject * obj = qobject_cast<WoolzObject*>(qVariantValue<QObject*>(comboObjectName->itemData(index)));
  if (!obj)
    return;
  if (m_object)
    disconnect( m_object , 0, this, 0);
  loadProperties(obj);
  m_objectListModel->setSelectObject(obj);
  connect( obj, SIGNAL(objectPropertyChanged()), this, SLOT(objectPropertyChanged()));
  connect( obj, SIGNAL(objectVisibilityChanged()), this, SLOT(objectPropertyChanged()));
}

void ObjectPropertyWidget::objectPropertyChanged() {
  WoolzObject* obj = qobject_cast<WoolzObject*>(sender());
  if (!obj)
    return;
  int index = comboObjectName->findData ( qVariantFromValue<QObject*>(obj), Qt::UserRole, Qt::MatchExactly);
  if (comboObjectName->itemText(index) != obj->name()) {
      WoolzObject * temp = m_object;
      m_object = NULL;
      comboObjectName->setItemText(index, obj->name());
      m_object = temp;
  }
  loadProperties(obj);
}

void ObjectPropertyWidget::loadProperties(WoolzObject *object) {
  m_object = NULL;  // disbale property update
  if (!object)
    return;
  colorpickerObject->setCurrentColor(object->qColour());
  pushButtonRemove->setVisible(object->removable());
  checkBoxVisible->setChecked(object->visible());
  m_object = object;
}


void ObjectPropertyWidget::nameChanged(const QString &str) {
  if (!m_object)
    return;
  int index = comboObjectName->currentIndex();
  if (index<0) 
    return;
  if (!qVariantCanConvert<QObject*>(comboObjectName->itemData(index)))
    return;
  WoolzObject* obj = qobject_cast<WoolzObject*>(qVariantValue<QObject*>(comboObjectName->itemData(index)));

  if (obj && obj==m_object) {
       m_objectListModel->addCommand(new RenameObject(obj, str));
  }
}

void ObjectPropertyWidget::objectSelected(WoolzObject* object) {
  if (m_object!= object) {
    int index = comboObjectName->findData ( qVariantFromValue<QObject*>(object), Qt::UserRole, Qt::MatchExactly);
    if (index>=0) {
      m_object = NULL;
      comboObjectName->setCurrentIndex(index);
      loadProperties(object);
    }
  }
}

void ObjectPropertyWidget::removeObject() {
  if (m_object)
       m_objectListModel->addCommand(new DeleteWoolzObject(m_objectListModel, m_object));

}

void ObjectPropertyWidget::visibleChanged(bool checked) {
  if (m_object)
       m_objectListModel->addCommand(new WoolzObjectChangeVisibility(m_object, checked));
}
