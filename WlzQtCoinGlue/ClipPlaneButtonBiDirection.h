#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _ClipPlaneButtonBiDirection_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         ClipPlaneButtonBiDirection.h
* \author       Zsolt Husz
* \date         December 2009
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
* \brief        Button for clip plane switch with bidirectional support
* \ingroup      UI
*/

#ifndef CLIPPLANEBUTTONBIDIRECTION_H
#define CLIPPLANEBUTTONBIDIRECTION_H

#include <QPushButton>
#include "ClipPlaneButton.h"

class ClipPlaneButtonBiDirection: public ClipPlaneButton
{
      Q_OBJECT
  public:
    typedef enum
    {
      ClipOn,
      ClipOff,
      ClipOnly
    } statetype; 			/*!< states of the button */

   /*!
    * \ingroup    UI
    * \brief      Constructor
    * \param      parent 		parent widget
    */
    ClipPlaneButtonBiDirection(QWidget *parent);

    /*!
     * \ingroup	UI
     * \brief     Returns if the plane is right of mirrored orientation
     * \param     plane 		orientation
     */
     bool isRight() {return m_isRight;}

     /*!
      * \ingroup  UI
      * \brief    Sets the isRight value, the orientation of the plane
      */
      void setIsRight(bool isRight)
      {
	if(isRight != m_isRight)
	{
	  m_isRight = isRight;
	}
      }

  protected slots:
   /*!
    * \ingroup	UI
    * \brief    Processes button click
    */
    virtual void buttonClicked(bool);

  private:
      bool m_isRight;                   /*!< is flipped */
      QIcon m_iconOnFliped;     	/*!< icon to use the plane and
					     manipulator if plane is flipped */
      QIcon m_iconVisibleFliped;        /*!< icon to use the plane but without
					     manipulator if plane is flipped */

};

#endif // CLIPPLANEBUTTONBIDIRECTION_H
