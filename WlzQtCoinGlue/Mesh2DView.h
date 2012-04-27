#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _Mesh2DView_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         Mesh2DView.h
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
* \brief        View for 2D mesh.
* \ingroup      Views
*
*/


#ifndef MESH2DVIEW_H
#define MESH2DVIEW_H
#include "ObjectView.h"
#include <Wlz.h>

class SoCoordinate3;
class SoIndexedFaceSet;
class SoSeparator;
class SoMaterial;
class SoDrawStyle;

/*!
* \brief	View providing 2D mesh visualisation
* \ingroup      Views
*/
class Mesh2DView : public ObjectView
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
  *                Mesh2DView.cpp
  */
  Mesh2DView ( QObject * parent, WoolzObject * object );

 /*!
  * \ingroup      Views
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *                Mesh2DView.cpp
  */
  virtual ~Mesh2DView ( );

protected:
  // protected attributes
  SoMaterial *m_material;                    /*!< material of the mesh */
  SoDrawStyle *m_drawStyle;                  /*!< style of the mesh */

public:

 /*!
  * \ingroup      Visualisation
  * \brief        Sets the transparency value of the mesh.
  *
  *           Implements View::setTransparency().
  * \param        transparency new transparency value between 0 (non transparent) and 100 (transparent)
  * \return       void
  * \par      Source:
  *                Mesh2DView.cpp
  */
  virtual void setTransparency ( int transparency );

 /*!
  * \ingroup      Visualisation
  * \brief        Sets default visibility for new viewers. If true then any new compatible viewer will
  *               include this view when it is created.
  *
  *           Reimplements View::setVisibility().
  * \param        visibility new visibility value
  * \return       void
  * \par      Source:
  *                Mesh2DView.cpp
  */
  virtual void setVisibility ( bool visibility );

 /*!
  * \ingroup      Visualisation
  * \brief        Returns the number of available visualisations that is 1 for Mesh2DView.
  *
  *           Implements View::getVisualisationTypesNo().
  * \return       Return the number of available visualisations (1)
  * \par      Source:
  *                Mesh2DView.cpp
  */
  virtual int getVisualisationTypesNo () {return 1;}

 /*!
  * \ingroup      Visualisation
  * \brief        Returns the list of available visualisations.
    *
  *           Implements View::getVisualisationTypes().
  * \return       List of visualistion type names.
  * \par      Source:
  *                Mesh2DView.cpp
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
  *                Mesh2DView.cpp
  */
  virtual int visualisationType () {return 0;}

 /*!
  * \ingroup      Views
  * \brief        Redefines standard behaviour of adding view instead as last, but as first view.
  *
  *           Reimplements View::addAsFirst().
  * \return       void
  * \par      Source:
  *                Mesh2DView.cpp
  */
  virtual bool addAsFirst ( ) {return true;}

protected:
 /*!
  * \ingroup      Visualisation
  * \brief        Builds the scene graph of the view.
  *
  *           Reimplements View::generateSceneGraph().
  * \param        bForce, if true force update of the scene graph elements
  * \return       void
  * \par      Source:
  *                Mesh2DView.cpp
  */
  virtual void generateSceneGraph ( bool bForce = false);

 /*!
  * \ingroup      Visualisation
  * \brief        Updates view material.
  *
  * \return       void
  * \par      Source:
  *                Mesh2DView.cpp
  */
  void updateMaterial();

 /*!
  * \ingroup      Visualisation
  * \brief        Returns if the current object is compatible with the view
  * \return       true of object is compatible, false if not.
  * \par      Source:
  *                Mesh2DView.cpp
  */
  virtual bool compatible( );

private:

 /*!
  * \ingroup      Visualisation
  * \brief        Generates vertices of the mesh.
  *
  * \param        vertices previous (empty) list of vertices
  * \param        errNum result error number
  * \return       SoCoordinate3 object with added new vertices.
  * \par      Source:
  *                Mesh2DView.cpp
  */
  SoCoordinate3 * Vertices2D(SoCoordinate3 * vertices, WlzErrorNum& errNum );

 /*!
  * \ingroup      Visualisation
  * \brief        Generates lines of the mesh.
  *
  * \param        faces previous (empty) list of faces
  * \param        errNum result error number
  * \return       SoIndexedFaceSet object with added new faces.
  * \par      Source:
  *                Mesh2DView.cpp
  */
  SoIndexedFaceSet * Faces2D(SoIndexedFaceSet * faces, WlzErrorNum& errNum );

public slots:

 /*!
  * \ingroup      Visualisation
  * \brief        Processes object colour change.
  *
  * \return       void.
  * \par      Source:
  *                Mesh2DView.cpp
  */
  void objectColourChanged ();
};

#endif // MESH2DVIEW_H
