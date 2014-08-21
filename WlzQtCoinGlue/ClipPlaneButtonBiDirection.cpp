#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _ClipPlaneButtonBiDirection_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         ClipPlaneButtonBiDirection.cpp
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
* \brief        Button for clip plane switch
* \ingroup      UI
*/

#include "ClipPlaneButtonBiDirection.h"

ClipPlaneButtonBiDirection::
ClipPlaneButtonBiDirection(
  QWidget *parent):
ClipPlaneButton(
  parent)
{
  m_iconOnFliped.addPixmap(QPixmap(QString::fromUtf8(
      ":/icons/images/cuttingplaneonfliped.png")), QIcon::Normal, QIcon::Off);
  m_iconVisibleFliped.addPixmap(QPixmap(QString::fromUtf8(
      ":/icons/images/cuttingplanevisiblefliped.png")), QIcon::Normal,
      QIcon::Off);
  m_isRight = true;
}

void ClipPlaneButtonBiDirection::
buttonClicked(
  bool)
{
  switch(m_state)
  {
    case ClipPlaneButton::ClipOn:
      m_state = ClipPlaneButton::ClipOnly;
      if(m_isRight)
      {
	setIcon(m_iconOnFliped);
      }
      else
      {
	setIcon(m_iconOff);
      }
      break;
    case ClipPlaneButton::ClipOnly:
      if(m_isRight)
      {
	m_isRight = false;
	m_state = ClipPlaneButton::ClipOn;
	setIcon(m_iconVisibleFliped);
      }
      else
      {
	m_isRight = true;
	m_state = ClipPlaneButton::ClipOff;
	setIcon(m_iconOn);
      }
      break;
    case ClipPlaneButton::ClipOff:
      m_state = ClipPlaneButton::ClipOn;
      setIcon(m_iconVisible);
      break;
  }
  emit stateChanged(m_state);
}
