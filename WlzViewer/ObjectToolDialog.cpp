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
*
*/

//Qt includes
#include <QFileDialog>

//project includes
#include "ObjectToolDialog.h"
#include "ObjectListModelAbstract.h"
#include "ObjectDelegate.h"

ObjectToolDialog::
ObjectToolDialog(
  ObjectListModelAbstract * model,
  QWidget *parent):
QDialog(
  parent),
Ui::ObjectToolDialog()
{
  setupUi(this); // this sets up GUI

  if(model)
  {
    objectList->setModel(model);
    objectList->horizontalHeader()->setStretchLastSection(true);
  }
  objectList->setItemDelegate(new ObjectDelegate(objectList));

  connect(pushButtonRemove, SIGNAL(clicked()),
          this, SLOT(removeObject()));
  connect(pushButtonExport, SIGNAL(clicked()),
          this, SLOT(exportObject()));

  connect(objectList->selectionModel(),
          SIGNAL(currentChanged( const QModelIndex &, const QModelIndex & )),
          this,
          SLOT(currentRowChanged( const QModelIndex &, const QModelIndex & )));

 connect(model, SIGNAL(objectSelected(WoolzObject *)),
         this, SLOT(objectSelected(WoolzObject *)));
}

ObjectToolDialog::
~ObjectToolDialog()
{
}

void ObjectToolDialog::
removeObject()
{
  ObjectListModelAbstract* model = 
      qobject_cast<ObjectListModelAbstract*>(objectList->model());
  if(!model)
  {
    return;
  }
  model->removeObject(objectList->currentIndex());
}

void ObjectToolDialog::
exportObject()
{
  ObjectListModelAbstract* model =
      qobject_cast<ObjectListModelAbstract*>(objectList->model());
  if(!model)
  {
    return;
  }
  WoolzObject * object = model->getObject(objectList->currentIndex());
  if(!object) 
  {
    return;
  }
  QString filename = 
      QFileDialog::getSaveFileName(this, tr("Export Woolz object"), ".",
                                   tr("Woolz object (*.wlz)"));
  if(filename.isEmpty())
  {
    return;
  }

  object->saveAs(filename);
}

void ObjectToolDialog::
currentRowChanged(
  const QModelIndex & current,
  const QModelIndex & /*previous*/)
{
  bool removeEnabled = false;
  bool exportEnabled = false;
  ObjectListModelAbstract * model =
      qobject_cast<ObjectListModelAbstract*>(objectList->model());
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
  ObjectListModelAbstract* model =
      qobject_cast<ObjectListModelAbstract*>(objectList->model());
  if(model->getObject(objectList->currentIndex()) !=obj)
  {
    objectList->setCurrentIndex(model->getObjIndex(obj));
  }
}
