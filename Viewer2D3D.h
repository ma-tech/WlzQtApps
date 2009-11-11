#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _Viewer2D3D_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         Viewer2D3D.h
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
* \brief        Interface for wrapping both View2D and View3D classes
* \ingroup      UI
*
*/
#ifndef VIEWER2D3D_H
#define VIEWER2D3D_H

#include <Inventor/Qt/viewers/SoQtFullViewer.h>

class SoCamera;

/*!
* \brief	Interface class for View2D and View3D
*
* \ingroup      UI
*/
class Viewer2D3D : public SoQtFullViewer
{
public:
  /*!
  * \ingroup      UI
  * \brief        Redefines as public the superclass protected method.
  * \param        cam pointer to the changed camera
  *
  *               Reimplements SoQtFullViewer::changeCameraValues().
  *
  * \return       void
  * \par      Source:
  *                Viewer2D3D.cpp
  */
  Viewer2D3D() :  SoQtFullViewer(*this) {};
  virtual void changeCameraValues(SoCamera * camera);
};

#endif
