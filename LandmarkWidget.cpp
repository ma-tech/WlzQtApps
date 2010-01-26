#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _LandmarkWidget_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         LandmarkWidget.cpp
* \author       Zsolt Husz
* \date         January 2010
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
* \brief        Landmark dialog
* \ingroup      UI
*
*/
#include "LandmarkWidget.h"
#include "LandmarkController.h"

LandmarkWidget::LandmarkWidget(LandmarkController* controller, QWidget *parent) :
  QDockWidget(parent), Ui::LandmarkWidget(), m_controller(controller) {
  setupUi( this ); // this sets up GUI

  if (controller)
    viewLandmark->setModel(controller->getModel());

  connect(pushButtonRemove, SIGNAL(clicked()), this, SLOT(removeLandmark()));
  connect(pushButtonRemoveAll, SIGNAL(clicked()), controller, SLOT(removeAllLandmarks()));

  connect(viewLandmark->selectionModel(), 
     SIGNAL(currentChanged( const QModelIndex &, const QModelIndex & )),
     this,
     SLOT(currentRowChanged( const QModelIndex &, const QModelIndex & )));

  connect(controller,
     SIGNAL(setHighlight(const int, const bool)), 
     this,
     SLOT(setHighlight(const int, const bool)));
}

LandmarkWidget::~LandmarkWidget() {
}

void LandmarkWidget::removeLandmark() {
  Q_ASSERT(m_controller);
  m_controller->removeLandmark(viewLandmark->currentIndex());
}

void LandmarkWidget::currentRowChanged ( const QModelIndex & current,
   const QModelIndex & /*previous*/ ) {
  m_controller->highlight(current, true);
}

void LandmarkWidget::setHighlight(const int row, const bool on) {
  if (on) {
   LandmarkModel* model = qobject_cast<LandmarkModel*>(viewLandmark->model());
   //keep active column
   if (model)
     viewLandmark->setCurrentIndex(model->index(row, viewLandmark->currentIndex().column(), QModelIndex()));
  }
}
