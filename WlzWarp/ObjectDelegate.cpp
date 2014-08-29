#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _ObjectDelegate_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         ObjectDelegate.cpp
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
* \brief        Delegate for object tool tree view
* \ingroup      UI
*/

//project includes
#include "ObjectDelegate.h"

//Qt includes
#include <QPainter>
#include <QAbstractItemView>
#include "qtcolorpicker.h"

//#include <QItemEditorFactory>

ObjectDelegate::
ObjectDelegate(
  QAbstractItemView *view,
  QObject *parent):
QStyledItemDelegate(parent),
m_view (view)
{
}

void ObjectDelegate::
paint(
  QPainter *painter,
  const QStyleOptionViewItem &option,
  const QModelIndex &index) const
{
  if(index.column() == 2 && qVariantCanConvert<QColor>(index.data()))
  {
    m_view->openPersistentEditor(index);
  }
  QStyledItemDelegate::paint(painter, option, index);
}

QWidget * ObjectDelegate::
createEditor(
  QWidget * parent,
  const QStyleOptionViewItem & option,
  const QModelIndex & index) const
{
  if(index.column() == 2 && qVariantCanConvert<QColor>(index.data()))
  {
    QtColorPicker * colorpicker = new QtColorPicker(parent);
    colorpicker->setStandardColors();
    connect(colorpicker, SIGNAL(colorChanged(QColor)),
            this, SLOT(emitCommitData()));
    return(colorpicker);
  }
  else
  {
    return(QStyledItemDelegate::createEditor(parent, option, index));
  }
}

void ObjectDelegate::
setEditorData(
  QWidget * editor,
  const QModelIndex & index) const
{
  if(index.column() == 2 && qVariantCanConvert<QColor>(index.data()))
  {
    QtColorPicker * colorpicker = qobject_cast <QtColorPicker*>(editor);
    if(colorpicker)
    {
      colorpicker->setCurrentColor(qVariantValue<QColor>(index.data()));
    }
    else
    {
      Q_ASSERT(false);
    }
  }
  else
  {
    QStyledItemDelegate::setEditorData(editor, index);
  }
}

void ObjectDelegate::
setModelData(
  QWidget * editor,
  QAbstractItemModel * model,
  const QModelIndex & index) const
{
  if(index.column() == 2 && qVariantCanConvert<QColor>(index.data()))
  {
    QtColorPicker * colorpicker = qobject_cast <QtColorPicker*>(editor);
    if(colorpicker)
    {
      model->setData(index, colorpicker->currentColor());
    }
    else
    {
      Q_ASSERT(false);
    }
  }
  else
  {
    QStyledItemDelegate::setModelData(editor, model, index);
  }
}

void ObjectDelegate::
emitCommitData()
{
  emit commitData(qobject_cast<QWidget *>(sender()));
}
