#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ConvHull3DView_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ConvHull3DView.h
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
* \brief        View for 3D convex hull.
* \ingroup      Views
*
*/

#ifndef CONVHULL3DVIEW_H
#define CONVHULL3DVIEW_H
#include "ObjectView.h"
#include <Wlz.h>

class SoCoordinate3;
class SoIndexedFaceSet;
class SoSeparator;
class SoMaterial;
class SoClipPlane;

/*!
* \brief	View providing 3D convex hull visualisation
* \ingroup      Views
*/
class ConvHull3DView : public ObjectView
{
  Q_OBJECT
public:

  /*!
  * \ingroup      Views
  * \brief        Constructor
  * \param        parent parent viewer
  * \param        object Woolz object to be visualised
  * \return       void
  */
  ConvHull3DView ( QObject * parent, WoolzObject * object );

  /*!
  * \ingroup      Views
  * \brief        Destructor
  * \return       void
  */
  virtual ~ConvHull3DView ( );

public:
/*!
  * \ingroup      Visualisation
  * \brief        Sets the transparency value of the convex hull.
  *
  *           Implements View::setTransparency().
  * \param        transparency new transparency value between 0 (non transparent) and 100 (transparent)
  * \return       void
  */
  virtual void setTransparency ( int transparency );

/*!
  * \ingroup      Visualisation
  * \brief        Returns the number of available visualisations that is 1 for ConvHull3DView.
  *
  *           Implements View::getVisualisationTypesNo().
  * \return       Return the number of available visualisations (1)
  */
  virtual int getVisualisationTypesNo () {return 1;}

/*!
  * \ingroup      Visualisation
  * \brief        Returns the list of available visualisations.
    *
  *           Implements View::getVisualisationTypes().
  * \return       List of visualistion type names.
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
  */
  virtual int visualisationType () {return 0;}

protected:
  /*!
  * \ingroup      Visualisation
  * \brief        Builds the scene graph of the view.
  *
  *           Reimplements View::generateSceneGraph().
  * \param        bForce, if true force update of the scene graph elements
  * \return       void
  */
  virtual void generateSceneGraph ( bool bForce = false);

  /*!
  * \ingroup      Visualisation
  * \brief        Updates view material.
  *
  * \return       void
  */
  void updateMaterial();

 /*!
  * \ingroup      Visualisation
  * \brief        Returns if the current object is compatible with the view
  * \return       true of object is compatible, false if not.
  */
  virtual bool compatible( );

private:
  /*!
  * \ingroup      Visualisation
  * \brief        Generates vertices of the convex hull.
  *
  * \param        vertices previous (empty) list of vertices
  * \param        errNum result error number
  * \return       SoCoordinate3 object with added new vertices.
  */
  SoCoordinate3 * Vertices3D(WlzConvHullDomain3 *cv, SoCoordinate3 * vertices, WlzErrorNum& errNum );

  /*!
  * \ingroup      Visualisation
  * \brief        Generates faces of the convex hull.
  *
  * \param        faces previous (empty) list of faces
  * \param        errNum result error number
  * \return       SoIndexedFaceSet object with added new faces.
  */
  SoIndexedFaceSet * Faces3D(WlzConvHullDomain3 *cv, SoIndexedFaceSet * faces);

public slots:

  /*!
  * \ingroup      Visualisation
  * \brief        Processes object colour change.
  *
  * \return       void.
  */
  void objectColourChanged ();

  /*!
  * \ingroup      Visualisation
  * \brief        Sets up or deletes clip plane
  * \param        plane new plane to use
  * \return       void.
  */
  void addedClipPlane(SoClipPlane * plane);

protected:
  // protected attributes
  SoMaterial *m_material;                    /*!< material of the convex hull */
  bool        m_clipOn;                      /*!< if the clip plane is on */
  SoClipPlane *m_clipPlane;                  /*!< reference clip plane from viewer*/
};

#endif // CONVHULL3DVIEW_H
