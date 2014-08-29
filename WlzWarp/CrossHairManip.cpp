#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _CrossHairManip_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         CrossHairManip.cpp
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
* \brief        2D manipulator
* \ingroup      Controls
*/

//project inlcudes
#include "CrossHairManip.h"
#include "CrossHairDragger.h"
#include "LandmarkView.h"
#include <QSettings>

SO_NODE_SOURCE(CrossHairManip);

void CrossHairManip::
initClass()
{
  if(CrossHairDragger::getClassTypeId().isBad())
  {
    CrossHairDragger::initClass();
  }
  SO_NODE_INIT_CLASS(CrossHairManip, SoTransformManip, "CrossHairManip");
}

//  Constructor
CrossHairManip::
CrossHairManip(
  PointPair *pp):
LandmarkManip(pp)
{
  SO_NODE_CONSTRUCTOR(CrossHairManip);

  CrossHairDragger    *myDrag = new CrossHairDragger ;
  initialiseCallback(myDrag);
  QSettings settings;
  setDragger(myDrag);
  update();
}

CrossHairManip::
~CrossHairManip()
{
}


void CrossHairManip::update()
{
  QSettings settings;
  LandmarkDragger *myDrag = (LandmarkDragger *)getDragger();
  myDrag->setScale(settings.value("markers/2D/size", 1.00).value<float>());
}
