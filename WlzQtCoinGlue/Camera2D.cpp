#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _Camera2D_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         Camera2D.cpp
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
* \brief        Reduced and unequal bounding border camera to maximise image
* 		with full zoom.
* \ingroup      Visualisation
*/

#include "Camera2D.h"

Camera2D::
Camera2D():
SoOrthographicCamera()
{
}

Camera2D::
~Camera2D()
{
}

// mainly copied from SoOrthographicCamera, however without bounding
// sphere computation
void Camera2D::
viewBoundingBox(
  const SbBox3f & box,
  float aspect,
  float slack)
{
  // First, we want to move the camera in such a way that it is
  // pointing straight at the center of the scene bounding box -- but
  // without modifiying the rotation value (so we can't use
  // SoCamera::pointAt()).
  SbVec3f cameradirection;
  this->orientation.getValue().multVec(SbVec3f(0, 0, -1), cameradirection);
  this->position.setValue(box.getCenter() + -cameradirection);

  float sizeX, sizeY, sizeZ;
  box.getSize(sizeX, sizeY, sizeZ);

  // Make sure that everything will still be inside the viewing volume
  // even if the aspect ratio "favorizes" width over height.
  float dim= sizeX > sizeY ? sizeX : sizeY;

  if(aspect < 1.0f)
  {
    this->height = dim/ aspect;
  }
  else
  {
    this->height = dim;
  }

  // Move the camera to the edge of the bounding sphere, while still
  // pointing at the scene.
  SbVec3f direction = this->position.getValue() - box.getCenter();
  (void) direction.normalize(); // we know this is not a null vector
  this->position.setValue(box.getCenter() + direction  /* * radius*/);

  // Set up the clipping planes according to the slack value (a value
  // of 1.0 will yield clipping planes that are tangent to the
  // bounding sphere of the scene).
  float distance_to_midpoint =
    (this->position.getValue() - box.getCenter()).length();
  this->nearDistance = distance_to_midpoint - dim/2 *  slack;
  this->farDistance = distance_to_midpoint +  dim/2 *  slack;

  // The focal distance is simply the distance from the camera to the
  // scene midpoint. This field is not used in rendering, its just
  // provided to make it easier for the user to do calculations based
  // on the distance between the camera and the scene.
  this->focalDistance = distance_to_midpoint;
}
