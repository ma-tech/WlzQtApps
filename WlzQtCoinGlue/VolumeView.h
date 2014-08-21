#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _VolumeView_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         VolumeView.h
* \author	Zsolt Husz
* \date		October 2008
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
* \brief	Volume rendering for 3D oobjects
* \ingroup	Views
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
* \ingroup	Views
*/
class VolumeView : public ObjectView
{
  Q_OBJECT
  public:

   /*!
    * \ingroup	Views
    * \brief	Constructor
    * \param	parent			parent viewer
    * \param	object			Woolz object to be visualised
    */
    VolumeView(QObject * parent, WoolzObject * object);

    /*!
     * Empty Destructor
     */
    virtual ~VolumeView();

    /*!
    * \ingroup	Views
    * \brief	Destructor
    * \return	void
    */

  public:

   /*!
    * \ingroup	Visualisation
    * \brief	Sets the transparency value of the object.
    *
    *	      	Implements View::setTransparency().
    * \param	transparency		new transparency value between
    * 					0 (non transparent) and 100
    * 					(transparent)
    */
    virtual void setTransparency(int transparency);

   /*!
    * \return	the number of available visualisations (1)
    * \ingroup	Visualisation
    * \brief	Returns the number of available visualisations that is 1 for
    * 		VolumeView.
    *
    *	      	Implements View::getVisualisationTypesNo().
    */
    virtual int getVisualisationTypesNo() {return 1;}

   /*!
    * \return	list of visualistion type names.
    * \ingroup	Visualisation
    * \brief	Returns the list of available visualisations.
    *
    *	      	Implements View::getVisualisationTypes().
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

    /*!
     * \return	scale factor
     * \ingroup	Visualisation
     * \brief	Returns the properties of the view.
     */
     virtual QString getProperties();

  protected:
   /*!
    * \ingroup	Visualisation
    * \brief	Builds the scene graph of the view.
    *
    *	      	Reimplements View::generateSceneGraph().
    * \param 	bForce			if true force update of the scene
    * 					graph elements
    */
    virtual void generateSceneGraph (bool bForce = false);

   /*!
    * \ingroup	Visualisation
    * \brief	Updates view material.
    */
    void updateMaterial();

   /*!
    * \return	true of object is compatible, false if not.
    * \ingroup	Visualisation
    * \brief	Returns if the current object is compatible with the view
    */
    virtual bool compatible();

  private:
   /*!
    * \ingroup	Visualisation
    * \brief	Frees voxel data, if any allocated
    */
    void freeVolumeData();

  public slots:
   /*!
    * \ingroup	Visualisation
    * \brief	Processes object colour change.
    */
    void objectColourChanged ();

   /*!
    * \ingroup	Visualisation
    * \brief	Sets up or deletes clip plane
    * \param	plane			new plane to use
    */
    void addedClipPlane(SoClipPlane * plane);

   /*!
    * \ingroup	Visualisation
    * \brief	Sets up or deletes clip plane
    * \param	plane			new plane to use
    * \param	on			true if oblique slice is on
    */
    void setObliqueSlice(bool on);

  protected:
    // protected attributes
    SoMaterial *m_material;		/*!< material of the object */
    SoSeparator *m_section;		/*!< section view */
    SoSeparator *m_volumerenderSep;	/*!< volume renderer separator
					     volume renderer and sectioning
					     plane must be place in a
					     separator, otherwise ortho plane
					     will be sectioned  */
    void ***m_data;			/*!< voxel data*/
    SoClipPlane *m_clipPlane;		/*!< reference clip plane from viewer*/
    bool m_orthoOn;			/*!< if clip plane is on*/
    static SoTransferFunction *m_tfSection; /*!< static transfer function,
    					     used for all oblique views */
    int m_scaleFactor;			/*!< volume scale factor */

    static const long maxVoxels;
};

#endif // VOLUMEVIEW_H
