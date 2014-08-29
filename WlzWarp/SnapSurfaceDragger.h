#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _SnapSurfaceDragger_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         SnapSurfaceDragger.h
* \author       Zsolt Husz
* \date         July 2009
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
* \brief	3D dragger
* 		clThis cCode is based on an example from the
* 		Inventor Toolmaker, chapter 8.
*  		Resource names and part names for this dragger are:
*  		<table width="500" border="0">
   		<tr>
   		  <td><b>Resource Name</b></td>
		  <td><b>Part Name</b></td>
   		</tr>
   		<tr>
   		  <td>translateSnapTranslator</td>
		  <td>translator</td>
   		</tr>
   		<tr>
   		  <td>translateSnapTranslatorActive</td>
   		  <td>translatorActive</td>
   		</tr>
   		<tr>
   		  <td>translateSnapFeedback</td>
   		  <td>feedback</td>
   		</tr>
   		<tr>
   		  <td>translateSnapFeedbackActive</td>
   		  <td>feedbackActive</td>
   		</tr>
		</table>
*
* \ingroup	Controls
*/

#ifndef  SNAPSURFACEDRAGGER_H
#define  SNAPSURFACEDRAGGER_H

#include "LandmarkDragger.h"

// Inventor includes
#include <Inventor/draggers/SoDragger.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/sensors/SoFieldSensor.h>

/*!
 * \brief	Dragger class for manipulating 3D targets that snap to
 * 		surfaces
 * \ingroup	Controls
 */
class SnapSurfaceDragger : public LandmarkDragger
{
  SO_KIT_HEADER(SnapSurfaceDragger);

  // Catalog entries for new parts added by this class...
  SO_KIT_CATALOG_ENTRY_HEADER(materialSwitch);
  SO_KIT_CATALOG_ENTRY_HEADER(validitySwitch);

  SO_KIT_CATALOG_ENTRY_HEADER(materialPlaced);
  SO_KIT_CATALOG_ENTRY_HEADER(materialActive);
  SO_KIT_CATALOG_ENTRY_HEADER(materialNormal);
  SO_KIT_CATALOG_ENTRY_HEADER(materialInvalid);
  SO_KIT_CATALOG_ENTRY_HEADER(translatorSwitch);
  SO_KIT_CATALOG_ENTRY_HEADER(translator);
  SO_KIT_CATALOG_ENTRY_HEADER(translatorActive);
  SO_KIT_CATALOG_ENTRY_HEADER(feedbackRotate);
  SO_KIT_CATALOG_ENTRY_HEADER(feedbackSwitch);
  SO_KIT_CATALOG_ENTRY_HEADER(feedback);
  SO_KIT_CATALOG_ENTRY_HEADER(feedbackActive);
  SO_KIT_CATALOG_ENTRY_HEADER(scale);

  public:
    /*!
     * \ingroup	Controls
     * \brief	Constructor
     */
    SnapSurfaceDragger();

    /*!
     * \ingroup	Controls
     * \brief	Initializes the type ID for this dragger node.
     *
     *            This should be called once after SoInteraction::init().
     */
    static void initClass();

    /*!
     * \ingroup	Controls
     * \brief	Sets the size/scaling of the dragger
     * \param	scale			the new scaling factor
     */
    virtual void setScale(float scale = 1.0);

  protected:
    /*!
     * \ingroup	Controls
     * \brief	Sets the feedbackRotation node.
     *
     * 		Sets the feedbackRotation node so that the feedback geometry
     * 		will be aligned with the direction of motion in local space.
     */

    void orientFeedbackGeometry(const SbVec3f &localDir);

    /*!
     * \ingroup	Controls
     * \brief	Dragging callback.
     *
     * 		Static callback function invoked by SoDragger when the
     *		when mouse button goes down over this dragger
     */
    static void startCB(void *, SoDragger *);

    /*!
     * \ingroup	Controls
     * \brief	Dragger moving callback.
     *
     *		Static callback function invoked by SoDragger when the
     *		when the mouse drags
     */
    static void motionCB(void *, SoDragger *);

    /*!
     * \ingroup	Controls
     * \brief	Dragging mouse button released callback.
     *
     *		Static callback function invoked by SoDragger when the
     *		when the mouse button is released
     */
    static void finishCB(void *, SoDragger *);


    /*!
     * \ingroup	Controls
     * \brief	Strart drag processing.
     *
     *		Invoked by the static callback startCB()
     *		functions, do the work of moving the dragger.
     */
    void dragStart();

    /*!
     * \ingroup	Controls
     * \brief	Drag processing.
     *
     *		Invoked by the static callback startCB()
     *		functions, do the work of moving the dragger.
     */
    void drag();

    /*!
     * \ingroup	Controls
     * \brief	Drag finished.
     *
     *		Invoked by the static callback motionCB()
     *		functions, do the work of moving the dragger.
     */
    void dragFinish();

    SoFieldSensor *fieldSensor; 		/*!< watches for changes to the
    						     translation field */

    /*!
     * \ingroup	Controls
     * \brief	Field sensor callback.
     *
     *		Called when sensor signals change of the field value.
     */
    static void fieldSensorCB(void *, SoSensor *);

    /*!
     * \ingroup	Controls
     * \brief	Update translation field
     *
     *		This callback updates the translation field when
     *		the dragger is moved.
     */
    static void valueChangedCB(void *, SoDragger *);

    /*!
     * \ingroup	Controls
     * \brief	Update translation field
     * \param	onOff			connection state
     * \param	doItAlways		foreces reconnect, even if state
     * 					change is not required
     *
     *		This will detach/attach the fieldSensor.
     *		It is called at the end of the constructor (to attach).
     *		and at the start/end of SoBaseKit::readInstance()
     *		and on the new copy at the start/end of SoBaseKit::copy()
     *		Returns the state of the node when this was called.
     *		This callback updates the translation field when
     *		the dragger is moved.
     * \return	state of the node
     */
    virtual SbBool setUpConnections( SbBool onOff,
	SbBool doItAlways = FALSE);

  public:
    SoSFVec3f translation; 		/*!< Field that will always contain
  					     the dragger's position */

  private:
    static const char geomBuffer[];   	/*!< inventor model of the dragger */

    /*!
     * \ingroup	Controls
     * \brief	Destructor
     */
    ~SnapSurfaceDragger();
};

#endif  /* SNAPSURFACEDRAGGER_H */
