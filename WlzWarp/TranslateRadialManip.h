#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _TranslateRadialManip_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         TranslateRadialManip.h
* \author       Zsolt Husz
* \date         MArch 2009
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
* \brief	3D manipulator
* 		This code is based on an example from the Inventor
* 		Toolmaker, chapter 8.

* \ingroup	Controls
*/

#ifndef  TRANSLATERADIALMANIP_H
#define  TRANSLATERADIALMANIP_H

#include "LandmarkManip.h"

struct PointPair;
class SoPickAction;

/*!
 * \brief	Manipulator class for manipulating 3D targets by radial
 * 		dragging
 * \ingroup	Controls
 */
class TranslateRadialManip : public LandmarkManip
{
  SO_NODE_HEADER(TranslateRadialManip);

  public:
    /*!
     * \ingroup	Controls
     * \brief	Constructor
     * \param	EnabledMove		pointer to the boolean variable
     * 					allowing or not dragging
     * \param	pp			pointer to the PointPair visualised
     * 					by the dragger
     */
    TranslateRadialManip(PointPair *pp = NULL);

    /*!
     * \ingroup	Controls
     * \brief	Initializes the type ID for this manipulator node.
     *
     *		This should be called once after SoInteraction::init() and
     *		after CrossHairDragger::init().
     */
    static void initClass();

  private:
    /*!
     * \ingroup	Controls
     * \brief	Destructor
     */
    ~TranslateRadialManip();
};

#endif  /* TRANSLATERADIALMANIP_H */
