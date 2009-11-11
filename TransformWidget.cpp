#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _TransformWidget_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         TransformWidget.cpp
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

//project includes
#include "TransformWidget.h"
#include "ObjectListModel.h"
#include "WoolzObject.h"
#include "WoolzDynWarpedObject.h"
#include "Commands.h"

TransformWidget::TransformWidget(QWidget *parent,
   ObjectListModel *objectListModel, LandmarkModel *landmarkModel): 
       QDockWidget(parent), m_objectListModel(objectListModel), m_landmarkModel(landmarkModel),
       m_object(NULL), m_sourceObject(NULL) {
  setupUi( this );

  setMinimumSize(300,0);
  setMaximumSize(500,200);
  resize(300,1);

  connect( comboSourceObject, SIGNAL(currentIndexChanged(int)),
           this, SLOT(sourceObjectChanged(int)));
  connect( objectListModel, SIGNAL(removedObjectSignal(WoolzObject*)),
           this, SLOT(removedObjectSignal(WoolzObject*)));
  connect( objectListModel, SIGNAL(addObjectSignal(WoolzObject*)),
           this, SLOT(addObjectSignal(WoolzObject*)));
  connect( objectListModel, SIGNAL(objectSelected(WoolzObject*)),
           this, SLOT(objectSelected(WoolzObject*)));

  connect( pushButtonNew, SIGNAL(clicked(bool)),
           this, SLOT(createNew()));
  connect( pushButtonBaseGo, SIGNAL(clicked(bool)),
           this, SLOT(goBaseGo()));
  connect( pushButtonUpdate, SIGNAL(clicked(bool)),
           this, SLOT(update()));
  connect( checkBoxAutoUpdate, SIGNAL(clicked(bool)),
           this, SLOT(setAutoUpdate(bool)));
}

void TransformWidget::createNew() {
  WoolzDynWarpedObject *newTarget;
  newTarget = new WoolzDynWarpedObject(m_objectListModel, m_landmarkModel) ;

  Q_ASSERT(newTarget);
  if (newTarget) {
    newTarget->setSourceObj(m_sourceObject);

    /*double delta = doubleSpinBoxDelta->value();
    newTarget->setDelta(delta);
    bool useIMQ = radioButtonIMQ->isChecked();
    newTarget->setBasisFnType(useIMQ ? WoolzDynWarpedObject::basis_IMQ : WoolzDynWarpedObject::basis_MQ);*/
    if (m_sourceObject) 
        newTarget->setName("Warped " + m_sourceObject->name());
    newTarget->generateNewColour();
    newTarget->setAutoUpdate(true);
    m_objectListModel->addCommand(new CreateWoolzObject(m_objectListModel, newTarget));
    m_object = newTarget;
  }
}

TransformWidget::~TransformWidget() {
}

void TransformWidget::recomputeObject() {
  if (!m_object)
    return ;
  m_object->setSourceObj(m_sourceObject);
  m_object->update();
}

void TransformWidget::loadProperties(WoolzDynWarpedObject *object) {
  m_object = NULL;  // disbale property update
  if (!object)
    return;

  checkBoxAutoUpdate->setChecked(object->autoUpdate());
  selectSource(object->sourceObj());
  m_object = object;
}


void TransformWidget::objectSelected(WoolzObject* object) {
  if (!object)
      return;

  WoolzDynWarpedObject * obj =  qobject_cast<WoolzDynWarpedObject *>(object);

  m_sourceObject = object;

  pushButtonNew->setEnabled(object!=NULL && !object->isWarpedPreRead() && !object->isContourPreRead());

  if (m_object)
    disconnect( m_object , 0, this, 0);

  m_object =  NULL;  // do not update object
  if (obj!=NULL) {
    connect( obj, SIGNAL(objectPropertyChanged()), this, SLOT(objectPropertyChanged()));

    comboSourceObject->clear();
    QList <WoolzObject*>  list = m_objectListModel->getObjects(true, true, false);
    for (int i=0; i<list.size(); i++)
      if (obj->isAllowedSource(list.at(i)))
        comboSourceObject->addItem(list.at(i)->name(),qVariantFromValue<QObject*>(list.at(i)));
    loadProperties(obj);
  }

  //bool visible = obj!=NULL;
  bool visible=object!=NULL && (object->isValuePreRead() || object->isMeshPreRead());

  checkBoxAutoUpdate->setVisible(visible);

  pushButtonUpdate->setVisible(visible  && obj);
  labelBase->setVisible(visible && obj);
  comboSourceObject->setVisible(visible  && obj);
  pushButtonBaseGo->setVisible(visible && obj);

  labelNotCreated->setVisible(!(visible && obj) && object && (object->isValuePreRead() || object->isMeshPreRead()));  /// can't be contour!!!
  checkBoxAutoUpdate->setEnabled(visible && (!obj || obj->sourceObj()));

  resize(300,0);
}

void TransformWidget::removedObjectSignal(WoolzObject* obj) {
  int index = comboSourceObject->findData( qVariantFromValue<QObject*>(obj), Qt::UserRole, Qt::MatchExactly);
  if (index>=0) {
     WoolzDynWarpedObject *  temp = m_object;
     m_object= NULL;
     comboSourceObject->removeItem(index); 
     m_object= temp ;
  }
  if (obj == m_object) {
    objectSelected(NULL);
  }
}

void TransformWidget::sourceObjectChanged(int index) {
  WoolzObject*  sourceObject = 
      qobject_cast<WoolzObject*>(qVariantValue<QObject*>(comboSourceObject->itemData(index)));

  if (m_object) {
    if (!m_object->sourceObj() && sourceObject) {  // if no source was in the combo, remove it
       WoolzDynWarpedObject *  temp = m_object;
       m_object= NULL;
       comboSourceObject->removeItem(0);
       m_object= temp ;
    }
/*    m_object->setSourceObj(sourceObject);
    m_object->update();*/
    m_objectListModel->addCommand(new WoolzDynObjectSetSourceObj(m_object, sourceObject));
  }
  checkBoxAutoUpdate->setEnabled((sourceObject) || (m_object != NULL && m_object->sourceObj()));

}

void TransformWidget::addObjectSignal(WoolzObject* obj) {
  connect( obj, SIGNAL(objectPropertyChanged()), this, SLOT(objectPropertyChanged()));
  if (obj && obj->isValue() && obj != m_object)
    comboSourceObject->addItem(obj->name(),qVariantFromValue<QObject*>(obj));
}

void TransformWidget::objectPropertyChanged() {
  WoolzDynWarpedObject* obj = qobject_cast<WoolzDynWarpedObject*>(sender());
  if (!obj || obj != m_object)
    return;

  loadProperties(obj);
}


void TransformWidget::setAutoUpdate(bool enabled) {
 if (m_object)
    m_objectListModel->addCommand(new WoolzDynObjectSetAutoUpdate(m_object, enabled));
}

void TransformWidget::update() {
  if (m_object) {  // force update
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_object->update(true);
    QApplication::restoreOverrideCursor();
  }
}

void TransformWidget::goBaseGo() {
  if (m_object && m_object->sourceObj())
    m_objectListModel->setSelectObject(m_object->sourceObj());
}

void TransformWidget::selectSource(WoolzObject * source) {
  if (source ) {
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
