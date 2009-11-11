#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _WoolzDynObject_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         WoolzDynWarpedObject .cpp
* \author       Zsolt Husz
* \date         June 2009
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
* \brief        Dynamicaly generated Woolz object class
* \ingroup      Control
*
*/
#include "WoolzDynWarpedObject.h"
#include "LandmarkModel.h"
#include "ObjectListModel.h"
#include "WarperConfig.h"
#include <QtXml/QDomElement>
#include <QtXml/QXmlStreamWriter>

const char* WoolzDynWarpedObject::xmlTag = "WarpedObject";

WoolzDynWarpedObject ::WoolzDynWarpedObject (ObjectListModel * objectListModel, LandmarkModel *landmarkModel) :
       WoolzDynObject(objectListModel), m_landmarkModel(landmarkModel), m_outdatedWarp(true)  {
  connect(m_landmarkModel, SIGNAL(removedLandmark(int)), this, SLOT(updateWarp()));
  connect(m_landmarkModel, SIGNAL(addedLandmarkPair(int)), this, SLOT(updateWarp()));
  connect(m_landmarkModel, SIGNAL(movedSourceLandmark(const int, const WlzDVertex3)),
    this, SLOT(updateWarp()));
  connect(m_landmarkModel, SIGNAL(movedTargetLandmark(const int, const WlzDVertex3)),
    this, SLOT(updateWarp()));
  connect(m_landmarkModel, SIGNAL(warpingChanged()),
    this, SLOT(updateWarp()));

  /*connect(m_objectListModel, SIGNAL(replaceWarpMesh(WoolzObject *)),
    this, SLOT(update()));*/
}

WoolzDynWarpedObject ::~WoolzDynWarpedObject () {
}

void WoolzDynWarpedObject ::setTypeFromSource () {
  WoolzObject *srcObj = sourceObj();
  if (srcObj)
     changeWoolzObjectType(((srcObj->type())==source)  ? target : source);
}

void WoolzDynWarpedObject ::warp () {
  const WlzInterpolationType interp = WLZ_INTERPOLATION_NEAREST;
  WlzErrorNum errNum = WLZ_ERR_NONE;
  WlzBasisFnTransform *basisTr=NULL;
  WlzCMeshTransform *cMeshTrans = NULL;
  WoolzObject *srcObj = sourceObj();

  WlzFreeObj(m_obj);
  m_obj = NULL;

  //does object has a source object?
  if (!srcObj) {
    return;
  }

  statusChange("Warping " + m_name+ " started.", 0);

  if (!m_landmarkModel->isValidLandmakSet()) {
     m_obj = WlzAssignObject( srcObj->getObj(), &errNum );
     statusChange("Warping " + m_name+ " failed: invalid landmarks.", 0);
     return ;
  }

  WoolzObject *meshObj = m_objectListModel->getMeshObject();
  if (!meshObj) {
     m_obj = WlzAssignObject( srcObj->getObj(), &errNum );
     statusChange("Warping " + m_name+ " failed: no mesh.", 0);
     return ;
  }

  bool isSourceMesh = meshObj->type() == WoolzObject::source;
  bool isSourceSourceObj = srcObj->type() == WoolzObject::source;
  bool direct  = isSourceMesh == isSourceSourceObj;

  //create Cmesh transform
  if (errNum == WLZ_ERR_NONE) {
    if (srcObj->is3D())
       cMeshTrans = WlzMakeCMeshTransform3D(meshObj->getObj()->domain.cm3, &errNum);
    else
       cMeshTrans = WlzMakeCMeshTransform2D(meshObj->getObj()->domain.cm2, &errNum);
  }

  // create basis transform by generated by the landmarks
  if (errNum == WLZ_ERR_NONE) {
    basisTr = m_landmarkModel->getBasisTransform(cMeshTrans, errNum, isSourceMesh);
//    basisTr = m_landmarkModel->getBasisTransform(cMeshTrans, m_delta, m_basisFnType == basis_IMQ, errNum, isSourceMesh);
  }
  if (errNum == WLZ_ERR_NONE) {
    if (direct && !srcObj->isMesh()) {
      errNum = WlzBasisFnSetCMesh(cMeshTrans, basisTr);
    } else {
      WlzCMeshP     invMesh;
      WlzCMeshP     dirMesh;
      dirMesh.m3=meshObj->getObj()->domain.cm3;
      invMesh = WlzCMeshCopy(dirMesh, 0, NULL, NULL, &errNum);
      WlzFreeCMeshTransform(cMeshTrans);  // release it later? is BasisTr usingit?!
      if (errNum == WLZ_ERR_NONE) {
         if (srcObj->is3D())
            cMeshTrans = WlzMakeCMeshTransform3D(invMesh.m3, &errNum);
          else
            cMeshTrans = WlzMakeCMeshTransform2D(invMesh.m2, &errNum);
       }
       if (errNum == WLZ_ERR_NONE)
            errNum = WlzBasisFnSetCMesh(cMeshTrans, basisTr);

       if (errNum == WLZ_ERR_NONE)
            errNum = WlzCMeshTransformInvert(cMeshTrans);

        if (errNum == WLZ_ERR_NONE && srcObj->isMesh()) {          // mesh transform
            WlzDomain     dom;
            WlzValues     val;
            val.core = NULL;
            if (srcObj->is3D())  {
                dom.cm3 = invMesh.m3;
                m_obj = WlzMakeMain(WLZ_CMESH_3D, dom, val, NULL, NULL, &errNum);
             } else {
                dom.cm2 = invMesh.m2;
                m_obj = WlzMakeMain(WLZ_CMESH_2D, dom, val, NULL, NULL, &errNum);
             }
        }
    }
  }
  if (errNum == WLZ_ERR_NONE) {
    if (!srcObj->isMesh())
      m_obj  = WlzCMeshTransformObj( srcObj->getObj(), cMeshTrans, interp, &errNum);  //volume transform
  }
  if (cMeshTrans) {
    WlzFreeCMeshTransform(cMeshTrans);
    cMeshTrans = NULL;
  }
  if (errNum == WLZ_ERR_NONE) {
      statusChange("Warping " + m_name+ " done.", 0);
      saveUsedParameters();
      m_obj = WlzAssignObject( m_obj, &errNum);
  }
  else {
      if (m_obj)
        WlzFreeObj(m_obj);
      m_obj = WlzAssignObject( srcObj->getObj(), &errNum );
      statusChange("Warping " + m_name+ " failed.", 0);
      emit invalidWarping();
  }
}

void WoolzDynWarpedObject ::doUpdate ( ) {
  if (!sourceObj())
      return;
  warp();
  setTypeFromSource();
  setupTransferFunction();
  emit objectChanged();
}

QString WoolzDynWarpedObject::notes ( )  {
  QString notes = "Warped ";
  notes  += (type()==source) ? "source" : "target";
  notes += isMesh() ?" mesh" : " value";
  if (sourceObj())
    notes += " of " + sourceObj()->name();
  notes += m_autoUpdate ? " (autoupdate)" : " (no autoupdate)";
  return notes;
}

void WoolzDynWarpedObject::setupConnections(QObject *target) {
    WoolzDynObject::setupConnections(target);
    connect( target, SIGNAL(updateAllWarpedSignal()), this, SLOT(update()));
}

void WoolzDynWarpedObject::update ( bool force ) {
  if ((m_autoUpdate && config.globalAutoUpdate() && config.globalWarpUpdate() && needsUpdate()) || force) {
     removeCachedVisualisation();     doUpdate();
  }
}

bool WoolzDynWarpedObject::saveAsXml(QXmlStreamWriter *xmlWriter) {
  xmlWriter->writeStartElement(xmlTag);
  saveAsXmlProperties(xmlWriter);
  xmlWriter->writeEndElement();
  return true;
}

void WoolzDynWarpedObject::updateWarp() {
    m_outdatedWarp = true;
    update();
}

bool  WoolzDynWarpedObject::needsUpdate() {
    return m_outdatedWarp  || WoolzDynObject::needsUpdate();
}

void WoolzDynWarpedObject::saveUsedParameters() {
    m_outdatedWarp = false;
    WoolzDynObject::saveUsedParameters();
}
