#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _ViewToolWidget_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         ViewToolWidget.cpp
* \author       Zsolt Husz
* \date         January 2010
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
* \brief        Viewer and view tool dialog
* \ingroup      UI
*/

//project includes
#include "ViewToolWidget.h"
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


ViewToolWidget::
ViewToolWidget(
  ObjectViewerModel *objectViewerModel,
  MainWindow *parent):
QDockWidget(parent)
{
  setupUi(this);   // this sets up GUI

  QItemSelectionModel *selectionModel =
      new QItemSelectionModel(objectViewerModel);
  connect(selectionModel,
          SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
          this,
          SLOT(currentChanged(const QModelIndex &, const QModelIndex &)));

  viewTree->setModel(objectViewerModel);
  viewTree->setSelectionModel(selectionModel);
  viewTree->setItemDelegate(new ViewerDelegate(parent->getWorkspace()));
}


ViewToolWidget::
~ViewToolWidget()
{
}

void ViewToolWidget::
EditView()
{
}

void ViewToolWidget::
currentChanged(
  const QModelIndex & current,
  const QModelIndex & previous)
{
  if(viewTree && current != previous) 	//check if view has been changed
  {
    ObjectViewerModel *objectViewerModel =
        qobject_cast<ObjectViewerModel *>(viewTree->model());
    if(objectViewerModel)
    {
      ObjectViewer * viewer = objectViewerModel ->getViewerOf(current);
      ObjectViewer * viewer_previous =
	  objectViewerModel->getViewerOf(previous);
      if(viewer && viewer != viewer_previous)	// final check if view changed
      {
        // and different then initial
        viewer->activate();
        QWidget *parentWidget = qobject_cast<QWidget *>(viewer->parent());
        if(parentWidget)
        {
          // change focus of MDI
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
      if(view)
      {
        view->selectView();
      }
    }
  }
}
