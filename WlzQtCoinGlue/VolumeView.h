#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _VolumeView_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         VolumeView.h
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
* \brief        Volume rendering for 3D oobjects
* \ingroup      Views
* \todo         editable properties
*
*/

#ifndef VOLUMEVIEW_H
#define VOLUMEVIEW_H
#include "ObjectView.h"

#include <Wlz.h>

//Coin3D classes
class SoCoordinate3;
class SoIndexedFaceSet;
class SoSeparator;
class SoMaterial;
class SoClipPlane;
class SoTransferFunction;
/*!
* \brief	View providing 2D object visualisation
* \ingroup      Views
*/
class VolumeView : public ObjectView
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
  *                VolumeView.cpp
  */
  VolumeView ( QObject * parent, WoolzObject * object );

  /*!
   * Empty Destructor
   */
  virtual ~VolumeView ( );

  /*!
  * \ingroup      Views
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *                VolumeView.cpp
  */

public:

 /*!
  * \ingroup      Visualisation
  * \brief        Sets the transparency value of the object.
  *
  *           Implements View::setTransparency().
  * \param        transparency new transparency value between 0 (non transparent) and 100 (transparent)
  * \return       void
  * \par      Source:
  *                VolumeView.cpp
  */
  virtual void setTransparency ( int transparency );

 /*!
  * \ingroup      Visualisation
  * \brief        Returns the number of available visualisations that is 1 for VolumeView.
  *
  *           Implements View::getVisualisationTypesNo().
  * \return       Return the number of available visualisations (1)
  * \par      Source:
  *                VolumeView.cpp
  */
  virtual int getVisualisationTypesNo () {return 1;}

 /*!
  * \ingroup      Visualisation
  * \brief        Returns the list of available visualisations.
    *
  *           Implements View::getVisualisationTypes().
  * \return       List of visualistion type names.
  * \par      Source:
  *                VolumeView.cpp
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
  *                VolumeView.cpp
  */
  virtual int visualisationType () {return 0;}

  /*!
   * \ingroup      Visualisation
   * \brief        Returns the properties of the view.
   *
   * \return       scale factor
   * \par      Source:
   *                VolumeView.cpp
   */
   virtual QString getProperties ( );

protected:
 /*!
  * \ingroup      Visualisation
  * \brief        Builds the scene graph of the view.
  *
  *           Reimplements View::generateSceneGraph().
  * \param        bForce, if true force update of the scene graph elements
  * \return       void
  * \par      Source:
  *                VolumeView.cpp
  */
  virtual void generateSceneGraph ( bool bForce = false);

 /*!
  * \ingroup      Visualisation
  * \brief        Updates view material.
  *
  * \return       void
  * \par      Source:
  *                VolumeView.cpp
  */
  void updateMaterial();

 /*!
  * \ingroup      Visualisation
  * \brief        Returns if the current object is compatible with the view
  * \return       true of object is compatible, false if not.
  * \par      Source:
  *                VolumeView.cpp
  */
  virtual bool compatible();

private:
 /*!
  * \ingroup      Visualisation
  * \brief        Frees voxel data, if any allocated
  * \return       void
  * \par      Source:
  *                VolumeView.cpp
  */
  void freeVolumeData();

public slots:
 /*!
  * \ingroup      Visualisation
  * \brief        Processes object colour change.
  *
  * \return       void.
  * \par      Source:
  *                VolumeView.cpp
  */
  void objectColourChanged ();

 /*!
  * \ingroup      Visualisation
  * \brief        Sets up or deletes clip plane
  * \param        plane new plane to use
  * \return       void.
  * \par      Source:
  *                VolumeView.cpp
  */
  void addedClipPlane(SoClipPlane * plane);

 /*!
  * \ingroup      Visualisation
  * \brief        Sets up or deletes clip plane
  * \param        plane new plane to use
  * \param        on true if oblique slice is on
  * \return       void.
  * \par      Source:
  *                VolumeView.cpp
  */
  void setObliqueSlice(bool on);

protected:
  // protected attributes
  SoMaterial *m_material;                    /*!< material of the object */
  SoSeparator *m_section;                    /*!< section view */
  SoSeparator *m_volumerenderSep;            /*!< volume renderer separator
                                               volume renderer and sectioning plane must be place in a
                                               separator, otherwise ortho plane will be sectioned  */
  void ***m_data;                            /*!< voxel data*/
  SoClipPlane *m_clipPlane;                  /*!< reference clip plane from viewer*/
  bool m_orthoOn;                            /*!< if clip plane is on*/
  static SoTransferFunction *m_tfSection;    /*!< static transfer function, used for all oblique views */
  int m_scaleFactor;                         /*!< volume scale factor */

  static const long maxVoxels;
};

#endif // VOLUMEVIEW_H
