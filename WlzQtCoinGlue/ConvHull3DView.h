#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _ConvHull3DView_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         ConvHull3DView.h
* \author       Bill Hill
* \date         August 2014
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
* \brief        View for 3D convex hull.
* \ingroup      Views
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
* \ingroup	Views
*/
class ConvHull3DView: public ObjectView
{
  Q_OBJECT
  public:

    /*!
    * \ingroup	Views
    * \brief	Constructor
    * \param	parent 			parent viewer
    * \param	object 			Woolz object to be visualised
    * \return	void
    */
    ConvHull3DView(QObject * parent, WoolzObject * object);

    /*!
    * \ingroup	Views
    * \brief	Destructor
    * \return	void
    */
    virtual ~ConvHull3DView();

  public:
  /*!
    * \ingroup	Visualisation
    * \brief	Sets the transparency value of the convex hull.
    *
    *	        Implements View::setTransparency().
    * \param	transparency 		new transparency value between 0 (non
    * 					transparent) and 100 (transparent)
    */
    virtual void setTransparency(int transparency);

  /*!
    * \return	Return the number of available visualisations (1)
    * \ingroup	Visualisation
    * \brief	Returns the number of available visualisations that is 1 for
    * 		ConvHull3DView.
    *
    *	      	Implements View::getVisualisationTypesNo().
    */
    virtual int getVisualisationTypesNo() {return 1;}

  /*!
    * \ingroup	Visualisation
    * \brief  	Returns the list of available visualisations.
    *
    *	      	Implements View::getVisualisationTypes().
    * \return	List of visualistion type names.
    */
    virtual QStringList getVisualisationTypes();

  /*!
    * \ingroup	Views
    * \brief	Current visualisation type.
    *
    *   	Currently only one visualisation is implemented, therefore
    *   	it returns 0.
    *
    *	      	Implements View::visualisationType().
    */
    virtual int visualisationType() {return 0;}

  protected:
    /*!
    * \ingroup	Visualisation
    * \brief	Builds the scene graph of the view.
    *
    *	      	Reimplements View::generateSceneGraph().
    * \param	bForce			 if true force update of the scene
    * 					 graph elements
    */
    virtual void generateSceneGraph( bool bForce = false);

    /*!
    * \ingroup	  Visualisation
    * \brief	  Updates view material.
    */
    void updateMaterial();

   /*!
    * \return	  true of object is compatible, false if not.
    * \ingroup	  Visualisation
    * \brief	  Returns if the current object is compatible with the view
    */
    virtual bool compatible();

  private:
    /*!
    * \return	  SoCoordinate3 object with added new vertices.
    * \ingroup	  Visualisation
    * \brief	  Generates vertices of the convex hull.
    * \param	  vertices 		previous (empty) list of vertices
    * \param	  errNum 		result error number
    */
    SoCoordinate3 * Vertices3D(WlzConvHullDomain3 *cv,
    			       SoCoordinate3 * vertices,
			       WlzErrorNum& errNum );

    /*!
    * \return	  SoIndexedFaceSet object with added new faces.
    * \ingroup	  Visualisation
    * \brief	  Generates faces of the convex hull.
    * \param	  faces 		previous (empty) list of faces
    * \param	  errNum 		result error number
    */
    SoIndexedFaceSet * Faces3D(WlzConvHullDomain3 *cv,
    			       SoIndexedFaceSet * faces);

  public slots:

    /*!
    * \ingroup	  Visualisation
    * \brief	  Processes object colour change.
    */
    void objectColourChanged();

    /*!
    * \ingroup	  Visualisation
    * \brief	  Sets up or deletes clip plane
    * \param	  plane new plane to use
    */
    void addedClipPlane(SoClipPlane * plane);

  protected:
    // protected attributes
    SoMaterial *m_material;		/*!< material of the convex hull */
    bool m_clipOn;			/*!< if the clip plane is on */
    SoClipPlane *m_clipPlane;		/*!< reference clip plane from viewer*/
};

#endif // CONVHULL3DVIEW_H
