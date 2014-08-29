#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _CrossHairDragger_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         CrossHairDragger.cpp
* \author       Zsolt Husz, 
* \date         February 2009
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
* \brief        2D dragger
*
*               The implementation of the dragger is based on Inventor Toolkit 
*               manual's dragger example.
* \ingroup      Controls
*/

// Inventor includes
#include <Inventor/SoPath.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/projectors/SbPlaneProjector.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoBlinker.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoMaterial.h>

// Include file for our new class.
#include "CrossHairDragger.h"

// This file contains the variable 
// CrossHairDragger::geomBuffer, which describes 
// the default geometry resources for this dragger.
#include "draggers/CrossHairDraggerGeom.h"

SO_KIT_SOURCE(CrossHairDragger);

void CrossHairDragger::
initClass()
{
  SO_KIT_INIT_CLASS(CrossHairDragger, SoDragger, "Dragger");		
}

CrossHairDragger::
CrossHairDragger()
{
   SO_KIT_CONSTRUCTOR(CrossHairDragger);

   // Put this under geomSeparator so it draws efficiently.
   SO_KIT_ADD_CATALOG_ENTRY(validitySwitch, SoBlinker, TRUE,
                            geomSeparator, ,FALSE);

   SO_KIT_ADD_CATALOG_ENTRY(materialSwitch, SoSwitch, TRUE,
                            validitySwitch, ,FALSE);

   SO_KIT_ADD_CATALOG_ENTRY(materialPlaced, SoMaterial, TRUE,
                            materialSwitch, ,TRUE);
   SO_KIT_ADD_CATALOG_ENTRY(materialNormal, SoMaterial, TRUE,
                            materialSwitch, ,TRUE);
   SO_KIT_ADD_CATALOG_ENTRY(materialActive, SoMaterial, TRUE,
                            materialSwitch, ,TRUE);

   SO_KIT_ADD_CATALOG_ENTRY(materialInvalid, SoMaterial, TRUE,
                            validitySwitch, ,TRUE);

   SO_KIT_ADD_CATALOG_ENTRY(scale, SoScale, TRUE,
                            geomSeparator, ,TRUE);
   SO_KIT_ADD_CATALOG_ENTRY(translator, SoSeparator, TRUE,
                            geomSeparator, ,TRUE);


   // Read geometry resources. Only do this the first time we 
   // construct one.  'geomBuffer' contains our compiled in 
   // defaults. The user can override these by specifying new 
   // scene graphs in the file:
   // $(SO_DRAGGER_DIR)/crossHairDragger.iv
   if(SO_KIT_IS_FIRST_INSTANCE())
   {
     readDefaultParts("crossHairDragger.iv", geomBuffer,
     		      sizeof(geomBuffer) - 1);
   }
	
   // Field that always shows current position of the dragger.
   SO_KIT_ADD_FIELD(translation, (0.0, 0.0, 0.0));

   // Creates the parts list for this nodekit
   SO_KIT_INIT_INSTANCE();

   // Create the parts of the dragger. This dragger has five 
   // parts that we need to create: "translator", 
   // "translatorActive", "feedback," and "feedbackActive" will 
   // be created using the resource mechanism. They are looked 
   // up in the global dictionary.
   // "rotator," used to position the feedback so it points in 
   // the direction selected by the user, will just be a plain 
   // old SoRotation node.
   // We call 'setPartAsDefault' because we are installing 
   // default geometries from the resource files. By calling
   // 'setPartAsDefault' instead of 'setPart', we insure that 
   // these parts will not write to file unless they are 
   // changed later. 
   setPartAsDefault("translator", "translateHairTranslator");
   setPartAsDefault("materialNormal", "translateNormalMaterialCross");
   setPartAsDefault("materialActive", "translateActiveMaterialCross");
   setPartAsDefault("materialPlaced", "translatePlacedMaterialCross");
   setPartAsDefault("materialInvalid", "translateInvalidMaterialCross");
   setPartAsDefault("scale", "scaleCross");

   // Set the switch parts to 0 to display the inactive parts.
   // The parts "translatorSwitch" and "feedbackSwitch"
   // are not public parts. (i.e., when making the catalog, the 
   // isPublic flag was set FALSE, so users cannot access them)
   // To retrieve the parts we must use the SO_GET_ANY_PART 
   // macro which calls the protected method getAnyPart().
   SoSwitch *sw;
   sw = SO_GET_ANY_PART(this, "materialSwitch", SoSwitch);
   setSwitchValue(sw, 0);

   sw = SO_GET_ANY_PART(this, "validitySwitch", SoBlinker);
   setSwitchValue(sw, 0);


   // This dragger does motion in a plane,
   // so we create a plane projector.
   planeProj = new SbPlaneProjector();

   // Add the callback functions that will be called when
   // the user clicks, drags, and releases.
   addStartCallback(&CrossHairDragger::startCB);
   addMotionCallback(&CrossHairDragger::motionCB);
   addFinishCallback(&CrossHairDragger::finishCB);

   // Updates the translation field when the dragger moves.
   addValueChangedCallback(&CrossHairDragger::valueChangedCB);

   // Updates the motionMatrix (and thus moves the dragger 
   // through space) to a new location whenever the translation
   // field is changed from the outside.
   fieldSensor = new SoFieldSensor(&CrossHairDragger::fieldSensorCB, this);
   fieldSensor->setPriority(0);

   setUpConnections( TRUE, TRUE );
}

CrossHairDragger::
~CrossHairDragger()
{
   // Delete what we created in the constructor.
   delete planeProj;
   if(fieldSensor)
   {
     delete fieldSensor;
   }
}

SbBool CrossHairDragger::
setUpConnections(
  SbBool onOff,
  SbBool doItAlways)
{
  if( !doItAlways && connectionsSetUp == onOff)
  {
    return(onOff);
  }

  if(onOff)
  {
    // We connect AFTER base class.
    SoDragger::setUpConnections(onOff, doItAlways);

    // Call the sensor CB to make things up-to-date.
    fieldSensorCB(this, NULL);

    // Connect the field sensor
    if(fieldSensor->getAttachedField() != &translation)
    {
      fieldSensor->attach( &translation );
    }
  }
  else
  {
    // We disconnect BEFORE base class.

    // Disconnect the field sensor.
    if(fieldSensor->getAttachedField())
    {
      fieldSensor->detach();
    }

    SoDragger::setUpConnections(onOff, doItAlways);
  }

  return(!(connectionsSetUp = onOff));
}

//  Static callback functions called by SoDragger when when the 
//  mouse goes down (over this dragger), drags, and releases.
void
CrossHairDragger::
startCB(
  void *,
  SoDragger *dragger)
{
  CrossHairDragger *myself = (CrossHairDragger *)dragger;
  myself->dragStart();
}

void CrossHairDragger::
motionCB(
  void *,
  SoDragger *dragger)
{
  CrossHairDragger *myself = (CrossHairDragger *)dragger;
  myself->drag();
}
void
CrossHairDragger::
finishCB(
  void *,
  SoDragger *dragger)
{
  CrossHairDragger *myself = (CrossHairDragger *) dragger;
  myself->dragFinish();
}

//  Called when user clicks down on this dragger. Sets up the 
//  projector and switches parts to their "active" versions.
void CrossHairDragger::
dragStart()
{
   // Display the 'active' parts...

   // Establish the projector line.
   // The direction of translation goes from the center of the
   // dragger toward the point that was hit, in local space. 
   // For the center, use (0,0,0).
   SbVec3f startLocalHitPt = getLocalStartingPoint();
   planeProj->setPlane(SbPlane(SbVec3f(0.0f, 0.0f, 1.0f), startLocalHitPt));

}

//  Called when the mouse translates during dragging. Moves
//  the dragger based on the mouse motion.
void CrossHairDragger::
drag()
{
  // Things can change between renderings. To be safe, update 
  // the projector with the current values.
  planeProj->setViewVolume(getViewVolume());    
  planeProj->setWorkingSpace(getLocalToWorldMatrix());

  // Find the new intersection on the projector.
  SbVec3f newHitPt = planeProj->project(getNormalizedLocaterPosition()); 

  // Get initial point expressed in our current local space.
  SbVec3f startHitPt = getLocalStartingPoint();

  // Motion in local space is difference between old and
  // new positions.
  SbVec3f motion = newHitPt - startHitPt;

  // Append this to the startMotionMatrix, which was saved
  // automatically at the beginning of the drag, to find 
  // the current motion matrix.
  setMotionMatrix(appendTranslation(getStartMotionMatrix(), motion));
}

//  Called when mouse button is released and drag is completed.
void CrossHairDragger::
dragFinish()
{
}

// Called when the motionMatrix changes. Sets the 'translation'
// field based on the new motionMatrix.
void CrossHairDragger::
valueChangedCB(
  void *,
  SoDragger *inDragger)
{
  CrossHairDragger *myself = (CrossHairDragger *) inDragger;

  // Get translation by decomposing motionMatrix.
  SbMatrix motMat = myself->getMotionMatrix();
  SbVec3f trans, scale;
  SbRotation rot, scaleOrient;
  motMat.getTransform(trans, rot, scale, scaleOrient);

  // Set "translation", disconnecting sensor while doing so.
  myself->fieldSensor->detach();
  if(myself->translation.getValue() != trans)
  {
    myself->translation = trans;
  }
  myself->fieldSensor->attach(&(myself->translation));
}

// If the "translation" field is set from outside, update
// motionMatrix accordingly.
void CrossHairDragger::
fieldSensorCB(
  void *inDragger,
  SoSensor *)
{
  CrossHairDragger *myself = (CrossHairDragger *) inDragger;
  SbMatrix motMat = myself->getMotionMatrix();
  myself->workFieldsIntoTransform(motMat);
  myself->setMotionMatrix(motMat);
}

void CrossHairDragger::
setScale(
  float size)
{
   SoScale *scale;
   scale = SO_GET_ANY_PART(this, "scale", SoScale);
   scale->scaleFactor.setValue(size,size,size);
}
