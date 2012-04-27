#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _CrossHairManip_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         CrossHairManip.h
* \author       Zsolt Husz
* \date         February 2009
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
* \brief        2D manipulator
* \ingroup      Controls
*
*/

#ifndef  CROSSHAIRMANIP_H
#define  CROSSHAIRMANIP_H

#include "LandmarkManip.h"

/*!
 * \brief	 Manipulator class for manipulating 2D cross hair manipulator
 * \ingroup      Controls
 */
class CrossHairManip : public LandmarkManip
{
   SO_NODE_HEADER(CrossHairManip);

  public:

  /*!
  * \ingroup      Controls
  * \brief        Constructor
  * \param        EnabledMove pointer to the boolean variable allowing or not dragging
  * \param        pp pointer to the PointPair visualised by the dragger
  * \return       void
  * \par      Source:
  *                CrossHairManip.cpp
  */
   CrossHairManip(PointPair *pp = NULL);

  /*!
  * \ingroup      Controls
  * \brief        Initializes the type ID for this manipulator node.
  *
  *               This should be called once after SoInteraction::init() and after 
  *             CrossHairDragger::init().
  * \return       void
  * \par      Source:
  *                CrossHairManip.cpp
  */
  static void initClass();

 /*!
  * \ingroup      Controls
  * \brief        Update landmark geometry
  * \return       void
  * \par      Source:
  *                CrossHairManip.cpp
  */
  virtual void update();

private:

  /*!
  * \ingroup      Controls
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *                CrossHairManip.cpp
  */
   virtual ~CrossHairManip();
};

#endif  /* CROSSHAIRMANIP_H */
