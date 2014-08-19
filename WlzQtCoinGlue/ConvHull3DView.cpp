#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ConvHull3DView_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ConvHull3DView.cpp
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
* \brief        View for 3D convex hull.
* \ingroup      Views
*
*/

#include "ConvHull3DView.h"
#include "WoolzObject.h"

// Qt includes
#include <QStringList>

// Coin includes
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSelection.h>
//#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoClipPlane.h>
//for PI
#include <math.h>

// Constructors/Destructors
//  

ConvHull3DView::
ConvHull3DView(
  QObject * parent,
  WoolzObject * object):
ObjectView(parent, object),
m_clipOn(false),
m_clipPlane(NULL)
{
  Q_ASSERT(obj->isConvHull() && obj->is3D());

  m_material = new SoMaterial;
  m_material->ref();

  m_transparency = 0;
  connect(object, SIGNAL(objectVisualisationChange()),
          this, SLOT(objectColourChanged()));
  connect(parent, SIGNAL(addedClipPlane(SoClipPlane *)),
          this, SLOT(addedClipPlane(SoClipPlane *)));
}

ConvHull3DView::
~ConvHull3DView ()
{
  if (m_material)
  {
    m_material->unref();
  }
}

bool
ConvHull3DView::
compatible()
{
 return(obj->isConvHull() && obj->is3D());
}

void
ConvHull3DView::
generateSceneGraph(
  bool /*bForce*/)
{
  m_material->diffuseColor.setValue(obj->sbColour());
  updateMaterial();
  root->addChild(m_material);

  SoGroup *cache;
  if(obj->isCachedVisualisation())
  {
     cache = obj->cachedVisualisation();
  }
  else
  {
    cache = new SoGroup;
    WlzConvHullDomain3  *cv;
    SoCoordinate3 * nodes;
    SoIndexedFaceSet * faces;
    WlzErrorNum errNum = WLZ_ERR_NONE;

    if(!this->compatible())
    {
      errNum = WLZ_ERR_DOMAIN_TYPE;
    }
    else
    {
      cv = obj->getObj()->domain.cvh3;
      if(cv == NULL)
      {
	errNum = WLZ_ERR_DOMAIN_NULL;
      }
    }
    if(errNum == WLZ_ERR_NONE)
    {
      nodes = ConvHull3DView::Vertices3D(cv, new SoCoordinate3, errNum);
    }
    if(errNum == WLZ_ERR_NONE)
    {
      faces = ConvHull3DView::Faces3D(cv, new SoIndexedFaceSet);
    }
    if(errNum != WLZ_ERR_NONE || nodes == NULL || faces == NULL)
    {
        return ;
    }

    SoShapeHints *h1= new SoShapeHints;
    Q_ASSERT(h1);
    h1->vertexOrdering = SoShapeHints::CLOCKWISE;
    h1->shapeType = SoShapeHints::UNKNOWN_SHAPE_TYPE;
    h1->creaseAngle = M_PI/2;

    cache->addChild(h1);
    cache->addChild(nodes);
    cache->addChild(faces);
    obj->setVisualisation(cache);
  }
  root->addChild(cache);

  if (m_clipPlane)
  {
   root->insertChild(m_clipPlane, 1);
  }
}

SoCoordinate3 *
ConvHull3DView::
Vertices3D(
  WlzConvHullDomain3 *cv,
  SoCoordinate3 * vertices, 
  WlzErrorNum& errNum )
{
  int           i;

  if(errNum == WLZ_ERR_NONE)
  {
    /* Get the vertices. */
    switch(cv->vtxType)
    {
      case WLZ_VERTEX_I3:
        {
          WlzIVertex3 *vtx;

          vtx = cv->vertices.i3;
          for(i = 0; i < cv->nVertices; ++i)
          {
            vertices->point.set1Value(i, vtx[i].vtX, vtx[i].vtY, vtx[i].vtZ);
          }
        }
        break;
      case WLZ_VERTEX_D3:
        {
          WlzDVertex3 *vtx;

          vtx = cv->vertices.d3;
          for(i = 0; i < cv->nVertices; ++i)
          {
            vertices->point.set1Value(i, vtx[i].vtX, vtx[i].vtY, vtx[i].vtZ);
          }
        }
        break;
      default:
        errNum = WLZ_ERR_DOMAIN_DATA;
        break;
    }
  }
  return(vertices);
}

SoIndexedFaceSet *
ConvHull3DView::
Faces3D(
  WlzConvHullDomain3 *cv,
  SoIndexedFaceSet * faces)
{
  int		i,
  		n,
		f = 0,
		g = 0;

  n = cv->nFaces;
  for(i = 0; i < n; ++i)
  {
    faces->coordIndex.set1Value(f++, cv->faces[g++]);
    faces->coordIndex.set1Value(f++, cv->faces[g++]);
    faces->coordIndex.set1Value(f++, cv->faces[g++]);
    faces->coordIndex.set1Value(f++, -1);
  }
  return(faces);
}

void
ConvHull3DView::
updateMaterial()
{
  if(m_material) //only if scene graph exists  
  {
    m_material->transparency = (double) m_transparency/100.0;
  }
}

void
ConvHull3DView::
setTransparency(
  int transparency)
{
  ObjectView::setTransparency (transparency);
  updateMaterial();
}

void 
ConvHull3DView::
objectColourChanged()
{
  m_material->transparency = (double) m_transparency/100.0;
  m_material->diffuseColor.setValue(obj->sbColour());
}

QStringList
ConvHull3DView::
getVisualisationTypes()
{
  return QStringList("3D convex hull");
}

void
ConvHull3DView::
addedClipPlane(SoClipPlane * plane)
{
  if(m_clipOn)
  {
    root->removeChild(1);
    m_clipOn = false;
  }

  if(m_clipPlane)
  {
    m_clipPlane ->unref();
    m_clipPlane = NULL;
  }
  if(!plane)
  {
    return;
  }
  m_clipPlane = plane;
  m_clipPlane ->ref();

  root->insertChild(plane, 1);
  m_clipOn = true;
}
