#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _WlzQtCoinVersion_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         WlzQtCoinVersion.cpp
* \author       Bill Hill
* \date         August 2014
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
* \brief	Version numbering.
* \ingroup	Control
*/

extern "C" {

/*!
* \return       Woolz Qt Coin glue library version string.
* \ingroup      WlzDebug
* \brief        Returns the current version of the library.
*/
const char      *WlzQtCoinVersion()
{
#ifdef PACKAGE_VERSION
  static char version[] = PACKAGE_VERSION;
#else
  static char version[] = "unknown";
#endif

  return(version);
}

}

