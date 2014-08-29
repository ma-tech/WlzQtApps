#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _DynamicObjectWidget_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         DynamicObjectWidget.cpp
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
#include "SegmentationDialog.h"

#include "ObjectListModel.h"
#include "LandmarkModel.h"
#include "WoolzObject.h"
#include "WoolzDynThresholdedObj.h"
//#include "WarperConfig.h"

//own includes
#include "MainWindow.h"

class LandmarkModel;

SegmentationDialog::
SegmentationDialog(
  QWidget *parent,
  ObjectListModel *objectListModel):
QDockWidget(parent),
m_targetObject(NULL),
m_targetWoolzObject(NULL),
m_objectListModel(objectListModel) 
{

  setupUi( this );

  setMinimumSize(150,0);
  setMaximumSize(500,200);
  resize(150,1);

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

QSize SegmentationDialog::
sizeHint() const
{
  return(QSize(150,1));
}

void SegmentationDialog::
createNew()
{
  unsigned char lowThreshold  = spinBoxLowThreshold->value();
  unsigned char highThreshold = spinBoxHighThreshold->value();
  WoolzDynThresholdedObj *newTarget;
  newTarget = new WoolzDynThresholdedObj;
  Q_ASSERT(newTarget);

  if(newTarget)
  {
    if(m_targetWoolzObject)
    { 
      newTarget->setName("Segmented " + m_targetWoolzObject->name());
    }
    else
    {
      newTarget->setName("Segmented");
    }
    newTarget->setParameters(lowThreshold, highThreshold);
    newTarget->setSourceObj(m_targetWoolzObject);
    newTarget->generateNewColour();
    newTarget->setAutoUpdate(true);  // connect to button & take current state

    newTarget->update(true);
    m_objectListModel->addObject(newTarget);
    m_targetObject = newTarget;
  }
}

SegmentationDialog::
~SegmentationDialog()
{
}

void SegmentationDialog::
recomputeObject()
{
  if(!m_targetObject)
  {
    return;
  }

  // do not upate if parameters are set from code and not by  the user
  unsigned char lowThreshold  = spinBoxLowThreshold->value();
  unsigned char highThreshold = spinBoxHighThreshold->value();
  m_targetObject->setParameters(lowThreshold, highThreshold);
  if(radioRed->isChecked()) 
  {
    m_targetObject->setChannel(WoolzDynThresholdedObj::Red);
  }
  if(radioGreen->isChecked()) 
  {
    m_targetObject->setChannel(WoolzDynThresholdedObj::Green);
  }
  if(radioBlue->isChecked()) 
  {
    m_targetObject->setChannel(WoolzDynThresholdedObj::Blue);
  }
  if(radioGrey->isChecked()) 
  {
    m_targetObject->setChannel(WoolzDynThresholdedObj::Grey);
  }
  m_targetObject->update();
}

void SegmentationDialog::
objectSelected(
  WoolzObject* object)
{
  WoolzDynThresholdedObj * obj = qobject_cast<WoolzDynThresholdedObj*>(object);
  m_targetWoolzObject = object;

  pushButtonNew->setEnabled(object!=NULL && !object->isWarped());

  if(obj != NULL)
  {
    //disable autoupdate while properties are populated
    m_targetObject =  NULL;  // do not update object

    horizontalLowSlider->setValue(obj->lowTh());
    horizontalHighSlider->setValue(obj->highTh());

    comboSourceObject->clear();

    QList <WoolzObject*> list = m_objectListModel->getObjects(true, true, true);
    for(int i = 0; i < list.size(); i++)
    {
      comboSourceObject->addItem(list.at(i)->name(),
	  qVariantFromValue<QObject*>(list.at(i)));
    }

    comboSourceObject->removeItem(
	comboSourceObject->findData(qVariantFromValue<QObject*>(obj),
	Qt::UserRole, Qt::MatchExactly));

    if(obj->sourceObj())
    {
      int index = comboSourceObject->findData(
	  qVariantFromValue<QObject*>(obj->sourceObj()),
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
      comboSourceObject->insertItem(0, tr("Source removed"),
      				    qVariantFromValue<QObject*>(NULL));
      comboSourceObject->setCurrentIndex(0);
    }
  }

  m_targetObject =  obj;
  bool visible = obj!=NULL;

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
  groupBoxChannels->setVisible(visible && object->isColour() );

  resize(150,0);
}

void SegmentationDialog::
removedObjectSignal(
  WoolzObject* obj)
{
  int index = comboSourceObject->findData(qVariantFromValue<QObject*>(obj),
                                          Qt::UserRole, Qt::MatchExactly);
  if(index>=0)
  {
    comboSourceObject->removeItem(index);
  }
  if(obj == m_targetWoolzObject)
  {
    objectSelected(NULL);
  }
}

void SegmentationDialog::
sourceObjectChanged(
  int index)
{
  WoolzObject *  sourceObject = qobject_cast<WoolzObject*>(
      qVariantValue<QObject*>(comboSourceObject->itemData(index)));
  if(m_targetObject)
  {
    m_targetObject->setSourceObj(sourceObject);
    m_targetObject->update();
  }
}

void SegmentationDialog::
addObjectSignal(
  WoolzObject* obj)
{
  connect(obj, SIGNAL(objectPropertyChanged()),
          this, SLOT(objectPropertyChanged()));

  if(obj && obj->isValue() && obj != m_targetObject)
  {
    comboSourceObject->addItem(obj->name(),qVariantFromValue<QObject*>(obj));
  }
}

void SegmentationDialog::
objectPropertyChanged()
{
  WoolzObject* obj = qobject_cast<WoolzObject*>(sender());
  if(!obj)
  {
    return;
  }

  int index = comboSourceObject->findData(
      qVariantFromValue<QObject*>(obj), Qt::UserRole, Qt::MatchExactly);
  if(index<0)
  {
    return ;
  }
  if(comboSourceObject->itemText(index) != obj->name())
  {
     comboSourceObject->setItemText(index, obj->name());
  }
}

void SegmentationDialog::
setAutoUpdate(
  bool enabled)
{
  if(m_targetObject)
  {
    m_targetObject->setAutoUpdate(enabled);
  }
}

void SegmentationDialog::
update()
{
 if(m_targetObject)
 {
   m_targetObject->update(true);  // force update
 }
}

void SegmentationDialog::
goBaseGo()
{
 if(m_targetObject && m_targetObject->sourceObj())
 {
    m_objectListModel->setSelectObject(m_targetObject->sourceObj());
 }
}
