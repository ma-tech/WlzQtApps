#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _Camera2D_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         Camera2D.h
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
* \brief        Reduced and unequal bounding border camera to maximise image with full zoom.
* \ingroup      Visualisation
*
*/
#ifndef CAMERA2D_H
#define CAMERA2D_H

#include <Inventor/nodes/SoOrthographicCamera.h>

/*! 
* \brief	2D camera for texture rendered image visualisation.It avoids 
*               empty bounding regions around the image
* \ingroup      Visualisation
*/
class Camera2D : public SoOrthographicCamera
{
public:
  /*!
  * \ingroup      Visualisation
  * \brief        Constructor
  * \return       void
  * \par      Source:
  *                Camera2D.cpp
  */
  Camera2D();

  /*!
  * \ingroup      Visualisation
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *                Camera2D.cpp
  */
   ~Camera2D();

  /*!
  * \ingroup      Visualisation
  * \brief        Convenience method for setting up the camera definition to tightly cover 2D domain.
  *
  * Multiplies the exact dimensions with a slack factor to have 
  * some space between the rendered model and the borders of the rendering area. 
  * No bounding sphere is used, therefore unequal top/bottom and side borders are allowed.
  *
  *           Reimplemented from SoCamera::viewBoundingBox().
  * \param        box bounding box
  * \param        aspect  aspect ratio
  * \param        slack slack
  * \return       void
  * \par      Source:
  *                Camera2D.cpp
  */
  void viewBoundingBox(const SbBox3f & box, float aspect, float slack);
};

#endif
