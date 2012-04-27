#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _LandmarkView_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         LandmarkView.cpp
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
* \brief        View for landmarks.
* \ingroup      Views
*
*/
#include "LandmarkView.h"
#include "LandmarkManip.h"
#include "CrossHairManip.h"
#include "SnapSurfaceManip.h"

// Coin includes
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>

LandmarkView::LandmarkView ( LandmarkController *landmarkController, LandmarkModel::IndexType type): View (landmarkController), indexType(type), m_landmarkController (landmarkController) {
  lastLandmarkNode = NULL;
  LandmarkModel *landmarkModel = m_landmarkController->getModel();
  if (type == LandmarkModel::targetV) {
    connect(landmarkController, SIGNAL(addedTargetLandmark(const WlzDVertex3)),
       this, SLOT(addLandmark(const WlzDVertex3)));
    connect(landmarkModel, SIGNAL(movedTargetLandmark(const int, const WlzDVertex3)),
       this, SLOT(movedLandmark(const int, const WlzDVertex3)));
    connect(landmarkModel, SIGNAL(setTargetLandmarkValid(const int, const bool)),
       this, SLOT(setLandmarkValid(const int, const bool)));
  } else {
    connect(landmarkController, SIGNAL(addedSourceLandmark(const WlzDVertex3)),
       this, SLOT(addLandmark(const WlzDVertex3)));
    connect(landmarkModel, SIGNAL(movedSourceLandmark(const int, const WlzDVertex3)),
       this, SLOT(movedLandmark(const int, const WlzDVertex3)));
    connect(landmarkModel, SIGNAL(setSourceLandmarkValid(const int, const bool)),
       this, SLOT(setLandmarkValid(const int, const bool)));
  }

  connect(landmarkModel, SIGNAL(addedLandmarkPair(const int)),
       this, SLOT(addedLandmarkPair(const int)));
  connect(landmarkController, SIGNAL(setHighlight(const int, const bool)),
       this, SLOT(setHighlight(const int, const bool)));

  connect(landmarkModel, SIGNAL(removedLandmark(const int)),
       this, SLOT(removedLandmark(const int)));
  connect(landmarkController, SIGNAL(removedSingleLandmark()),
       this, SLOT(removedSingleLandmark()));

  setVisibility(true);
}

LandmarkView::~LandmarkView ( ) {
   if (lastLandmarkNode) {  //remove if present
     lastLandmarkNode->unref();
     lastLandmarkNode = NULL;
   }
}

void LandmarkView::generateSceneGraph ( bool /*bForce*/ ) {
  //add landarks created in other viewers
  int numberExistingLandmarks = m_landmarkController->getModel()->landmarkNumber();
  int i;
  for (i=0; i<numberExistingLandmarks; i++)
     addedLandmarkPair(i);
  return ;
}

void LandmarkView::addLandmark(const WlzDVertex3 point) {
   if (lastLandmarkNode==NULL) {
        SoSeparator* sep = new SoSeparator;

        LandmarkManip* featurePoint = m_landmarkController->getModel()->get3D() ?
           (LandmarkManip*)(new SnapSurfaceManip(NULL)) :
           (LandmarkManip*)(new CrossHairManip(NULL));

        featurePoint->setView(this);
        featurePoint->translation.setValue(point.vtX, point.vtY, point.vtZ);

        sep->addChild(featurePoint);

        root->addChild(sep);
        root->touch();
        lastLandmarkNode= sep;
        lastLandmarkNode->ref();
   } else {
        SoTransform * translation = (SoTransform *) lastLandmarkNode->getChild(0); //was 0
        Q_ASSERT(translation);
        translation ->translation.setValue(point.vtX, point.vtY, point.vtZ);
   }
}

void LandmarkView::addedLandmarkPair(const int index) {

   if (lastLandmarkNode) {  //remove if temporary view was present
     root->removeChild(lastLandmarkNode);
     lastLandmarkNode->unref();
     lastLandmarkNode = NULL;
   }

   SoSeparator * sep = new SoSeparator;
   LandmarkManip* featurePoint = m_landmarkController->getModel()->get3D() ?
           (LandmarkManip*)(new SnapSurfaceManip(m_landmarkController->getModel()->getPointPair(index))) :
           (LandmarkManip*)(new CrossHairManip(m_landmarkController->getModel()->getPointPair(index)));

   featurePoint->setView(this);
   featurePoint->setOn(false);
   featurePoint->translation.setValue(m_landmarkController->getModel()->getHalfPointPair(index, indexType));
   sep->addChild(featurePoint);

   root->insertChild(sep, index);
   root->touch();
}

void LandmarkView::setHighlight(int index, bool on) {
   SoSeparator *sep= (SoSeparator *)(root->getChild(index));
   LandmarkManip *landmark = (LandmarkManip*)(sep->getChild(0));
   landmark->setOn(on);
}

void LandmarkView::highlighLandmark(LandmarkManip *landmark, bool on ) {
  m_landmarkController->highlight(landmark->pointPair, on);
}

void LandmarkView::removedLandmark(const int index) {
   root->removeChild(index);
}

void LandmarkView::removedSingleLandmark() {
 if (lastLandmarkNode) {  //remove if temporary view was present
     root->removeChild(lastLandmarkNode);
     lastLandmarkNode->unref();
     lastLandmarkNode = NULL;
   }
}

void LandmarkView::movedManipulator(LandmarkManip *landmark) {
    m_landmarkController->move(landmark->pointPair, landmark->translation.getValue(), indexType);
}

void LandmarkView::movedLandmark(const int index, const WlzDVertex3 point) {
   SoSeparator *sep= (SoSeparator *)(root->getChild(index));
   LandmarkManip *landmark = (LandmarkManip*)(sep->getChild(0));
   Q_ASSERT(landmark);
   landmark->translation.setValue(point.vtX, point.vtY, point.vtZ);
}

void LandmarkView::updateLandmarks() {
   for (int index =0;index<root->getNumChildren();index++) {
      SoSeparator *sep= (SoSeparator *)(root->getChild(index));
      LandmarkManip *landmark = (LandmarkManip*)(sep->getChild(0));
      Q_ASSERT(landmark);
      landmark->update();
   }
}

QStringList LandmarkView::getVisualisationTypes () {
  return QStringList("Landmarks");
}

void LandmarkView::setLandmarkValid(int index, const bool on) {
   SoSeparator *sep= (SoSeparator *)(root->getChild(index));
   LandmarkManip *landmark = (LandmarkManip*)(sep->getChild(0));
   landmark->setValid(on);
}
