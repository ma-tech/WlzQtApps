#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _WoolzFileObject_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         WoolzFileObject.h
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

#ifndef WOOLZFILEOBJECT_H
#define WOOLZFILEOBJECT_H

#include "WoolzObject.h"

#include <QtXml/QXmlStreamWriter>
class QDomElement;

/*!
 * \brief	Class storing and managing and loaded woolz object
 * \ingroup      Control
 */
class WoolzFileObject : public WoolzObject
{

public:
 /*!
  * \ingroup      Control
  * \brief        Constructor
  * \return       void
  * \par      Source:
  *                WoolzFileObject.cpp
  */
  WoolzFileObject();

 /*!
  * \ingroup      Control
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *                WoolzFileObject.cpp
  */
  virtual ~WoolzFileObject();

 /*!
  * \ingroup      Control
  * \brief        Loads a WlzObject from a file
  * \param        filename filename from witch object is read
  * \param        type of the loaded object
  * \return       void
  * \par      Source:
  *                WoolzFileObject.cpp
  */
  virtual void open ( QString filename, WoolzObjectType type);

   /*!
  * \ingroup      Control
  * \brief        Sets the object filename
  * \param        filename filename from witch object is to be read
  * \return       void
  * \par      Source:
  *                WoolzFileObject.cpp
  */
  virtual void setFileName ( QString filename);

 /*!
  * \ingroup      Control
  * \brief        Reloads WlzObject from a file if needed
  * \return       void
  * \par      Source:
  *                WoolzFileObject.cpp
  */
  virtual void doUpdate ();

 /*!
  * \ingroup      Control
  * \brief        Reads only the object type of the WoolzObject
  * \param        filename filename from witch object is read
  * \return       filename
  * \par      Source:
  *                WoolzFileObject.cpp
  */
  void readType ( QString filename);

 /*!
  * \ingroup      Control
  * \brief        Returns notes of the object.
  * \return       Return the file name as a note
  * \par      Source:
  *                WoolzFileObject.cpp
  */
  virtual QString notes ( )   {
    return "file=" + stripedFilePath(m_filename);
  }

 /*!
  * \ingroup      Control
  * \brief        Checkes if the object is a Mesh
  * \return       true if is a 2D or 3D mesh
  * \par      Source:
  *                WoolzFileObject.cpp
  */
  virtual bool isMesh ( ) ;

 /*!
  * \ingroup      Control
  * \brief        Checkes if the object is a contour
  * \return       true if is a 2D or 3D contour
  * \par      Source:
  *                WoolzFileObject.cpp
  */
  virtual bool isContour ( ) ;

 /*!
  * \ingroup      Control
  * \brief        Checkes if the object is a value object
  * \return       true if is a value object
  * \par      Source:
  *                WoolzFileObject.cpp
  */
  virtual bool isValue ( ) ;

 /*!
  * \ingroup      Control
  * \brief        Checks without reading the whole file if the object is a Mesh
  * \return       true if is a 2D or 3D mesh
  * \par      Source:
  *                WoolzFileObject.cpp
  */
  virtual bool isMeshPreRead ();

 /*!
  * \ingroup      Control
  * \brief        Checks without reading the whole file if the object is a Value object
  * \return       true if is a 2D or 3D value object
  * \par      Source:
  *                WoolzFileObject.cpp
  */
  virtual bool isValuePreRead ();

 /*!
  * \ingroup      Control
  * \brief        Checks without reading the whole file if the object is a Contour
  * \return       true if is a 2D or 3D contour
  * \par      Source:
  *                WoolzFileObject.cpp
  */
  virtual bool isContourPreRead ();

 /*!
  * \ingroup      Control
  * \brief        Returns the types and extensions of value formats to be used as QFileDialog format parameter
  * \return       string of formats for QFileDialog
  * \par      Source:
  *                WoolzFileObject.cpp
  */
  static  QString getValueFormats();

 /*!
  * \ingroup      Control
  * \brief        Saves object details in xml format.
  * \param        xmlWriter output xml stream
  * \return       true if succeded, false if not
  * \par      Source:
  *                WoolzFileObject.cpp
  */
  virtual bool saveAsXml(QXmlStreamWriter *xmlWriter);

 /*!
  * \ingroup      Control
  * \brief        Saves object details in xml format.
  * \param        xmlWriter output xml stream
  * \return       true if succeded, false if not
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool saveAsXmlProperties(QXmlStreamWriter *xmlWriter) ;

 /*!
  * \ingroup      Control
  * \brief        Reads current token if known
  * \param        element current element of the DOM tree
  * \return       true if succeded, false if not
  * \par      Source:
  *                WoolzFileObject.cpp
  */
  virtual bool parseDOMLine(const QDomElement &element);

 /*!
  * \ingroup      Control
  * \brief        Sets up connections to target
  * \param        target connection target
  * \return       void
  * \par      Source:
  *                WoolzFileObject.cpp
  */
  virtual void setupConnections(QObject *target);

/*!
 * \ingroup      Control
 * \brief        Sets the base dir of all objects
 * \param        baseDir base directory of all object
 * \return       void
 * \par      Source:
 *                WoolzFileObject.cpp
 */
 static void setBaseDir(QString baseDir);

protected:
 /*!
  * \ingroup      Control
  * \brief        Return the file path striped of the project base path
  * \return       file relative to the project
  * \par      Source:
  *                WoolzFileObject.h
  */
  QString stripedFilePath(QString file);

 /*!
  * \ingroup      Control
  * \brief        Return the full file path
  * \return       file relative to the project
  * \par      Source:
  *                WoolzFileObject.h
  */
  QString fullFilePath(QString file);

public slots:
 /*!
  * \ingroup      Control
  * \brief        Updates object if auto update is on or if forced.
  *
  *               Uses doUpdate() for updateing.
  * \param        force forces update, even if auto update is false
  * \return       void
  * \par      Source:
  *                WoolzDynObject.cpp
  */
  virtual void update ( bool force = false);

protected:
  QString m_filename;            /*!< the object filenam where is physicaly stored*/
  WlzObjectType m_fileObjType;   /*!< object type as read from the file*/
  static QString m_baseDir;      /*!< the base dir of all objects*/

public:
  static const char* xmlTag;     /*!< xml section tag string */

};

#endif

