#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _Viewer3D_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         Viewer3D.cpp
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
* \brief        3D viewer wrapping SoQtExaminerViewer
* \ingroup      UI
*
*/

//project includes
#include "Viewer3D.h"
#include "ObjectSimpleViewer.h"

// Qt includes
#include <QSlider>
#include <QGridLayout>
#include <QLabel>
#include <QListIterator>
#include <QObject>
#include <QPushButton>
#include <QIcon>

Viewer3D::Viewer3D(QWidget * parent, QLayout *slider, QList <QWidget*> *buttons) : SoQtExaminerViewer(parent, NULL, TRUE,
                            SoQtFullViewer::BUILD_ALL, SoQtFullViewer::EDITOR,
                            FALSE) , m_slider (slider), m_buttons(buttons)
{
    // Explicitly trigger the construction of viewer decorations.
    QWidget * widget = this->buildWidget(this->getParentWidget());
    this->setBaseWidget(widget);
    setTransparencyType(SoGLRenderAction::SORTED_OBJECT_BLEND);
}

Viewer3D::~Viewer3D() {
   if (m_buttons) {
     QListIterator<QWidget*> i(*m_buttons);
     while (i.hasNext())
        delete i.next();
     delete m_buttons;
   }
}

void Viewer3D::setCamera( SoCamera *camera ) {
   SoQtExaminerViewer::setCamera(camera); //call base class

   // forward to manager class the camera change
   ObjectSimpleViewer* objectViewer = qobject_cast <ObjectSimpleViewer*> (getBaseWidget()->parent());
   if (objectViewer)
     objectViewer->replacedCamera(camera);
}

void Viewer3D::createViewerButtons(QWidget * parent, SbPList * buttonlist) {
   SoQtExaminerViewer::createViewerButtons(parent, buttonlist); //avoids creating X,Y,Z buttons

   ((QPushButton *)buttonlist->get(0))->setToolTip("Edit mode");
   ((QPushButton *)buttonlist->get(1))->setToolTip("View mode");
   ((QPushButton *)buttonlist->get(2))->setToolTip("Home");
   ((QPushButton *)buttonlist->get(3))->setToolTip("Set home");
   ((QPushButton *)buttonlist->get(4))->setToolTip("View all");
   ((QPushButton *)buttonlist->get(5))->setVisible(false); //don't show: button is reffered elsewhere, can't be removed
   ((QPushButton *)buttonlist->get(6))->setToolTip("Projection mode");

   if (m_buttons) {
     QListIterator<QWidget*> i(*m_buttons);
     while (i.hasNext())
        buttonlist->append(i.next());
   }
}

QWidget * Viewer3D::buildRightTrim (QWidget * parent) {
  QWidget * w = SoQtExaminerViewer::buildRightTrim (parent);
  if (m_slider) {
    QGridLayout * l = (QGridLayout*)(w->layout());
    l->addLayout(m_slider,1,0);
  }
  return w;
}
