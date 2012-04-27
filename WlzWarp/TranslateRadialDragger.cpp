#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _TTranslateRadialDragger_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         TranslateRadialDragger.cpp
* \author       Zsolt Husz
* \date         MArch 2009
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
*
*
* Code is based on an example from the
* Inventor Toolmaker, chapter 8.
*
*  Resource names and part names for this dragger are:
*      Resource Name:                        Part Name:
*       translateRadialTranslator             translator
*       translateRadialTranslatorActive       translatorActive
*       translateRadialFeedback               feedback
*       translateRadialFeedbackActive         feedbackActive
*
*------------------------------------------------------------
*
* \brief        3D dragger
* \ingroup      Controls
*/

//#include <Inventor/SoPath.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/projectors/SbLineProjector.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoBlinker.h>
#include <Inventor/nodes/SoMaterial.h>

// Include file for our new class.
#include "TranslateRadialDragger.h"

// This file contains the variable 
// TranslateRadialDragger::geomBuffer, which describes 
// the default geometry resources for this dragger.
#include "draggers/TranslateRadialDraggerGeom.h"

SO_KIT_SOURCE(TranslateRadialDragger);

//  Initializes the type ID for this dragger node. This
//  should be called once after SoInteraction::init().
void TranslateRadialDragger::initClass() {
   SO_KIT_INIT_CLASS(TranslateRadialDragger, SoDragger, "Dragger");		
}

TranslateRadialDragger::TranslateRadialDragger() {
   SO_KIT_CONSTRUCTOR(TranslateRadialDragger);

   SO_KIT_ADD_CATALOG_ENTRY(validitySwitch, SoBlinker, TRUE,
                            geomSeparator, ,FALSE);

   SO_KIT_ADD_CATALOG_ENTRY(materialSwitch, SoSwitch, TRUE,
                            validitySwitch, ,FALSE);

   // Put this under geomSeparator so it draws efficiently.
   SO_KIT_ADD_CATALOG_ENTRY(translatorSwitch, SoSwitch, TRUE,
                            geomSeparator, ,FALSE);
   SO_KIT_ADD_CATALOG_ENTRY(translator, SoSeparator, TRUE,
                            translatorSwitch, ,TRUE);
   SO_KIT_ADD_CATALOG_ENTRY(translatorActive,SoSeparator,TRUE,
                            translatorSwitch, ,TRUE);
   SO_KIT_ADD_CATALOG_ENTRY(feedbackRotate, SoRotation, TRUE,
                            geomSeparator, ,TRUE);
   SO_KIT_ADD_CATALOG_ENTRY(feedbackSwitch, SoSwitch, TRUE,
                            geomSeparator, ,FALSE);
   SO_KIT_ADD_CATALOG_ENTRY(feedback, SoSeparator, TRUE,
                            feedbackSwitch, ,TRUE);
   SO_KIT_ADD_CATALOG_ENTRY(feedbackActive, SoSeparator, TRUE,
                            feedbackSwitch, ,TRUE);


   SO_KIT_ADD_CATALOG_ENTRY(materialPlaced, SoMaterial, TRUE,
                            materialSwitch, ,TRUE);
   SO_KIT_ADD_CATALOG_ENTRY(materialNormal, SoMaterial, TRUE,
                            materialSwitch, ,TRUE);
   SO_KIT_ADD_CATALOG_ENTRY(materialActive, SoMaterial, TRUE,
                            materialSwitch, ,TRUE);
   SO_KIT_ADD_CATALOG_ENTRY(materialInvalid, SoMaterial, TRUE,
                            validitySwitch, ,TRUE);

   // Read geometry resources. Only do this the first time we 
   // construct one.  'geomBuffer' contains our compiled in 
   // defaults. The user can override these by specifying new 
   // scene graphs in the file:
   // $(SO_DRAGGER_DIR)/translateRadialDragger.iv
   if (SO_KIT_IS_FIRST_INSTANCE())
      readDefaultParts("translateRadialDragger.iv", geomBuffer,
      sizeof(geomBuffer)-1);

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
   setPartAsDefault("translator",
                    "translateRadialTranslator");
   setPartAsDefault("translatorActive",
                    "translateRadialTranslatorActive");
   setPartAsDefault("feedback",
                    "translateRadialFeedback");
   setPartAsDefault("feedbackActive",
                    "translateRadialFeedbackActive");
   setPartAsDefault("materialNormal",
                    "translateNormalMaterial");
   setPartAsDefault("materialActive",
                    "translateActiveMaterial");
   setPartAsDefault("materialPlaced",
                    "translatePlacedMaterial");
   setPartAsDefault("materialInvalid",
                    "translateInvalidMaterial");

   // Set the switch parts to 0 to display the inactive parts.
   // The parts "translatorSwitch" and "feedbackSwitch"
   // are not public parts. (i.e., when making the catalog, the 
   // isPublic flag was set FALSE, so users cannot access them)
   // To retrieve the parts we must use the SO_GET_ANY_PART 
   // macro which calls the protected method getAnyPart().
   SoSwitch *sw;
   sw = SO_GET_ANY_PART(this, "translatorSwitch", SoSwitch);
   setSwitchValue(sw, 0);
   sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
   setSwitchValue(sw, 0);
   sw = SO_GET_ANY_PART(this, "materialSwitch", SoSwitch);
   setSwitchValue(sw, 0);
   sw = SO_GET_ANY_PART(this, "validitySwitch", SoBlinker);
   setSwitchValue(sw, 0);

   // This dragger does motion along a line,
   // so we create a line projector.
   lineProj = new SbLineProjector();

   // Add the callback functions that will be called when
   // the user clicks, drags, and releases.
   addStartCallback(&TranslateRadialDragger::startCB);
   addMotionCallback(&TranslateRadialDragger::motionCB);
   addFinishCallback(&TranslateRadialDragger::finishCB);

   // Updates the translation field when the dragger moves.
   addValueChangedCallback(
      &TranslateRadialDragger::valueChangedCB);

   // Updates the motionMatrix (and thus moves the dragger 
   // through space) to a new location whenever the translation
   // field is changed from the outside.
   fieldSensor = new SoFieldSensor(
      &TranslateRadialDragger::fieldSensorCB, this);
   fieldSensor->setPriority(0);
   setUpConnections( TRUE, TRUE );
}

TranslateRadialDragger::~TranslateRadialDragger() {
   // Delete what we created in the constructor.
   delete lineProj;
   if (fieldSensor)
      delete fieldSensor;
}

SbBool TranslateRadialDragger::setUpConnections( SbBool onOff,
                            SbBool doItAlways) {
   if ( !doItAlways && connectionsSetUp == onOff)
      return onOff;
   if (onOff) {
      // We connect AFTER base class.
      SoDragger::setUpConnections(onOff, doItAlways);

      // Call the sensor CB to make things up-to-date.
      fieldSensorCB(this, NULL);

      // Connect the field sensor
      if (fieldSensor->getAttachedField() != &translation)
         fieldSensor->attach( &translation );
   }
   else {
      // We disconnect BEFORE base class.

      // Disconnect the field sensor.
      if (fieldSensor->getAttachedField())
         fieldSensor->detach();

      SoDragger::setUpConnections(onOff, doItAlways);
   }
   return !(connectionsSetUp = onOff);
}

void TranslateRadialDragger::startCB(void *, SoDragger *dragger) {
   TranslateRadialDragger *myself 
      = (TranslateRadialDragger *) dragger;
   myself->dragStart();
}

void TranslateRadialDragger::motionCB(void *, SoDragger *dragger) {
   TranslateRadialDragger *myself 
      = (TranslateRadialDragger *) dragger;
   myself->drag();
}

void TranslateRadialDragger::finishCB(void *, SoDragger *dragger) {
   TranslateRadialDragger *myself 
      = (TranslateRadialDragger *) dragger;
   myself->dragFinish();
}

void TranslateRadialDragger::dragStart() {
   // Display the 'active' parts...
   SoSwitch *sw;
   sw = SO_GET_ANY_PART(this, "translatorSwitch", SoSwitch);
   setSwitchValue(sw, 1);
   sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
   setSwitchValue(sw, 1);

   // Establish the projector line.
   // The direction of translation goes from the center of the
   // dragger toward the point that was hit, in local space. 
   // For the center, use (0,0,0).
   SbVec3f startLocalHitPt = getLocalStartingPoint();
   lineProj->setLine(SbLine(SbVec3f(0,0,0), startLocalHitPt));

   // orient the feedback geometry.
   orientFeedbackGeometry(startLocalHitPt);
}

void TranslateRadialDragger::orientFeedbackGeometry( const SbVec3f &localDir) {
   // By default, feedback geometry aligns with the x axis.
   // Rotate so that it points in the given direction.
   SbRotation rotXToDir = SbRotation(SbVec3f(1,0,0), localDir);

   // Give this rotation to the "feedbackRotate" part.
   SoRotation *myPart = SO_GET_ANY_PART(this, "feedbackRotate", 
      SoRotation);
   myPart->rotation.setValue(rotXToDir);
}

void TranslateRadialDragger::drag() {
   // Things can change between renderings. To be safe, update 
   // the projector with the current values.
   lineProj->setViewVolume(getViewVolume());    
   lineProj->setWorkingSpace(getLocalToWorldMatrix());

   // Find the new intersection on the projector.
   SbVec3f newHitPt 
      = lineProj->project(getNormalizedLocaterPosition()); 

   // Get initial point expressed in our current local space.
   SbVec3f startHitPt = getLocalStartingPoint();

   // Motion in local space is difference between old and
   // new positions.
   SbVec3f motion = newHitPt - startHitPt;

   // Append this to the startMotionMatrix, which was saved
   // automatically at the beginning of the drag, to find 
   // the current motion matrix.
   setMotionMatrix(
      appendTranslation(getStartMotionMatrix(), motion));
}

void TranslateRadialDragger::dragFinish() {
   // Display inactive versions of parts...
   SoSwitch *sw;
   sw = SO_GET_ANY_PART(this, "translatorSwitch", SoSwitch);
   setSwitchValue(sw, 0);
   sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
   setSwitchValue(sw, 0);

   // Get rid of the "feedbackRotate" part.  We don't need
   // it since we aren't showing the feedback any more.
   setAnyPart("feedbackRotate", NULL);
}

void TranslateRadialDragger::valueChangedCB(void *,
   SoDragger *inDragger) {
   TranslateRadialDragger *myself 
      = (TranslateRadialDragger *) inDragger;

   // Get translation by decomposing motionMatrix.
   SbMatrix motMat = myself->getMotionMatrix();
   SbVec3f trans, scale;
   SbRotation rot, scaleOrient;
   motMat.getTransform(trans, rot, scale, scaleOrient);

   // Set "translation", disconnecting sensor while doing so.
   myself->fieldSensor->detach();
   if (myself->translation.getValue() != trans)
      myself->translation = trans;
   myself->fieldSensor->attach(&(myself->translation));
}

void TranslateRadialDragger::fieldSensorCB(void *inDragger, 
   SoSensor *) {
   TranslateRadialDragger *myself 
      = (TranslateRadialDragger *) inDragger;
   SbMatrix motMat = myself->getMotionMatrix();
   myself->workFieldsIntoTransform(motMat);
   myself->setMotionMatrix(motMat);
}
