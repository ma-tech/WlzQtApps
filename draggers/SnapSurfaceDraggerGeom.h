/*!
* \file         SnapSurfaceDraggerGeom.h
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
* \brief        snapSurfaceDragger.iv geometry 
* \ingroup      Control
*
* Automatically generated on Thu Oct 29 11:37:21 GMT 2009 with 
*    iv2h.sh snapSurfaceDragger.iv SnapSurfaceDragger
* 
*/

#ifndef SNAPSURFACEDRAGGER_IV_H
#define SNAPSURFACEDRAGGER_IV_H

 const char SnapSurfaceDragger::geomBuffer[] =
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
  "DEF translatePlacedMaterialSnap Material { \n"
  "        diffuseColor 1 0 0 \n"
  "        shininess 0\n"
  "        ambientColor 0 0 0\n"
  "        emissiveColor 0 0 0\n"
  "}\n"
  "\n"
  "DEF translateActiveMaterialSnap Material { \n"
  "        diffuseColor 1 0 0 \n"
  "        shininess 0\n"
  "        ambientColor 0 0 0\n"
  "        emissiveColor 0 0 0\n"
  "}\n"
  "\n"
  "DEF translateNormalMaterialSnap Material { \n"
  "        diffuseColor 0 1 0 \n"
  "        shininess 0\n"
  "        ambientColor 0 0 0\n"
  "        emissiveColor 0 0 0\n"
  "}\n"
  "\n"
  "DEF translateInvalidMaterialSnap Material { \n"
  "        diffuseColor 0 0 0 \n"
  "        shininess 0\n"
  "        ambientColor 0 0 0\n"
  "        emissiveColor 0.94 0.77 0.25\n"
  "}\n"
  "\n"
  "DEF translateSnapTranslator Separator {\n"
  "    DrawStyle { style FILLED }\n"
  "\n"
  "    Sphere {\n"
  "	radius 0.5\n"
  "    }\n"
  "}\n"
  "\n"
  "DEF translateSnapTranslatorActive Separator {\n"
  "    DrawStyle { style LINES }\n"
  "    Sphere {\n"
  "       radius 0.5\n"
  "    }\n"
  "}\n"
  "\n"
  "\n"
  "DEF translateSnapFeedback Separator {\n"
  "}\n"
  "\n"
  "DEF translateSnapFeedbackActive Separator {\n"
  "    Material { diffuseColor .5  .9 .9 }\n"
  "\n"
  "}\n"
  "\n"
  "DEF scaleSnap Scale {\n"
  "        scaleFactor 1 1 1\n"
  "    }\n";

#endif /* ! SNAPSURFACEDRAGGER_IV_H */
