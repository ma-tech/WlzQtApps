#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _BinaryTransferFunction_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         BinaryTransferFunction.cpp
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
* 		function.
* \ingroup      UI
*/

#include "BinaryTransferFunction.h"

const char* BinaryTransferFunction::xmlTag = "BinaryTransferFunction";

BinaryTransferFunction::
BinaryTransferFunction(
  QObject *parent):
TransferFunction(
  parent)
{
  int		ir;

  predefColorMap = SoTransferFunction::NONE;
  colorMapType = SoTransferFunction::RGBA;
  m_lowCutOff  = 254;
  m_highCutOff = 255;
  for(ir = 0; ir < 254; ++ir)
  {
    m_colorMap.set1Value(ir*4 + 0, 0.0f);
    m_colorMap.set1Value(ir*4 + 1, 0.0f);
    m_colorMap.set1Value(ir*4 + 2, 0.0f);
    m_colorMap.set1Value(ir*4 + 3, 0.0f);
  }
  for(ir = 254; ir < 256; ++ir)
  {
    m_colorMap.set1Value(ir*4 + 0, 1.0f);
    m_colorMap.set1Value(ir*4 + 1, 1.0f);
    m_colorMap.set1Value(ir*4 + 2, 1.0f);
    m_colorMap.set1Value(ir*4 + 3, 0.1f);
  }
}


void BinaryTransferFunction::
update()
{
  int 		ir;

  colorMap.copyFrom(m_colorMap);
  for(ir = 0; ir < m_lowCutOff; ++ir)
  {
    colorMap.set1Value(ir * 4 + 3, 0.0f);
  }
  for(ir = m_highCutOff+1; ir < 256; ++ir)
  {
    colorMap.set1Value(ir * 4 + 3, 0.0f);
  }
  emit updated();
}
