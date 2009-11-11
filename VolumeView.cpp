#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _VolumeView_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         VolumeView.cpp
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
* \brief        Volume rendering for 3D oobjects
* \ingroup      Views
*
*/

#include "VolumeView.h"
#include "WoolzObject.h"
#include "TransferFunction.h"

// Qt includes
#include <QStringList>

// Coin includes
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/manips/SoClipPlaneManip.h>
#include <Inventor/fields/SoMFFloat.h>

//simvoleon
#include <VolumeViz/nodes/SoTransferFunction.h>
#include <VolumeViz/nodes/SoVolumeData.h>
#include <VolumeViz/nodes/SoVolumeRender.h>

#include <VolumeViz/nodes/SoObliqueSlice.h>
#include <VolumeViz/nodes/SoTransferFunction.h>

//intialise section transfer function
SoTransferFunction *VolumeView::m_tfSection = NULL;

// Constructors/Destructors
//  
VolumeView::VolumeView ( QObject * parent, WoolzObject * object ): ObjectView(parent, object), m_section(NULL), m_volumerenderSep(NULL), m_data(NULL), m_clipPlane(NULL), m_orthoOn(false)/*, m_clipOn(false)*/ {
  Q_ASSERT(obj->isValue() && obj->is3D());

  m_transparency = 0;
  m_section = new SoSeparator;
  m_section ->ref();
  m_material = new SoMaterial;
  m_material->ref();
  connect(object, SIGNAL(objectVisualisationChange()), this, SLOT(objectColourChanged()));
  connect(parent, SIGNAL(addedClipPlane(SoClipPlane *)), this, SLOT(addedClipPlane(SoClipPlane *)));
}

void VolumeView::freeVolumeData() {
    if (m_data) {
      Alc3Free(m_data);
      m_data = NULL;
    }
}

VolumeView::~VolumeView ( ) { 
    if (m_material)
      m_material->unref();
    if (m_section)
      m_section->unref();
    if (m_volumerenderSep)
      m_volumerenderSep->unref();
    freeVolumeData();
}

bool VolumeView::compatible( ) {
 return obj->isValue() && obj->is3D();
}

/*
  Volume renderer organisation
     0 - SoMaterial
     1 - cache wot SoVolumeData
     2 - SoSeparator
           (0) - SoClipPlane
         1 (1) - TransferFunction
         2 (2) - SoVolumeRenderer
     3 - SoSepator
         0 - SoTransferFunction
         1 - SoObliqueSlice
*/
void VolumeView::generateSceneGraph ( bool /*bForce*/ ) {

  // chacable root
  SoGroup *cache;

  m_material -> diffuseColor.setValue(obj->sbColour());
  updateMaterial();
  root->addChild(m_material);

  //empty object
  if (!obj->getObj()) {
      return;
  }

  if (obj->isCachedVisualisation())
     cache = obj->cachedVisualisation();
  else {
     WlzErrorNum	errNum = WLZ_ERR_NONE;
     WlzIBox3	        bBox = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
     WlzIVertex3	sz = {0.0,0.0,0.0},
                        org = {0.0,0.0,0.0};
     SbVec3s            dim;

     cache = new SoGroup;

     freeVolumeData();

     if(errNum == WLZ_ERR_NONE)
        bBox = WlzBoundingBox3I(obj->getObj(), &errNum);

      /* Get array of data. */
     if(errNum == WLZ_ERR_NONE) {
        sz.vtX = bBox.xMax - bBox.xMin + 1;
        sz.vtY = bBox.yMax - bBox.yMin + 1;
        sz.vtZ = bBox.zMax - bBox.zMin + 1;
        org.vtX = bBox.xMin;
        org.vtY = bBox.yMin;
        org.vtZ = bBox.zMin;
        errNum = WlzToArray3D(&m_data, obj->getObj(), sz, org, 0, WLZ_GREY_UBYTE);
     }
     dim = SbVec3s(sz.vtX, sz.vtY, sz.vtZ);
     uint8_t *voxData;
     voxData = (uint8_t *)**m_data;
     if(voxData!=NULL && errNum == WLZ_ERR_NONE)
     {

        // Add SoVolumeData to scene graph
        SoVolumeData *volData;
        volData = new SoVolumeData();
        volData ->setVolumeData(dim, voxData, SoVolumeData::UNSIGNED_BYTE);
    
        //sets volume location and size
        SbBox3f box( SbVec3f(bBox.xMin, bBox.yMin, bBox.zMin), SbVec3f(bBox.xMax, bBox.yMax, bBox.zMax));
        volData->setVolumeSize(box);
    
        cache->addChild(volData);
        obj->setVisualisation(cache);
    }
   }
  Q_ASSERT(cache);
  root->addChild(cache);

  SoVolumeRender *volRend = new SoVolumeRender();
  volRend->interpolation = SoVolumeRender::LINEAR;
  volRend->composition = SoVolumeRender::ALPHA_BLENDING;

  if (m_volumerenderSep)
      m_volumerenderSep->unref();  // remove previous separator

  m_volumerenderSep = new SoSeparator;
  m_volumerenderSep->ref();
  if (m_clipPlane)
      m_volumerenderSep->addChild(m_clipPlane);
  TransferFunction *transferFunction= obj->transferFunction();
  Q_ASSERT(transferFunction);
  m_volumerenderSep->addChild(transferFunction);
  m_volumerenderSep->addChild(volRend);

  root->addChild(m_volumerenderSep);
  root->addChild(m_section);
  if (m_orthoOn)
      setObliqueSlice(true);
  return ;
}

void VolumeView::addedClipPlane(SoClipPlane * plane){
    if (!m_volumerenderSep) {
        m_clipPlane = plane;
        if (m_clipPlane)
          m_clipPlane ->ref();
        return;
    }

    if (m_clipPlane) {
     m_volumerenderSep->removeChild(0);
     m_clipPlane ->unref();
   }
   if (!plane) {
      m_clipPlane  = NULL;
      m_section->removeAllChildren();  // remove ortho plane
      return;
   }

   bool needOrthoslice = m_orthoOn && !m_clipPlane;
   m_clipPlane = plane;
   m_clipPlane ->ref();

   if (needOrthoslice)
      setObliqueSlice(m_orthoOn);

   m_volumerenderSep->insertChild(plane,0);
}

void VolumeView::updateMaterial() {
  if (m_material) {//only if scene graph exists  
   //avoid sim voleon bug of multiple opaque volumes
   //https://mailman.coin3d.org/pipermail/coin-discuss/2004-July/004571.html
   if (m_transparency==0)
     m_material->transparency = (double) 0.01;
   else
     m_material->transparency = (double) m_transparency/100.0;
  }
}

void VolumeView::setTransparency ( int transparency ) {
  ObjectView::setTransparency (transparency);
  updateMaterial();
}

void VolumeView::objectColourChanged ()   {
   //avoid sim voleon bug of multiple opaque volumes
   //https://mailman.coin3d.org/pipermail/coin-discuss/2004-July/004571.html
   if (m_transparency==0)
     m_material->transparency = (double) 0.01;
   else
     m_material->transparency = (double) m_transparency/100.0;
   m_material->diffuseColor.setValue(obj->sbColour());
}

QStringList VolumeView::getVisualisationTypes () {
  return QStringList("3D volume");
}

void VolumeView::setObliqueSlice(bool on) {
  if (!m_clipPlane)
      return;
  m_orthoOn = on;
  if (on) {
    SoObliqueSlice *ob = new SoObliqueSlice;
    ob->interpolation = SoObliqueSlice::LINEAR;
    ob->alphaUse = SoObliqueSlice::ALPHA_AS_IS;
    ob->plane.connectFrom(&m_clipPlane->plane);

    //generate static transfer function if not yet generated
    if (!m_tfSection) {
      m_tfSection = new SoTransferFunction ;
      m_tfSection->ref();
      m_tfSection->predefColorMap = SoTransferFunction::NONE;
      m_tfSection->colorMapType = SoTransferFunction::LUM_ALPHA;
      for (int ir=0;ir<256;ir++) {
         m_tfSection->colorMap.set1Value(ir*2, ir/255.0f);
//         m_tfSection->colorMap.set1Value(ir*2+1, 0.95f);
         m_tfSection->colorMap.set1Value(ir*2+1, 1.00f);
      }
      WlzPixelV pixel=WlzGetBackground(obj->getObj(),NULL);
/*      if (pixel.type==WLZ_GREY_UBYTE)
        m_tfSection->colorMap.set1Value(pixel.v.ubv*2+1, 0.0f);*/
    }
    m_section->addChild(m_tfSection);
    m_section->addChild(ob);
  } else {
    m_section->removeAllChildren();
  }
}
