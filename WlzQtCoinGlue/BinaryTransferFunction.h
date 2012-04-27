#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _BinaryTransferFunction_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         BinaryTransferFunction.h
* \author       Zsolt Husz
* \date         January 2010
* \version      $Id$
* \par
* Address:
*               MRC Human Genetics Unit,
*               Western General Hospital,
*               Edinburgh, EH4 2XU, UK.
* \par
* Copyright (C) 2010 Medical research Council, UK.
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
* \brief        Extends the TransferFunction to provide binary transfer function.
* \ingroup      UI
*
*/

#ifndef BINARYTRANSFERFUNCTION_H
#define BINARYTRANSFERFUNCTION_H
#include "TransferFunction.h"

/*!
 * \brief	 Extends the TransferFunction to provide binary transfer function.
 *               Only the first and last value of the transfer function array are effectively used.
 * \ingroup      UI
 */
class BinaryTransferFunction : public TransferFunction
{
Q_OBJECT
public:
    /*!
  * \ingroup      UI
  * \brief        Constructor
  * \param        parent parent object
  * \par      Source:
  *                BinaryTransferFunction.cpp
  */
  BinaryTransferFunction(QObject *parent = 0);

public:
  static const char * xmlTag;             /*!< xml section tag string */
  
public:
 /*!
  * \ingroup      UI
  * \brief        Returns the xml tag used by the object
  * \return       xml tag defined in static member xmlTag
  * \par      Source:
  *                BinaryTransferFunction.cpp
  */
  virtual const char * getXmlTag() { return xmlTag;}

private:
 /*!
  * \ingroup      UI
  * \brief        Sets the actual colour map using the current settings
  * \return       void
  * \par      Source:
  *                BinaryTransferFunction.cpp
  */
  virtual void update();
};

#endif // BINARYTRANSFERFUNCTION_H
