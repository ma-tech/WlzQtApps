#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _LandmarkManip_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         LandmarkManip.cpp
* \author       Zsolt Husz
* \date         February 2009
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
* \brief        Manipulator superclass
* \ingroup      Controls
*
*/

// Inventor includes
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/actions/SoHandleEventAction.h>


// Project includes
#include "LandmarkManip.h"
#include "LandmarkDragger.h"
#include "LandmarkView.h"

//  Constructor
LandmarkManip::LandmarkManip(PointPair *pp) :
       pointPair(pp) {
}

void LandmarkManip::handleEvent (SoHandleEventAction *action) {
  // if move is not enabled, then disable all button down actions over the draggers
  const SoEvent * event = action->getEvent();
  if (!(view->isMoveEnabed())) {
    if (event->isOfType(SoMouseButtonEvent ::getClassTypeId())) {
      const SoMouseButtonEvent * mbe = (SoMouseButtonEvent *)action->getEvent();
      if (mbe->getButton() == SoMouseButtonEvent::BUTTON1 &&
        mbe->getState() == SoButtonEvent::DOWN) {
        return ;
      }
    }
  }

  if (pointPair && event->isOfType(SoLocation2Event::getClassTypeId())) {
      const SoPickedPoint * pp = action->getPickedPoint();
      if (pp && pp->getPath()->containsPath(action->getCurPath())) {
        view->highlighLandmark(this, true);
      }
      else {
        view->highlighLandmark(this, false);  // reset highlighting
      }
  }
  // do the default event processing
  SoTransformManip::handleEvent(action);
}


void LandmarkManip::moveAndFinishCB(void *data, SoDragger *) {
 if (data)
    ((LandmarkManip*)data)->view->movedManipulator((LandmarkManip*)data);
}

void LandmarkManip::setOn(bool On) {
    LandmarkDragger *myDrag = (LandmarkDragger *)getDragger();
    Q_ASSERT(myDrag);
    if (On)
      myDrag->setType(LandmarkDragger::on);
    else
      myDrag->setType(LandmarkDragger::off);
}

void LandmarkManip::setValid(bool Valid) {
    LandmarkDragger *myDrag = (LandmarkDragger *)getDragger();
    Q_ASSERT(myDrag);
    myDrag->setValid(Valid);
}

void LandmarkManip::initialiseCallback(SoDragger * dragger) {
   dragger->addFinishCallback(LandmarkManip::moveAndFinishCB, this);
   dragger->addMotionCallback(LandmarkManip::moveAndFinishCB, this);
}
