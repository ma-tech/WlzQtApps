/*!
* \file         CrossHairDraggerGeom.h
* \author       Zsolt Husz
* \date         March 2009
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
* \brief        crossHairDragger.iv geometry 
* \ingroup      Control
*
* Automatically generated on Thu Oct 29 11:37:20 GMT 2009 with 
*    iv2h.sh crossHairDragger.iv CrossHairDragger
* 
*/

#ifndef CROSSHAIRDRAGGER_IV_H
#define CROSSHAIRDRAGGER_IV_H

 const char CrossHairDragger::geomBuffer[] =
  "#Inventor V2.0 ascii\n"
  "#\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "DEF translatePlacedMaterialCross Material { \n"
  "        diffuseColor 0 0 0 \n"
  "        shininess 0\n"
  "        ambientColor 0 0 0\n"
  "        emissiveColor 1 0 0\n"
  "}\n"
  "\n"
  "DEF translateActiveMaterialCross Material { \n"
  "        diffuseColor 0 0 0 \n"
  "        shininess 0\n"
  "        ambientColor 0 0 0\n"
  "        emissiveColor 1 0 0\n"
  "}\n"
  "\n"
  "DEF translateNormalMaterialCross Material { \n"
  "        diffuseColor 0 0 0 \n"
  "        shininess 0\n"
  "        ambientColor 0 0 0\n"
  "        emissiveColor 0 1 0\n"
  "}\n"
  "\n"
  "DEF translateInvalidMaterialCross Material { \n"
  "        diffuseColor 0 0 0 \n"
  "        shininess 0\n"
  "        ambientColor 0 0 0\n"
  "        emissiveColor 0.94 0.77 0.25\n"
  "}\n"
  "\n"
  "DEF translateHairTranslatorActive Separator {\n"
  "    Material { diffuseColor .6 .6 0 }\n"
  "    DrawStyle { style LINES  }\n"
  "    Sphere {\n"
  "      radius 0.5\n"
  "    }\n"
  "}\n"
  "\n"
  "DEF targetCylinder Cylinder {\n"
  "   height 2.4 radius 0.2\n"
  "}\n"
  "\n"
  "DEF crossCylinder Cylinder {\n"
  "   height 1.3 radius 0.05\n"
  "}\n"
  "\n"
  "DEF scaleCross Scale {\n"
  "        scaleFactor 1 1 1\n"
  "    }\n"
  "\n"
  "DEF translateHairTranslator Separator {\n"
  "    Scale {\n"
  "        scaleFactor 5 5 5\n"
  "	\n"
  "    }   \n"
  "    Translation { translation 1 0 0 }\n"
  "    USE targetCylinder\n"
  "    Translation { translation -2 0 0 }\n"
  "    USE targetCylinder\n"
  "    Translation { translation 1 0 0 }\n"
  "    Rotation { rotation 0 0  1 1.57 }\n"
  "\n"
  "    Translation { translation 1 0 0 }\n"
  "    USE targetCylinder\n"
  "    Translation { translation -2 0 0 }\n"
  "    USE targetCylinder\n"
  "    Translation { translation 1 0 0 }\n"
  "    Rotation { rotation 0 0  1 -1.57 }\n"
  "\n"
  "\n"
  "    Translation { translation 0 1.0 0 }\n"
  "    USE crossCylinder \n"
  "    Translation { translation 0 -2.0 0 }\n"
  "    USE crossCylinder \n"
  "\n"
  "    Translation { translation 0 1.0 0 }\n"
  "    Rotation { rotation 0 0  1 1.57 }\n"
  "    Translation { translation 0 1.0 0 }\n"
  "    USE crossCylinder \n"
  "    Translation { translation 0 -2.0 0 }\n"
  "    USE crossCylinder \n"
  "    Translation { translation 0 1.0 0 }\n"
  "\n"
  "    Material {\n"
  "        transparency 1\n"
  "    }\n"
  "    Sphere {\n"
  "      radius 2\n"
  "    }\n"
  "}\n";

#endif /* ! CROSSHAIRDRAGGER_IV_H */
