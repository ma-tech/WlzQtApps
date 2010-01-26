#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ObjectToolWidget_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ObjectToolWidget.cpp
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
* \brief        Object tool dialog
* \ingroup      UI
*
*/

//Qt includes
#include <QFileDialog>

//project includes
#include "ObjectToolWidget.h"
#include "ObjectListModel.h"
#include "ObjectDelegate.h"

#include "Commands.h"

ObjectToolWidget::ObjectToolWidget(ObjectListModel * model, QWidget *parent): QDockWidget(parent) {
  setupUi( this );
  if (model) {
    objectTree->setModel(model);
    objectTree->setUniformRowHeights(true);
  }
  objectTree->setItemDelegate(new ObjectDelegate(objectTree));

  connect(pushButtonRemove, SIGNAL(clicked()), this, SLOT(removeObject()));
  connect(pushButtonExport, SIGNAL(clicked()), this, SLOT(exportObject()));

  connect(objectTree->selectionModel(),
      SIGNAL(currentChanged( const QModelIndex &, const QModelIndex & )),
      this,
      SLOT(currentRowChanged( const QModelIndex &, const QModelIndex & )));

 connect(model, SIGNAL(objectSelected(WoolzObject *)), this, SLOT(objectSelected(WoolzObject *)));
}

ObjectToolWidget::~ObjectToolWidget() {
}

void ObjectToolWidget::removeObject() {
  ObjectListModel* model = qobject_cast<ObjectListModel*>(objectTree->model());
  if (model)
       model->addCommand(new DeleteWoolzObject(model, model->getObject(objectTree->currentIndex())));
}

void ObjectToolWidget::exportObject() {
  ObjectListModel* model = qobject_cast<ObjectListModel*>(objectTree->model());
  if (!model)
    return;
  WoolzObject * object = model->getObject(objectTree->currentIndex());
  if (!object)
    return;
  QString filename = QFileDialog::getSaveFileName(this, tr("Export Woolz object"),
                                                ".",
                                                tr("Woolz object (*.wlz)"));
  if (filename.isEmpty())
     return;

  object->saveAs(filename);
}

void ObjectToolWidget::currentRowChanged ( const QModelIndex & current,
       const QModelIndex & /*previous*/ ) {
   bool removeEnabled = false;
   bool exportEnabled = false;
   ObjectListModel* model = qobject_cast<ObjectListModel*>(objectTree->model());
   if (model) {
       WoolzObject *object = model->getObject(current);
     if (object) {
       removeEnabled = object->removable();
       exportEnabled = true;
       model->setSelectObject(object);
     }
   }
   pushButtonRemove->setEnabled(removeEnabled);
   pushButtonExport->setEnabled(exportEnabled);
}

void ObjectToolWidget::objectSelected(WoolzObject *obj) {
  ObjectListModel* model = qobject_cast<ObjectListModel*>(objectTree->model());
  if (model->getObject(objectTree->currentIndex()) !=obj )
    objectTree->setCurrentIndex(model->getObjIndex(obj));
}
