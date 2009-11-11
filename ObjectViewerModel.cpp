#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ObjectViewerModel_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ObjectViewerModel.cpp
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
* \brief        Model managing the viewers and views.
* \ingroup      Control
*
*/

//project includes
#include "ObjectViewerModel.h"
#include "WoolzObject.h"
#include "ObjectView.h"
#include "View.h"
#include "Mesh3DView.h"
#include "ObjectViewer.h"
#include "MainWindow.h"
#include "WarperSourceViewer.h"
#include "WarperTargetViewer.h"
#include "WarperResultViewer.h"


//Inventor includes
#include <Inventor/nodes/SoSeparator.h>

//Qt includes
#include <QAction>
#include <QMdiSubWindow>
#include <QMdiArea>
#include <QtXml/QXmlStreamWriter>

ObjectViewerModel::ObjectViewerModel (QMdiArea *mdiArea, QObject * parent):  QAbstractItemModel (parent), m_mdiArea(mdiArea),
   deleteInProgress(false) {
}

int ObjectViewerModel::rowCount(const QModelIndex & parent) const {
    if (!parent.isValid()) { //this is the root node
      QList<QMdiSubWindow *> windows = m_mdiArea->subWindowList();
      return windows.size();
    }

    ObjectViewer * viewer = getViewer(parent);

    if (!viewer) // if it is a view then is chiled, and therefore can't have more children
        return 0 ;

    //otherwise is a viewer and has possibily multiple views
    return viewer->getViews().size();
}

ObjectViewer *ObjectViewerModel::getViewer(const QModelIndex & index) const {
    if (index.isValid()) {
        return qobject_cast<ObjectViewer*>(static_cast<QObject *>(index.internalPointer()));
                //can also be View
    }
    return NULL;
}

bool ObjectViewerModel::isView(const QModelIndex & index) const {
  return getView(index)!=NULL;
}

bool ObjectViewerModel::isViewer(const QModelIndex & index) const {
  return getViewer(index)!=NULL;
}

ObjectView *ObjectViewerModel::getView(const QModelIndex & index) const {
    if (index.isValid()) {
      return qobject_cast<ObjectView*>((QObject *)(index.internalPointer()));  //can also be View
    }
    return NULL;
}

int ObjectViewerModel::columnCount(const QModelIndex & /*parent*/) const {
    return 7;
}

QVariant ObjectViewerModel::data(const QModelIndex & index, int role ) const {
  if (!index.isValid())
    return QVariant();
  if (role == Qt::DisplayRole) {
    ObjectViewer *viewer = getViewer(index);
    if (!viewer ) {//this is a view
      ObjectView* view = getView(index);

      if (!view) 
        return QVariant();

      switch (index.column()) {
          case 0:
            return view->getTitle();
          case 1:
            return QVariant();
          case 2:
              return view->getQColour();
          case 3:
            return view->getVisualisationTypes().at(view->visualisationType());
          case 4:
            return QString("%1%").arg(view->getTransparency());
          case 5:
            return view->getProperties();
      }
    } else { //this is a viewer, since it has no parent
        if (index.column() == 0) {
              return viewer->windowTitle();
        } else if (index.column() == 6) {
          ObjectViewer *viewer_link = viewer->getLinkedTo();
          if (viewer_link)
            return viewer_link->windowTitle();
          else
            return QString("No link");
        } // else-if
    } //if (window)
  } else if (role == Qt::CheckStateRole) {
    ObjectViewer *viewer = getViewer(index);
    if (!viewer ) {//this is a view
      ObjectView* view = getView(index);

      if (!view) 
        return QVariant();

      switch (index.column()) {
          case 1:
            return view->getVisibility() ? Qt::Checked : Qt::Unchecked;
      }
    }
  } else if (role == Qt::EditRole) {
    ObjectViewer *viewer = getViewer(index);
    if (!viewer ) {//this is a view
      ObjectView* view = getView(index);

      if (!view) 
        return QVariant();

      switch (index.column()) {
          case 0:
            return view->getTitle();
          case 3:
            return view->getVisualisationTypes();
          case 4:
            return view->getTransparency();
      }
    } else {  //is a viewer
      switch (index.column()) {
          case 0:
            return viewer->windowTitle();
          case 6:
            return qVariantFromValue<QObject*>(viewer);  // transmits the view not the linked to field, the let the 
                                                         // editor remove from the options the actual viewer
      }
    }
  }
  return QVariant();
}

QVariant ObjectViewerModel::headerData(int section, Qt::Orientation orientation, int role ) const {
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
    switch (section) {
      case 0:
        return QString(tr("View/Object"));
      case 1:
        return QString(tr("Visible"));
      case 2:
        return QString(tr("Colour"));
      case 3:
        return QString(tr("Visualisation"));
      case 4:
        return QString(tr("Transparency"));
      case 5:
        return QString(tr("Properties"));
      case 6:
        return QString(tr("Linked to"));
    }
  }
  return QVariant();
}

ObjectViewerModel::~ObjectViewerModel ( ) {
}

Qt::ItemFlags ObjectViewerModel::flags(const QModelIndex &index) const {
  Qt::ItemFlags flags = QAbstractItemModel::flags(index);
  if (isViewer(index)) {
    if (index.column()==0 || index.column()==6)
      flags |= Qt::ItemIsEditable;
  }
  if (isView(index)) {
    if (index.column() == 1)
      flags |= Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    else {
       ObjectView* view = getView(index);
       Q_ASSERT(view);
       if (index.column() > 2 && index.column()<5 && (index.column()!=3 || view->getVisualisationTypesNo()>1))
        flags |= Qt::ItemIsEditable;
    }
  }
  return flags;
}

bool ObjectViewerModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if (!index.isValid())
      return false;

  if (role == Qt::EditRole) {
    if (isViewer(index)) {
      if (index.column()==0 && !value.toString().isEmpty()) {
        ObjectViewer * viewer = getViewer(index);
        viewer->setWindowTitle(value.toString());
        emit dataChanged(index, index);
        return true;
      } else if (index.column()==6 ) {
        ObjectViewer * viewer = getViewer(index);
        ObjectViewer * linkedToViewer = NULL;
        if (value.canConvert<QObject *>()) {
          linkedToViewer  = qobject_cast <ObjectViewer*>(value.value<QObject *>());
        }
        if (linkedToViewer != viewer) {
          viewer->setLinkedTo(linkedToViewer);
          emit dataChanged(index, index);
          return true;
        }
      }
    } else if (isView(index)) { // is view
      if (index.column()==4 && !value.toString().isEmpty()) {
        View * view = getView(index);
        view->setTransparency(value.toInt());
        emit dataChanged(index, index);
        return true;
      } else if (index.column()==3) {
        View * view = getView(index);
        view->setVisualisationType(value.toInt());
        emit dataChanged(index, index);
      }
    }
  } else if (role == Qt::CheckStateRole) {
      if (index.column()==1) {
        View * view = getView(index);
        view->setVisibility(value == Qt::Checked);
        emit dataChanged(index, index);
        return true;
    }
  }
  return false;
}


QModelIndex ObjectViewerModel::index(int row, int column, const QModelIndex &parent ) const {
  if (row<0 || column <0)
    return QModelIndex();
  if (!parent.isValid()) {//viewers
      QMdiSubWindow *window = m_mdiArea->subWindowList().at(row);
      return createIndex(row, column, window->widget());
  } else { // views
      ObjectViewer *viewer = getViewer(parent);
      if (viewer)
        return createIndex(row, column, viewer->getViews().at(row));
      Q_ASSERT(false);
  }
  return QModelIndex();
}

QModelIndex ObjectViewerModel::parent(const QModelIndex &child) const {
  if (child.isValid()) {
      ObjectView * view = getView(child);
      if (view) {  // is is a view
        //find row number
        ObjectViewer * viewer = (ObjectViewer *) view->getParent();
        int row = getRowOfViewer(viewer);
        if (row >= 0)
          return createIndex(row, 0, viewer );
      } // else this is viewer, parent is root
  }
  return QModelIndex();
}

int ObjectViewerModel::getRowOfViewer(const ObjectViewer * viewer) const {
  if (!viewer) 
    return -1;

  QMdiSubWindow * window = qobject_cast<QMdiSubWindow *>(viewer->parentWidget ());
  if (!window) 
    return -1;

  QList<QMdiSubWindow *> windows = m_mdiArea->subWindowList();
  return m_mdiArea->subWindowList().indexOf(window);
}

void ObjectViewerModel::addedViewer(ObjectViewer * viewer) {
  if (viewer) {
      int row = getRowOfViewer(viewer);
      if (row>=0) {
        beginInsertRows(QModelIndex(), row, row);
        endInsertRows();
      }
  }
}

void ObjectViewerModel::removedViewerStart() {
  ObjectViewer * viewer = qobject_cast<ObjectViewer *>(sender());
  if (viewer) {
      int row = getRowOfViewer(viewer);
      if (row>=0) {
          beginRemoveRows(QModelIndex(), row, row); //force view update 
          deleteInProgress = true;
      }
  }
}

void ObjectViewerModel::removedViewerFinished() {
  if (deleteInProgress) {
     endRemoveRows();
     deleteInProgress = false;
  }
}

void ObjectViewerModel::removedViewStart(ObjectView *view) {
  ObjectViewer * viewer = qobject_cast<ObjectViewer *>(sender());
  if (viewer) {
     int row = viewer->getViews().indexOf(view);
     Q_ASSERT(row>=0);
     int rowparent = getRowOfViewer(viewer);
     Q_ASSERT(rowparent>=0);
     if (rowparent>=0 && row>=0) {
       QModelIndex parent = createIndex(rowparent,0,viewer);
       deleteInProgress = true;
       beginRemoveRows( parent, row, row);
     }
  }
}

void ObjectViewerModel::removedViewFinish(ObjectView */*view*/) {
  if (deleteInProgress) {
     endRemoveRows();
     deleteInProgress = false;
  }
}

void ObjectViewerModel::changedObjectView(ObjectView *view) {
  ObjectViewer * viewer = qobject_cast<ObjectViewer *>(sender());
  if (viewer) {
    int row = viewer->getViews().indexOf(view);
    Q_ASSERT(row>=0);
    if (row>=0)
      changePersistentIndex(createIndex(row, 0, view), createIndex(row, 2, view));  //change name and colour
      emit layoutChanged();
  }
}

void ObjectViewerModel::changedViewer() {
  ObjectViewer * viewer = qobject_cast<ObjectViewer *>(sender());
  if (viewer) {
    int row = getRowOfViewer(viewer);
    Q_ASSERT(row>=0);
    if (row>=0)
      changePersistentIndex(createIndex(row, 0, viewer), createIndex(row, 7, viewer));
      emit layoutChanged();
  }
}

void ObjectViewerModel::listenToObject(ObjectView *view) {
    connect(view, SIGNAL(viewPropertyChanged()), this, SLOT(viewPropertyChanged()));
}

void ObjectViewerModel::addedView(ObjectView *view) {
  ObjectViewer * viewer = qobject_cast<ObjectViewer *>(sender());
  if (viewer && view) {
      int rowparent = getRowOfViewer(viewer);
      int row = viewer->getViews().indexOf(view);
      if (rowparent>=0 && row>=0) {
        beginInsertRows(createIndex(rowparent,0,viewer), row, row);
        endInsertRows();
      }
  }
}

ObjectViewer * ObjectViewerModel::getViewerOf(const QModelIndex & index) const {
  ObjectViewer * viewer = getViewer(index);
  if (viewer) // if index is viwer
    return viewer;	
  ObjectView* view = getView(index);

  if (!view)
    return NULL;

  // if index is a view
  return (ObjectViewer *)view->getParent();
}
