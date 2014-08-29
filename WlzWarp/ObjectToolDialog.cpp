#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _ObjectToolDialog_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         ObjectToolDialog.cpp
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
* \brief        Object tool dialog
* \ingroup      UI
*/

//Qt includes
#include <QFileDialog>

//project includes
#include "ObjectToolDialog.h"
#include "ObjectListModel.h"
#include "ObjectDelegate.h"

#include "Commands.h"

ObjectToolDialog::
ObjectToolDialog(
  ObjectListModel * model,
  QWidget *parent):
QDialog(parent),
  Ui::ObjectToolDialog()
{
  setupUi(this);   // this sets up GUI

  if(model)
  {
    objectTree->setModel(model);
    objectTree->setUniformRowHeights(true);
  }
  objectTree->setItemDelegate(new ObjectDelegate(objectTree));

  connect(pushButtonRemove, SIGNAL(clicked()), this, SLOT(removeObject()));
  connect(pushButtonExport, SIGNAL(clicked()), this, SLOT(exportObject()));

  connect(objectTree->selectionModel(),
          SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
          this,
          SLOT(currentRowChanged(const QModelIndex &, const QModelIndex &)));

  connect(model, SIGNAL(objectSelected(WoolzObject *)), this,
          SLOT(objectSelected(WoolzObject *)));
}

ObjectToolDialog::
~ObjectToolDialog()
{
}

void ObjectToolDialog::
removeObject()
{
  ObjectListModel* model = qobject_cast<ObjectListModel*>(
      objectTree->model());
  if(model)
  {
    model->addCommand(new DeleteWoolzObject(
	  model, model->getObject(objectTree->currentIndex())));
  }
}

void ObjectToolDialog::
exportObject()
{
  ObjectListModel* model = qobject_cast<ObjectListModel*>(
      objectTree->model());
  if(!model)
  {
    return;
  }
  WoolzObject * object = model->getObject(objectTree->currentIndex());
  if(!object)
  {
    return;
  }
  QString filename = QFileDialog::getSaveFileName(this,
      tr("Export Woolz object"), ".",
      tr("Woolz object (*.wlz)"));
  if(filename.isEmpty())
  {
    return;
  }

  object->saveAs(filename);
}

void ObjectToolDialog::
currentRowChanged(const QModelIndex & current,
    const QModelIndex & /*previous*/)
{
  bool removeEnabled = false;
  bool exportEnabled = false;
  ObjectListModel* model = qobject_cast<ObjectListModel*>(objectTree->model());
  if(model)
  {
    WoolzObject *object = model->getObject(current);
    if(object)
    {
      removeEnabled = object->removable();
      exportEnabled = true;
      model->setSelectObject(object);
    }
  }
  pushButtonRemove->setEnabled(removeEnabled);
  pushButtonExport->setEnabled(exportEnabled);
}

void ObjectToolDialog::
objectSelected(WoolzObject *obj)
{
  ObjectListModel* model = qobject_cast<ObjectListModel*>(
      objectTree->model());
  if(model->getObject(objectTree->currentIndex()) != obj)
  {
    objectTree->setCurrentIndex(model->getObjIndex(obj));
  }
}
