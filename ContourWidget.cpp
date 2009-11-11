#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ContourWidget_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ContourWidget.cpp
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
* \brief        Widget for manipulating contour and ISO Woolz Object properties (see WoolzDynContourObj)
* \ingroup      UI
*
*/

//project includes
#include "ContourWidget.h"

#include "ObjectListModel.h"
#include "WoolzObject.h"
#include "WoolzDynThresholdedObj.h"

ContourWidget::ContourWidget(QWidget *parent,
   ObjectListModel *objectListModel): QDockWidget(parent),  
   m_object(NULL), m_sourceObject(NULL), m_objectListModel(objectListModel) {

  setupUi( this );
  setMinimumSize(150,0);
  setMaximumSize(500,200);
  resize(150,1);

  connect( comboSourceObject, SIGNAL(currentIndexChanged(int)),
           this, SLOT(sourceObjectChanged(int)));
  connect( horizontalSlider, SIGNAL(valueChanged(int)),
           this, SLOT(recomputeObject()));
  connect( checkBoxAutoUpdate, SIGNAL(clicked(bool)),
           this, SLOT(setAutoUpdate(bool)));
  connect( pushButtonUpdate, SIGNAL(clicked(bool)),
           this, SLOT(update()));
  connect( pushButtonNew, SIGNAL(clicked(bool)),
           this, SLOT(createNew()));
  connect( pushButtonBaseGo, SIGNAL(clicked(bool)),
           this, SLOT(goBaseGo()));
  connect( objectListModel, SIGNAL(removedObjectSignal(WoolzObject*)),
           this, SLOT(removedObjectSignal(WoolzObject*)));
  connect( objectListModel, SIGNAL(addObjectSignal(WoolzObject*)),
           this, SLOT(addObjectSignal(WoolzObject*)));
  connect( objectListModel, SIGNAL(objectSelected(WoolzObject*)),
           this, SLOT(objectSelected(WoolzObject*)));
}

void ContourWidget::createNew() {
  unsigned char threshold  = spinBoxThreshold->value();
  WoolzDynThresholdedObj *newTarget;
  newTarget = new WoolzDynThresholdedObj;

  Q_ASSERT(newTarget);
  if (newTarget) {
    if (m_sourceObject)
      newTarget->setName("Contour " + m_sourceObject->name());
    else
      newTarget->setName("Contour");
    newTarget->setParameters(lowThreshold, highThreshold);
    newTarget->setSourceObj(m_sourceObject);
    newTarget->changeWoolzObjectType(m_sourceObject->type());
    newTarget->generateNewColour();
    newTarget->setAutoUpdate(true);  ///// connect to button!!! & take current state
    newTarget->update(true);

    m_objectListModel->addObject(newTarget);
    m_object = newTarget;
  }
}

ContourWidget::~ContourWidget() {
}

void ContourWidget::recomputeObject() {
  if (!m_object)
    return ;
  unsigned char threshold  = spinBoxThreshold->value();
  unsigned char highThreshold = spinBoxHighThreshold->value();
  m_object->setParameters(lowThreshold, highThreshold);
  m_object->update();
}


void ContourWidget::objectSelected(WoolzObject* object) {
  WoolzDynThresholdedObj* obj =  qobject_cast<WoolzDynThresholdedObj*>(object);
  m_sourceObject = object;
  pushButtonNew->setEnabled(object!=NULL && !object->isWarped());

  if (obj!=NULL) {
    //disable autoupdate while properties are populated
    m_object =  NULL;  // do not update object
    horizontalThresholdSlider->setValue(obj->lowTh());
    checkBoxAutoUpdate->setChecked(obj->autoUpdate());
    comboSourceObject->clear();
    QList <WoolzObject*>  list = m_objectListModel->getObjects(true, true, true);
    for (int i=0; i<list.size(); i++)
      if (obj->isAllowedSource(list.at(i)))
        comboSourceObject->addItem(list.at(i)->name(),qVariantFromValue<QObject*>(list.at(i)));
    selectSource(obj->sourceObj());
  }
  m_object =  obj;
  bool visible = obj!=NULL;
  horizontalThresholdSlider->setVisible(visible);
  labelThreshold->setVisible(visible);
  spinBoxThreshold->setVisible(visible);
  labelBase->setVisible(visible);
  pushButtonUpdate->setVisible(visible);
  comboSourceObject->setVisible(visible);
  checkBoxAutoUpdate->setVisible(visible);
  pushButtonBaseGo->setVisible(visible);
  checkBoxAutoUpdate->setEnabled(visible && obj->sourceObj());
  groupBoxChannels->setVisible(visible && object->isColour() );

  resize(150,0);
}

void ContourWidget::removedObjectSignal(WoolzObject* obj) {
  int index = comboSourceObject->findData( qVariantFromValue<QObject*>(obj), Qt::UserRole, Qt::MatchExactly);
  if (index>=0) {
     WoolzDynThresholdedObj*  temp = m_object;
     m_object= NULL;
     comboSourceObject->removeItem(index); 
     m_object= temp ;
  }
  if (obj == m_object)
    objectSelected(NULL);
}

void ContourWidget::sourceObjectChanged(int index) {
  WoolzObject*  sourceObject =
      qobject_cast<WoolzObject*>(qVariantValue<QObject*>(comboSourceObject->itemData(index)));
  if (m_object) {
    if (!m_object->sourceObj() && sourceObject) {  // if no source was in the combo, remove it
       WoolzDynThresholdedObj*  temp = m_object;
       m_object= NULL;
       comboSourceObject->removeItem(0);
       m_object= temp ;
    }
    m_object->setSourceObj(sourceObject);
    m_object->changeWoolzObjectType(sourceObject->type());
    m_object->update();
  }
  checkBoxAutoUpdate->setEnabled(m_object != NULL && m_object->sourceObj());
}

void ContourWidget::addObjectSignal(WoolzObject* obj) {
  connect( obj, SIGNAL(objectPropertyChanged()), this, SLOT(objectPropertyChanged()));
  if (obj && obj->isValue() && obj != m_object)
    comboSourceObject->addItem(obj->name(),qVariantFromValue<QObject*>(obj));
}

void ContourWidget::objectPropertyChanged() {
  WoolzDynObject* obj = qobject_cast<WoolzDynObject*>(sender());
  if (!obj || obj != m_object)
    return;
  WoolzDynThresholdedObj*  temp = m_object;
  m_object= NULL;  //disable updates
  selectSource(obj->sourceObj());
  m_object= temp ;
}

void ContourWidget::setAutoUpdate(bool enabled) {
 if (m_object)
   m_object->setAutoUpdate(enabled);
}

void ContourWidget::update() {
 if (m_object)
   m_object->update(true);  // force update
}

void ContourWidget::goBaseGo() {
 if (m_object && m_object->sourceObj())
    m_objectListModel->setSelectObject(m_object->sourceObj());
}

void ContourWidget::selectSource(WoolzObject * source) {
    if (source) {
      int index= comboSourceObject->findData ( qVariantFromValue<QObject*>(source),
                                     Qt::UserRole, Qt::MatchExactly);
      if (index >= 0)
        comboSourceObject->setCurrentIndex(index);
      else
        Q_ASSERT(false);
    } else {
      comboSourceObject->insertItem(0, tr("*removed*"), qVariantFromValue<QObject*>(NULL));
      comboSourceObject->setCurrentIndex(0);
    }
}

//QSize ContourWidget::sizeHint ()  const { return QSize(150,1);}
