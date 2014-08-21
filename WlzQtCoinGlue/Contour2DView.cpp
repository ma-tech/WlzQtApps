#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _Contour2DView_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         Contour2DView.cpp
* \author       Zsolt Husz, Bill Hill
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
* \brief        View for 2D contour.
* \ingroup      Views
*/

#include "Contour2DView.h"
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


// Constructors/Destructors
Contour2DView::
Contour2DView(
  QObject * parent,
  WoolzObject * object):
ObjectView(
  parent,
  object)
{
  Q_ASSERT(obj->isContour() && !obj->is3D());

  m_material = new SoMaterial;
  m_material->ref();
  m_transparency = 0;
  connect(object, SIGNAL(objectVisualisationChange()),
          this, SLOT(objectColourChanged()));

}

Contour2DView::
~Contour2DView()
{ 
  if(m_material)
  {
    m_material->unref();
  }
}

bool Contour2DView::
compatible()
{
 return(obj->isContour() && !obj->is3D());
}

void Contour2DView::
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
    cache = new SoGroup;
    SoCoordinate3 * nodes;                   // nodes of the contour
    SoIndexedLineSet * lineset;              // lines of the contour
    WlzErrorNum errNum = WLZ_ERR_NONE;

    nodes = Contour2DView::Vertices2D(new SoCoordinate3, errNum);
    if(errNum == WLZ_ERR_NONE)
    {
      lineset = Contour2DView::Lines2D(new SoIndexedLineSet, errNum);
    }
    if((errNum != WLZ_ERR_NONE) || (nodes == NULL) || (lineset == NULL))
    {
      return ;
    }
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


WlzErrorNum Contour2DView::
CheckContour(
  WlzContour **ctr,
  WlzGMModel **model)
{
  WlzErrorNum	errNum = WLZ_ERR_NONE;

  if(!(obj->isContour() && !obj->is3D()))
  {
    errNum = WLZ_ERR_DOMAIN_TYPE;
  }
  else if(((*ctr = obj->getObj()->domain.ctr) == NULL) ||
          ((*model = (*ctr)->model) == NULL))
  {
    errNum = WLZ_ERR_DOMAIN_NULL;
  }
  else
  {
    /* Check the model type. */
    switch((*model)->type)
    {
      case WLZ_GMMOD_2I: /* FALLTHROUGH */
      case WLZ_GMMOD_2D: /* FALLTHROUGH */
      case WLZ_GMMOD_2N:
	break;
      default:
	errNum = WLZ_ERR_DOMAIN_TYPE;
	break;
    }
  }
  return(errNum);
}

SoCoordinate3 * Contour2DView::
Vertices2D(
  SoCoordinate3 * vertices,
  WlzErrorNum& errNum)
{
  int           idI,
		iCnt,
		iCount;
  AlcVector     *vec;
  WlzGMElemP    eP;
  WlzDVertex2   vtx;
  WlzContour 	*ctr;
  WlzGMModel 	*model;
  WlzGMResIdxTb	*resIdxTb = NULL;

  errNum = CheckContour(&ctr, &model);
  if(errNum == WLZ_ERR_NONE)
  {
    /* Index the verticies. */
    resIdxTb = WlzGMModelResIdx(model, WLZ_GMELMFLG_VERTEX, &errNum);
  }
  if(errNum == WLZ_ERR_NONE)
  {
    /* Check there are verticies! */
    if(resIdxTb->vertex.idxCnt < 1)
    {
      errNum = WLZ_ERR_DOMAIN_DATA;
    }
  }
  if(errNum == WLZ_ERR_NONE)
  {
    /* Output the vertex geometries. */
    idI = 0;
    iCount = 0;
    vec = model->res.vertex.vec;
    iCnt = model->res.vertex.numIdx;
    while((errNum == WLZ_ERR_NONE) && (iCnt-- > 0))
    {
      eP.vertex = (WlzGMVertex *)AlcVectorItemGet(vec, idI);
      if(eP.vertex->idx >= 0)
      {
	(void )WlzGMVertexGetG2D(eP.vertex, &vtx);
	vertices->point.set1Value(iCount++, vtx.vtX, vtx.vtY, 0);
      }
      ++idI;
    }
  }
  if(resIdxTb)
  {
    WlzGMModelResIdxFree(resIdxTb);
  }
  return(vertices);
}

SoIndexedLineSet * Contour2DView::
Lines2D(
  SoIndexedLineSet * lineset,
  WlzErrorNum& errNum)
{

  int           iFcount = 0; //face index count
  int		idI,
		iCnt;
  AlcVector	*vec;
  WlzGMElemP	eP;
  WlzGMEdgeT	*tET;
  WlzContour 	*ctr;
  WlzGMModel 	*model;
  WlzGMResIdxTb	*resIdxTb = NULL;

  errNum = CheckContour(&ctr, &model);
  if(errNum == WLZ_ERR_NONE)
  {
    /* Index the verticies. */
    resIdxTb = WlzGMModelResIdx(model, WLZ_GMELMFLG_VERTEX, &errNum);
  }
  if(errNum == WLZ_ERR_NONE)
  {
    /* Check there are verticies! */
    if(resIdxTb->vertex.idxCnt < 1)
    {
      errNum = WLZ_ERR_DOMAIN_DATA;
    }
  }
  if(errNum == WLZ_ERR_NONE)
  {
    idI = 0;
    vec = model->res.edge.vec;
    iCnt = model->res.edge.numIdx;
    while(iCnt-- > 0)
    {
      eP.edge = (WlzGMEdge *)AlcVectorItemGet(vec, idI++);
      if(eP.edge->idx >= 0)
      {
	tET = eP.edge->edgeT;
	lineset->coordIndex.set1Value(iFcount++, *(resIdxTb->vertex.idxLut +
	    tET->vertexT->diskT->vertex->idx));
	lineset->coordIndex.set1Value(iFcount++, *(resIdxTb->vertex.idxLut +
	    tET->opp->vertexT->diskT->vertex->idx));
	lineset->coordIndex.set1Value(iFcount++, -1);
      }
    }
  }
  if(resIdxTb)
  {
    WlzGMModelResIdxFree(resIdxTb);
  }
  return(lineset);
}


void Contour2DView::
setTransparency(
  int transparency)
{
  ObjectView::setTransparency(transparency);
}

void Contour2DView::
objectColourChanged()
{
  m_material->transparency = 0;
  m_material->diffuseColor.setValue(obj->sbColour());
}

QStringList Contour2DView::
getVisualisationTypes()
{
  return QStringList("2D contour");
}
