#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _LandmarkManip_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         LandmarkManip.h
* \author       Zsolt Husz
* \date         February 2009
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
* \brief	Manipulator superclass
* \ingroup	Controls
*/

#ifndef  LANDMARKMANIP_H
#define  LANDMARKMANIP_H

#include <Inventor/manips/SoTransformManip.h>

struct PointPair;
class LandmarkView;

/*!
* \brief	Manipulators superclass
* \ingroup	Controls
*/
class LandmarkManip : public SoTransformManip
{
  public:

    /*!
     * \ingroup	Controls
     * \brief	Event handler
     * \param	action			action to handle
     *
     */
    virtual void handleEvent (SoHandleEventAction *action);

    /*!
     * \ingroup	Controls
     * \brief	Sets parent view
     * \param	view			parent view
     *
     */
    void setView(LandmarkView *v) {view=v;}

    /*!
     * \ingroup	Controls
     * \brief	Sets the dragger state on or off
     * \param	on			new state
     *
     */
    virtual void setOn(bool On);

    /*!
     * \ingroup	Controls
     * \brief	Sets the dragger validity
     * \param	on			new validity
     *
     */
    virtual void setValid(bool Valid);

    /*!
     * \ingroup	Controls
     * \brief	Update landmark geometry
     */
    virtual void update() {}

  protected:
    /*!
     * \ingroup	Controls
     * \brief	Constructor
     * \param	pp			pointer to the PointPair visualised by
     * 					the dragger
     *
     */
    LandmarkManip(PointPair *pp = NULL);

    /*!
     * \ingroup	Controls
     * \brief	Static callback for forwarding draging event to the view
     * \param	data			pointer to the class
     *
     */
    static void moveAndFinishCB(void *data, SoDragger *);

    /*!
     * \ingroup	Controls
     * \brief	Sets up the static callback function for a dragger
     * \param	dragger			that will have the callbacks attached
     *
     */
    void initialiseCallback(SoDragger * dragger);

  public:
    PointPair *pointPair;   		/*!<  pointer to the PointPair 
    					      visualised by the dragger */
    LandmarkView *view;     		/*!<  pointer to the parent view */
};

#endif  /* LANDMARKMANIP_H */
