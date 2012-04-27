#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ImageView_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ImageView.h
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
* \brief        View for 2D image as texture mapped rectangle.
* \ingroup      Views
*
*/

#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H
#include "ObjectView.h"

#include <Wlz.h>

class SoSeparator;
class SoMaterial;
class SoTexture2;

/*!
 * \brief	 View providing 2D image visualisation
 * \ingroup      Views
 */
class ImageView : public ObjectView
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
  *                ImageView.cpp
  */
  ImageView ( QObject * parent, WoolzObject * object );

 /*!
  * \ingroup      Views
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *                ImageView.cpp
  */
  virtual ~ImageView ( );

public:

/*!
  * \ingroup      Visualisation
  * \brief        Sets the transparency value of the image.
  *
  *           Implements View::setTransparency().
  * \param        transparency new transparency value between 0 (non transparent) and 100 (transparent)
  * \return       void
  * \par      Source:
  *                ImageView.cpp
  */
  virtual void setTransparency ( int transparency );

/*!
  * \ingroup      Visualisation
  * \brief        Returns the number of available visualisations that is 1 for ImageView.
  *
  *           Implements View::getVisualisationTypesNo().
  * \return       Return the number of available visualisations (1)
  * \par      Source:
  *                ImageView.cpp
  */
  virtual int getVisualisationTypesNo () {return 1;}

/*!
  * \ingroup      Visualisation
  * \brief        Returns the list of available visualisations.
    *
  *           Implements View::getVisualisationTypes().
  * \return       List of visualistion type names.
  * \par      Source:
  *                ImageView.cpp
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
  *                ImageView.cpp
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
  *                ImageView.cpp
  */
  virtual void generateSceneGraph ( bool bForce = false);

 /*!
  * \ingroup      Visualisation
  * \brief        Updates view material.
  *
  * \return       void
  * \par      Source:
  *                ImageView.cpp
  */
  void updateMaterial();

/*!
  * \ingroup      Visualisation
  * \brief        Returns if the current object is compatible with the view
  * \return       true of object is compatible, false if not.
  * \par      Source:
  *                ImageView.cpp
  */
  virtual bool compatible( );

public slots:

 /*!
  * \ingroup      Visualisation
  * \brief        Processes object colour change.
  *
  * \return       void.
  * \par      Source:
  *                ImageView.cpp
  */
  void objectColourChanged ();

private:
 /*!
  * \ingroup      Visualisation
  * \brief        Converts grey or RGA 2D domain objects to an arrays that has both the values and the alpha chanel.
  *
  * \param        buffer Alocated memory region to store the image data
  * \param        obj Woolz object to convert
  * \return       Woolz error code
  * \par      Source:
  *                ImageView.cpp
  */
  WlzErrorNum convertObjToArray(void * buffer, WlzObject* obj);

protected:
  SoMaterial *m_material;                   /*!< material of the image */
  int m_chanels;                           /*!< number of colour chanels: 0 - invalid, 2 - grey (+alpha), 4 - RGB (+alpha) */
};

#endif // IMAGEVIEW_H
