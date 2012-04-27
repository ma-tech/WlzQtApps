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
* \file         BinaryTransferFunction.cpp
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

#include "BinaryTransferFunction.h"

const char* BinaryTransferFunction::xmlTag = "BinaryTransferFunction";

BinaryTransferFunction::BinaryTransferFunction(QObject *parent) :
    TransferFunction(parent)
{

    for (int ir=0;ir<256;ir++) {
      m_colorMap.set1Value(ir*4+3, 0.10f);
    }
    if (m_highCutOff == 255)
      colorMap.setValues(4, 4, m_colorMap.getValues(255*4));
    m_lowCutOff = 0;
}


void BinaryTransferFunction::update() {
   colorMap.copyFrom(m_colorMap);
   int ir=0;
   for (ir = 0; ir< 256;ir++)
      colorMap.set1Value(ir*4+3, 0.0f);
   if (m_highCutOff == 255)
     colorMap.setValues(4, 4, m_colorMap.getValues(255*4));
   emit updated();
}
