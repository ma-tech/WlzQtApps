#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _WarpingWidget_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         WarpingWidget.cpp
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
#include "WarpingWidget.h"
#include "LandmarkController.h"
#include "Commands.h"

WarpingWidget::WarpingWidget(QWidget *parent, LandmarkController *landmarkController):
       QDockWidget(parent), m_landmarkController(landmarkController), m_enableUpdate(true)
       {
  setupUi( this );

  setMinimumSize(200,0);
  setMaximumSize(500,200);
  resize(200,1);


  connect( radioButtonIMQ, SIGNAL(clicked(bool)),
           this, SLOT(basisFnChanged()));
  connect( radioButtonMQ, SIGNAL(clicked(bool)),
           this, SLOT(basisFnChanged()));

  connect( doubleSpinBoxDelta, SIGNAL(valueChanged(double)),
           this, SLOT(deltaChanged()));

  connect( pushButtonUpdate, SIGNAL(clicked(bool)),
           this, SLOT(update()));
  connect( checkBoxAutoUpdate, SIGNAL(clicked(bool)),
           this, SLOT(setAutoUpdate(bool)));

  //not needed, these could be controled from global auto update
  checkBoxAutoUpdate->setVisible(false);
  pushButtonUpdate->setVisible(false);
  update();
}

WarpingWidget::~WarpingWidget() {
}


void WarpingWidget::setAutoUpdate(bool /*enabled*/) {
}

void WarpingWidget::deltaChanged() {
  double delta = doubleSpinBoxDelta->value();
  if (m_enableUpdate && m_landmarkController)
     m_landmarkController->addCommand(new WarpingSetDelta(m_landmarkController->getModel(), delta));
}

void WarpingWidget::basisFnChanged() {
  bool useIMQ = radioButtonIMQ->isChecked();
  if (m_enableUpdate && m_landmarkController)
    m_landmarkController->addCommand(new WarpingtSetBasisFnType(m_landmarkController->getModel(), useIMQ ? LandmarkModel::basis_IMQ : LandmarkModel::basis_MQ));
}

void WarpingWidget::update() {
  m_enableUpdate = false;
  if (m_landmarkController->getModel()->basisFnType() == LandmarkModel::basis_IMQ)
      radioButtonIMQ->setChecked(true);
  else
      radioButtonMQ->setChecked(true);
  doubleSpinBoxDelta->setValue(m_landmarkController->getModel()->delta());
  m_enableUpdate = true;
}
