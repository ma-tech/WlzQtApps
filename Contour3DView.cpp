#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _Contour3DView_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         Contour3DView.cpp
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
* \brief        View for 3D contour.
* \ingroup      Views
*
*/

#include "Contour3DView.h"
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

Contour3DView::Contour3DView (QObject * parent, WoolzObject * object ): ObjectView(parent, object), m_clipOn(false), m_clipPlane(NULL) {
  Q_ASSERT(obj->isContour() && obj->is3D());

  m_material = new SoMaterial;
  m_material->ref();

  m_transparency = 0;
  connect(object, SIGNAL(objectVisualisationChange()), this, SLOT(objectColourChanged()));
  connect(parent, SIGNAL(addedClipPlane(SoClipPlane *)), this, SLOT(addedClipPlane(SoClipPlane *)));
}

Contour3DView::~Contour3DView ( ) {
    if (m_material)
      m_material->unref();
}

bool Contour3DView::compatible( ) {
 return obj->isContour() && obj->is3D();
}

void Contour3DView::generateSceneGraph ( bool /*bForce*/ ) {
  m_material->diffuseColor.setValue(obj->sbColour());
  updateMaterial();
  root->addChild(m_material);

  SoGroup *cache;
  if (obj->isCachedVisualisation())
     cache = obj->cachedVisualisation();
  else {
    cache = new SoGroup;
    WlzErrorNum errNum = WLZ_ERR_NONE;
    SoCoordinate3 * nodes;                   // nodes of the contour
    SoIndexedFaceSet * faces;                // faces of the contour

    nodes = Vertices3D(new SoCoordinate3, errNum);
    faces = Faces3D(new SoIndexedFaceSet, errNum);

    if (errNum != WLZ_ERR_NONE || nodes == NULL || faces == NULL)
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
     root->insertChild(m_clipPlane, 1);

  return ;
}

SoCoordinate3 * Contour3DView::Vertices3D(SoCoordinate3 * vertices, WlzErrorNum& errNum )
{
  WlzGMResIdxTb	*resIdxTb = NULL;
  int           idI,
                iCnt,
                iCount;
  AlcVector     *vec;
  WlzGMElemP    eP;
  WlzDVertex3   vtx;

  // check if 3D contour
  if(!(obj->isContour() && obj->is3D())) {
      errNum = WLZ_ERR_DOMAIN_TYPE;
  }

  WlzContour *ctr = obj->getObj()->domain.ctr;
  WlzGMModel *model = ctr->model;;

  if(model == NULL) {
    errNum = WLZ_ERR_DOMAIN_NULL;
  }
  else {
    /* Check the model type. */
    switch(model->type) {

      case WLZ_GMMOD_3I: /* FALLTHROUGH */
      case WLZ_GMMOD_3D: /* FALLTHROUGH */
      case WLZ_GMMOD_3N:
        break;
      default:
        errNum = WLZ_ERR_DOMAIN_TYPE;
        break;
    }
  }
  if(errNum == WLZ_ERR_NONE) {
    /* Index the verticies. */
    resIdxTb = WlzGMModelResIdx(model, WLZ_GMELMFLG_VERTEX, &errNum);
  }
  if(errNum == WLZ_ERR_NONE) {
    /* Check there are verticies! */
    if(resIdxTb->vertex.idxCnt < 1)
    {
      errNum = WLZ_ERR_DOMAIN_DATA;
    }
  }

  if(errNum == WLZ_ERR_NONE) {
    /* Output the vertex geometries. */
    idI = 0;
    iCount = 0;

    vec = model->res.vertex.vec;
    iCnt = model->res.vertex.numIdx;
    while((errNum == WLZ_ERR_NONE) && (iCnt-- > 0)) {
      eP.vertex = (WlzGMVertex *)AlcVectorItemGet(vec, idI);
      if(eP.vertex->idx >= 0) {
        (void )WlzGMVertexGetG3D(eP.vertex, &vtx);
        vertices->point.set1Value(iCount++, vtx.vtX, vtx.vtY, vtx.vtZ);
//        vertices->point.set1Value(idI, vtx.vtX, vtx.vtY, vtx.vtZ);
      } //else
//        vertices->point.set1Value(idI, 0, 0, 0);
      idI++;
    }
  }
  if(resIdxTb) {
    WlzGMModelResIdxFree(resIdxTb);
  }
 return vertices;
}

SoIndexedFaceSet * Contour3DView::Faces3D(SoIndexedFaceSet * faces, WlzErrorNum& errNum ) {
  int           iFcount = 0; //face index count
  int           idI,
                iCnt;
  AlcVector     *vec;
  WlzGMElemP    eP;
  WlzGMEdgeT    *tET;
  WlzGMResIdxTb *resIdxTb = NULL;
  WlzContour *ctr = obj->getObj()->domain.ctr;
  WlzGMModel *model = ctr->model;;

  if(model == NULL) {
    errNum = WLZ_ERR_DOMAIN_NULL;
  }
  else {
    /* Check the model type. */
    switch(model->type) {
      case WLZ_GMMOD_3I: /* FALLTHROUGH */
      case WLZ_GMMOD_3D: /* FALLTHROUGH */
      case WLZ_GMMOD_3N:
        break;
      default:
        errNum = WLZ_ERR_DOMAIN_TYPE;
        break;
    }
  }
  if(errNum == WLZ_ERR_NONE) {
    /* Index the verticies. */
    resIdxTb = WlzGMModelResIdx(model, WLZ_GMELMFLG_VERTEX, &errNum);
  }
  if(errNum == WLZ_ERR_NONE) {
    /* Check there are verticies! */
    if(resIdxTb->vertex.idxCnt < 1) {
      errNum = WLZ_ERR_DOMAIN_DATA;
    }
  }
  if (errNum == WLZ_ERR_NONE) {
    idI = 0;
    vec = model->res.face.vec;
    iCnt = model->res.face.numIdx;
    while(iCnt-- > 0) {
      eP.face = (WlzGMFace *)AlcVectorItemGet(vec, idI++);
      if(eP.face->idx >= 0) {
        /* Face IS a triangle, in 3D nothing else is allowed. */
        tET = eP.face->loopT->edgeT;

        faces->coordIndex.set1Value(iFcount++, *(resIdxTb->vertex.idxLut +
                    tET->vertexT->diskT->vertex->idx));
        faces->coordIndex.set1Value(iFcount++, *(resIdxTb->vertex.idxLut +
                    tET->next->vertexT->diskT->vertex->idx));
        faces->coordIndex.set1Value(iFcount++, *(resIdxTb->vertex.idxLut +
                    tET->prev->vertexT->diskT->vertex->idx));
        faces->coordIndex.set1Value(iFcount++, -1);
      }
    }
  }

  if(resIdxTb) {
    WlzGMModelResIdxFree(resIdxTb);
  }
  return faces;
}

void Contour3DView::updateMaterial() {
  if (m_material) //only if scene graph exists  
    m_material->transparency = (double) m_transparency/100.0;
}

void Contour3DView::setTransparency ( int transparency ) {
  ObjectView::setTransparency (transparency);
  updateMaterial();
}

void Contour3DView::objectColourChanged ()   {
   m_material->transparency = (double) m_transparency/100.0;
   m_material->diffuseColor.setValue(obj->sbColour());
}

QStringList Contour3DView::getVisualisationTypes () {
  return QStringList("3D contour");
}

void Contour3DView::addedClipPlane(SoClipPlane * plane){
  if (m_clipOn) {
    root->removeChild(1);
    m_clipOn = false;
  }

  if (m_clipPlane) {
    m_clipPlane ->unref();
    m_clipPlane = NULL;
  }
  if (!plane)
    return;
  m_clipPlane = plane;
  m_clipPlane ->ref();

  root->insertChild(plane, 1);
  m_clipOn = true;
}
