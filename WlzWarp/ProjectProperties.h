#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _ProjectProperties_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         ProjectProperties.h
* \author       Zsolt Husz
* \date         October 2008
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
* \brief	Managing project setting
* \ingroup	Control
*/

#ifndef PREOJECTPROPERTIES_H
#define PREOJECTPROPERTIES_H

#include <QString>
/*!
 * \brief	Stores the setting related to the current project.
 *
 *                Setting include:
 *                     - mode (mesh editing or warping);
 *                     - opened files.
 */
class ProjectProperties
{

  public:
    typedef enum {noneProject = 0, mesheditProject = 1,
        warpingProject = 2} ProjectTypes; 	/*!< project modes that define
						     menu configuration*/

    ProjectTypes projectType;                   /*!< current project mode */
    QString      m_projectName;
    QString      m_fileName;
    bool         m_isNameSet;
    /*!
     * \ingroup	Control
     * \brief	Constructor
     */
    ProjectProperties();

    /*!
     * \ingroup	Control
     * \brief	Resets property values
     */
    void reset();

    /*!
     * \ingroup	Control
     * \brief	Destructor
     */
    ~ProjectProperties();
};
#endif  //PREOJECTPROPERTIES
