#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _ViewerListEditor_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         ViewerListEditor.cpp
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
* \brief        Provides a combo box for viewer selection
* \ingroup      UI
*/

#include <QtGui>
#include <QList>
#include <QMdiSubWindow>
#include <QListIterator>
#include "ViewerListEditor.h"
#include "ObjectViewer.h"

ViewerListEditor::
ViewerListEditor(
  QMdiArea * mdiArea,
  QWidget *widget):
QComboBox(widget),
m_mdiArea(mdiArea)
{
  populateList();
}

QVariant ViewerListEditor::
viewer() const
{
  return(itemData(currentIndex()));
}

void ViewerListEditor::
setViewer(
  QVariant viewer)
{
  ObjectViewer* viewerobj = qobject_cast <ObjectViewer*>(
      viewer.value<QObject *>());
  if(viewerobj)
  {
    QVariant current = qVariantFromValue<QObject *>(viewerobj->getLinkedTo());
    setCurrentIndex(findData(current, int(Qt::UserRole)));
    removeItem(findData(viewer, int(Qt::UserRole)));
  }
}

void ViewerListEditor::
populateList()
{
  QList<QMdiSubWindow *> windows = m_mdiArea->subWindowList();
  QListIterator<QMdiSubWindow *> i(windows);
  addItem(tr("No link"), qVariantFromValue<QObject *>(NULL));
  while(i.hasNext())
  {
    ObjectViewer* viewer = qobject_cast<ObjectViewer*>(i.next()->widget());
    if(viewer)
    {
      addItem(viewer->windowTitle(), qVariantFromValue<QObject *>(viewer));
    }
  }
}
