#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _WoolzDynMeshObject_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         WoolzDynMeshObject.h
* \author       Zsolt Husz
* \date         March 2009
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
* \brief        Dynamicaly generated mesh Woolz objects
* \ingroup      Control
*
*/
#ifndef WOOLZDYNMESHOBJECT_H
#define WOOLZDYNMESHOBJECT_H

#include <WoolzDynObject.h>

/*!
 * \brief	Class storing and managing and generated mesh woolz object
 * \ingroup      Control
 */
class WoolzDynMeshObject : public WoolzDynObject
{
public:
 /*!
  * \ingroup      Control
  * \brief        Constructor
  * \return       void
  * \par      Source:
  *                WoolzDynMeshObject.cpp
  */
  WoolzDynMeshObject(ObjectListModel * objectListModel);

 /*!
  * \ingroup      Control
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *                WoolzDynMeshObject.cpp
  */
  virtual ~WoolzDynMeshObject();

 /*!
  * \ingroup      Control
  * \brief        Returns notes of the object.
  * \return       Return the "generated" string
  * \par      Source:
  *                WoolzDynMeshObject.cpp
  */
  virtual QString notes ( )   {
    return "generated";
  }

 /*!
  * \ingroup      Control
  * \brief        Sets mesh generation minimum distance
  * \param        meshMinDist minimum mesh distance
  * \return       void
  * \par      Source:
  *                WoolzDynMeshObject.cpp
  */
  void setMinDist ( double meshMinDist)  {
    m_meshMinDist = meshMinDist;
  }

 /*!
  * \ingroup      Control
  * \brief        Sets mesh generation maximum distance
  * \param        meshMaxDist maximum mesh distance
  * \return       void
  * \par      Source:
  *                WoolzDynMeshObject.cpp
  */
  void setMaxDist ( double meshMaxDist) {
    m_meshMaxDist = meshMaxDist;
  }

 /*!
  * \ingroup      Control
  * \brief        Saves object details in xml format.
  * \param        xmlWriter output xml stream
  * \return       true if succeded, false if not
  * \par      Source:
  *                WoolzDynMeshObject.cpp
  */
  virtual bool saveAsXml(QXmlStreamWriter *xmlWriter);
protected:
 /*!
  * \ingroup      Control
  * \brief        Updates the object.
  *
  * \return       void
  * \par      Source:
  *                WoolzDynMeshObject.cpp
  */
  virtual void doUpdate ();

/*!
  * \ingroup      Control
  * \brief        Saves object details in xml format.
  * \param        xmlWriter output xml stream
  * \return       true if succeded, false if not
  * \par      Source:
  *                WoolzDynMeshObject.cpp
  */
  virtual bool saveAsXmlProperties(QXmlStreamWriter *xmlWriter) ;

 /*!
  * \ingroup      Control
  * \brief        Reads current token if known
  * \param        element current element of the DOM tree
  * \return       true if succeded, false if not
  * \par      Source:
  *                WoolzDynMeshObject.cpp
  */
  virtual bool parseDOMLine(const QDomElement &element);

 /*!
  * \ingroup      Control
  * \brief        Checks if object parameters changes requiring update
  * \return       true if object needs update
  * \par      Source:
  *                WoolzDynMeshObject.cpp
  */
  virtual bool needsUpdate();

 /*!
  * \ingroup      Control
  * \brief        Save parameters used for dynamic object
  * \return       void
  * \par      Source:
  *               WoolzDynMeshObject.cpp
  */
  virtual void saveUsedParameters();

 /*!
  * \ingroup      Control
  * \brief        Sets up connections to target
  * \param        target connection target
  * \return       void
  * \par      Source:
  *                WoolzDynMeshObject.cpp
  */
  virtual void setupConnections(QObject *target);

 /*!
  * \ingroup      Control
  * \brief        Checks without reading the whole file if the object is a Contour
  * \return       true if is a 2D or 3D contour
  * \par      Source:
  *                WoolzDynMeshObject.cpp
  */
  virtual bool isMeshPreRead () { return (m_obj== NULL && m_srcObjID!=-1) || isMesh();}

protected:
  double m_meshMinDist;           /*!< mesh minimum distance */
  double m_meshMaxDist;           /*!< mesh maximum distance */
  double m_meshMinDistUsed;       /*!< mesh minimum distance used in dynamic object*/
  double m_meshMaxDistUsed;       /*!< mesh maximum distance used in dynamic object*/
public:
  static const char * xmlTag;     /*!< xml section tag string */
};

#endif
