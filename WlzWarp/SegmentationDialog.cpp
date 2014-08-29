#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _SegmentationDialog_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         SegmentationDialog.cpp
* \author       Zsolt Husz
* \date         October 2008
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
* \brief        Segmented object generation dialog
* \ingroup      UI
*/

//Qt inlcudes

//#include <QList>
#include "SegmentationWidget.h"

#include "ObjectListModel.h"
#include "LandmarkModel.h"
#include "WoolzObject.h"
#include "WoolzDynThresholdedObj.h"
//#include "WarperConfig.h"

//own includes
#include "MainWindow.h"

class LandmarkModel;

SegmentationWidget::
SegmentationWidget(
  QWidget *parent,
  ObjectListModel *objectListModel):
QDockWidget(parent),
m_object(NULL),
m_sourceObject(NULL),
m_objectListModel(objectListModel)
{
  setupUi(this);

  setMinimumSize(150, 0);
  setMaximumSize(500, 200);
  resize(150, 1);

  connect(comboSourceObject, SIGNAL(currentIndexChanged(int)),
          this, SLOT(sourceObjectChanged(int)));

  connect(pushButtonNew, SIGNAL(clicked(bool)),
          this, SLOT(createNew()));
  connect(pushButtonBaseGo, SIGNAL(clicked(bool)),
          this, SLOT(goBaseGo()));


  connect(horizontalHighSlider, SIGNAL(valueChanged(int)),
          this, SLOT(recomputeObject()));
  connect(horizontalLowSlider, SIGNAL(valueChanged(int)),
          this, SLOT(recomputeObject()));

  connect(radioGrey, SIGNAL(clicked(bool)),
          this, SLOT(recomputeObject()));
  connect(radioRed, SIGNAL(clicked(bool)),
          this, SLOT(recomputeObject()));
  connect(radioGreen, SIGNAL(clicked(bool)),
          this, SLOT(recomputeObject()));
  connect(radioBlue, SIGNAL(clicked(bool)),
          this, SLOT(recomputeObject()));
  connect(checkBoxAutoUpdate, SIGNAL(clicked(bool)),
          this, SLOT(setAutoUpdate(bool)));
  connect(pushButtonUpdate, SIGNAL(clicked(bool)),
          this, SLOT(update()));

  connect(objectListModel, SIGNAL(removedObjectSignal(WoolzObject*)),
          this, SLOT(removedObjectSignal(WoolzObject*)));
  connect(objectListModel, SIGNAL(addObjectSignal(WoolzObject*)),
          this, SLOT(addObjectSignal(WoolzObject*)));
  connect(objectListModel, SIGNAL(objectSelected(WoolzObject*)),
          this, SLOT(objectSelected(WoolzObject*)));

}

QSize SegmentationWidget::
sizeHint()  const
{
  return(QSize(150, 1));
}


void SegmentationWidget::
createNew()
{
  unsigned char lowThreshold  = spinBoxLowThreshold->value();
  unsigned char highThreshold = spinBoxHighThreshold->value();
  WoolzDynThresholdedObj *newTarget;
  newTarget = new WoolzDynThresholdedObj;

  Q_ASSERT(newTarget);

  if(newTarget)
  {
    if(m_sourceObject)
    {
      newTarget->setName("Segmented " + m_sourceObject->name());
    }
    else
    {
      newTarget->setName("Segmented");
    }
    newTarget->setParameters(lowThreshold, highThreshold);
    newTarget->setSourceObj(m_sourceObject);
    newTarget->changeWoolzObjectType(m_sourceObject->type());
    newTarget->generateNewColour();
    newTarget->setAutoUpdate(true);  // connect to button & take current state
    newTarget->update(true);

    m_objectListModel->addObject(newTarget);
    m_object = newTarget;
  }
}

SegmentationWidget::
~SegmentationWidget()
{
}

void SegmentationWidget::recomputeObject()
{

  if(!m_object)
    return ;

  unsigned char lowThreshold  = spinBoxLowThreshold->value();
  unsigned char highThreshold = spinBoxHighThreshold->value();
  m_object->setParameters(lowThreshold, highThreshold);
  if(radioRed->isChecked())
  {
     m_object->setChannel(WoolzDynThresholdedObj::Red);
  }
  if(radioGreen->isChecked())
  {
    m_object->setChannel(WoolzDynThresholdedObj::Green);
  }
  if(radioBlue->isChecked())
  {
    m_object->setChannel(WoolzDynThresholdedObj::Blue);
  }
  if(radioGrey->isChecked())
  {
    m_object->setChannel(WoolzDynThresholdedObj::Grey);
  }
  m_object->update();
}


void SegmentationWidget::
objectSelected(
  WoolzObject* object)
{
  WoolzDynThresholdedObj* obj = qobject_cast<WoolzDynThresholdedObj*>(object);
  m_sourceObject = object;

  pushButtonNew->setEnabled(object != NULL && !object->isWarped() &&
                            object->isValue());

  if(obj != NULL)
  {
    //disable autoupdate while properties are populated
    m_object =  NULL;  // do not update object
    horizontalLowSlider->setValue(obj->lowTh());
    horizontalHighSlider->setValue(obj->highTh());
    checkBoxAutoUpdate->setChecked(obj->autoUpdate());

    comboSourceObject->clear();

    QList <WoolzObject*>  list = 
        m_objectListModel->getObjects(true, true, true);
    for(int i = 0; i < list.size(); i++)
    {
      if(obj->isAllowedSource(list.at(i)))
      {
        comboSourceObject->addItem(list.at(i)->name(),
	                           qVariantFromValue<QObject*>(list.at(i)));
      }
    }
    setSource(obj->sourceObj());
  }

  m_object =  obj;
  bool visible = obj != NULL;

  horizontalHighSlider->setVisible(visible);
  spinBoxHighThreshold->setVisible(visible);
  labelHighThreshold->setVisible(visible);

  horizontalLowSlider->setVisible(visible);
  spinBoxLowThreshold->setVisible(visible);
  labelLowThreshold->setVisible(visible);
  labelBase->setVisible(visible);
  pushButtonUpdate->setVisible(visible);
  comboSourceObject->setVisible(visible);
  checkBoxAutoUpdate->setVisible(visible);
  pushButtonBaseGo->setVisible(visible);
  checkBoxAutoUpdate->setEnabled(visible && obj->sourceObj());
  groupBoxChannels->setVisible(visible && object->isColour());

  resize(150, 0);
}

void SegmentationWidget::
removedObjectSignal(
  WoolzObject* obj)
{
  int index = comboSourceObject->findData(
      qVariantFromValue<QObject*>(obj), Qt::UserRole, Qt::MatchExactly);

  if(index >= 0)
  {
    WoolzDynThresholdedObj*  temp = m_object;
    m_object = NULL;
    comboSourceObject->removeItem(index);
    m_object = temp ;
  }
  if(obj == m_object)
  {
    objectSelected(NULL);
  }
}

void SegmentationWidget::
sourceObjectChanged(
  int index)
{
  WoolzObject*  sourceObject = qobject_cast<WoolzObject*>(
      qVariantValue<QObject*>(comboSourceObject->itemData(index)));

  if(m_object)
  {
    if(!m_object->sourceObj() && sourceObject)
    {
      // if no source was in the combo, remove it
      WoolzDynThresholdedObj*  temp = m_object;
      m_object = NULL;
      comboSourceObject->removeItem(0);
      m_object = temp ;
    }
    m_object->setSourceObj(sourceObject);
    m_object->changeWoolzObjectType(sourceObject->type());
    m_object->update();
  }
  checkBoxAutoUpdate->setEnabled(m_object != NULL && m_object->sourceObj());
}

void SegmentationWidget::addObjectSignal(WoolzObject* obj)
{
  connect(obj, SIGNAL(objectPropertyChanged()),
          this, SLOT(objectPropertyChanged()));

  if(obj && obj->isValue() && obj != m_object)
  {
    comboSourceObject->addItem(obj->name(), qVariantFromValue<QObject*>(obj));
  }
}

void SegmentationWidget::
objectPropertyChanged()
{
  WoolzDynObject* obj = qobject_cast<WoolzDynObject*>(sender());
  if(!obj || obj != m_object)
  {
    return;
  }

  WoolzDynThresholdedObj*  temp = m_object;
  m_object = NULL;		 // disable updates
  setSource(obj->sourceObj());
  m_object = temp ;
}

void SegmentationWidget::
setAutoUpdate(
  bool enabled)
{
  if(m_object)
  {
    m_object->setAutoUpdate(enabled);
  }
}

void SegmentationWidget::
  update()
{
  if(m_object)
  {
    m_object->update(true);  // force update
  }
}

void SegmentationWidget::
goBaseGo()
{
  if(m_object && m_object->sourceObj())
  {
    m_objectListModel->setSelectObject(m_object->sourceObj());
  }
}

void SegmentationWidget::
setSource(
  WoolzObject * source)
{
  if(source)
  {
    int index = comboSourceObject->findData(
	qVariantFromValue<QObject*>(source),
        Qt::UserRole, Qt::MatchExactly);
    if(index >= 0)
    {
      comboSourceObject->setCurrentIndex(index);
    }
    else
    {
      Q_ASSERT(false);
    }
  }
  else
  {
    comboSourceObject->insertItem(0, tr("*removed*"),
	                          qVariantFromValue<QObject*>(NULL));
    comboSourceObject->setCurrentIndex(0);
  }
}
