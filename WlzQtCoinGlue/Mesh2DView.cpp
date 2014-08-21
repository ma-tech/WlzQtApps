#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _Mesh2DView_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         Mesh2DView.cpp
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
* \brief        View for 2D mesh.
* \ingroup      Views
*/

#include "Mesh2DView.h"
#include "WoolzObject.h"

// Qt includes
#include <QStringList>

// Coin includes
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoDrawStyle.h>

Mesh2DView::
Mesh2DView(
  QObject * parent,
  WoolzObject * object):
ObjectView(
  parent,
  object)
{
  Q_ASSERT(obj->isMesh() && !obj->is3D());
  m_material = new SoMaterial;
  Q_ASSERT(m_material);
  m_material->ref();

  m_drawStyle = new SoDrawStyle;
  Q_ASSERT(m_drawStyle);
  m_drawStyle ->ref();
  m_drawStyle->style = SoDrawStyle::LINES ;

  m_transparency = 0;
  connect(object, SIGNAL(objectVisualisationChange()),
          this, SLOT(objectColourChanged()));
}

Mesh2DView::
~Mesh2DView()
{ 

  if(m_material)
  {
    m_material->unref();
  }
  if(m_drawStyle)
  {
    m_drawStyle->unref();
  }
}

bool Mesh2DView::
compatible()
{
 return(obj->isMesh() && !obj->is3D());
}

void Mesh2DView::
generateSceneGraph(
  bool /*bForce*/)
{
  m_material->diffuseColor.setValue(obj->sbColour());
  updateMaterial();
  root->addChild(m_material);
  root->addChild(m_drawStyle);

  SoGroup *cache;
  if(obj->isCachedVisualisation())
  {
    cache = obj->cachedVisualisation();
  }
  else
  {
    cache = new SoGroup;
    WlzErrorNum errNum = WLZ_ERR_NONE;

    SoCoordinate3 * nodes;                   // nodes of the mesh
    SoIndexedFaceSet * faceset;              // lines of the mesh

    nodes = Vertices2D(new SoCoordinate3, errNum);
    if(errNum == WLZ_ERR_NONE)
    {
      faceset = Faces2D(new SoIndexedFaceSet, errNum);
    }
    if( (errNum != WLZ_ERR_NONE) || (nodes == NULL) || (faceset == NULL))
    {
      return ;
    }

    SoShapeHints *h1= new SoShapeHints;
    Q_ASSERT(h1);
    h1->vertexOrdering = SoShapeHints::CLOCKWISE;
    h1->shapeType = SoShapeHints::UNKNOWN_SHAPE_TYPE;
    cache->addChild(h1);
    cache->addChild(nodes);
    cache->addChild(faceset);
    obj->setVisualisation(cache);
  }
  root->addChild(cache);
}


SoCoordinate3 * Mesh2DView::
Vertices2D(
  SoCoordinate3 * vertices,
  WlzErrorNum& errNum)
{
  int           nNod = 0;
  WlzCMeshNod2D	*nod;
  WlzCMesh2D 	*mesh=obj->getObj()->domain.cm2;

  if(!(mesh && (mesh->type == WLZ_CMESH_2D) &&
	((nNod = mesh->res.nod.maxEnt) > 0)))
  {
    errNum = WLZ_ERR_DOMAIN_TYPE;
  }
  if(errNum == WLZ_ERR_NONE)
  {
    /* Output all nodes but use (0, 0, 0) for those that are invalid. */
    for(int i = 0; i < nNod; ++i)
    {
      nod = (WlzCMeshNod2D *)AlcVectorItemGet(mesh->res.nod.vec, i);
      if(nod->idx >= 0)
      {
	vertices->point.set1Value(i, nod->pos.vtX, nod->pos.vtY,
	                          -0.0001);  //view mesh in front
      }
      else
      {
	vertices->point.set1Value(i, 0, 0, 0);
      }
    }
  }
  return(vertices);
}

SoIndexedFaceSet * Mesh2DView::
Faces2D(
  SoIndexedFaceSet * faces,
  WlzErrorNum& errNum)
{
  int		nElm = 0;
  WlzCMeshElm2D *elm;

  int           iFcount = 0; //face index count

  WlzCMesh2D *mesh=obj->getObj()->domain.cm2;

  // check if it is 2D mesh and if it has faces
  if(!(mesh && (mesh->type == WLZ_CMESH_2D) &&
	((nElm = mesh->res.elm.maxEnt) > 0)))
  {
    errNum = WLZ_ERR_DOMAIN_TYPE;
  }

  if (errNum == WLZ_ERR_NONE)
  {
    for(int i = 0; i < nElm; ++i)
    {
      elm = (WlzCMeshElm2D *)AlcVectorItemGet(mesh->res.elm.vec, i);
      if(elm->idx >= 0)
      {
	faces->coordIndex.set1Value(iFcount++, elm->edu[0].nod->idx);
	faces->coordIndex.set1Value(iFcount++, elm->edu[1].nod->idx);
	faces->coordIndex.set1Value(iFcount++, elm->edu[2].nod->idx);
	faces->coordIndex.set1Value(iFcount++, -1);
      }
    }
  }
  return(faces);
}

void Mesh2DView::
updateMaterial()
{
  if(m_material)
  {
    //only if scene graph exists  
    m_material->transparency = (double) m_transparency/100.0;
  }
  if(m_drawStyle)
  {
    m_drawStyle->style = SoDrawStyle::LINES;
  }
}

void Mesh2DView::
setTransparency(
  int transparency)
{
  ObjectView::setTransparency(transparency);
  updateMaterial();
}

void Mesh2DView::
setVisibility(
  bool visibility) 
{
  ObjectView::setVisibility(visibility);
  updateMaterial();
}

void Mesh2DView::
objectColourChanged()
{
   m_material->transparency = (double )m_transparency / 100.0;
   m_material->diffuseColor.setValue(obj->sbColour());
}

QStringList Mesh2DView::
getVisualisationTypes()
{
  return QStringList("2D mesh");
}
