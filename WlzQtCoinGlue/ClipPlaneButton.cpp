#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ClipPlaneButton_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ClipPlaneButton.h
* \author       Zsolt Husz
* \date         July 2009
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
* \brief        Button for clip plane switch
* \ingroup      UI
*
*/
#include "ClipPlaneButton.h"

ClipPlaneButton::ClipPlaneButton(QWidget *parent) : QPushButton(parent)
{
       m_iconOn.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/cuttingplaneon.png")), QIcon::Normal, QIcon::Off);
       m_iconOff.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/cuttingplaneoff.png")), QIcon::Normal, QIcon::Off);
       m_iconVisible.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/cuttingplanevisible.png")), QIcon::Normal, QIcon::Off);
       setIcon(m_iconOn);
       setToolTip("Clip plane manipulator on/off");
    //   setFlat(true);

       setFocusPolicy(Qt::NoFocus);
       m_state = ClipOff;
       connect(this, SIGNAL(clicked(bool)), this, SLOT(buttonClicked(bool)));
}

void ClipPlaneButton::buttonClicked(bool) {
   switch (m_state) {
       case ClipOn:
          m_state = ClipOnly;
          setIcon(m_iconOff);
          break;
       case ClipOnly:
          m_state = ClipOff;
          setIcon(m_iconOn);
          break;
       case ClipOff:
          m_state = ClipOn;
          setIcon(m_iconVisible);
          break;
   }
   emit stateChanged(m_state);
}
