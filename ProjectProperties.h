#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ProjectProperties_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ProjectProperties.h
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
* \brief        Managing project setting
* \ingroup      Control
*
*/

#ifndef PREOJECTPROPERTIES_H
#define PREOJECTPROPERTIES_H

#include <QString>
/*!
 * \brief         Stores the setting related to the current project.
 *
 *                Setting include:
 *                     - mode (mesh editing or warping);
 *                     - opened files.
 */
class ProjectProperties{

public:
    typedef enum { noneProject = 0, mesheditProject = 1, warpingProject = 2 } ProjectTypes;
                    /*!< project modes that define menu configuration*/

    ProjectTypes projectType;                   /*!< current project mode */
    QString      m_projectName;
    QString      m_fileName;
    bool         m_isNameSet;
 /*!
  * \ingroup      Control
  * \brief        Constructor
  * \return       void
  * \par      Source:
  *                ProjectProperties.cpp
  */
  ProjectProperties();

 /*!
  * \ingroup      Control
  * \brief        Resets property values
  * \return       void
  * \par      Source:
  *                ProjectProperties.cpp
  */
  void reset();


 /*!
  * \ingroup      Control
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *                ProjectProperties.cpp
  */
   ~ProjectProperties();

};
#endif  //PREOJECTPROPERTIES
