#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _WoolzFileObject_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         WoolzFileObject.cpp
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
#include <QFileInfo>
#include <WlzExtFF.h>

#include <QtXml/QXmlStreamWriter>
#include <QtXml/QDomElement>
#include <QStringList>
#include "WoolzFileObject.h"

const char* WoolzFileObject::xmlTag = "FileObject";
QString WoolzFileObject::m_baseDir = "";

WoolzFileObject::WoolzFileObject() : WoolzObject() {
 m_filename = "";
 m_fileObjType = WLZ_DUMMY_ENTRY;
 m_obj = NULL;
}

WoolzFileObject::~WoolzFileObject() {
}

void WoolzFileObject::setFileName ( QString filename) {
    m_filename = QFileInfo(filename).canonicalFilePath();
}

void WoolzFileObject::open ( QString filename, WoolzObjectType type) {
    setFileName(filename);
    m_name = QFileInfo(m_filename).fileName();
    m_type = type;
    doUpdate();
}

void WoolzFileObject::doUpdate () {
    if (m_obj)
       return;

    statusChange(QString("Object loading"),0);
    WlzErrorNum errNum = WLZ_ERR_NONE;

    m_obj = WlzEffReadObj(NULL , m_filename.toAscii(),
        WlzEffStringFormatFromFileName(m_filename.toAscii()), 0, &errNum);

    if (! m_obj)
        return;

    if (errNum == WLZ_ERR_DOMAIN_TYPE) {
        WlzFreeObj(m_obj);
        m_obj = NULL;
        return;
    }

    m_obj = WlzAssignObject( m_obj, &errNum );
    if (m_obj->type == WLZ_3D_DOMAINOBJ && m_obj->domain.p != NULL) {
        if (m_obj->domain.p->plane1==m_obj->domain.p->lastpl) { // if this is a single plane 3D object
           WlzObject **objects;
           int size;
           WlzExplode3D(&size, &objects, m_obj);
           WlzFreeObj(m_obj);
           Q_ASSERT(size==1);
           m_obj = WlzAssignObject( objects[0], &errNum );
           AlcFree(objects);
            m_fileObjType = m_obj->type;
       }
    }
    setupTransferFunction();
    statusChange(QString("Object loaded"), 0);
    emit objectChanged();
}

void WoolzFileObject::readType ( QString filename) {
    WlzErrorNum errNum = WLZ_ERR_NONE;
    m_filename = filename;
    FILE *fp;
    if ((fp=fopen(filename.toAscii(), "rb"))!=NULL) {
      m_fileObjType=WlzReadObjType(fp, &errNum);
      fclose(fp);
    }
}

bool WoolzFileObject::isMeshPreRead ( ) {
  return (m_fileObjType== WLZ_CMESH_2D) || (m_fileObjType == WLZ_CMESH_3D);
}

bool WoolzFileObject::isValuePreRead ( ) {
  if (WlzEffStringFormatFromFileName(m_filename.toAscii()) != WLZEFF_FORMAT_WLZ)
    return true;
  return (m_fileObjType == WLZ_3D_DOMAINOBJ) ||
         (m_fileObjType == WLZ_2D_DOMAINOBJ);
}

bool WoolzFileObject::isContourPreRead ( ) {
  return (m_fileObjType == WLZ_CONTOUR);
}

void WoolzFileObject:: update( bool /*force*/ ) {
  if (!m_obj)
    doUpdate();
}

QString WoolzFileObject::getValueFormats() {
    int i;
    QString formats;
    QString wlzformat;
    QString extensions;
    const char *name,*extension;
    int numFormats = WlzEffNumberOfFormats();
    for(i=1; i <= numFormats; i++){
      name = WlzEffStringFromFormat( (WlzEffFormat)i, &extension);
      if (i==WLZEFF_FORMAT_WLZ )
        wlzformat = QString("%1 (*.%2);;").arg(name).arg(extension) ;
      else
        formats = formats + QString("%1 (*.%2);;").arg(name).arg(extension) ;
      extensions += QString("*.%1 ").arg(extension) ;
    }
    formats = wlzformat + QString("All value formats (%1);;").arg(extensions) + formats + QString("All (*)");
    return formats;
}

bool WoolzFileObject::saveAsXml(QXmlStreamWriter *xmlWriter) {
  xmlWriter->writeStartElement(xmlTag);
  saveAsXmlProperties(xmlWriter);
  xmlWriter->writeEndElement();
  return true;
}

bool WoolzFileObject::saveAsXmlProperties(QXmlStreamWriter *xmlWriter) {
  WoolzObject::saveAsXmlProperties(xmlWriter);
  xmlWriter->writeTextElement("Filename", stripedFilePath(m_filename));
  return true;
}

bool WoolzFileObject::parseDOMLine(const QDomElement &element) {
  if (element.tagName() == "Filename") {
     setFileName(fullFilePath(element.text()));
     readType(m_filename);
     return true;
  } else
     return WoolzObject::parseDOMLine(element);
}


bool WoolzFileObject::isMesh ( ) {
  return (m_obj && WoolzObject::isMesh()) || (!m_obj && isMeshPreRead());
}

bool WoolzFileObject::isValue ( ) {
  return (m_obj && WoolzObject::isValue()) || (!m_obj && isValuePreRead());
}

bool WoolzFileObject::isContour ( ) {
  return (m_obj && WoolzObject::isContour()) || (!m_obj && isContourPreRead());
}

void WoolzFileObject::setupConnections(QObject *target) {
    WoolzObject::setupConnections(target);
    connect( target, SIGNAL(loadAllSignal()), this, SLOT(update()));
}

QString WoolzFileObject::fullFilePath(QString file) {
    return (file.length()>1 && file[0]!='/') ? m_baseDir + '/' + file : file;
}

void WoolzFileObject::setBaseDir(QString baseDir) {
    m_baseDir = baseDir;
}

QString RelativePath( QString absolutePath, QString relativeTo, bool bIsFile /*= false*/ )
{
QStringList absoluteDirectories = absolutePath.split( '/', QString::SkipEmptyParts );
QStringList relativeDirectories = relativeTo.split( '/', QString::SkipEmptyParts );

//Get the shortest of the two paths
int length = absoluteDirectories.count() < relativeDirectories.count() ? absoluteDirectories.count() : relativeDirectories.count();

//Use to determine where in the loop we exited
int lastCommonRoot = -1;
int index;

//Find common root
for (index = 0; index < length; index++)
if (absoluteDirectories[index] == relativeDirectories[index])
lastCommonRoot = index;
else
break;

//If we didn't find a common prefix then throw
if (lastCommonRoot == -1)
throw QString("Paths do not have a common base");

//Build up the relative path
QString relativePath;

//Add on the ..
for (index = lastCommonRoot + 1; index < relativeDirectories.length(); index++)
if (relativeDirectories[index].length() > 0)
relativePath.append("../");

//Add on the folders
for (index = lastCommonRoot + 1; index < absoluteDirectories.length() - 1; index++)
relativePath.append(absoluteDirectories[index] + "/");

relativePath.append(absoluteDirectories[absoluteDirectories.length() - 1]);

return relativePath;
}

QString WoolzFileObject::stripedFilePath(QString file) {
   return RelativePath(file, m_baseDir, true);
}
