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
* \file         WoolzDynWarpedObject .h
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
#ifndef WOOLZDYNWARPEDOBJECT_H
#define WOOLZDYNWARPEDOBJECT_H

#include "WoolzDynObject.h"

class WoolzTransform;

/*!
 * \brief	Container class for WlzObject
 *
 *              Inherits QObject, hence signaling mechanism is available
 * \ingroup      Control
 */
class WoolzDynWarpedObject  : public WoolzDynObject
{
  Q_OBJECT
public:

 /*!
  * \ingroup      Control
  * \brief        Constructor
  * \param        objectListModel object list model
  * \param        woolzTransform transformation model
  * \return       void
  * \par      Source:
  *                WoolzDynWarpedObject.cpp
  */
  WoolzDynWarpedObject (ObjectListModel * objectListModel, WoolzTransform *woolzTransform);

 /*!
  * \ingroup      Control
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *                WoolzDynWarpedObject.cpp
  */
  virtual ~WoolzDynWarpedObject ();

 /*!
  * \ingroup      Control
  * \brief        Returns notes of the object.
  * \return       Notes about the warped object
  * \par      Source:
  *                WoolzDynWarpedObject.cpp
  */
  virtual QString notes ( );

 /*!
  * \ingroup      Control
  * \brief        Returns if the object is warped.
  * \return       true
  * \par      Source:
  *                WoolzDynWarpedObject.cpp
  */
  virtual bool isWarped() {return true;}

 /*!
  * \ingroup      Control
  * \brief        Sets the type of the object beeing the complement of its source
  * \return       void
  * \par      Source:
  *                WoolzDynWarpedObject.cpp
  */
  virtual void setTypeFromSource();

 /*!
  * \ingroup      Control
  * \brief        Sets up connections to target
  * \param        target connection target
  * \return       void
  * \par      Source:
  *                WoolzDynWarpedObject.cpp
  */
  virtual void setupConnections(QObject *target);

 /*!
  * \ingroup      Control
  * \brief        Saves object details in xml format.
  * \param        xmlWriter output xml stream
  * \return       true if succeded, false if not
  * \par      Source:
  *                WoolzDynWarpedObject.cpp
  */
  virtual bool saveAsXml(QXmlStreamWriter *xmlWriter);

 /*!
  * \ingroup      Control
  * \brief        Checks without reading the whole file if the object is Warped
  * \return       false
  * \par      Source:
  *                WoolzDynWarpedObject.cpp
  */
  virtual bool isWarpedPreRead() {return (m_obj == NULL && m_srcObjID!=-1) || isWarped();}

 /*!
  * \ingroup      Control
  * \brief        Checks without reading the whole file if the object is a Mesh
  * \return       true if is a 2D or 3D contour
  * \par      Source:
  *                WoolzDynWarpedObject.cpp
  */
  virtual bool isMeshPreRead () { return (!m_obj && (sourceObj()) && sourceObj()->isMeshPreRead()) || isMesh();}

 /*!
  * \ingroup      Control
  * \brief        Checks without reading the whole file if the object is a Value object
  * \return       true if is a 2D or 3D value object
  * \par      Source:
  *                WoolzDynWarpedObject.cpp
  */
  virtual bool isValuePreRead () { return (!m_obj && (sourceObj()) && sourceObj()->isValuePreRead()) || isValue();}

 /*!
  * \ingroup      Control
  * \brief        Checks without reading the whole file if the object is colour
  * \return       true if is a colour value object
  * \par      Source:
  *                WoolzDynWarpedObject.cpp
  */
  virtual bool isColourPreRead () { return (!m_obj && (sourceObj()) && sourceObj()->isColourPreRead()) || isColour();}

/*!
  * \ingroup      Control
  * \brief        Checks if object parameters changes requiring update
  * \return       true if object needs update
  * \par      Source:
  *                WoolzDynWarpedObject.cpp
  */
  virtual bool needsUpdate();


 /*!
  * \ingroup      Control
  * \brief        Save parameters used for dynamic object
  * \return       void
  * \par      Source:
  *               WoolzDynWarpedObject.cpp
  */
  virtual void saveUsedParameters();

public slots:
 /*!
  * \ingroup      Control
  * \brief        Updates object if auto update is on or if forced.
  *
  *               Uses doUpdate() for updateing.
  * \param        force forces update, even if auto update is false
  * \return       void
  * \par      Source:
  *                WoolzDynWarpedObject.cpp
  */
  virtual void update ( bool force = false);

 /*!
  * \ingroup      Control
  * \brief        Processes tranformation update
  * \return       void
  * \par      Source:
  *                WoolzDynWarpedObject.cpp
  */
  virtual void updateWarp();

protected:
 /*!
  * \ingroup      Control
  * \brief        Replaces a previous warping with a newly generated object.
  *
  *               Calls private method warp() do to the job.
  * \return       void
  * \par      Source:
  *                WoolzDynWarpedObject.cpp
  */
  virtual void doUpdate ();

signals:
 /*!
  * \ingroup      Control
  * \brief        Signals if warp can not be completed due to an error
  * \return       void
  * \par      Source:
  *                WoolzDynWarpedObject.cpp
  */
  void invalidWarping();

private:
 /*!
  * \ingroup      Control
  * \brief        Performs the warp
  * \return       void
  * \par      Source:
  *                WoolzDynWarpedObject.cpp
  */
  void warp ();

protected:
  WoolzTransform *m_woolzTransform;    /*!< tranformation object */
  bool m_outdatedWarp;                 /*!< if true then warping is out of date due to transform change*/
public:
  static const char * xmlTag;          /*!< xml section tag string */
};
#endif
