#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _LandmarkDragger_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         LandmarkDragger.cpp
* \author       Zsolt Husz
* \date         February 2009
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
* \brief        Dragger superclass
* \ingroup      Controls
*/

#include "LandmarkDragger.h"
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoBlinker.h>

SO_KIT_SOURCE(LandmarkDragger);

void LandmarkDragger::
setType(
  dragger_mode mode)
{
  if((mode >=created) && (mode <= on))
  {
    SoSwitch *sw;
    sw = SO_GET_ANY_PART(this, "materialSwitch", SoSwitch);
    if(sw)
    {
      setSwitchValue(sw, mode);
    }
  }
}

void LandmarkDragger::
setValid(
  bool valid)
{
  SoBlinker *sw;
  sw = SO_GET_ANY_PART(this, "validitySwitch", SoBlinker);
  if(sw)
  {
    sw->on = valid ? false : true;
    if(sw)
    {
      setSwitchValue(sw, 0);  //valid
    }
  }
}

void LandmarkDragger::
setScale(
  float)
{
  //not resizeable
}
