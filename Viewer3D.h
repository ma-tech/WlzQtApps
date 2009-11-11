#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _Viewer3D_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         Viewer3D.h
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
* \brief        3D viewer wrapping SoQtExaminerViewer
* \ingroup      UI
*
*/
#ifndef VIEWER3D_H
#define VIEWER3D_H

#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <QList>

class SoCamera;
class QObject;
class QLayout;

/*!
 * \brief	Wraping class for SoQtPlaneViewer
 *
 * \ingroup      UI
 */
class Viewer3D : public SoQtExaminerViewer
{
public:
 /*!
  * \ingroup      UI
  * \brief        Constructor
  * \param        parent parent widget
  * \param        slider, if exists mixing slider is generated and signals will be passed to this object
  *
  * \return       void
  * \par      Source:
  *                Viewer3D.cpp
  */
  Viewer3D(QWidget * parent, QLayout *slider = NULL, QList <QWidget*> *buttons = NULL);

 /*!
  * \ingroup      UI
  * \brief        Destructor
  *
  * \return       void
  * \par      Source:
  *                Viewer3D.cpp
  */
  ~Viewer3D();

 /*!
  * \ingroup      UI
  * \brief        Replaces viewing camera and calls Viewer::replaceCamera() callback of the parent
  * \param        cam new camera
  *
  *               Reimplements SoQtPlaneViewer::setCamera().
  *
  * \return       void
  * \par      Source:
  *                Viewer3D.cpp
  */
  void setCamera  ( SoCamera *cam );

protected:
 /*!
  * \ingroup      UI
  * \brief        Creates view buttons
  * \param        parent parent widget
  * \param        buttonList additional buttons
  *
  * \return       void
  * \par      Source:
  *                Viewer2D.cpp
  */
  virtual void createViewerButtons(QWidget * parent, SbPList * buttonlist);

 /*!
  * \ingroup      UI
  * \brief        Redefines the right trim with and adds mixing scrollbar if requested
  * \param        parent parent widget
  *
  * \return       void
  * \par      Source:
  *                Viewer2D.cpp
  */
  virtual QWidget * buildRightTrim ( QWidget * parent);

protected:
  QLayout *m_slider;                 /*!< object to witch mixing slider signals are connected */
  QList <QWidget*> *m_buttons;       /*!< other buttons*/
};
#endif
