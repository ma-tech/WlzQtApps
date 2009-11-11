#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _WarperResultViewer_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         WarperResultViewer.h
* \author       Zsolt Husz
* \date         October 2008
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
* \brief        Viewer displaying result (warped) objects
* \ingroup      UI
*
*/

#ifndef WARPERRESULTVIEWER_H
#define WARPERRESULTVIEWER_H

#include "ObjectViewer.h"

class QXmlStreamWriter;

/*!
* \brief        Warping result object viewer class.
*
*               Viewer class displaying result (warped) objects.
*
* \ingroup      UI
*/
class WarperResultViewer : public ObjectViewer
{
  Q_OBJECT

public:
 /*!
  * \ingroup      UI
  * \brief        Constructor
  * \param        objectViewerModel model managing the viewer
  * \param        is3D if viewer is for 3D objects
  *
  * \return       void
  * \par      Source:
  *                WarperViewer.cpp
  */
  WarperResultViewer (ObjectViewerModel *objectViewerModel, bool is3D);

 /*!
  * \ingroup      UI
  * \brief        Checks if viewer accepts object.
  *
  * \return       true if viewer object is a warped object
  * \par      Source:
  *                WarperSourceViewer.cpp
  */
  virtual bool accepting(WoolzObject * object );

 /*!
  * \ingroup      UI
  * \brief        Returns the default object transparency.
  * \param        object the new object
  * \return       object transparency
  * \par      Source:
  *                ObjectViewer.cpp
  */
  virtual int initialTransparency(WoolzObject *object);

 /*!
  * \ingroup      UI
  * \brief        Configures the view
  *
  * \return       void
  * \par      Source:
  *                WarperSourceViewer.cpp
  */
  virtual void init();

 /*!
  * \ingroup      Control
  * \brief        Saves model in xml format.
  * \param        xmlWriter output xml stream
  * \return       true if succeded, false if not
  * \par      Source:
  *                WarperSourceViewer.cpp
  */
  virtual bool saveAsXml(QXmlStreamWriter *xmlWriter);

public slots:
 /*!
  * \ingroup      UI
  * \brief        Processes transparency slider change if slider is present
  * \param        transparency new transparency
  *
  *               Reimplemented form ObjectViewer
  * \return       void
  * \par      Source:
  *                ObjectViewer.cpp
  */
  virtual void transparencyChanged(int transparency);

protected:
 /*!
  * \ingroup      UI
  * \brief        Returns the background colour of the viewer
  *
  *               Reimplemented form ObjectViewer
  * \return       colour
  * \return       void
  * \par      Source:
  *                ObjectViewer.cpp
  */
  virtual QColor getBackgroundColour();

public:
    static const char * xmlTag;        /*!< xml section tag string */
};

#endif // WARPERRESULTVIEWER_H
