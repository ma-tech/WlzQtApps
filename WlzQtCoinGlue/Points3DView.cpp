#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _Points2DView_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         Points3DView.cpp
* \author       Bill Hill
* \date         October 2015
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
* \brief        View for 3D convex hull.
* \ingroup      Views
*/

#include "Points3DView.h"
#include "WoolzObject.h"

// Qt includes
#include <QStringList>

// Coin includes
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoClipPlane.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoMarkerSet.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoLineSet.h>


Points3DView::
Points3DView(
  QObject * parent,
  WoolzObject * object):
ObjectView(parent, object)
{
  Q_ASSERT(obj->isPoints() && obj->is3D());

  m_material = new SoMaterial;
  m_material->ref();

  m_transparency = 0;
  connect(object, SIGNAL(objectVisualisationChange()),
          this, SLOT(objectColourChanged()));

}

Points3DView::
~Points3DView()
{ 
  if(m_material)
  {
    m_material->unref();
  }
}

bool Points3DView::
compatible()
{
 return(obj->isPoints() && obj->is3D());
}

void Points3DView::
generateSceneGraph(
  bool /*bForce*/)
{
  m_material->diffuseColor.setValue(obj->sbColour());
  root->addChild(m_material);

  SoGroup *cache;
  if(obj->isCachedVisualisation())
  {
     cache = obj->cachedVisualisation();
  }
  else 
  {
    WlzPoints		*pd;
    SoCoordinate3 *	nodes;
    SoDrawStyle *       drawStyle;
    WlzErrorNum 	errNum = WLZ_ERR_NONE;

    if(!this->compatible())
    {
	errNum = WLZ_ERR_DOMAIN_TYPE;
    }
    else
    {
      cache = new SoGroup;
      pd = obj->getObj()->domain.pts;
      if(pd == NULL)
      {
	errNum = WLZ_ERR_DOMAIN_NULL;
      }
    }
    if(errNum == WLZ_ERR_NONE)
    {
      /* Check there are points! */
      if(pd->nPoints < 1)
      {
	errNum = WLZ_ERR_DOMAIN_DATA;
      }
    }
    if(errNum == WLZ_ERR_NONE)
    {
      drawStyle = new SoDrawStyle();
      nodes = Points3DView::Vertices3D(pd, new SoCoordinate3, errNum);
    }
    if(errNum != WLZ_ERR_NONE || nodes == NULL || drawStyle == NULL)
        return ;

    drawStyle->style = SoDrawStyle::POINTS;
    m_points = new SoMarkerSet();
    Q_ASSERT(m_points);
    SoMFInt32 markerIndex;
    markerIndex.setValue(SoMarkerSet::CIRCLE_FILLED_5_5);
    m_points->markerIndex = markerIndex;
    m_points->vertexProperty.setValue(nodes);
    m_points->numPoints.setValue(pd->nPoints);
    cache->addChild(drawStyle);
    cache->addChild(nodes);
    cache->addChild(new SoLineSet());
    cache->addChild(m_points);
    obj->setVisualisation(cache);
  }
  root->addChild(cache);
}

SoCoordinate3 * Points3DView::
Vertices3D(
  WlzPoints *pd,
  SoCoordinate3 * vertices,
  WlzErrorNum& errNum )
{
  int           i;

  if(errNum == WLZ_ERR_NONE)
  {
    /* Get the vertices. */
    switch(pd->type)
    {
      case WLZ_POINTS_3I:
	{
	  WlzIVertex3 *vtx;

	  vtx = pd->points.i3;
	  for(i = 0; i < pd->nPoints; ++i)
	  {
	    vertices->point.set1Value(i, vtx[i].vtX, vtx[i].vtY, vtx[i].vtZ);
	  }
	}
	break;
      case WLZ_POINTS_3D:
	{
	  WlzDVertex3 *vtx;

	  vtx = pd->points.d3;
	  for(i = 0; i < pd->nPoints; ++i)
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

void Points3DView::
updateMaterial()
{
  if(m_material) //only if scene graph exists  
  {
    m_material->transparency = (double) m_transparency/100.0;
  }
}

void Points3DView::
setTransparency(
  int transparency)
{
  ObjectView::setTransparency(transparency);
  updateMaterial();
}

void Points3DView::
objectColourChanged()
{
   m_material->transparency = 0;
   m_material->diffuseColor.setValue(obj->sbColour());
}

void Points3DView::
addedClipPlane(SoClipPlane * plane)
{
  if(m_clipOn)
  {
    root->removeChild(1);
    m_clipOn = false;
  }
  if(m_clipPlane)
  {
    m_clipPlane->unref();
    m_clipPlane = NULL;
  }
  if(plane)
  {
    m_clipPlane = plane;
    m_clipPlane->ref();
    root->insertChild(plane, 1);
    m_clipOn = true;
  }
}

QStringList Points3DView::
getVisualisationTypes()
{
  return(QStringList("3D points object"));
}
