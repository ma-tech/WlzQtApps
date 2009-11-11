#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _SnapSurfaceManip_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         SnapSurfaceManip.cpp
* \author       Zsolt Husz
* \date         July 2009
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
* \brief        3D manipulator
* \ingroup      Controls
*
* Code is based on an example from the
* Inventor Toolmaker, chapter 8.
*/

//project inlcudes
#include "SnapSurfaceDragger.h"
#include "SnapSurfaceManip.h"
#include "LandmarkView.h"
#include "CrossHairDragger.h"
#include <QSettings>

SO_NODE_SOURCE(SnapSurfaceManip);

//  Initializes the type ID for this manipulator node. This
//  should be called once after SoInteraction::init().
//  and after TranslateRadialDragger::initClass()
void SnapSurfaceManip::initClass() {
    if (SnapSurfaceDragger::getClassTypeId().isBad())
        SnapSurfaceDragger::initClass();

   SO_NODE_INIT_CLASS(
      SnapSurfaceManip, SoTransformManip, "SnapSurfaceManip");
}

//  Constructor
SnapSurfaceManip::SnapSurfaceManip(PointPair *pp) :
  LandmarkManip (pp) {
   SO_NODE_CONSTRUCTOR(SnapSurfaceManip);
   SnapSurfaceDragger *myDrag = new SnapSurfaceDragger;
   initialiseCallback(myDrag);
   setDragger(myDrag);
   update();
}

SnapSurfaceManip::~SnapSurfaceManip() {
}

void SnapSurfaceManip::update() {
  QSettings settings;
  LandmarkDragger *myDrag = (LandmarkDragger *)getDragger();
  myDrag->setScale(settings.value("markers/3D/size", 1.00).value<float>());
}
