#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _Viewer2D_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         Viewer2D.cpp
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
* \brief        2D viewer wrapping SoQtPlaneViewer
* \ingroup      UI
*/

//project includes
#include "Viewer2D.h"
#include "ObjectSimpleViewer.h"

//inventor includes
#include <Inventor/Qt/viewers/SoQtFullViewer.h>

// Qt includes
#include <QSlider>
#include <QGridLayout>
#include <QLabel>
#include <QObject>
#include <QPushButton>

Viewer2D::
Viewer2D(
  QWidget * parent,
  QLayout *slider,
  QList <QWidget*> *buttons):
SoQtPlaneViewer(
  parent,
  NULL,
  TRUE,
  SoQtFullViewer::BUILD_ALL,
  SoQtFullViewer::EDITOR,
  FALSE),
m_slider(
  slider),
m_buttons(
  buttons)
{
  // Explicitly trigger the construction of viewer decorations.
  QWidget * widget = this->buildWidget(this->getParentWidget());
  this->setBaseWidget(widget);
  setTransparencyType(SoGLRenderAction::SORTED_OBJECT_BLEND);
}

Viewer2D::~Viewer2D()
{
  if(m_buttons)
  {
    QListIterator<QWidget*> i(*m_buttons);
    while (i.hasNext())
    {
      delete i.next();
    }
    delete m_buttons;
  }
  if(getBaseWidget())
  {
    delete getBaseWidget();
  }
}

void Viewer2D::
setCamera(
  SoCamera *camera)
{
   SoQtViewer::setCamera(camera); 	// call a base class method,
   					// avoid ading camera button
   ObjectSimpleViewer * objectViewer =
       qobject_cast <ObjectSimpleViewer*> (getBaseWidget()->parent());
   if(objectViewer)
   {
     objectViewer->replacedCamera(camera);
   }
}

void Viewer2D::
createViewerButtons(
  QWidget * parent,
  SbPList * buttonlist)
{
  SoQtPlaneViewer::createViewerButtons(parent, buttonlist);
  //remove unused button
  delete (QPushButton *)buttonlist->get(8);
  buttonlist->remove(8);
  delete (QPushButton *)buttonlist->get(7);
  buttonlist->remove(7);
  delete (QPushButton *)buttonlist->get(6);
  buttonlist->remove(6);
  delete (QPushButton *)buttonlist->get(5);
  buttonlist->remove(5);
  delete (QPushButton *)buttonlist->get(3);
  buttonlist->remove(3);
  delete (QPushButton *)buttonlist->get(2);
  buttonlist->remove(2);

  ((QPushButton *)buttonlist->get(0))->setToolTip("Edit mode");
  ((QPushButton *)buttonlist->get(1))->setToolTip("View mode");
  ((QPushButton *)buttonlist->get(2))->setToolTip("View all");
  //don't show: button is reffered elsewhere, can't be removed
  ((QPushButton *)buttonlist->get(3))->setVisible(false);

  if(m_buttons)
  {
    QListIterator<QWidget*> i(*m_buttons);
    while (i.hasNext())
    {
      buttonlist->append(i.next());
    }
  }
}

QWidget * Viewer2D::
buildRightTrim(
  QWidget * parent)
{
  QWidget * w = SoQtPlaneViewer::buildRightTrim (parent);
  if(m_slider)
  {
    QGridLayout * l = (QGridLayout*)(w->layout());
    l->addLayout(m_slider, 1, 0);
  }
  return w;
}
