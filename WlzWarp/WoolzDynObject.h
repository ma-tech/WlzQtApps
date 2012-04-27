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
* \file         WoolzDynObject.h
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
* \brief        Container class for WlzObject type
* \ingroup      Control
*
*/
#ifndef WOOLZDYNOBJECT_H
#define WOOLZDYNOBJECT_H

#include "WoolzObject.h"

class ObjectListModel;

/*!
 * \brief	Class storing and managing and generated woolz object
 * \ingroup      Control
 */
class WoolzDynObject : public WoolzObject
{
  Q_OBJECT

public:
 /*!
  * \ingroup      Control
  * \brief        Constructor
  * \return       void
  * \par      Source:
  *               WoolzDynObject.cpp
  */
  WoolzDynObject(ObjectListModel * objectListModel);

 /*!
  * \ingroup      Control
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *               WoolzDynObject.cpp
  */
  ~WoolzDynObject();

 /*!
  * \ingroup      Control
  * \brief        Returns if auto update is on. Source must be set, otherwise it returns false
  * \return       true if auto update on, false otherwise
  * \par      Source:
  *               WoolzDynWarped.cpp
  */
  bool autoUpdate( ) {return m_autoUpdate && (sourceObj()!= NULL);}

 /*!
  * \ingroup      Control
  * \brief        Check if object can be used as source for the object
  * \param        src object to be used as source
  * \return       true is object can be source, false otherwise
  * \par      Source:
  *               WoolzDynWarped.cpp
  */
  bool isAllowedSource(WoolzObject *src);

 /*!
  * \ingroup      Control
  * \brief        Sets the type of the object from its source
  * \return       void
  * \par      Source:
  *               WoolzDynWarped.cpp
  */
  virtual void setTypeFromSource();

 /*!
  * \ingroup      Control
  * \brief        Sets up connections to target
  * \param        target connection target
  * \return       void
  * \par      Source:
  *               WoolzDynWarped.cpp
  */
  virtual void setupConnections(QObject *target);

 /*!
  * \ingroup      Control
  * \brief        Sets the source object
  * \param        src source object
  * \return       void
  * \par      Source:
  *               WoolzDynWarped.cpp
  */
  void setSourceObj(WoolzObject *src) ;

 /*!
  * \ingroup      Control
  * \brief        Sets the source object by ID
  * \param        srcID source object
  * \return       void
  * \par      Source:
  *                WoolzDynWarped.cpp
  */
  void setSourceObj(int srcID) ;

/*!
  * \ingroup      Control
  * \brief        Returns the source object
  * \return       source object
  * \par      Source:
  *                WoolzDynWarped.cpp
  */
  WoolzObject *sourceObj();

 /*!
  * \ingroup      Control
  * \brief        Returns the source object ID
  * \return       source object ID
  * \par      Source:
  *                WoolzDynWarped.cpp
  */
  int sourceObjID() { return m_srcObjID; }

public slots:
 /*!
  * \ingroup      Control
  * \brief        Updates object if auto update is on or if forced.
  *
  *               Uses doUpdate() for updateing.
  * \param        force forces update, even if auto update is false
  * \return       void
  * \par      Source:
  *               WoolzDynObject.cpp
  */
  virtual void update (bool force = false);

 /*!
  * \ingroup      Control
  * \brief        Processes the update of source object change
  * \return       void
  * \par      Source:
  *               WoolzDynObject.cpp
  */
  virtual void sourceChanged ();

 /*!
  * \ingroup      Control
  * \brief        Sets auto update
  * \param        newAutoupdate new auto update value
  * \return       void
  * \par      Source:
  *               WoolzDynWarped.cpp
  */
  void setAutoUpdate( bool newAutoupdate);

 /*!
  * \ingroup      Control
  * \brief        Processes source object removal
  * \param        newAutoupdate new auto update value
  * \return       void
  * \par      Source:
  *               WoolzDynWarped.cpp
  */
  void removedSourceObject();

  /*!
   * \ingroup      Control
   * \brief        Checks if object parameters changes requireing update
   * \return       true if object needs update
   * \par      Source:
   *                WoolzDynMeshObject.cpp
   */
   virtual bool needsUpdate();

protected:
  /*!
  * \ingroup      Control
  * \brief        Saves object details in xml format.
  * \param        element current element of the DOM tree
  * \return       true if succeded, false if not
  * \par      Source:
  *                WoolzDynWarped.cpp
  */
  virtual bool saveAsXmlProperties(QXmlStreamWriter *xmlWriter) ;

 /*!
  * \ingroup      Control
  * \brief        Reads current token if known
  * \param        element current element of the DOM tree
  * \return       true if succeded, false if not
  * \par      Source:
  *                WoolzDynWarped.cpp
  */
  virtual bool parseDOMLine(const QDomElement &element);


 /*!
  * \ingroup      Control
  * \brief        Save parameters used for dynamic object
  * \return       void
  * \par      Source:
  *               WoolzDynObject.cpp
  */
  virtual void saveUsedParameters();

 /*!
  * \ingroup      Control
  * \brief        Checks without reading the whole file if the object is 3D
  * \return       true if is a 3D and false if not
  * \par      Source:
  *                WoolzDynObject.cpp
  */
 bool is3DPreRead ( ) { return (!m_obj && (sourceObj()) && sourceObj()->is3DPreRead()) || is3D();}

  /*!
  * \ingroup      Control
  * \brief        Checks without reading the whole file if the object is 2D
  * \return       true if is a 2D and false if not
  * \par      Source:
  *                WoolzDynObject.cpp
  */
  bool is2DPreRead ( ) { return (!m_obj && (sourceObj()) && sourceObj()->is2DPreRead()) || is2D();}


  /*!
   * \ingroup      Control
   * \brief        Signals when an object parameters are changed and object needs update.
   * \return       void
   * \par      Source:
   *                WoolzDynObject.cpp
   */
//   void outdated();

protected:
  bool m_autoUpdate;                   /*!< auto update flag: update() performs
                                            update only if flag is true*/
  int m_srcObjID;                      /*!< source object ID*/
  int m_srcObjIDUsed;                  /*!< source object ID used for the object*/
  ObjectListModel *m_objectListModel;  /*!< model that provides the source object*/
};

#endif
