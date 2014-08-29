#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _WoolzDynMeshObject_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         WoolzDynMeshObject.h
* \author       Zsolt Husz
* \date         March 2009
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
* \brief	Dynamicaly generated mesh Woolz objects
* \ingroup	Control
*/

#ifndef WOOLZDYNMESHOBJECT_H
#define WOOLZDYNMESHOBJECT_H

#include "WoolzDynObject.h"

/*!
 * \brief	Class storing and managing and generated mesh woolz object
 * \ingroup	Control
 */
class WoolzDynMeshObject : public WoolzDynObject
{
  public:
    /*!
     * \ingroup	Control
     * \brief	Constructor
     */
    WoolzDynMeshObject(ObjectListModel * objectListModel);

    /*!
     * \ingroup	Control
     * \brief	Destructor
     */
    virtual ~WoolzDynMeshObject();

    /*!
     * \ingroup	Control
     * \brief	Returns notes of the object.
     * \return	Return the "generated" string
     */
    virtual QString notes () {return("generated");}

    /*!
     * \ingroup	Control
     * \brief	Sets mesh generation minimum distance
     * \param	meshMinDist		minimum mesh distance
     */
    void setMinDist(double meshMinDist)
    {
      m_meshMinDist = meshMinDist;
      emit updated(false);
    }

    /*!
     * \ingroup	Control
     * \brief	Sets mesh generation maximum distance
     * \param	meshMaxDist		maximum mesh distance
     */
    void setMaxDist(double meshMaxDist)
    {
      m_meshMaxDist = meshMaxDist;
      emit updated(false);
    }

    /*!
     * \ingroup	Control
     * \brief	Saves object details in xml format.
     * \param	xmlWriter		output xml stream
     * \return	true if succeded, false if not
     */
    virtual bool saveAsXml(QXmlStreamWriter *xmlWriter);

  protected:
    /*!
     * \ingroup	Control
     * \brief	Updates the object.
     *
     */
    virtual void doUpdate ();

    /*!
     * \ingroup	Control
     * \brief	Saves object details in xml format.
     * \param	xmlWriter		output xml stream
     * \return	true if succeded, false if not
     */
    virtual bool saveAsXmlProperties(QXmlStreamWriter *xmlWriter);

    /*!
     * \ingroup	Control
     * \brief	Reads current token if known
     * \param	element			current element of the DOM tree
     * \return	true if succeded, false if not
     */
    virtual bool parseDOMLine(const QDomElement &element);

    /*!
     * \ingroup	Control
     * \brief	Checks if object parameters changes requiring update
     * \return	true if object needs update
     */
    virtual bool needsUpdate();

    /*!
     * \ingroup	Control
     * \brief	Save parameters used for dynamic object
     */
    virtual void saveUsedParameters();

    /*!
     * \ingroup	Control
     * \brief	Sets up connections to target
     * \param	target			connection target
     */
    virtual void setupConnections(QObject *target);

    /*!
     * \ingroup	Control
     * \brief	Checks without reading the whole file if the object is
     * 		a Contour
     * \return	true if is a 2D or 3D contour
     */
    virtual bool isMeshPreRead()
    {
      return((m_obj== NULL && m_srcObjID!=-1) || isMesh());
    }

  protected:
    double m_meshMinDist;		/*!< mesh minimum distance */
    double m_meshMaxDist;           	/*!< mesh maximum distance */
    double m_meshMinDistUsed;       	/*!< mesh minimum distance used in
    					     dynamic object */
    double m_meshMaxDistUsed;       	/*!< mesh maximum distance used in
    					     dynamic object */
  public:
    static const char * xmlTag;     	/*!< xml section tag string */
};

#endif
