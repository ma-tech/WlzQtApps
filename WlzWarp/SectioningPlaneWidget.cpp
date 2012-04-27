#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _SectioningPlaneWidget_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         SectioningPlaneWidget.cpp
* \author       Zsolt Husz
* \date         March 2010
* \version      $Id$
* \par
* Address:
*               MRC Human Genetics Unit,
*               Western General Hospital,
*               Edinburgh, EH4 2XU, UK.
* \par
* Copyright (C) 2010 Medical research Council, UK.
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
* \brief        Widget for manipulating cutting plane in viewers
* \ingroup      UI
*
*/

//project includes
#include "SectioningPlaneWidget.h"
#include "ObjectViewerController.h"
#include "ObjectViewerController.h"
#include "ObjectViewer.h"
#include "ObjectViewerModel.h"
#include "Commands.h"

//inventor includes
#include <Inventor/SbLinear.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/manips/SoClipPlaneManip.h>

SectioningPlaneWidget::SectioningPlaneWidget(QWidget *parent,
   ObjectViewerController *objectViewerController): QDockWidget(parent), m_objectViewerController(objectViewerController),
   m_viewer(NULL) {

  setupUi( this );
  connect( comboBoxLinked, SIGNAL(currentIndexChanged(int)),
           this, SLOT(changedLink()));
  connect( pushButtonApply, SIGNAL(clicked(bool)),
           this, SLOT(apply()));
  comboBoxLinked->addItem(tr("No link"), qVariantFromValue<QObject *>(NULL));
  m_fieldSensor = new SoFieldSensor( &SectioningPlaneWidget::fieldSensorCB, this);
}

SectioningPlaneWidget::~SectioningPlaneWidget() {
}

void SectioningPlaneWidget::changedLink() {
    bool notLinked = !(qVariantValue<QObject*>(comboBoxLinked->itemData(comboBoxLinked->currentIndex())));
    doubleSpinBoxNX->setEnabled(notLinked);
    doubleSpinBoxNY->setEnabled(notLinked);
    doubleSpinBoxNZ->setEnabled(notLinked);
    doubleSpinBoxPositionX->setEnabled(notLinked);
    doubleSpinBoxPositionY->setEnabled(notLinked);
    doubleSpinBoxPositionZ->setEnabled(notLinked);
    doubleSpinBoxDist->setEnabled(notLinked);
}

void SectioningPlaneWidget::apply() {
  if (!m_viewer)
      return ;
  SoClipPlaneManip *cl= m_viewer->clipPlaneManip();
  if (!cl)
      return;
  ObjectViewer *temp = m_viewer;
  m_viewer = NULL;
  int index = comboBoxLinked->currentIndex();
  ObjectViewer *link = qobject_cast<ObjectViewer*>(qVariantValue<QObject*>(comboBoxLinked->itemData(index)));
  if (link) {
      temp->setLinkedPlaneTo(link);
  } else {
      cl->draggerPosition.setValue(doubleSpinBoxPositionX->value(), doubleSpinBoxPositionY->value(), doubleSpinBoxPositionZ->value());
      cl->plane.setValue(SbPlane(SbVec3f(doubleSpinBoxNX->value(), doubleSpinBoxNY->value(), doubleSpinBoxNZ->value()), doubleSpinBoxDist->value()));
  }
  m_viewer = temp;
}

void SectioningPlaneWidget::viewerSelected() {
  ObjectViewer *ow =  m_objectViewerController->activeWindow();
  if (!ow)
    return;
  SoClipPlaneManip *cl= ow->clipPlaneManip();
  loadProperties(ow);
  if (!cl)
      return;
  m_fieldSensor->attach(&(cl->plane));
}

void SectioningPlaneWidget::addedClipPlane() {
    ObjectViewer *ow =  qobject_cast<ObjectViewer*>(sender());
    if (!ow)
        return;
    SoClipPlaneManip *cl= ow->clipPlaneManip();
    if (!cl)
        return;
    m_viewer = NULL;
    m_fieldSensor->attach(&(cl->plane));
    loadProperties(ow);
}

void SectioningPlaneWidget::planePropertyChanged() {
    if (!m_viewer)
        return;
    loadProperties(m_viewer);
}

void SectioningPlaneWidget::loadProperties(ObjectViewer* viewer) {
    if (!viewer)
        return;
    disconnect(viewer, NULL, this, NULL);
    SoClipPlaneManip *cl= viewer->clipPlaneManip();
    if (cl) {
        if (m_viewer)
        m_viewer = NULL;
        SbPlane plane = cl->plane.getValue();
        SbVec3f normal = plane.getNormal();
        doubleSpinBoxNX->setValue(normal[0]);
        doubleSpinBoxNY->setValue(normal[1]);
        doubleSpinBoxNZ->setValue(normal[2]);
        SbVec3f pos = cl->draggerPosition.getValue();
        doubleSpinBoxPositionX->setValue(pos[0]);
        doubleSpinBoxPositionY->setValue(pos[1]);
        doubleSpinBoxPositionZ->setValue(pos[2]);
        doubleSpinBoxDist->setValue(plane.getDistanceFromOrigin());
        comboBoxLinked->clear();
        QList<ObjectViewer*> list = m_objectViewerController->model()->getViewers();
        QListIterator<ObjectViewer*> i(list);
        comboBoxLinked->addItem(tr("No link"), qVariantFromValue<QObject *>(NULL));
        while (i.hasNext()) {
            ObjectViewer* cur_viewer = qobject_cast<ObjectViewer*>(i.next());
            if (cur_viewer && viewer!=cur_viewer && cur_viewer->clipPlaneManip())
               comboBoxLinked->addItem(cur_viewer->windowTitle(), qVariantFromValue<QObject *>(cur_viewer));
        }
        int index= comboBoxLinked->findData(qVariantFromValue<QObject *>(viewer->getLinkedPlaneTo()));
        if (index<0) index = 0;
        comboBoxLinked->setCurrentIndex(index);
        m_viewer = viewer;
    }
    connect(viewer , SIGNAL(addedClipPlane(SoClipPlane *)), this, SLOT(addedClipPlane()));
    setWindowTitle("Setion plane of "+ viewer->windowTitle());
}

void SectioningPlaneWidget::fieldSensorCB(void *inWidget,  SoSensor *) {
   SectioningPlaneWidget *myself
      = (SectioningPlaneWidget *) inWidget;
   myself->planePropertyChanged();
}
