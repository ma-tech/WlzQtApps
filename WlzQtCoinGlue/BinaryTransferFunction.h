#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _BinaryTransferFunction_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         BinaryTransferFunction.h
* \author       Zsolt Husz
* \date         January 2010
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
* \brief        Extends the TransferFunction to provide binary transfer
*		function.
* \ingroup      UI
*/

#ifndef BINARYTRANSFERFUNCTION_H
#define BINARYTRANSFERFUNCTION_H
#include "TransferFunction.h"

/*!
 * \brief	 Extends the TransferFunction to provide binary transfer
 * 		 function.
 *               Only the first and last value of the transfer function
 *               array are effectively used.
 * \ingroup      UI
 */
class BinaryTransferFunction: public TransferFunction
{
  Q_OBJECT
  public:
      /*!
    * \ingroup 	UI
    * \brief   	Constructor
    * \param   	parent 		parent object
    */
    BinaryTransferFunction(QObject *parent = 0);

  public:
    static const char * xmlTag;             /*!< xml section tag string */
    
  public:
   /*!
    * \return   xml tag defined in static member xmlTag
    * \ingroup	UI
    * \brief    Returns the xml tag used by the object
    */
    virtual const char * getXmlTag() { return xmlTag;}

  private:
   /*!
    * \ingroup  UI
    * \brief    Sets the actual colour map using the current settings
    */
    virtual void update();
};

#endif // BINARYTRANSFERFUNCTION_H
