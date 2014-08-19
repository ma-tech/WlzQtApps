#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ConvHull2DView_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ConvHull2DView.cpp
* \author       Bill Hill
* \date         August 2014
* \version      $Id$
* \par
* Address:
*               MRC Human Genetics Unit,
*               Western General Hospital,
*               Edinburgh, EH4 2XU, UK.
* \par
* Copyright (C) 2014 Medical research Council, UK.
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
* \brief        View for 2D convex hull.
* \ingroup      Views
*/
#include "ConvHull2DView.h"
#include "WoolzObject.h"

// Qt includes
#include <QStringList>

// Coin includes
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>

#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoDrawStyle.h>


ConvHull2DView::
ConvHull2DView(
  QObject * parent,
  WoolzObject * object):
ObjectView(parent, object)
{
  Q_ASSERT(obj->isConvHull() && !obj->is3D());

  m_material = new SoMaterial;
  m_material->ref();

  m_transparency = 0;
  connect(object, SIGNAL(objectVisualisationChange()),
          this, SLOT(objectColourChanged()));

}

ConvHull2DView::
~ConvHull2DView()
{ 
  if (m_material)
  {
    m_material->unref();
  }
}

bool
ConvHull2DView::
compatible()
{
 return(obj->isConvHull() && obj->is2D());
}

void
ConvHull2DView::
generateSceneGraph(
  bool /*bForce*/)
{
  m_material->diffuseColor.setValue(obj->sbColour());
  root->addChild(m_material);

  SoGroup *cache;
  if (obj->isCachedVisualisation())
  {
     cache = obj->cachedVisualisation();
  }
  else 
  {
    cache = new SoGroup;
    WlzConvHullDomain2	*cv;
    SoCoordinate3 	* nodes;
    SoIndexedLineSet 	* lineset;
    WlzErrorNum 	errNum = WLZ_ERR_NONE;

    if(!this->compatible())
    {
	errNum = WLZ_ERR_DOMAIN_TYPE;
    }
    else
    {
      cv = obj->getObj()->domain.cvh2;
      if(cv == NULL)
      {
	errNum = WLZ_ERR_DOMAIN_NULL;
      }
    }
    if(errNum == WLZ_ERR_NONE)
    {
      /* Check there are verticies! */
      if(cv->nVertices < 1)
      {
	errNum = WLZ_ERR_DOMAIN_DATA;
      }
    }
    if(errNum == WLZ_ERR_NONE)
    {
      nodes = ConvHull2DView::Vertices2D(cv, new SoCoordinate3, errNum);
    }
    if(errNum == WLZ_ERR_NONE)
    {
      lineset = ConvHull2DView::Lines2D(cv, new SoIndexedLineSet);
    }

    if (errNum != WLZ_ERR_NONE || nodes == NULL || lineset == NULL)
        return ;

    SoShapeHints *h1= new SoShapeHints;
    Q_ASSERT(h1);
    h1->vertexOrdering = SoShapeHints::CLOCKWISE;
    h1->shapeType = SoShapeHints::UNKNOWN_SHAPE_TYPE;
    cache->addChild(h1);

    cache->addChild(nodes);
    cache->addChild(lineset);
    obj->setVisualisation(cache);
  }
  root->addChild(cache);
}


SoCoordinate3 *
ConvHull2DView:: Vertices2D(
  WlzConvHullDomain2 *cv,
  SoCoordinate3 * vertices,
  WlzErrorNum& errNum )
{
  int           i;

  if(errNum == WLZ_ERR_NONE)
  {
    /* Get the vertices. */
    switch(cv->vtxType)
    {
      case WLZ_VERTEX_I2:
	{
	  WlzIVertex2 *vtx;

	  vtx = cv->vertices.i2;
	  for(i = 0; i < cv->nVertices; ++i)
	  {
	    vertices->point.set1Value(i, vtx[i].vtX, vtx[i].vtY, 0);
	  }
	}
	break;
      case WLZ_VERTEX_D2:
	{
	  WlzDVertex2 *vtx;

	  vtx = cv->vertices.d2;
	  for(i = 0; i < cv->nVertices; ++i)
	  {
	    vertices->point.set1Value(i, vtx[i].vtX, vtx[i].vtY, 0);
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



SoIndexedLineSet *
ConvHull2DView::
Lines2D(
  WlzConvHullDomain2 *cv,
  SoIndexedLineSet * lineset )
{

  int           i,
		n,
  		f = 0; //face index count

  n = cv->nVertices - 1;
  for(i = 0; i < n; ++i)
  {
    lineset->coordIndex.set1Value(f++, i);
    lineset->coordIndex.set1Value(f++, (i + 1) % n);
    lineset->coordIndex.set1Value(f++, -1);
  }
  return(lineset);
}


void
ConvHull2DView::
setTransparency(
  int transparency)
{
  ObjectView::setTransparency (transparency);
}

void
ConvHull2DView::
objectColourChanged()
{
   m_material->transparency = 0;
   m_material->diffuseColor.setValue(obj->sbColour());
}

QStringList
ConvHull2DView::
getVisualisationTypes()
{
  return(QStringList("2D convex hull"));
}
