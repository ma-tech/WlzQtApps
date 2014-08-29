#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _ViewerDelegate_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         ViewerDelegate.cpp
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
* \brief        Delegate for viewer tool tree view
* \ingroup      UI
*/

//project includes
#include "ViewerDelegate.h"
#include "ViewerListEditor.h"
#include "ObjectViewerModel.h"
#include "ObjectView.h"

//Qt includes
#include <QCheckBox>
#include <QApplication>
#include <QtGui>
#include <QItemEditorCreatorBase>

ViewerDelegate::
ViewerDelegate(
  QMdiArea * mdiArea,
  QObject *parent):
QStyledItemDelegate(parent)
{
  // register ViewerListEditor
  QItemEditorFactory *factory = new QItemEditorFactory;
  QItemEditorCreatorBase *viewerListCreator =
      new ViewerListEditorBase(mdiArea);
  factory->registerEditor(
      (QVariant::Type)QMetaType::QObjectStar, viewerListCreator);
  setItemEditorFactory(factory);
}


void ViewerDelegate::
paint(
  QPainter *painter,
  const QStyleOptionViewItem &option,
  const QModelIndex &index) const
{

  const int LeftRightBorder = 2;
  const int TopBottomBorder = 2;
  if(index.column() == 2 && qVariantCanConvert<QColor>(index.data()))
  {
    QColor color = qVariantValue<QColor>(index.data());

    painter->save();
    QBrush brush(color, Qt::SolidPattern);
    QRect rect(option.rect);
    rect.adjust(-LeftRightBorder, -TopBottomBorder,
	        -LeftRightBorder, -TopBottomBorder);
    painter->fillRect(rect, brush);
    painter->drawRect(rect);
    painter->restore();
  }
  else
  {
    QStyledItemDelegate::paint(painter, option, index);
  }
}


QWidget * ViewerDelegate::
createEditor(
  QWidget * parent,
  const QStyleOptionViewItem & option,
  const QModelIndex & index) const
{
  if(index.column() == 3)
  {
    ObjectViewerModel *model = (ObjectViewerModel *)index.model();
    if(model)
    {
      ObjectView* view = model->getView(index);
      if(view)
      {
        QComboBox *comboBox = new QComboBox(parent);
        comboBox->addItems(view->getVisualisationTypes());
        return(comboBox);
      }
    }
  }
  return(QStyledItemDelegate::createEditor(parent, option, index));
}

void ViewerDelegate::
setEditorData(
  QWidget *editor,
  const QModelIndex &index) const
{
  if(index.column() == 3)
  {
    QComboBox *combo = qobject_cast<QComboBox *> (editor);
    int type = index.model()->data(index, Qt::EditRole).toInt();
    combo->setCurrentIndex(type);
  }
  else
  {
    QStyledItemDelegate::setEditorData(editor, index);
  }
}

void ViewerDelegate::
setModelData(
  QWidget *editor,
  QAbstractItemModel *model,
  const QModelIndex &index) const
{
  if(index.column() == 3)
  {
    QComboBox *combo = qobject_cast<QComboBox *> (editor);
    int type = combo->currentIndex();
    model->setData(index, type);
  }
  else
  {
    QStyledItemDelegate::setModelData(editor, model, index);
  }
}
