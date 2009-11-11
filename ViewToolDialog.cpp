#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ViewToolDialog_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ViewToolDialog.cpp
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
* \brief        Viewer and view tool dialog
* \ingroup      UI
*
*/

//project includes
#include "ViewToolDialog.h"
#include "ObjectViewer.h"
#include "ObjectView.h"
#include "WarperSourceViewer.h"
#include "View.h"
//#include "MeshViewPropertiesDialog.h"
#include "ObjectViewerModel.h"
#include "ViewerDelegate.h"
#include "MainWindow.h"

//Qt includes
#include <QModelIndex>
#include <QItemSelectionModel>
#include <QMdiSubWindow>


ViewToolDialog::ViewToolDialog(ObjectViewerModel *objectViewerModel, MainWindow *parent):
 QDialog(parent) {
  setupUi( this ); // this sets up GUI
  connect(pushButtonEditView, SIGNAL(clicked()), this, SLOT(EditView()));

  QItemSelectionModel *selectionModel = new QItemSelectionModel(objectViewerModel);
  connect(selectionModel, 
       SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), 
       this,
       SLOT(currentChanged(const QModelIndex &, const QModelIndex &)));

  viewTree->setModel(objectViewerModel);
  viewTree->setSelectionModel(selectionModel);
  viewTree->setItemDelegate(new ViewerDelegate(parent->getWorkspace()));
}


ViewToolDialog::~ViewToolDialog() {
}

void ViewToolDialog::EditView() {
    /*MeshViewPropertiesDialog * meshViewPropertiesDialog = 
          new MeshViewPropertiesDialog(this);
    Q_ASSERT(meshViewPropertiesDialog);
    meshViewPropertiesDialog->exec();*/
}

void ViewToolDialog::currentChanged ( const QModelIndex & current,
 const QModelIndex & previous ) {
  if (viewTree && current != previous ) {  //chek if view has been changed
    ObjectViewerModel *objectViewerModel =
       qobject_cast<ObjectViewerModel *>(viewTree->model());
    if (objectViewerModel) {
      ObjectViewer * viewer = objectViewerModel ->getViewerOf(current);
      ObjectViewer * viewer_previous = objectViewerModel ->getViewerOf(previous);
      if (viewer && viewer != viewer_previous) { // final check if view changed 
                                                 // and different then initial
        viewer->activate();
        QWidget *parentWidget=qobject_cast<QWidget *>(viewer->parent());
        if (parentWidget) {
          //change focus of MDI
          parentWidget->raise(); 
          parentWidget->activateWindow(); 
          parentWidget->setFocus(); 
        }
        // keep focus of the dialog
        raise();
        activateWindow();
        setFocus();
      }
      ObjectView * view = objectViewerModel->getView(current);
      if (view)
         view->selectView();
    }
  }
}
