#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _Contour2DView_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         Contour2DView.h
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
* \brief        View for 2D contour.
* \ingroup      Views
*
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
class Contour2DView : public ObjectView
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
  *                Contour2DView.cpp
  */
  Contour2DView ( QObject * parent, WoolzObject * object );

 /*!
  * \ingroup      Views
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *                Contour2DView.cpp
  */
  virtual ~Contour2DView ( );

protected:

  // protected attributes
  //
  SoMaterial *m_material;                    /*!< material of the contour */

public:

/*!
  * \ingroup      Visualisation
  * \brief        Sets the transparency value of the contour.
  *
  *           Implements View::setTransparency().
  * \param        transparency new transparency value between 0 (non transparent) and 100 (transparent)
  * \return       void
  * \par      Source:
  *                Contour2DView.cpp
  */
  virtual void setTransparency ( int transparency );

/*!
  * \ingroup      Visualisation
  * \brief        Returns the number of available visualisations that is 1 for Contour2DView.
  *
  *           Implements View::getVisualisationTypesNo().
  * \return       Return the number of available visualisations (1)
  * \par      Source:
  *                Contour2DView.cpp
  */
  virtual int getVisualisationTypesNo () {return 1;}

/*!
  * \ingroup      Visualisation
  * \brief        Returns the list of available visualisations.
    *
  *           Implements View::getVisualisationTypes().
  * \return       List of visualistion type names.
  * \par      Source:
  *                Contour2DView.cpp
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
  *                Contour2DView.cpp
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
  * \par      Source:
  *                Contour2DView.cpp
  */
  virtual void generateSceneGraph ( bool bForce = false);

 /*!
  * \ingroup      Visualisation
  * \brief        Returns if the current object is compatible with the view
  * \return       true of object is compatible, false if not.
  * \par      Source:
  *                Contour2DView.cpp
  */
  virtual bool compatible( );

private:
  /*!
  * \ingroup      Visualisation
  * \brief        Generates vertices of the contour.
  *
  * \param        vertices previous (empty) list of vertices
  * \param        errNum result error number
  * \return       SoCoordinate3 object with added new vertices.
  * \par      Source:
  *                Contour2DView.cpp
  */
  SoCoordinate3 * Vertices2D(SoCoordinate3 * vertices, WlzErrorNum& errNum );

  /*!
  * \ingroup      Visualisation
  * \brief        Generates lines of the contour.
  *
  * \param        lines previous (empty) list of lines
  * \param        errNum result error number
  * \return       SoIndexedLineSet object with added new lines.
  * \par      Source:
  *                Contour2DView.cpp
  */
  SoIndexedLineSet * Lines2D(SoIndexedLineSet * lines, WlzErrorNum& errNum );

public slots:

  /*!
  * \ingroup      Visualisation
  * \brief        Processes object colour change.
  *
  * \return       void.
  * \par      Source:
  *                Contour2DView.cpp
  */
  void objectColourChanged ();
};

#endif // CONTOUR2DVIEW_H
