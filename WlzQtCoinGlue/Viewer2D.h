#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _Viewer2D_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         Viewer2D.h
* \author	Zsolt Husz
* \date		October 2008
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
* \brief	2D viewer wrapping SoQtPlaneViewer
* \ingroup	UI
*/

#ifndef VIEWER2D_H
#define VIEWER2D_H

#include <QList>
#include <Inventor/Qt/viewers/SoQtPlaneViewer.h>

class SoCamera;
class QObject;
class QLayout;

/*!
* \ingroup	UI
* \brief	Wraping class for SoQtPlaneViewer
*
*		Removes SoQtPlaneViewer Z, Y and Z buttons.
*/
class Viewer2D: public SoQtPlaneViewer
{
  public:
   /*!
    * \ingroup	UI
    * \brief	Constructor
    * \param	parent			parent widget
    * \param  	slider			if exists mixing slider is generated
    * 					and signals will be passed to this
    * 					object
    * \param	buttons			custom buttons at the right handside
    */
    Viewer2D(QWidget * parent, QLayout *slider = NULL, QList <QWidget*> *buttons = NULL);

   /*!
    * \ingroup	UI
    * \brief	Destructor
    * \return	void
    */
    ~Viewer2D();

   /*!
    * \ingroup	UI
    * \brief	Replaces viewing camera and calls Viewer::replaceCamera()
    * 		callback of the parent
    *
    *		Reimplements SoQtPlaneViewer::setCamera().
    * \param	cam			new camera
    */
    void setCamera  ( SoCamera *cam );

  protected:
   /*!
    * \ingroup	UI
    * \brief	Creates view buttons, without adding Z, Y and Z plane
    * 		selection buttons.
    * \param	parent			parent widget
    * \param	buttonList		additional buttons
    */
    virtual void createViewerButtons(QWidget * parent, SbPList * buttonlist);

   /*!
    * \ingroup	UI
    * \brief	Redefines the right trim with and adds mixing scrollbar if
    * 		requested
    * \param	parent			parent widget
    */
    virtual QWidget * buildRightTrim ( QWidget * parent);

  protected:
    QLayout *m_slider;		     	/*!< object to witch mixing slider
					       signals are connected */
    QList <QWidget*> *m_buttons;	     	/*!< other buttons */
};

#endif
