#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _ImageView_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         ImageView.cpp
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
* \brief        View for 2D image as texture mapped rectangle.
* \ingroup      Views
*/

#include "ImageView.h"
#include "WoolzObject.h"

// Qt includes
#include <QStringList>

// Coin includes
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoTextureScalePolicy.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoTexture2Transform.h>

// Other
#include <math.h>

ImageView::
ImageView(
  QObject * parent,
  WoolzObject * object):
ObjectView(
  parent,
  object)
{
  Q_ASSERT(obj->isValue() && !obj->is3D());

  if(obj->getObj()->values.core)
  {
    //has value table
    WlzGreyType greyType;      	/*!< texture which visualises the object */

    WlzErrorNum   errNum =  WLZ_ERR_NONE;
    greyType = WlzGreyTypeFromObj(obj->getObj(), &errNum);
    m_chanels = 0;
    if (errNum == WLZ_ERR_NONE)
    {
      switch(greyType)
      {
	case WLZ_GREY_UBYTE:
	case WLZ_GREY_SHORT:
	case WLZ_GREY_INT:
	  m_chanels = 2;
	  break;
	case WLZ_GREY_RGBA:
	  m_chanels = 4;
	  break;
	default:
	  // m_chanels = 0;
	  break;
      }
    }
  }
  else
  {
    m_chanels = 2;
  }
  m_material = new SoMaterial;
  Q_ASSERT(m_material);
  m_material->ref();
  m_transparency = 0;
  connect(object, SIGNAL(objectVisualisationChange()),
          this, SLOT(objectColourChanged()));
}

bool ImageView::
compatible()
{
 return(obj->isValue() && !obj->is3D());
}

ImageView::~ImageView()
{ 
  if (m_material)
  {
    m_material->unref();
  }
}

WlzErrorNum ImageView::convertObjToArray(void * buffer, WlzObject* obj)
{
  int           i;          //counters
  int           width;
  WlzIntervalWSpace iwsp;
  WlzGreyWSpace gwsp;
  WlzGreyType   gType = WLZ_GREY_ERROR;
  WlzErrorNum   errNum = WLZ_ERR_NONE;
  unsigned char *cbuffer=(unsigned char*)buffer;

  int bufi,bufj;     // buffer counters
  int firsti,firstj; // first line and columns
  if(!obj)
  {
    return WLZ_ERR_OBJECT_NULL;
  }
  if(obj->type != WLZ_2D_DOMAINOBJ)
  {
    return WLZ_ERR_OBJECT_TYPE;
  }
  width = obj->domain.i->lastkl - obj->domain.i->kol1 + 1;
  firstj = obj->domain.i->line1;
  firsti = obj->domain.i->kol1;
  if(obj->values.core)
  {
    gType = WlzGreyTypeFromObj(obj, &errNum);
    //scan the object
    if((errNum = WlzInitGreyScan(obj, &iwsp, &gwsp)) == WLZ_ERR_NONE)
    {
      while((errNum = WlzNextGreyInterval(&iwsp)) == WLZ_ERR_NONE)
      {
	WlzUInt val;
	bufj=iwsp.linpos-firstj;
	switch(gType)
	{
	  case WLZ_GREY_ERROR:   // if has no value table
	    for(i=iwsp.lftpos; i <= iwsp.rgtpos; i++){
	      bufi=i-firsti;
	      cbuffer[(bufi+bufj*width)*2] = 255;
	      cbuffer[(bufi+bufj*width)*2+1] = 255;
	    }
	    break;
	  case WLZ_GREY_UBYTE:
	    for(i=iwsp.lftpos; i <= iwsp.rgtpos; i++){
	      bufi=i-firsti;
	      cbuffer[(bufi+bufj*width)*2] =
	          gwsp.u_grintptr.ubp[i-iwsp.lftpos];
	      cbuffer[(bufi+bufj*width)*2+1] = 255;
	    }
	    break;
	  case WLZ_GREY_SHORT:
	    for(i=iwsp.lftpos; i <= iwsp.rgtpos; i++){
	      bufi=i-firsti;
	      cbuffer[(bufi+bufj*width)*2] =
	          gwsp.u_grintptr.shp[i-iwsp.lftpos];
	      cbuffer[(bufi+bufj*width)*2+1] = 255;
	    }
	    break;
	  case WLZ_GREY_INT:
	    for(i=iwsp.lftpos; i <= iwsp.rgtpos; i++){
	      bufi=i-firsti;
	      cbuffer[(bufi+bufj*width)*2] =
	          gwsp.u_grintptr.inp[i-iwsp.lftpos];
	      cbuffer[(bufi+bufj*width)*2+1] = 255;
	    }
	    break;

	  case WLZ_GREY_RGBA:
	    for(i=iwsp.lftpos; i <= iwsp.rgtpos; i++){
	      bufi=i-firsti;
	      val = gwsp.u_grintptr.rgbp[i-iwsp.lftpos];
	      cbuffer[(bufi+bufj*width)*4] = WLZ_RGBA_RED_GET(val);
	      cbuffer[(bufi+bufj*width)*4+1] = WLZ_RGBA_GREEN_GET(val);
	      cbuffer[(bufi+bufj*width)*4+2] = WLZ_RGBA_BLUE_GET(val);
	      cbuffer[(bufi+bufj*width)*4+3] = WLZ_RGBA_ALPHA_GET(val);
	    }
	    break;
	  default:
	    return WLZ_ERR_GREY_TYPE;
	}
      }
      if(errNum == WLZ_ERR_EOO)
      {
        errNum = WLZ_ERR_NONE;
      }
    }
  }
  else
  {
    //scan the object
    if((errNum = WlzInitRasterScan(obj, &iwsp,
                                   WLZ_RASTERDIR_ILIC)) == WLZ_ERR_NONE)
    {
      while((errNum = WlzNextInterval(&iwsp)) == WLZ_ERR_NONE)
      {
	bufj=iwsp.linpos-firstj;
	for(i=iwsp.lftpos; i <= iwsp.rgtpos; i++)
	{
	  bufi=i-firsti;
	  cbuffer[(bufi+bufj*width)*2] = 255;
	  cbuffer[(bufi+bufj*width)*2+1] = 255;
	}
      }
      if(errNum == WLZ_ERR_EOO)
      {
	errNum = WLZ_ERR_NONE;
      }
    }
  }
  return(errNum) ;
}


void ImageView::
generateSceneGraph(bool /*bForce*/)
{
  //if not grey or RGB then return
  if (m_chanels!=2 && m_chanels!=4)
  {
    return;
  }

  SoGroup *cache;
  m_material->diffuseColor.setValue(obj->sbColour());
  updateMaterial();
  root->addChild(m_material);
  SoShapeHints * hints = new SoShapeHints;
  hints->vertexOrdering = SoShapeHints::CLOCKWISE;
  hints->shapeType = SoShapeHints::SOLID;
  root->addChild(hints);

  if (obj->isCachedVisualisation())
  {
    cache = obj->cachedVisualisation();
  }
  else
  {
    cache = new SoGroup;
    WlzIBox2       box = WlzBoundingBox2I(obj->getObj(), NULL);

    void          *imgDat = NULL;
    WlzIVertex2   sz;
    sz.vtX = box.xMax - box.xMin + 1;
    sz.vtY = box.yMax - box.yMin + 1;

    imgDat = malloc ( sz.vtX * sz.vtY * m_chanels);
    memset(imgDat, 0, sz.vtX * sz.vtY * m_chanels);

    convertObjToArray(imgDat , obj->getObj());

    SoTexture2 *texture;           	// texture which visualises the object
    texture = new SoTexture2;
    texture->model = SoTexture2::MODULATE;//BLEND;

    if(imgDat && sz.vtX*sz.vtY<10000000)
    {
      texture->image.setValue(SbVec2s(sz.vtX, sz.vtY), m_chanels,
                              (const unsigned char*)imgDat);
    }
    objectColourChanged();
    SoTextureScalePolicy * pcy = new SoTextureScalePolicy;
    pcy->policy = SoTextureScalePolicy::FRACTURE;  // disable subsampling and
                                                   // shown non 2^n textures
    cache->addChild(pcy);
    cache->addChild(texture);
    if (sz.vtY>=sz.vtX) {
      SoTexture2Transform * texturetrans = 
	new SoTexture2Transform; // different Woolz <> Coin3D texture
	                         // representation
      texturetrans->rotation = M_PI/2;
      texturetrans->scaleFactor.setValue(1,-1);
      cache->addChild(texturetrans);
    } 
    SoCoordinate3 * c3 = new SoCoordinate3;
    c3->point.setNum(4);
    SbVec3f * p = c3->point.startEditing();
    p[0] = SbVec3f(box.xMax, box.yMin, 0.0f);
    p[1] = SbVec3f(box.xMax, box.yMax, 0.0f);
    p[2] = SbVec3f(box.xMin, box.yMax, 0.0f);
    p[3] = SbVec3f(box.xMin, box.yMin, 0.0f);
    c3->point.finishEditing();
    cache->addChild(c3);
    SoFaceSet * fs = new SoFaceSet;
    fs->numVertices = 4;
    cache->addChild(fs);
    obj->setVisualisation(cache);
    free(imgDat);
  }
  root->addChild(cache);
}

void ImageView::
updateMaterial()
{
  if(m_material)
  {
    //only if scene graph exists
    m_material->transparency = (double )m_transparency/100.0;
  }
}

void ImageView::
setTransparency(
  int transparency)
{
  ObjectView::setTransparency(transparency);
  updateMaterial();
}

void ImageView::
objectColourChanged()
{
   m_material->transparency = (double )m_transparency/100.0;
   m_material->diffuseColor.setValue(obj->sbColour());
   m_material->ambientColor.setValue(SbColor(0,0,0));
}

QStringList ImageView::
getVisualisationTypes()
{
  return QStringList("2D image");
}
