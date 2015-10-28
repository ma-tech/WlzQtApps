#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _Points2DView_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         Points2DView.cpp
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
* \brief        View for 2D convex hull.
* \ingroup      Views
*/

#include "Points2DView.h"
#include "WoolzObject.h"

// Qt includes
#include <QStringList>

// Coin includes
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>

#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDrawStyle.h>


Points2DView::
Points2DView(
  QObject * parent,
  WoolzObject * object):
ObjectView(parent, object)
{
  Q_ASSERT(obj->isPoints() && !obj->is3D());

  m_material = new SoMaterial;
  m_material->ref();

  m_transparency = 0;
  connect(object, SIGNAL(objectVisualisationChange()),
          this, SLOT(objectColourChanged()));

}

Points2DView::
~Points2DView()
{ 
  if (m_material)
  {
    m_material->unref();
  }
}

bool Points2DView::
compatible()
{
 return(obj->isPoints() && obj->is2D());
}

void Points2DView::
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
    WlzPoints		*pd;
    SoCoordinate3 	* nodes;
    WlzErrorNum 	errNum = WLZ_ERR_NONE;

    if(!this->compatible())
    {
	errNum = WLZ_ERR_DOMAIN_TYPE;
    }
    else
    {
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
      nodes = Points2DView::Vertices2D(pd, new SoCoordinate3, errNum);
    }
    if (errNum != WLZ_ERR_NONE || nodes == NULL)
        return ;

    SoDrawStyle *ds = new SoDrawStyle;
    Q_ASSERT(ds);
    ds->style=SoDrawStyle::POINTS;
    ds->pointSize=2.0;
    cache->addChild(ds);
    cache->addChild(nodes);
    obj->setVisualisation(cache);
  }
  root->addChild(cache);
}

SoCoordinate3 * Points2DView::
Vertices2D(
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
      case WLZ_POINTS_2I:
	{
	  WlzIVertex2 *vtx;

	  vtx = pd->points.i2;
	  for(i = 0; i < pd->nPoints; ++i)
	  {
	    vertices->point.set1Value(i, vtx[i].vtX, vtx[i].vtY, 0);
	  }
	}
	break;
      case WLZ_POINTS_2D:
	{
	  WlzDVertex2 *vtx;

	  vtx = pd->points.d2;
	  for(i = 0; i < pd->nPoints; ++i)
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

void Points2DView::
setTransparency(
  int transparency)
{
  ObjectView::setTransparency (transparency);
}

void Points2DView::
objectColourChanged()
{
   m_material->transparency = 0;
   m_material->diffuseColor.setValue(obj->sbColour());
}

QStringList Points2DView::
getVisualisationTypes()
{
  return(QStringList("2D points object"));
}
