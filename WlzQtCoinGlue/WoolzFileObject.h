#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _WoolzFileObject_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         WoolzFileObject.h
* \author       Zsolt Husz, Bill Hill
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
* \brief	Container class for WlzObject type
* \ingroup	Control
*/

#ifndef WOOLZFILEOBJECT_H
#define WOOLZFILEOBJECT_H

#include "WoolzObject.h"

#include <QtXml/QXmlStreamWriter>
class QDomElement;

/*!
 * \brief	Class storing and managing and loaded woolz object
 * \ingroup	Control
 */
class WoolzFileObject: public WoolzObject
{
  public:
    /*!
     * \ingroup	Control
     * \brief	Constructor
     */
    WoolzFileObject();

    /*!
     * \ingroup	Control
     * \brief	Destructor
     */
    virtual ~WoolzFileObject();

    /*!
     * \ingroup	Control
     * \brief	Loads a WlzObject from a file
     * \param	filename		filename from witch object is read
     * \param	type			of the loaded object
     */
    virtual void open(QString filename, WoolzObjectType type);

    /*!
     * \ingroup	Control
     * \brief	Sets the object filename
     * \param	filename		filename from witch object is to be
     * 					read
     */
    virtual void setFileName(QString filename);

    /*!
     * \ingroup	Control
     * \brief	Reloads WlzObject from a file if needed
     */
    virtual void doUpdate();

    /*!
     * \ingroup	Control
     * \brief	Reads only the object type of the WoolzObject
     * \param	filename		filename from witch object is read
     * \return	filename
     */
    void readType(QString filename);

    /*!
     * \return	Return the file name as a note
     * \ingroup	Control
     * \brief	Returns notes of the object.
     */
    virtual QString notes()
    {
      return "file=" + stripedFilePath(m_filename);
    }

    /*!
     * \return	true if is a 2D or 3D mesh
     * \ingroup	Control
     * \brief	Checkes if the object is a Mesh
     */
    virtual bool isMesh() ;

    /*!
     * \return	true if is a 2D or 3D contour
     * \ingroup	Control
     * \brief	Checkes if the object is a contour
     */
    virtual bool isContour() ;

    /*!
     * \return	true if is a 2D or 3D convex hull
     * \ingroup	Control
     * \brief	Checkes if the object is a convex hull
     */
    virtual bool isConvHull() ;

    /*!
     * \return	true if is a value object
     * \ingroup	Control
     * \brief	Checkes if the object is a value object
     */
    virtual bool isValue() ;

    /*!
     * \return	true if is a 2D or 3D mesh
     * \ingroup	Control
     * \brief	Checks without reading the whole file if the object is a Mesh
     */
    virtual bool isMeshPreRead();

    /*!
     * \return	true if is a 2D or 3D value object
     * \ingroup	Control
     * \brief	Checks without reading the whole file if the object is a Value
     * 		object
     */
    virtual bool isValuePreRead();

    /*!
     * \return	true if is a 2D or 3D contour
     * \ingroup	Control
     * \brief	Checks without reading the whole file if the object is a
     * 		Contour
     */
    virtual bool isContourPreRead();

    /*!
     * \return	true if is a 2D or 3D convex hull
     * \ingroup	Control
     * \brief	Checks without reading the whole file if the object is
     * 		a convex hull
     */
    virtual bool isConvHullPreRead();

    /*!
     * \return	string of formats for QFileDialog
     * \ingroup	Control
     * \brief	Returns the types and extensions of value formats to be
     * 		used as QFileDialog format parameter
     */
    static  QString getValueFormats();

    /*!
     * \return	true if succeded, false if not
     * \ingroup	Control
     * \brief	Saves object details in xml format.
     * \param	xmlWriter		output xml stream
     */
    virtual bool saveAsXml(QXmlStreamWriter *xmlWriter);

    /*!
     * \return	true if succeded, false if not
     * \ingroup	Control
     * \brief	Saves object details in xml format.
     * \param	xmlWriter		output xml stream
     */
    virtual bool saveAsXmlProperties(QXmlStreamWriter *xmlWriter) ;

    /*!
     * \return	true if succeded, false if not
     * \ingroup	Control
     * \brief	Reads current token if known
     * \param	element			current element of the DOM tree
     */
    virtual bool parseDOMLine(const QDomElement &element);

    /*!
     * \ingroup	Control
     * \brief	Sets up connections to target
     * \param	target			connection target
     */
    virtual void setupConnections(QObject *target);

    /*!
     * \ingroup	Control
     * \brief	Sets the base dir of all objects
     * \param	baseDir			base directory of all object
     */
    static void setBaseDir(QString baseDir);

  protected:
    /*!
     * \return	file relative to the project
     * \ingroup	Control
     * \brief	Return the file path striped of the project base path
     */
    QString stripedFilePath(QString file);

    /*!
     * \return	file relative to the project
     * \ingroup	Control
     * \brief	Return the full file path
     */
    QString fullFilePath(QString file);

    public slots:
    /*!
     * \ingroup	Control
     * \brief	Updates object if auto update is on or if forced.
     *
     *          Uses doUpdate() for updateing.
     * \param	force			forces update, even if auto update
     * 					is false
     */
    virtual void update (bool force = false);

  protected:
    QString m_filename;            	/*!< the object filenam where is
    					     physicaly stored */
    WlzObjectType m_fileObjType;   	/*!< object type as read from the
      					     file */
    static QString m_baseDir;      	/*!< the base dir of all objects */

  public:
    static const char* xmlTag;     	/*!< xml section tag string */

};

#endif

