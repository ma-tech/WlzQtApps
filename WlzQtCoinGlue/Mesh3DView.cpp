#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _Mesh3DView_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         Mesh3DView.cpp
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
* \brief        View for 3D mesh.
* \ingroup      Views
*/

#include "Mesh3DView.h"
#include "WoolzObject.h"

// Coin includes
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/Qt/editors/SoQtMaterialEditor.h>
#include <Inventor/Qt/editors/SoQtColorEditor.h>
#include <Inventor/nodes/SoClipPlane.h>

//for PI
#include <math.h>

#include <QtXml/QXmlStreamWriter>
#include <QtXml/QDomElement>

Mesh3DView::
Mesh3DView(
  QObject * parent,
  WoolzObject * object):
ObjectView(
  parent, object),
m_clipOn(false),
m_clipPlane(NULL)
{
  Q_ASSERT(obj->isMesh() && obj->is3D());
  m_material = new SoMaterial;
  Q_ASSERT(m_material);
  m_material->ref();
  m_drawStyle = new SoDrawStyle;
  Q_ASSERT(m_drawStyle);
  m_drawStyle->ref();
  m_transparency = 0;
  m_visualStyle = FULL;
  connect(object, SIGNAL(objectVisualisationChange()),
          this, SLOT(objectColourChanged()));
  connect(object, SIGNAL(removeMeshElement(int)),
          this, SLOT(removeElement(int)));
  connect(parent, SIGNAL(addedClipPlane(SoClipPlane *)),
          this, SLOT(addedClipPlane(SoClipPlane *)));
}

Mesh3DView::
~Mesh3DView()
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

bool Mesh3DView::
compatible()
{
  return(obj->isMesh() && obj->is3D());
}

void Mesh3DView::
generateSceneGraph(
  bool /*bForce*/)
{
  m_material->diffuseColor.setValue(obj->sbColour());
  updateMaterial();

  root->addChild(m_material);
  root->addChild(m_drawStyle);

  SoGroup *cache;
  if (obj->isCachedVisualisation())
  {
    cache = obj->cachedVisualisation();
  }
  else
  {
    cache = new SoGroup;
    WlzErrorNum errNum = WLZ_ERR_NONE;
    SoCoordinate3 * nodes;
    SoIndexedFaceSet * faces;
    nodes = Vertices3D(new SoCoordinate3, errNum);
    if(errNum == WLZ_ERR_NONE)
    {
      faces = Faces3D(new SoIndexedFaceSet, errNum);
    }
    if((errNum != WLZ_ERR_NONE) || (nodes == NULL) || (faces == NULL))
    {
      return ;
    }

    SoShapeHints *h1= new SoShapeHints;
    Q_ASSERT(h1);
    h1->vertexOrdering = SoShapeHints::CLOCKWISE;
    h1->shapeType = SoShapeHints::UNKNOWN_SHAPE_TYPE;
    h1->creaseAngle = M_PI/2;

    cache->addChild(nodes);
    cache->addChild(h1);
    cache->addChild(faces);
    obj->setVisualisation(cache);
  }
  root->addChild(cache);

  if(m_clipPlane)
  {
    root->insertChild(m_clipPlane, 1);
  }
}

void Mesh3DView::
removeElement(
  int elemID)
{
  int           nElm = 0;
  SoGroup * 	cache = obj->cachedVisualisation();
  if(!cache)
  {
    Q_ASSERT(false);
    return;
  };

  SoIndexedFaceSet * faces = (SoIndexedFaceSet * )cache->getChild(2);
  /////////// chech if face
  nElm=m_elemArray.getNum();
  for(int i = 0; i < nElm; ++i)
  {
    if(m_elemArray[i] == elemID)
    {
      faces->coordIndex.set1Value(i*4+0, 0);
      faces->coordIndex.set1Value(i*4+1, 0);
      faces->coordIndex.set1Value(i*4+2, 0);
      faces->coordIndex.set1Value(i*4+3, -1);
    }
  }
}

SoCoordinate3 * Mesh3DView::
Vertices3D(
  SoCoordinate3 * vertices,
  WlzErrorNum& errNum)
{
  int 	     nNod = 0;
  WlzCMesh3D *mesh = obj->getObj()->domain.cm3;

  // check if 3D mesh and if it has vertices
  if(!(mesh && (mesh->type == WLZ_CMESH_3D) &&
	((nNod = mesh->res.nod.maxEnt) > 0)))
  {
    errNum = WLZ_ERR_DOMAIN_TYPE;
  }
  if(errNum == WLZ_ERR_NONE)
  {
    /* Output all nodes but use (0, 0, 0) for those that are invalid. */
    for(int i = 0; i < nNod; ++i)
    {
      WlzCMeshNod3D *nod;

      nod = (WlzCMeshNod3D *)AlcVectorItemGet(mesh->res.nod.vec, i);
      if(nod && nod->idx >= 0)
      {
	vertices->point.set1Value(i, nod->pos.vtX, nod->pos.vtY, nod->pos.vtZ);
      }
      else
      {
	vertices->point.set1Value(i, 0, 0, 0);
      }
    }
  }
  return vertices;
}

SoIndexedFaceSet * Mesh3DView::
Faces3D(
  SoIndexedFaceSet * faces,
  WlzErrorNum& errNum)
{
  int           iFcount = 0; //face index count
  int           iCount = 0;  //face number count index count
  int           nElm = 0;
  WlzCMesh3D *mesh = obj->getObj()->domain.cm3;
  // check if it is 3D mesh and if it has faces
  if(!(mesh && (mesh->type == WLZ_CMESH_3D) &&
       ((nElm = mesh->res.elm.maxEnt)>0)))
  {
    errNum = WLZ_ERR_DOMAIN_TYPE;
  }
  if (errNum == WLZ_ERR_NONE)
  {
    for(int idE = 0; idE < nElm; ++idE)
    {
      WlzCMeshElm3D  *elm;

      elm = (WlzCMeshElm3D *)AlcVectorItemGet(mesh->res.elm.vec, idE);
      if(elm->idx >= 0)
      {
	for(int idF = 0; idF < 4; ++idF)
	{
          WlzCMeshFace *fce;

	  fce = elm->face + idF;
	  if((fce->opp == NULL) ||
	      (fce->opp == fce) ||
	      (fce->elm->idx < fce->opp->elm->idx))
	  {
	    faces->coordIndex.set1Value(iFcount++, fce->edu[0].nod->idx);
	    faces->coordIndex.set1Value(iFcount++, fce->edu[1].nod->idx);
	    faces->coordIndex.set1Value(iFcount++, fce->edu[2].nod->idx);
	    faces->coordIndex.set1Value(iFcount++, -1);
	    m_elemArray.set1Value(iCount++, idE);
	  }
	}
      }
    }
  }
  return(faces);
}

void Mesh3DView::
updateMaterial()
{
  if(m_material)
  {
    //only if scene graph exists  
    m_material->transparency = (double )m_transparency/100.0;
  }
  if (m_drawStyle)
  {
    m_drawStyle->style = (m_visualStyle == FULL)? SoDrawStyle::FILLED:
                                                  SoDrawStyle::LINES;
  }
}

void Mesh3DView::
setTransparency(
  int transparency)
{
  ObjectView::setTransparency(transparency);
  updateMaterial();
}

void Mesh3DView::
objectColourChanged()
{
  m_material->diffuseColor.setValue(obj->sbColour());
  m_material->transparency = (double )m_transparency / 100.0;
}

int  Mesh3DView::visualisationType () {
 return m_visualStyle;
}

QStringList Mesh3DView::
getVisualisationTypes()
{
  QStringList list;
  list << "3D mesh full" <<  "3D mesh wireframe";
  return(list);
}

void Mesh3DView::
setVisualisationType(
  int type)
{
  switch(type)
  {
    case 0:
      m_visualStyle = FULL;
      break;
    case 1:
      m_visualStyle = LINES;
      break;
    default:
      break;
  }
  updateMaterial();
}

void Mesh3DView::
addedClipPlane(
  SoClipPlane * plane)
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
  if(plane)
  {
    m_clipPlane = plane;
    m_clipPlane ->ref();
    root->insertChild(plane, 1);
    m_clipOn = true;
  }
}

bool Mesh3DView::
saveAsXmlProperties(
  QXmlStreamWriter *xmlWriter)
{
  Q_ASSERT(xmlWriter);
  ObjectView::saveAsXmlProperties(xmlWriter);
  xmlWriter->writeTextElement("VisualisationType",
                              QString("%1").arg(visualisationType()));
  return(true);
}

bool Mesh3DView::
parseDOMLine(
  const QDomElement &element)
{
  if(element.tagName() == "VisualisationType")
  {
     setVisualisationType(element.text().toInt());
     return(true);
  }
  return(ObjectView::parseDOMLine(element));
}
