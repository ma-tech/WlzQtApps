#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _WoolzDynObject_h[] = "MRC HGU $Id$";
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
#ifndef WOOLZTRANSFORM_H
#define WOOLZTRANSFORM_H

#include <QObject>
#include "LandmarkModel.h"
#include "WoolzObject.h"
#include "Wlz.h"

class LandmarkModel;
class WoolzObject;

/*!
 * \brief	Class to manage warping process. Warps objects and manages landmarks, and warping structures.
 * \ingroup      Control
 */
class WoolzTransform : public QObject
{
    Q_OBJECT
public:
   /*!
    * \ingroup      Control
    * \brief        Constructor
    * \param        landmarkModel landmark model
    * \par      Source:
    *                WoolzTransform.cpp
    */
    WoolzTransform(LandmarkModel *landmarkModel);

   /*!
    * \ingroup      Control
    * \brief        Destructor
    * \return       void
    * \par      Source:
    *                WoolzTransform.cpp
    */
    ~WoolzTransform();

   /*!
    * \ingroup      Control
    * \brief        Returns mesh object
    * \return       the mesh woolz object or NULL if no mesh was set
    * \par      Source:
    *                WoolzTransform.cpp
    */
    WoolzObject *meshObject() { return  m_meshObject;}

   /*!
    * \ingroup      Control
    * \brief        Returns if landmarks are valid
    * \return       true if landmarks are valid, false otherwise
    * \par      Source:
    *                WoolzTransform.cpp
    */
    bool isValidLandmarkSet();

   /*!
    * \ingroup      Control
    * \brief        Returns if all parameters are set to warp
    * \param        errorMsg reference where error message is stored
    * \return       true if ready for warping, false otherwise
    * \par      Source:
    *                WoolzTransform.cpp
    */
    bool isReadyForWarp(QString &errorMsg);

   /*!
    * \ingroup      Control
    * \brief        Returns if parameters are changed and transform needs update
    * \return       true if warp has to recompute, false otherwise
    * \par      Source:
    *                WoolzTransform.cpp
    */
    bool needsupdate();

   /*!
    * \ingroup      Control
    * \brief        Transforms an input value object
    * \param        srcObj object to be transformed
    * \param        interp interpolation type WLZ_INTERPOLATION_NEAREST or WLZ_INTERPOLATION_LINEAR
    * \param        perrNum pointer to WlzErrorNum to store exit code. If NULL then there will be no errorcode returned
    * \return       the transformed value object
    * \par      Source:
    *                WoolzTransform.cpp
    */
    WlzObject* Transform(WoolzObject *srcObj,const WlzInterpolationType interp, WlzErrorNum *perrNum = NULL);

   /*!
    * \ingroup      Control
    * \brief        Transforms the mesh
    * \param        perrNum pointer to WlzErrorNum to store exit code. If NULL then there will be no errorcode returned
    * \return       the transformed mesh object
    * \par      Source:
    *                WoolzTransform.cpp
    */
    WlzObject* TransformedMesh(WlzErrorNum *perrNum = NULL);

   /*!
    * \ingroup      Control
    * \brief        Returns transformation object
    * \param        perrNum pointer to WlzErrorNum to store exit code. If NULL then there will be no errorcode returned
    * \return       the transformation object
    * \par      Source:
    *                WoolzTransform.cpp
    */
    WlzObject* getTransformObj(WlzErrorNum *perrNum = NULL);

protected:
   /*!
    * \ingroup      Control
    * \brief        Recomputes the basis transfrorm
    * \return       void
    * \par      Source:
    *                WoolzTransform.cpp
    */
    WlzErrorNum updateBasisTr();

   /*!
    * \ingroup      Control
    * \brief        Recomputes the direct transfrorm object
    * \return       void
    * \par      Source:
    *                WoolzTransform.cpp
    */
    WlzErrorNum updateDirectTransform();

   /*!
    * \ingroup      Control
    * \brief        Recomputes the inverse transfrorm object
    * \return       void
    * \par      Source:
    *                WoolzTransform.cpp
    */
    WlzErrorNum updateInverseTransform();

private slots:
   /*!
    * \ingroup      Control
    * \brief        Processes update request from managed objects
    * \return       void
    * \par      Source:
    *                WoolzTransform.cpp
    */
    void update();

   /*!
    * \ingroup      Control
    * \brief        Sets mesh object
    * \param        meshObject nem mesh object
    * \return       void
    * \par      Source:
    *                WoolzTransform.cpp
    */
    void setMeshObject(WoolzObject *meshObject);

signals:
   /*!
    * \ingroup      Control
    * \brief        Signals the transform was updated and all dependent objects must update
    * \return       void
    * \par      Source:
    *                WoolzTransform.cpp
    */
    void updated();

protected:
    LandmarkModel *m_landmarkModel;      /*!< landmark container */
    WoolzObject *m_meshObject;           /*!< mesh object */
    WlzBasisFnTransform *basisTr;        /*!< basis function transform */
    WlzObject * transformObj;            /*!< cached direct transform object */
    WlzObject * invertedtransformObj;    /*!< cached inverse transform object */
};

#endif // WOOLZTRANSFORM_H
