#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _VolumeView_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         VolumeView.cpp
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
* \brief        Volume rendering for 3D oobjects
* \ingroup      Views
*/

#include "VolumeView.h"
#include "WoolzObject.h"
#include "TransferFunction.h"

// Qt includes
#include <QStringList>

// Coin includes
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoScale.h>
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

#ifdef _32BITWARP
const long VolumeView::maxVoxels = 256 * 256 * 256;
#else
const long VolumeView::maxVoxels = 512 * 512 * 512;
#endif


// Constructors/Destructors
//  
VolumeView::
VolumeView(
  QObject * parent,
  WoolzObject * object):
ObjectView(
  parent,
  object),
m_section(NULL),
m_volumerenderSep(NULL),
m_data(NULL),
m_clipPlane(NULL),
m_orthoOn(false)
{
  Q_ASSERT(obj->isValue() && obj->is3D());

  m_transparency = 0;
  m_section = new SoSeparator;
  m_section ->ref();
  m_material = new SoMaterial;
  m_material->ref();
  connect(object, SIGNAL(objectVisualisationChange()),
          this, SLOT(objectColourChanged()));
  connect(parent, SIGNAL(addedClipPlane(SoClipPlane *)),
          this, SLOT(addedClipPlane(SoClipPlane *)));
}

void VolumeView::
freeVolumeData()
{
  if(m_data)
  {
    Alc3Free(m_data);
    m_data = NULL;
  }
}

VolumeView::
~VolumeView()
{ 
  if(m_material)
  {
    m_material->unref();
  }
  if(m_section)
  {
    m_section->unref();
  }
  if(m_volumerenderSep)
  {
    m_volumerenderSep->unref();
  }
  freeVolumeData();
}

bool VolumeView::
compatible()
{
 return(obj->isValue() && obj->is3D());
}

/*
  Volume renderer organisation
     0 - SoMaterial
     1 - cache:
         (0) SoScale
         (1) SoVolumeData
     2 - SoSeparator
           - (SoClipPlane)
           - (SoScale)
           - TransferFunction
           - SoVolumeRenderer
     3 - SoSepator
         ()- SoScale
         0 - SoTransferFunction
         1 - SoObliqueSlice
*/
void VolumeView::
generateSceneGraph(
  bool /*bForce*/)
{

  // cachable root
  SoGroup *cache;

  m_material->diffuseColor.setValue(obj->sbColour());
  updateMaterial();
  root->addChild(m_material);

  //empty object
  if(!obj->getObj())
  {
    return;
  }

  if(obj->isCachedVisualisation())
  {
    cache = obj->cachedVisualisation();
    m_scaleFactor = obj->cachedVisualisationScale();
  }
  else
  {
    WlzIBox3	bBox = {0, 0, 0, 0, 0, 0};
    WlzIVertex3	sz = {0, 0, 0},
		org = {0, 0, 0};
    SbVec3s     dim;
    WlzObject *sampledObj = NULL;
    WlzErrorNum	errNum = WLZ_ERR_NONE;

    cache = new SoGroup;
    freeVolumeData();

    bBox = WlzBoundingBox3I(obj->getObj(), &errNum);
    if(errNum == WLZ_ERR_NONE)
    {
      sz.vtX = bBox.xMax - bBox.xMin + 1;
      sz.vtY = bBox.yMax - bBox.yMin + 1;
      sz.vtZ = bBox.zMax - bBox.zMin + 1;
      m_scaleFactor = 1;
      if(maxVoxels < (sz.vtX * sz.vtY * sz.vtZ))
      {
	//do subsampling
	m_scaleFactor = ceil(pow(((double)sz.vtX * sz.vtY * sz.vtZ) / maxVoxels,
				 1.0f/3.0f));
	if((m_scaleFactor % 2) == 1)
	{
	  m_scaleFactor++; // make scale factor even
	}
	//TODO: the above line fixes some crashes, however is not understood why
	//other memory allocation problems may exists that cause the above crash

	Q_ASSERT(m_scaleFactor > 1);
	double scale = 1.0 / m_scaleFactor;
	WlzAffineTransform *trans =
	    WlzAffineTransformFromScale(WLZ_TRANSFORM_3D_AFFINE,
					scale, scale, scale, &errNum);
	if(errNum == WLZ_ERR_NONE)
	{
	  sampledObj = WlzAffineTransformObj(obj->getObj(),
					     trans, WLZ_INTERPOLATION_NEAREST,
					     &errNum);
	}
	WlzFreeAffineTransform(trans);
	if(errNum == WLZ_ERR_NONE)
	{
	  sampledObj = WlzAssignObject(sampledObj, &errNum);
	  if(errNum == WLZ_ERR_NONE)
	  {
	    bBox = WlzBoundingBox3I(sampledObj, &errNum);
	    sz.vtX = bBox.xMax - bBox.xMin + 1;
	    sz.vtY = bBox.yMax - bBox.yMin + 1;
	    sz.vtZ = bBox.zMax - bBox.zMin + 1;
	  }
	}
      }
      else
      {
	sampledObj = WlzAssignObject(obj->getObj(), &errNum);
      }
      org.vtX = bBox.xMin;
      org.vtY = bBox.yMin;
      org.vtZ = bBox.zMin;
    }

    /* Get array of data. */
    if(errNum == WLZ_ERR_NONE)
    {
      WlzGreyType greyType = obj->getWoolzGreyType();
      bool isUBYTE = (greyType == WLZ_GREY_UBYTE);

      if(isUBYTE || (sampledObj->values.core == NULL))
      {
	errNum = WlzToArray3D(&m_data, sampledObj, sz, org, 0, WLZ_GREY_UBYTE);
      }
      else
      {
        // convert to UBYTE
	WlzPixelV  greyMinV,
		   greyMaxV,
		   newGreyMinV,
		   newGreyMaxV;
	WlzObject  *cpyobj = NULL;
	errNum = WlzGreyRange(sampledObj, &greyMinV, &greyMaxV);
	if(errNum == WLZ_ERR_NONE)
	{
	  newGreyMinV.type = WLZ_GREY_UBYTE;
	  newGreyMaxV.type = WLZ_GREY_UBYTE;
	  newGreyMinV.v.ubv = 0;
	  newGreyMaxV.v.ubv = 255;

	  WlzValueConvertPixel(&newGreyMinV, newGreyMinV, greyType);
	  WlzValueConvertPixel(&newGreyMaxV, newGreyMaxV, greyType);
	  cpyobj= WlzCopyObject(sampledObj, &errNum);
	}
	if(errNum == WLZ_ERR_NONE)
	{
	  errNum = WlzGreySetRange(cpyobj, greyMinV, greyMaxV, newGreyMinV,
	                           newGreyMaxV, 0);
	}
	if(errNum == WLZ_ERR_NONE)
	{
	  errNum = WlzToArray3D(&m_data, cpyobj, sz, org, 0, WLZ_GREY_UBYTE );
	}
	if(cpyobj)
	{
	  WlzFreeObj(cpyobj);
	}
      }
    }
    if(sampledObj)
    {
      WlzFreeObj(sampledObj);
    }
    dim = SbVec3s(sz.vtX, sz.vtY, sz.vtZ);
    uint8_t *voxData = NULL;
    if((m_data != NULL) && (errNum == WLZ_ERR_NONE))
    {
      voxData = (uint8_t *)**m_data;
      // Add SoVolumeData to scene graph
      SoVolumeData *volData;
      volData = new SoVolumeData();
      volData->setVolumeData(dim, voxData, SoVolumeData::UNSIGNED_BYTE);
      //sets volume location and size
      SbBox3f box(SbVec3f(bBox.xMin, bBox.yMin, bBox.zMin),
                  SbVec3f(bBox.xMax, bBox.yMax, bBox.zMax));
      volData->setVolumeSize(box);

      cache->addChild(volData);
      obj->setVisualisation(cache, m_scaleFactor);
    }
  }
  Q_ASSERT(cache);
  root->addChild(cache);

  SoVolumeRender *volRend = new SoVolumeRender();
  volRend->interpolation = SoVolumeRender::NEAREST;  // linear is not good,
  						     // if object noisy
  volRend->composition = SoVolumeRender::ALPHA_BLENDING;

  if (m_volumerenderSep)
  {
    m_volumerenderSep->unref();  // remove previous separator
  }

  m_volumerenderSep = new SoSeparator;
  m_volumerenderSep->ref();
  if(m_clipPlane)
  {
    m_volumerenderSep->addChild(m_clipPlane);
  }

  if(m_scaleFactor!=1)
  {
    SoScale *scale= new SoScale();
    scale->scaleFactor.setValue((float)m_scaleFactor,
                                (float)m_scaleFactor, (float)m_scaleFactor);
    m_volumerenderSep->addChild(scale);
  }
  TransferFunction *transferFunction= obj->transferFunction();
  Q_ASSERT(transferFunction);
  m_volumerenderSep->addChild(transferFunction);
  m_volumerenderSep->addChild(volRend);
  root->addChild(m_volumerenderSep);
  root->addChild(m_section);
  if (m_orthoOn)
  {
    setObliqueSlice(true);
  }
}

void VolumeView::
addedClipPlane(
  SoClipPlane * plane)
{
  if(!m_volumerenderSep)
  {
    m_clipPlane = plane;
    if(m_clipPlane)
    {
      m_clipPlane ->ref();
    }
    return;
  }
  if(m_clipPlane)
  {
    m_volumerenderSep->removeChild(0);
    m_clipPlane ->unref();
  }
  if(!plane)
  {
    m_clipPlane  = NULL;
    m_section->removeAllChildren();  // remove ortho plane
    return;
  }

  bool needOrthoslice = m_orthoOn && !m_clipPlane;
  m_clipPlane = plane;
  m_clipPlane->ref();

  if(needOrthoslice)
  {
    setObliqueSlice(m_orthoOn);
  }

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

void VolumeView::
setTransparency(
  int transparency)
{
  ObjectView::setTransparency (transparency);
  updateMaterial();
}

void VolumeView::
objectColourChanged()
{
  //avoid sim voleon bug of multiple opaque volumes
  //https://mailman.coin3d.org/pipermail/coin-discuss/2004-July/004571.html
  if(m_transparency == 0)
  {
    m_material->transparency = (double )0.01;
  }
  else
  {
    m_material->transparency = (double )m_transparency/100.0;
  }
  m_material->diffuseColor.setValue(obj->sbColour());
}

QStringList VolumeView::
getVisualisationTypes()
{
  return(QStringList("3D volume"));
}

void VolumeView::
setObliqueSlice(bool on)
{
  if(!m_clipPlane)
  {
    return;
  }
  m_orthoOn = on;
  if(on)
  {
    SoObliqueSlice *ob = new SoObliqueSlice;
    ob->interpolation = SoObliqueSlice::LINEAR;
    ob->alphaUse = SoObliqueSlice::ALPHA_AS_IS;
    ob->plane.connectFrom(&m_clipPlane->plane);

    //generate static transfer function if not yet generated
    if(!m_tfSection)
    {
      m_tfSection = new SoTransferFunction ;
      m_tfSection->ref();
      m_tfSection->predefColorMap = SoTransferFunction::NONE;
      m_tfSection->colorMapType = SoTransferFunction::LUM_ALPHA;
      m_tfSection->colorMap.set1Value(511, 1.00f); //set color map size to 512
      if(obj && obj->isValueSet())
      {
	for(int ir=0; ir < 256; ++ir)
	{
	  m_tfSection->colorMap.set1Value(ir*2, ir/255.0f);
	  m_tfSection->colorMap.set1Value(ir*2+1, 1.00f);
	}
      }
      else
      {
	m_tfSection->colorMap.set1Value(0, 0.00f); // background is black
	m_tfSection->colorMap.set1Value(1, 1.00f);
	m_tfSection->colorMap.set1Value(2, 1.00f); // foreground is white
	m_tfSection->colorMap.set1Value(3, 1.00f);
      }
    }
    if(m_scaleFactor != 1)
    {
      SoScale *scale= new SoScale();
      scale->scaleFactor.setValue((float)m_scaleFactor,
                                  (float)m_scaleFactor, (float)m_scaleFactor);
      m_section->addChild(scale);
    }
    m_section->addChild(m_tfSection);
    m_section->addChild(ob);
  }
  else
  {
    m_section->removeAllChildren();
  }
}

QString VolumeView::
getProperties()
{
  return(QString("Scale factor: %1").arg(m_scaleFactor));
}
