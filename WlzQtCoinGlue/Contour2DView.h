#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _Contour2DView_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         Contour2DView.h
* \author       Zsolt Husz, Bill Hill
* \date         October 2014
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
* \brief        View for 2D contour.
* \ingroup      Views
*/

#ifndef CONTOUR2DVIEW_H
#define CONTOUR2DVIEW_H
#include "ObjectView.h"

// Qt includes
#include <QStringList>

// Woolz includes
#include <Wlz.h>

class SoCoordinate3;
class SoIndexedLineSet;
class SoSeparator;
class SoMaterial;
class SoDrawStyle;

/*!
* \brief	View providing 2D contour visualisation
* \ingroup      Views
*/
class Contour2DView: public ObjectView
{
  Q_OBJECT
  public:

   /*!
    * \ingroup	Views
    * \brief    Constructor
    * \param    parent 			parent viewer
    * \param    object 			Woolz object to be visualised
    */
    Contour2DView(QObject * parent, WoolzObject * object);

   /*!
    * \ingroup  Views
    * \brief    Destructor
    */
    virtual ~Contour2DView();

  protected:

    // protected attributes
    //
    SoMaterial *m_material;  		/*!< material of the contour */

  public:

  /*!
    * \ingroup  Visualisation
    * \brief   	Sets the transparency value of the contour.
    *
    *           Implements View::setTransparency().
    * \param    transparency 		new transparency value between
    * 					0 (non transparent) and 100
    * 					(transparent)
    */
    virtual void setTransparency(int transparency);

  /*!
    * \return   The number of available visualisations (1)
    * \ingroup  Visualisation
    * \brief    Returns the number of available visualisations that is
    * 		1 for Contour2DView.
    *
    *           Implements View::getVisualisationTypesNo().
    */
    virtual int getVisualisationTypesNo() {return 1;}

  /*!
    * \return   List of visualistion type names.
    * \ingroup  Visualisation
    * \brief    Returns the list of available visualisations.
    *
    *           Implements View::getVisualisationTypes().
    */
    virtual QStringList getVisualisationTypes();

  /*!
    * \ingroup  Views
    * \brief    Current visualisation type.
    *
    *   	Currently only one visualisation is implemented, therefore
    *   	it returns 0.
    *
    *           Implements View::visualisationType().
    */
    virtual int visualisationType() {return 0;}

  protected:
    /*!
    * \ingroup	Visualisation
    * \brief    Builds the scene graph of the view.
    *           Reimplements View::generateSceneGraph().
    * \param    bForce			if true force update of the scene
    * 					graph elements
    */
    virtual void generateSceneGraph(bool bForce = false);

   /*!
    * \return   true of object is compatible, false if not.
    * \ingroup  Visualisation
    * \brief    Returns if the current object is compatible with the view
    */
    virtual bool compatible();

  private:
    /*!
    * \return   SoCoordinate3 object with added new vertices.
    * \ingroup  Visualisation
    * \brief    Generates vertices of the contour.
    * \param    vertices 	previous (empty) list of vertices
    * \param	errNum 		result error number
    */
    SoCoordinate3 * Vertices2D(SoCoordinate3 * vertices, WlzErrorNum& errNum);

    /*!
    * \ingroup  Visualisation
    * \brief    Generates lines of the contour.
    * \param    lines 			previous (empty) list of lines
    * \param    errNum 			result error number
    * \return   SoIndexedLineSet object with added new lines.
    */
    SoIndexedLineSet * Lines2D(SoIndexedLineSet * lines, WlzErrorNum& errNum );

    /*!
    * \return	woolz error code
    * \ingroup	Visualisation
    * \brief	Checks that the object is a valid 2D contour.
    * \param	ctr			Contour to set and check.
    * \param	model			Model to set and check.
    */
    WlzErrorNum CheckContour(WlzContour **ctr, WlzGMModel **model);

  public slots:

    /*!
    * \ingroup 	Visualisation
    * \brief    Processes object colour change.
    */
    void objectColourChanged();
};

#endif // CONTOUR2DVIEW_H
