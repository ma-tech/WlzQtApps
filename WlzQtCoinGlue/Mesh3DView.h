#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _Mesh3DView_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         Mesh3DView.h
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
* \brief        View for 3D mesh.
* \ingroup      Views
*
*/

#ifndef MESH3DVIEW_H
#define MESH3DVIEW_H
#include "ObjectView.h"
#include <Wlz.h>
#include <Inventor/fields/SoMFInt32.h>

class SoCoordinate3;
class SoIndexedFaceSet;
class SoSeparator;
class SoMaterial;
class SoDrawStyle;
class SoClipPlane;

class QXmlStreamWriter;
class QDomElement;
/*!
* \brief	View providing 3D mesh visualisation
* \ingroup      Views
*/
class Mesh3DView : public ObjectView
{
  Q_OBJECT

public:
 /*!
  * \ingroup      Views
  * \brief        Constructor
  * \param        parent parent viewer
  * \param        object Woolz object to be visualised
  * \return       void
  * \par      Source:
  *                Mesh3DView.cpp
  */
  Mesh3DView ( QObject * parent, WoolzObject * object );

 /*!
  * \ingroup      Views
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *                Mesh3DView.cpp
  */
  virtual ~Mesh3DView ( );

 /*!
  * \ingroup      Visualisation
  * \brief        Sets the transparency value of the contour.
  *
  *           Implements View::setTransparency().
  * \param        transparency new transparency value between 0 (non transparent) and 100 (transparent)
  * \return       void
  * \par      Source:
  *                Mesh3DView.cpp
  */
  virtual void setTransparency ( int transparency );

 /*!
  * \ingroup      Visualisation
  * \brief        Sets the visualisation type
  *
  *             If 0, then visualisation is surface (full) rendered, 
  *             if 1 then visualisation is line  rendered
  *           Reimplements View::setVisualisationType().
  * \param        type new vvisualisation type
  * \return       void
  * \par      Source:
  *                Mesh3DView.cpp
  */
  void setVisualisationType(int type);

 /*!
  * \ingroup      Visualisation
  * \brief        Returns the number of available visualisations that is 2 for Mesh3DView.
  *
  *           Implements View::getVisualisationTypesNo().
  * \return       Return the number of available visualisations (2)
  * \par      Source:
  *                Mesh3DView.cpp
  */
  virtual int getVisualisationTypesNo () {return 2;}

 /*!
  * \ingroup      Visualisation
  * \brief        Returns the list of available visualisations.
  *
  *           Implements View::getVisualisationTypes().
  * \return       List of visualistion type names.
  * \par      Source:
  *                Mesh3DView.cpp
  */
  virtual QStringList getVisualisationTypes ();

 /*!
  * \ingroup      Views
  * \brief        Current visualisation type.
  *
  *   Currently only one visualisation is implemented, therefore it returns 0.
  *
  *           Implements View::visualisationType().
  * \return       void
  * \par      Source:
  *                Mesh3DView.cpp
  */
  virtual int visualisationType ();

protected:
 /*!
  * \ingroup      Visualisation
  * \brief        Builds the scene graph of the view.
  *
  *           Reimplements View::generateSceneGraph().
  * \param        bForce, if true force update of the scene graph elements
  * \return       void
  * \par      Source:
  *                Mesh3DView.cpp
  */
  virtual void generateSceneGraph ( bool bForce = false);

 /*!
  * \ingroup      Visualisation
  * \brief        Updates view material.
  *
  * \return       void
  * \par      Source:
  *                Mesh3DView.cpp
  */
  void updateMaterial();

 /*!
  * \ingroup      Visualisation
  * \brief        Returns if the current object is compatible with the view
  * \return       true of object is compatible, false if not.
  * \par      Source:
  *                Mesh3DView.cpp
  */
  virtual bool compatible( );

 /*!
  * \ingroup      Control
  * \brief        Saves view details in xml format.
  * \param        xmlWriter output xml stream
  * \return       true if succeded, false if not
  * \par      Source:
  *                Mesh3DView.cpp
  */
  virtual bool saveAsXmlProperties(QXmlStreamWriter *xmlWriter) ;

 /*!
  * \ingroup      Control
  * \brief        Reads current token if known
  * \param        element current element of the DOM tree
  * \return       true if succeded, false if not
  * \par      Source:
  *                Mesh3DView.cpp
  */
  virtual bool parseDOMLine(const QDomElement &element);

private:
 /*!
  * \ingroup      Visualisation
  * \brief        Generates vertices of the mesh.
  *
  * \param        vertices previous (empty) list of vertices
  * \param        errNum result error number
  * \return       SoCoordinate3 object with added new vertices.
  * \par      Source:
  *                Mesh3DView.cpp
  */
  SoCoordinate3 * Vertices3D(SoCoordinate3 * vertices, WlzErrorNum& errNum );

 /*!
  * \ingroup      Visualisation
  * \brief        Generates faces of the mesh.
  *
  * \param        faces previous (empty) list of faces
  * \param        errNum result error number
  * \return       SoIndexedFaceSet object with added new faces.
  * \par      Source:
  *                Mesh3DView.cpp
  */
  SoIndexedFaceSet * Faces3D(SoIndexedFaceSet * faces, WlzErrorNum& errNum );

public slots:
 /*!
  * \ingroup      Visualisation
  * \brief        Processes face removal
  *
  * \return       void.
  * \par      Source:
  *                Mesh3DView.cpp
  */
  void removeElement(int i);

 /*!
  * \ingroup      Visualisation
  * \brief        Processes object colour change.
  *
  * \return       void.
  * \par      Source:
  *                Mesh3DView.cpp
  */
  void objectColourChanged ();

 /*!
  * \ingroup      Visualisation
  * \brief        Sets up or deletes clip plane
  * \param        plane new plane to use
  * \return       void.
  * \par      Source:
  *                Contour3DView.cpp
  */
  void addedClipPlane(SoClipPlane * plane);

protected:
  // protected attributes
  SoMaterial*  m_material;                   /*!< material of the contour */
  SoDrawStyle* m_drawStyle;                  /*!< draw style of the contour */
  enum {FULL = 0, LINES} m_visualStyle;      /*!< visualisation style of the mesh */
  bool         m_clipOn;                     /*!< if the clip plane is on */
  SoClipPlane* m_clipPlane;                  /*!< reference clip plane from viewer */
  SoMFInt32    m_elemArray;                  /*!< vertex to face mapping */
};

#endif // MESH3DVIEW_H
