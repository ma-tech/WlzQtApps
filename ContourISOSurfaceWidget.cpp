#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ContourISOSurfaceWidget_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ContourISOSurfaceWidget.cpp
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
* \brief        Widget for manipulating segmented Woolz Object properties (see WoolzDynContourISO)
* \ingroup      UI
*
*/

//project includes
#include "ContourISOSurfaceWidget.h"
#include "ObjectListModel.h"
#include "WoolzObject.h"
#include "WoolzDynContourISO.h"

#include "Commands.h"

ContourISOSurfaceWidget::ContourISOSurfaceWidget(QWidget *parent,
   ObjectListModel *objectListModel): QDockWidget(parent), m_objectListModel(objectListModel),
   m_object(NULL), m_sourceObject(NULL)  {

  setupUi( this );
  setMinimumSize(300,0);
  setMaximumSize(500,500);
  resize(150,1);

  connect( comboSourceObject, SIGNAL(currentIndexChanged(int)),
           this, SLOT(sourceObjectChanged(int)));
  connect( horizontalValueSlider, SIGNAL(valueChanged(int)),
           this, SLOT(valueChanged()));
  connect( horizontalSubsample, SIGNAL(valueChanged(int)),
           this, SLOT(subSampleChanged()));

  connect( checkBoxAutoUpdate, SIGNAL(clicked(bool)),
           this, SLOT(setAutoUpdate(bool)));
  connect( checkBorder, SIGNAL(clicked(bool)),
           this, SLOT(setBorder(bool)));
  connect( checkFilter, SIGNAL(clicked(bool)),
           this, SLOT(setFilter(bool)));
  connect( checkBoxAdvanced, SIGNAL(clicked(bool)),
           this, SLOT(setAdvanced(bool)));

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

  shells[0]=checkBoxShell1;
  shells[1]=checkBoxShell2;
  shells[2]=checkBoxShell3;
  shells[3]=checkBoxShell4;
  shells[4]=checkBoxShell5;
  shells[5]=checkBoxShell6;
  shells[6]=checkBoxShell7;
  shells[7]=checkBoxShell8;
  shells[8]=checkBoxShell9;
  shells[9]=checkBoxShell10;
  shells[10]=checkBoxShell11;
  shells[11]=checkBoxShell12;
  shells[12]=checkBoxShell13;
  shells[13]=checkBoxShell14;
  shells[14]=checkBoxShell15;
  shells[15]=checkBoxShell16;
  for (int i=0; i<16; i++)
    connect( shells[i], SIGNAL(clicked(bool)),
             this, SLOT(setShellSelection()));
  checkBoxAdvanced->setChecked(false);
}

void ContourISOSurfaceWidget::createNew() {
  unsigned char value  = spinBoxValue->value();
  WoolzDynContourISO *newTarget;
  newTarget = new WoolzDynContourISO(m_objectListModel);
  bool border  = checkBorder->isChecked();

  Q_ASSERT(newTarget);
  if (newTarget) {
    if (m_sourceObject)
      newTarget->setName("Contour " + m_sourceObject->name());
    else
      newTarget->setName("Contour ");
    newTarget->setValue(value);
    newTarget->setSourceObj(m_sourceObject);
    newTarget->changeWoolzObjectType(m_sourceObject->type());
    newTarget->generateNewColour();
    newTarget->setBoundary(border);
    newTarget->setAutoUpdate(true);
    m_objectListModel->addCommand(new CreateWoolzObject(m_objectListModel, newTarget));
    m_object = newTarget;
  }
}

ContourISOSurfaceWidget::~ContourISOSurfaceWidget() {
}

void ContourISOSurfaceWidget::valueChanged() {
  if (!m_object)
    return ;
  unsigned char value  = spinBoxValue->value();
  m_objectListModel->addCommand(new WoolzDynISOObjSetValue(m_object, value));
}

void ContourISOSurfaceWidget::subSampleChanged() {
  if (!m_object)
    return ;
  unsigned char subsample  = spinBoxSubsample->value();
  m_objectListModel->addCommand(new WoolzDynISOObjSetSubsample(m_object, subsample));
}

void ContourISOSurfaceWidget::loadProperties(WoolzDynContourISO *object) {
  m_object = NULL;  // disbale property update
  if (!object)
    return;

  checkBoxAutoUpdate->setChecked(object->autoUpdate());
  selectSource(object->sourceObj());

  checkBorder->setChecked(object->boundary());
  setBorder(object->boundary());

  int selection = object->selection();
  int maxShells = object->shellno();
  int i;
  for (i=0; i<16; i++) {
       shells[i]->setVisible(i<maxShells);
   }

  // sets all checkboxes, except the last
  maxShells--;
  i = maxShells;
  if (i>16) i=16;
  for (; i>0;) {
       i--;
       shells[i]->setChecked((selection & (1<<i)) !=0 );
       shells[i]->setText(QString("Shell %1").arg(i+1));
  }

  /* the last check box is for the remaining shells  >= 16
     if only 16 shells are, then show "Shell 16" else, "Other shells"*/
  if (maxShells>=0) {
    if (maxShells < 16) {
      shells[maxShells]->setText(QString("Shell %1").arg(maxShells+1));
      shells[maxShells]->setChecked((selection & (1<<maxShells)) !=0 );
  } else {
      shells[15]->setText(QString("Other shells"));
      shells[15]->setChecked((selection & (1<<15)) !=0 );
  }
    for (i=maxShells+1; i<16; i++)
       shells[i]->setChecked(true);

}
  m_object = object;
}

void ContourISOSurfaceWidget::objectPropertyChanged() {
  WoolzDynContourISO* obj = qobject_cast<WoolzDynContourISO*>(sender());
  if (!obj)
    return;

  loadProperties(obj);
}

void ContourISOSurfaceWidget::objectSelected(WoolzObject* object) {
  if (!object)
      return;

  WoolzDynContourISO* obj =  qobject_cast<WoolzDynContourISO*>(object);
  m_sourceObject = object;
  pushButtonNew->setEnabled(object!=NULL && !object->isWarpedPreRead() && object->isValuePreRead());

  if (m_object)
    disconnect( m_object , 0, this, 0);

  m_object = NULL;

  bool visible=object!=NULL && (object->isValue() || object->isContourPreRead());
  horizontalValueSlider->setEnabled(visible && !checkBorder->isChecked());
  spinBoxValue->setEnabled(visible && !checkBorder->isChecked());
  labelValue->setEnabled(visible && !checkBorder->isChecked());
  horizontalSubsample->setEnabled(visible);
  spinBoxSubsample->setEnabled(visible);
  labelSubsample->setEnabled(visible);
  checkBorder->setEnabled(visible);
  groupBoxShells->setVisible(checkBoxAdvanced->isChecked());
  groupBoxShells->setEnabled(visible);
  checkFilter->setEnabled(visible);
  checkFilter->setVisible((visible && object->is3DPreRead()));

  checkBoxAdvanced->setEnabled(visible  && obj);
  if (!obj)
     checkBoxAdvanced->setChecked(false);

  pushButtonUpdate->setVisible(visible && obj);
  comboSourceObject->setVisible(visible  && obj);
  labelBase->setVisible(visible && obj);
  labelNotCreated->setVisible(!(visible && obj));
  pushButtonBaseGo->setVisible(visible && obj);

  checkBoxAutoUpdate->setEnabled(visible && (!obj || obj->sourceObj()));

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

  resize(300,0);
}

void ContourISOSurfaceWidget::removedObjectSignal(WoolzObject* obj) {
  int index = comboSourceObject->findData( qVariantFromValue<QObject*>(obj), Qt::UserRole, Qt::MatchExactly);
  if (index>=0) {
     WoolzDynContourISO*  temp = m_object;
     m_object= NULL;
     comboSourceObject->removeItem(index); 
     m_object= temp ;
  }
  if (obj == m_object)
    objectSelected(NULL);
}

void ContourISOSurfaceWidget::sourceObjectChanged(int index) {
  WoolzObject*  sourceObject =
      qobject_cast<WoolzObject*>(qVariantValue<QObject*>(comboSourceObject->itemData(index)));
  if (m_object) {
    if (!m_object->sourceObj() && sourceObject) {  // if no source was in the combo, remove it
       WoolzDynContourISO*  temp = m_object;
       m_object= NULL;
       comboSourceObject->removeItem(0);
       m_object= temp ;
    }
    m_objectListModel->addCommand(new WoolzDynObjectSetSourceObj(m_object, sourceObject));
  }
  checkBoxAutoUpdate->setEnabled((sourceObject) || (m_object != NULL && m_object->sourceObj()));
}

void ContourISOSurfaceWidget::addObjectSignal(WoolzObject* obj) {
  connect( obj, SIGNAL(objectPropertyChanged()), this, SLOT(objectPropertyChanged()));
  if (obj && obj->isValue() && obj != m_object)
    comboSourceObject->addItem(obj->name(),qVariantFromValue<QObject*>(obj));
}

void ContourISOSurfaceWidget::setAutoUpdate(bool enabled) {
 if (m_object)
    m_objectListModel->addCommand(new WoolzDynObjectSetAutoUpdate(m_object, enabled));
 }

void ContourISOSurfaceWidget::update() {
  if (m_object) {  // force update
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_object->update(true);
    QApplication::restoreOverrideCursor();
  }
}

void ContourISOSurfaceWidget::goBaseGo() {
 if (m_object && m_object->sourceObj())
    m_objectListModel->setSelectObject(m_object->sourceObj());
}

void ContourISOSurfaceWidget::selectSource(WoolzObject * source) {
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

void ContourISOSurfaceWidget::setBorder(bool enabled) {
horizontalValueSlider->setEnabled(!enabled);
spinBoxValue->setEnabled(!enabled);
labelValue->setEnabled(!enabled);
if (m_object)
    m_objectListModel->addCommand(new WoolzDynISOObjSetBoundary(m_object, enabled));
}

void ContourISOSurfaceWidget::setFilter(bool enabled) {
if (m_object)
    m_objectListModel->addCommand(new WoolzDynISOObjSetFilter(m_object, enabled));
}

void ContourISOSurfaceWidget::setAdvanced(bool enabled) {
  groupBoxShells->setVisible(m_object!=NULL && enabled);
  resize(150,0);
}

void ContourISOSurfaceWidget::setShellSelection () {
  if (m_object) {
    int i, selection = 0;
    int shellno=m_object->shellno();
    if (shellno>16)
        shellno=16;
    for (i=0; i<shellno; i++)
       if (shells[i]->isChecked())
          selection |= 1 << i;
      m_objectListModel->addCommand(new WoolzDynISOObjSetShellSelection(m_object, selection));
  }
}

