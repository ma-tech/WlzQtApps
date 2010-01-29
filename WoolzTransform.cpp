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
* \file         WoolzTransform.cpp
* \author       Zsolt Husz
* \date         January 2010
* \version      $Id$
* \par
* Address:
*               MRC Human Genetics Unit,
*               Western General Hospital,
*               Edinburgh, EH4 2XU, UK.
* \par
* Copyright (C) 2010 Medical research Council, UK.
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
* \brief        Provides warping of obejects, manages landmarks and woolz transform structures
* \ingroup      Control
*
*/

#include "WoolzTransform.h"
#include "Wlz.h"
#include <QApplication>


WoolzTransform::WoolzTransform(LandmarkModel *landmarkModel, QObject *parent) : QObject(parent), m_landmarkModel(landmarkModel), m_meshObject (NULL) {
    Q_ASSERT(landmarkModel); // must be not NULL, no further update is posssible
    basisTr = NULL;
    transformObj = NULL;
    invertedtransformObj = NULL;
    connect(m_landmarkModel, SIGNAL(removedLandmark(int)), this, SLOT(update()));
    connect(m_landmarkModel, SIGNAL(addedLandmarkPair(int)), this, SLOT(update()));
    connect(m_landmarkModel, SIGNAL(movedSourceLandmark(const int, const WlzDVertex3)), this, SLOT(update()));
    connect(m_landmarkModel, SIGNAL(movedTargetLandmark(const int, const WlzDVertex3)), this, SLOT(update()));
    connect(m_landmarkModel, SIGNAL(warpingChanged()), this, SLOT(update()));

    connect(this, SIGNAL(statusChange(QString, int)), parent, SLOT(statusChanged(QString, int)));
}

WoolzTransform::~WoolzTransform() {
  if (transformObj)
    WlzFreeObj(transformObj);
  if (invertedtransformObj)
    WlzFreeObj(invertedtransformObj);
}

void WoolzTransform::update() {
    basisTr = NULL;
    //  NOTE: basisTr is managed and freed by LandmarkModel
    emit updated();
}

void WoolzTransform::setMeshObject( WoolzObject *meshObject) {
     m_meshObject = meshObject;
     basisTr = NULL;
}

bool WoolzTransform::isValidLandmarkSet() {
    return m_landmarkModel ? m_landmarkModel->isValidLandmakSet() : false;
}

bool WoolzTransform::isReadyForWarp(QString &errorMsg) {
    if (!m_landmarkModel->isValidLandmakSet()) {
       errorMsg = "invalid landmarks";
       return false;
    }
    if (!m_meshObject || !m_meshObject->getObj()) {
       errorMsg = "no mesh";
       return false;
    }
    return true;
}

WlzErrorNum WoolzTransform::updateBasisTr() {
    WlzErrorNum errNum = WLZ_ERR_NONE;
    if (!basisTr) {
      statusChange("Computing basis transform.",0);
      basisTr = m_landmarkModel->getBasisTransform(m_meshObject, errNum, m_meshObject->type() == WoolzObject::source);
      statusChange("Computing basis transform done.",0);
      if (transformObj) {
        WlzFreeObj(transformObj);
        transformObj = NULL;
      }
      if (invertedtransformObj) {
        WlzFreeObj(invertedtransformObj);
        invertedtransformObj = NULL;
      }
   }
   return errNum;
}

WlzErrorNum WoolzTransform::updateDirectTransform() {
    WlzErrorNum errNum = WLZ_ERR_NONE;
    errNum = updateBasisTr();
    if (transformObj)
        return errNum;
    WlzCMeshP meshp;
    meshp.m3 = m_meshObject->getObj()->domain.cm3;
    statusChange("Computing direct transform.",0);
    transformObj = WlzBasisFnMakeCMeshTr(basisTr, meshp, &errNum);
    statusChange("Computing direct transform done.",0);
    return errNum;
}


WlzErrorNum WoolzTransform::updateInverseTransform() {
    WlzErrorNum errNum = WLZ_ERR_NONE;
    updateBasisTr();
    if (invertedtransformObj)
       return errNum;

    if (errNum == WLZ_ERR_NONE) {
         WlzCMeshP meshp;
         meshp.m3 = m_meshObject->getObj()->domain.cm3;
         statusChange("Computing direct transform.",0);
         invertedtransformObj = WlzBasisFnInvertMakeCMeshTr(basisTr, meshp, &errNum);
         statusChange("Computing inverse transform done.",0);
     }
    return errNum;
}

WlzObject* WoolzTransform::Transform(WoolzObject *srcObj,const WlzInterpolationType interp, WlzErrorNum *perrNum ) {
  WlzObject *result = NULL;
  WlzErrorNum errNum = WLZ_ERR_NONE;

  if (!srcObj->isValue())
      errNum = WLZ_ERR_VALUES_DATA;

  QApplication::setOverrideCursor(Qt::WaitCursor);
  bool direct  = (m_meshObject->type() == WoolzObject::source) == (srcObj->type() == WoolzObject::source);
  if (errNum == WLZ_ERR_NONE) {
    if (direct) {
       errNum = updateDirectTransform();
    } else {
       errNum = updateInverseTransform();
    }
  }

  if (errNum == WLZ_ERR_NONE) {
      statusChange("Computing transformed object.",0);
      result=WlzCMeshTransformObj( srcObj->getObj(), direct ? transformObj: invertedtransformObj, interp, &errNum);  //volume transform
      statusChange("Computing transformed done object.",0);
  }
  if (perrNum)
      *perrNum  = errNum;

  QApplication::restoreOverrideCursor();
  return result;
}

WlzObject* WoolzTransform::TransformedMesh(WlzErrorNum *perrNum) {
  WlzObject *result = NULL;
  WlzErrorNum errNum = WLZ_ERR_NONE;

  if (!m_meshObject || !m_meshObject->isMesh())
    return NULL;
  QApplication::setOverrideCursor(Qt::WaitCursor);

  updateInverseTransform();
  if (errNum == WLZ_ERR_NONE && invertedtransformObj) {          // mesh transform
       WlzValues     val;
       val.core = NULL;
       statusChange("Computing transformed mesh.", 0);
       result = WlzMakeMain(m_meshObject->is3D() ? WLZ_CMESH_3D : WLZ_CMESH_2D, invertedtransformObj->domain, val, NULL, NULL, &errNum);
       statusChange("Computing transformed mesh done.", 0);
  }
  if (perrNum)
      *perrNum  = errNum;
  QApplication::restoreOverrideCursor();
  return result;
}

bool WoolzTransform::needsupdate() {
    return basisTr == NULL;
}

WlzObject* WoolzTransform::getTransformObj(WlzErrorNum *perrNum) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    WlzObject *result = NULL;
    WlzErrorNum errNum = WLZ_ERR_NONE;
    if (m_meshObject->type() == WoolzObject::source) { // direct mesh is used
         errNum = updateDirectTransform();
         result = transformObj;
    } else { // inverted mesh is used
         errNum = updateInverseTransform();
         result = invertedtransformObj;
    }
    if (perrNum)
        *perrNum  = errNum;
    QApplication::restoreOverrideCursor();
    return result;
}
