#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _SegmentationWidget_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         SegmentationWidget.cpp
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

//project includes
#include "SegmentationWidget.h"
#include "ObjectListModel.h"
#include "WoolzObject.h"
#include "WoolzDynThresholdedObj.h"
#include "Commands.h"

SegmentationWidget::SegmentationWidget(QWidget *parent,
   ObjectListModel *objectListModel): QDockWidget(parent), m_objectListModel(objectListModel),
   m_object(NULL), m_sourceObject(NULL)  {

  setupUi( this );
  setMinimumSize(300,0);
  setMaximumSize(500,400);
  resize(300,1);

  connect( comboSourceObject, SIGNAL(currentIndexChanged(int)),
           this, SLOT(sourceObjectChanged(int)));
  connect( horizontalHighSlider, SIGNAL(valueChanged(int)),
           this, SLOT(highThresholdChanged()));
  connect( horizontalLowSlider, SIGNAL(valueChanged(int)),
           this, SLOT(lowThresholdChanged()));
  connect( radioGrey, SIGNAL(clicked(bool)),
           this, SLOT(channelChanged()));
  connect( radioRed, SIGNAL(clicked(bool)),
           this, SLOT(channelChanged()));
  connect( radioGreen, SIGNAL(clicked(bool)),
           this, SLOT(channelChanged()));
  connect( radioBlue, SIGNAL(clicked(bool)),
           this, SLOT(channelChanged()));
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

void SegmentationWidget::createNew() {
  unsigned char lowThreshold  = spinBoxLowThreshold->value();
  unsigned char highThreshold = spinBoxHighThreshold->value();
  WoolzDynThresholdedObj *newTarget;
  newTarget = new WoolzDynThresholdedObj(m_objectListModel);
  Q_ASSERT(newTarget);
  if (newTarget) {
    if (m_sourceObject)
      newTarget->setName("Segmented " + m_sourceObject->name());
    else
      newTarget->setName("Segmented");
    //newTarget->setParameters(lowThreshold, highThreshold);
    newTarget->setLowThreshold(lowThreshold);
    newTarget->setHighThreshold(highThreshold);
    newTarget->setSourceObj(m_sourceObject);
    newTarget->changeWoolzObjectType(m_sourceObject->type());
    newTarget->generateNewColour();
    newTarget->setAutoUpdate(true);
    m_objectListModel->addCommand(new CreateWoolzObject(m_objectListModel, newTarget));
    m_object = newTarget;
  }
}

SegmentationWidget::~SegmentationWidget() {
}

void SegmentationWidget::channelChanged() {
  if (!m_object)
    return ;
  WoolzDynThresholdedObj::channelTypes channel = WoolzDynThresholdedObj::Grey;
  if (radioRed->isChecked())  channel = WoolzDynThresholdedObj::Red;
  if (radioGreen->isChecked()) channel = WoolzDynThresholdedObj::Green;
  if (radioBlue->isChecked()) channel = WoolzDynThresholdedObj::Blue;
  if (m_object->channel()!= channel)
      m_objectListModel->addCommand(new WoolzDynThresholdedObjSetChannel(m_object, channel));
}

void SegmentationWidget::lowThresholdChanged() {
  if (!m_object)
    return ;
  unsigned char lowThreshold  = spinBoxLowThreshold->value();
  m_objectListModel->addCommand(new SetLowThreshold(m_object, lowThreshold));
}

void SegmentationWidget::highThresholdChanged() {
  if (!m_object)
    return ;
  unsigned char highThreshold  = spinBoxHighThreshold->value();
  m_objectListModel->addCommand(new SetHighThreshold(m_object, highThreshold));
}

void SegmentationWidget::loadProperties(WoolzDynThresholdedObj *object) {
  m_object = NULL;  // disbale property update
  if (!object)
    return;
  horizontalLowSlider->setValue(object->lowTh());
  horizontalHighSlider->setValue(object->highTh());
  checkBoxAutoUpdate->setChecked(object->autoUpdate());
  selectSource(object->sourceObj());
  m_object = object;
}

void SegmentationWidget::objectPropertyChanged() {
  WoolzDynThresholdedObj* obj = qobject_cast<WoolzDynThresholdedObj*>(sender());
  if (!obj)
    return;
  loadProperties(obj);
}

void SegmentationWidget::objectSelected(WoolzObject* object) {
  if (!object)
      return;

  WoolzDynThresholdedObj* obj =  qobject_cast<WoolzDynThresholdedObj*>(object);
  m_sourceObject = object;
  pushButtonNew->setEnabled(object!=NULL && !object->isWarped() && object->isValuePreRead());
  if (m_object)
    disconnect( m_object , 0, this, 0);
  m_object = NULL;
  if (obj!=NULL) {
    //disable autoupdate while properties are populated
    connect( obj, SIGNAL(objectPropertyChanged()),
           this, SLOT(objectPropertyChanged()));
    comboSourceObject->clear();
    QList <WoolzObject*>  list = m_objectListModel->getObjects(true, true, true);
    for (int i=0; i<list.size(); i++)
      if (obj->isAllowedSource(list.at(i)))
        comboSourceObject->addItem(list.at(i)->name(),qVariantFromValue<QObject*>(list.at(i)));
    loadProperties(obj);
  }
  bool visible=object!=NULL && !object->isWarpedPreRead() && object->isValuePreRead();
  horizontalHighSlider->setEnabled(visible);
  spinBoxHighThreshold->setEnabled(visible);
  labelHighThreshold->setEnabled(visible);
  horizontalLowSlider->setEnabled(visible);
  spinBoxLowThreshold->setEnabled(visible);
  labelLowThreshold->setEnabled(visible);
  checkBoxAutoUpdate->setEnabled(visible);

  groupBoxChannels->setEnabled(visible);
  if (object)
    groupBoxChannels->setVisible(object->isColourPreRead() );
  else
    groupBoxChannels->setVisible(true );
  pushButtonUpdate->setVisible(visible && obj);
  comboSourceObject->setVisible(visible  && obj);
  labelBase->setVisible(visible && obj);
  labelNotCreated->setVisible(!(visible && obj));
  pushButtonBaseGo->setVisible(visible && obj);
  checkBoxAutoUpdate->setEnabled(visible && (!obj || obj->sourceObj()));
  resize(300,0);
}

void SegmentationWidget::removedObjectSignal(WoolzObject* obj) {
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

void SegmentationWidget::sourceObjectChanged(int index) {
  WoolzObject*  sourceObject =
      qobject_cast<WoolzObject*>(qVariantValue<QObject*>(comboSourceObject->itemData(index)));
  if (m_object) {
    if (!m_object->sourceObj() && sourceObject) {  // if no source was in the combo, remove it
       WoolzDynThresholdedObj*  temp = m_object;
       m_object= NULL;
       comboSourceObject->removeItem(0);
       m_object= temp ;
    }
    m_objectListModel->addCommand(new WoolzDynObjectSetSourceObj(m_object, sourceObject));
  }
  checkBoxAutoUpdate->setEnabled((sourceObject) || (m_object != NULL && m_object->sourceObj()));
}

void SegmentationWidget::addObjectSignal(WoolzObject* obj) {
  connect( obj, SIGNAL(objectPropertyChanged()), this, SLOT(objectPropertyChanged()));
  if (obj && obj->isValue() && obj != m_object)
    comboSourceObject->addItem(obj->name(),qVariantFromValue<QObject*>(obj));
}

void SegmentationWidget::setAutoUpdate(bool enabled) {
 if (m_object)
    m_objectListModel->addCommand(new WoolzDynObjectSetAutoUpdate(m_object, enabled));
 }

void SegmentationWidget::update() {
  if (m_object) {  // force update
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_object->update(true);
    QApplication::restoreOverrideCursor();
  }
}

void SegmentationWidget::goBaseGo() {
 if (m_object && m_object->sourceObj())
    m_objectListModel->setSelectObject(m_object->sourceObj());
}

void SegmentationWidget::selectSource(WoolzObject * source) {
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
