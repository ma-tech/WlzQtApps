#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _Commands_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         Commands.cpp
* \author       Zsolt Husz
* \date         June 2009
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
* \brief        Undoable commands
* \ingroup      Control
*/

//project includes
#include "Commands.h"
#include "LandmarkModel.h"
#include "ObjectListModel.h"
#include "WoolzObject.h"
#include "WarperConfig.h"
#include "WoolzDynContourISO.h"
#include "LandmarkController.h"
#include "TransferFunction.h"

//Qt incldues
#include <QFile>
#include <QMessageBox>
#include <QApplication>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

// unique id definitions for mergable commands
int MoveLandmark::id () const {return 1001;}
int RenameObject::id () const {return 1002;}
int ColourObject::id () const {return 1003;}
int SetHighThreshold::id () const {return 1004;}
int SetLowThreshold::id () const {return 1005;}
int WoolzObjectChangeVisibility::id () const {return 1006;}
int WoolzDynThresholdedObjSetChannel::id () const {return 1007;}
int WoolzDynObjectSetAutoUpdate::id () const {return 1008;}
int WoolzDynObjectSetSourceObj::id () const {return 1009;}
int WoolzDynISOObjSetValue::id () const {return 1010;}
int WoolzDynISOObjSetBoundary::id () const {return 1011;}
int WoolzDynISOObjSetSubsample::id () const {return 1012;}
int WoolzDynISOObjSetFilter::id () const {return 1013;}
int WoolzDynISOObjSetShellSelection::id () const {return 1014;}
int TransferFunctionMapUpdate::id () const {return 1015;}
int TransferFunctionSetLowCutOff::id () const {return 1016;}
int TransferFunctionSetHighCutOff::id () const {return 1017;}
int WarpingSetDelta::id () const {return 1018;}
int WarpingtSetBasisFnType::id () const {return 1019;}
//int TransferFunctionLoad::id () const {return 1020;}

CreateWoolzObject::CreateWoolzObject(ObjectListModel *objectListModel, WoolzObject *object, QUndoCommand * parent) :
        QUndoCommand(parent), m_objectListModel(objectListModel),m_object(object), m_allive(false) { setText("Add object " + object->name());}

CreateWoolzObject::~CreateWoolzObject() {
   if (!m_allive)
     delete m_object;
}

void CreateWoolzObject::undo() {
   QApplication::setOverrideCursor(Qt::WaitCursor);
   m_objectListModel->removeObjectNoDelete(m_object);
   m_allive = false;
   QApplication::restoreOverrideCursor();
}

void CreateWoolzObject::redo() {
  QApplication::setOverrideCursor(Qt::WaitCursor);
  m_object->update(true);
  m_objectListModel->addObject(m_object);
  QApplication::restoreOverrideCursor();
  m_allive = true;
}

DeleteWoolzObject::DeleteWoolzObject(ObjectListModel *objectListModel, WoolzObject *object, QUndoCommand * parent) :
        QUndoCommand(parent), m_objectListModel(objectListModel),m_object(object), m_allive(true) { setText("Delete object " + object->name());}

DeleteWoolzObject::~DeleteWoolzObject() {
   if (!m_allive) {
     delete m_object;
    };
}

void DeleteWoolzObject::undo() {
  QApplication::setOverrideCursor(Qt::WaitCursor);
  m_object->update(true);
  m_objectListModel->addObject(m_object);
  m_allive = true;
  QApplication::restoreOverrideCursor();
}

void DeleteWoolzObject::redo() {
  QApplication::setOverrideCursor(Qt::WaitCursor);
  m_objectListModel->removeObjectNoDelete(m_object);
  m_allive = false;
  QApplication::restoreOverrideCursor();
}

RenameObject::RenameObject (WoolzObject *object, QString name, QUndoCommand * parent) :
        QUndoCommand(parent), m_object(object), m_name(name) { setText("Rename object");}

void RenameObject::undo() {
   m_object->setName(m_oldName);
}
void RenameObject::redo() {
   m_oldName=m_object->name();
   m_object->setName(m_name);
}

bool RenameObject::mergeWith ( const QUndoCommand * command ) {
  if (command->id() != id()) // make sure other is also an RenameObject command
    return false;
  const RenameObject *renameCommand = static_cast<const RenameObject*>(command);

  if (renameCommand ->m_object != m_object) // make sure other is also referes the same object
    return false;

  m_name  = renameCommand->m_name;
  return true;
}

ColourObject::ColourObject (WoolzObject *object, const QColor colour, QUndoCommand * parent) :
        QUndoCommand(parent), m_object(object), m_colour(colour) { setText("Object colour change");}

void ColourObject::undo() {
   m_object->setQColour(m_oldColour);
}

void ColourObject::redo() {
   m_oldColour=m_object->qColour();
   m_object->setQColour(m_colour);
}

bool ColourObject::mergeWith ( const QUndoCommand * command ) {
  if (command->id() != id()) // make sure other is also an ColourObject command
    return false;
  const ColourObject *colourCommand = static_cast<const ColourObject*>(command);
  if (colourCommand ->m_object != m_object) // make sure other is also referes the same object
    return false;
  m_colour  = colourCommand->m_colour;
  return true;
}

WoolzObjectChangeVisibility::WoolzObjectChangeVisibility(WoolzObject *object, const bool visible, QUndoCommand * parent) :
        QUndoCommand(parent), m_object(object), m_visible(visible) { setText("Object visibility change");}

void WoolzObjectChangeVisibility::undo() {
   m_object->setVisible(m_oldVisible);
}

void WoolzObjectChangeVisibility::redo() {
   m_oldVisible=m_object->visible();
   m_object->setVisible(m_visible);
}

bool WoolzObjectChangeVisibility::mergeWith ( const QUndoCommand * command ) {
  if (command->id() != id()) // make sure other is also an WoolzObjectChangeVisibility command
    return false;
  const WoolzObjectChangeVisibility *visiblityCommand = static_cast<const WoolzObjectChangeVisibility*>(command);
  if (visiblityCommand ->m_object != m_object) // make sure other is also referes the same object
    return false;
  m_visible  = visiblityCommand->m_visible;
  return true;
}



CreateMeshObject::CreateMeshObject(ObjectListModel *objectListModel, LandmarkModel *landmarkModel, WoolzObject *object, QUndoCommand * parent) :
        QUndoCommand(parent), m_objectListModel(objectListModel), m_landmarkModel(landmarkModel), m_object(object), m_oldObject(NULL), m_allive(false) { setText("Add object mesh");}

CreateMeshObject::~CreateMeshObject() {
   if (!m_allive)
     delete m_object;
   else if (m_oldObject)
     delete m_oldObject;
}

void CreateMeshObject::undo() {
  QApplication::setOverrideCursor(Qt::WaitCursor);
  if (m_oldObject)
      m_oldObject->update(true);
  m_landmarkModel->setMeshData(m_oldObject);
  m_objectListModel->addMeshObjectNoDelete(m_oldObject);
  m_landmarkModel->updateAllValidity();
  m_allive = false;
  QApplication::restoreOverrideCursor();
}

void CreateMeshObject::redo() {
  QApplication::setOverrideCursor(Qt::WaitCursor);
  if (m_object)
    m_object->update(true);
  m_oldObject=m_objectListModel->getMeshObject();
  m_landmarkModel->setMeshData(m_object);
  m_objectListModel->addMeshObjectNoDelete(m_object);
  m_landmarkModel->updateAllValidity();
  QApplication::restoreOverrideCursor();
  m_allive = true;
}

AddHalfLandmark::AddHalfLandmark (LandmarkController *controller, const WlzDVertex3 newPosition, LandmarkModel::IndexType indexType, QUndoCommand * parent)
         : QUndoCommand(parent), m_controller(controller), m_position(newPosition), m_indexType(indexType) {
           m_previouslySet = false;
    setText(QString("add ") + (indexType==LandmarkModel::sourceV?"source":"target")+" landmark");
  }

AddHalfLandmark::AddHalfLandmark (LandmarkController *controller, const SbVec3f newPosition, LandmarkModel::IndexType indexType, QUndoCommand * parent)
         : QUndoCommand(parent), m_controller(controller), m_indexType(indexType) {
           m_previouslySet = false;
    setText(QString("add ") + (indexType==LandmarkModel::sourceV?"source":"target")+" landmark");
    m_position.vtX = newPosition[0];
    m_position.vtY = newPosition[1];
    m_position.vtZ = newPosition[2];
  }

void AddHalfLandmark::undo() {
  QApplication::setOverrideCursor(Qt::WaitCursor);
  if (m_previouslySet)
    m_controller->addHalfLandmark(m_positionOld, m_indexType);
  else
    m_controller->cancelIncompleteLandmarks();
  QApplication::restoreOverrideCursor();
}

void AddHalfLandmark::redo() {
  QApplication::setOverrideCursor(Qt::WaitCursor);
  m_previouslySet = m_controller->getHalfLandmark(m_positionOld, m_indexType);
  m_controller->addHalfLandmark(m_position, m_indexType);
  QApplication::restoreOverrideCursor();
}


AddSecondHalfLandmark::AddSecondHalfLandmark (LandmarkController *controller, const WlzDVertex3 newPosition, LandmarkModel::IndexType indexType, QUndoCommand * parent)
         : QUndoCommand(parent), m_controller(controller), m_position(newPosition), m_indexType(indexType) {
  setText("add landmark");
}

void AddSecondHalfLandmark::undo() {
  QApplication::setOverrideCursor(Qt::WaitCursor);
  m_controller->getModel()->removeLast();
  m_controller->addHalfLandmark(m_positionOld, m_indexType == LandmarkModel::sourceV ? LandmarkModel::targetV : LandmarkModel::sourceV);
  QApplication::restoreOverrideCursor();
}

void AddSecondHalfLandmark::redo() {
  QApplication::setOverrideCursor(Qt::WaitCursor);
  m_controller->getHalfLandmark(m_positionOld, m_indexType == LandmarkModel::sourceV ? LandmarkModel::targetV : LandmarkModel::sourceV);
  m_controller->addSecondHalfLandmark(m_position, m_indexType);
  QApplication::restoreOverrideCursor();
}

EnableDisableUpdateCommand::EnableDisableUpdateCommand (bool isFirst, EnableDisableUpdateCommand * pair, QUndoCommand * parent) :
         QUndoCommand(parent), m_isFirst(isFirst), m_pair(pair), m_state(false) {
    Q_ASSERT(pair);
    m_pair->setPair(!m_isFirst, this);
}

EnableDisableUpdateCommand ::EnableDisableUpdateCommand (QUndoCommand * parent) :
         QUndoCommand(parent) { }

void EnableDisableUpdateCommand::setPair(bool isFirst, EnableDisableUpdateCommand * pair) {
  m_isFirst = isFirst;
  m_pair = pair;
}

void EnableDisableUpdateCommand::undo() {
   if (!m_isFirst) {
     m_state = config.globalWarpUpdate();
     config.setGlobalWarpUpdate(false);
  } else
     config.setGlobalWarpUpdate(m_pair->state());
}

void EnableDisableUpdateCommand::redo() {
   if (m_isFirst) {
     m_state = config.globalWarpUpdate();
     config.setGlobalWarpUpdate(false);
  } else
     config.setGlobalWarpUpdate(m_pair->state());
}

LoadLandmarks::LoadLandmarks(LandmarkModel *model, QString filename, QUndoCommand * parent)
         : QUndoCommand(parent), m_model(model), m_filename(filename) {}

void LoadLandmarks::undo() {
  m_model->removeAllLandmarks();
}

void LoadLandmarks::redo() {
  QApplication::setOverrideCursor(Qt::WaitCursor);
  m_model->removeAllLandmarks(); //make sure no landmarks are present

  QFile file(m_filename);
  if (!file.open(QFile::ReadOnly | QFile::Text)) {
    QMessageBox::warning(NULL, "Load landmarks", "Cannot open landmark file.");
    QApplication::restoreOverrideCursor();
    return;
  }

  QDomDocument doc;
  QString errorStr;
  int errorLine, errorColumn;
  if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) {
     QMessageBox::warning(NULL, "Load landmarks", QString("Parse error at line %1 column %2 :").arg(errorLine).arg(errorColumn) + errorStr);
     QApplication::restoreOverrideCursor();
     return;
  }

  QDomElement root = doc.documentElement();
  if (root.tagName() == LandmarkModel::xmlTag) {
     m_model->parseDOM(root);
  } else
     QMessageBox::warning(NULL, "Load landmarks", QString("Not a landmark file."));
  QApplication::restoreOverrideCursor();
}

RemoveLandmark ::RemoveLandmark (LandmarkModel *model, int index, QUndoCommand * parent)
         : QUndoCommand(parent), m_model(model), m_index(index) { setText("remove landmark"); }

void RemoveLandmark::undo() {
  QApplication::setOverrideCursor(Qt::WaitCursor);
  m_model->insertLandmarkPair(m_index, m_pp);
  QApplication::restoreOverrideCursor();
}

void RemoveLandmark::redo() {
  QApplication::setOverrideCursor(Qt::WaitCursor);
  m_pp = *(m_model->getPointPair(m_index));
  m_model->removeLandmark(m_index);
  QApplication::restoreOverrideCursor();
}

AddLandmark::AddLandmark(LandmarkModel *model, PointPair &pp, QUndoCommand * parent)
         : QUndoCommand(parent), m_model(model), m_pp(pp) { setText("add landmark"); }

void AddLandmark::undo() {
  QApplication::setOverrideCursor(Qt::WaitCursor);
  bool autoUpdate = config.globalAutoUpdate();
  config.setGlobalAutoUpdate(false);
  m_model->removeLast();
  config.setGlobalAutoUpdate(autoUpdate);//TODO: forces update->do somehow else
  QApplication::restoreOverrideCursor();
}

void AddLandmark::redo() {
  QApplication::setOverrideCursor(Qt::WaitCursor);
  m_model->insertLandmarkPair(-1, m_pp);
  QApplication::restoreOverrideCursor();
}

MoveLandmark::MoveLandmark(LandmarkModel *model, int index, SbVec3f newpp, LandmarkModel::IndexType indexType, QUndoCommand * parent)
         : QUndoCommand(parent), m_model(model), m_index(index), m_indexType(indexType), m_newpp(newpp) { setText("move landmark"); }

void MoveLandmark::undo() {
  QApplication::setOverrideCursor(Qt::WaitCursor);
  m_model->move(m_index, m_pp, m_indexType);
  QApplication::restoreOverrideCursor();
}

void MoveLandmark::redo() {
  QApplication::setOverrideCursor(Qt::WaitCursor);
  m_pp = *(m_model->getPointPair(m_index));
  m_model->move(m_index, m_newpp, m_indexType);
  QApplication::restoreOverrideCursor();
}

bool MoveLandmark::mergeWith ( const QUndoCommand * command ) {
  if (command->id() != id()) // make sure other is also an MoveLandmark command
    return false;
  const MoveLandmark *moveCommand = static_cast<const MoveLandmark*>(command);

  if (moveCommand->m_index != m_index || moveCommand->m_indexType != m_indexType) // make sure other is also referes the same landmark
    return false;

  m_newpp = moveCommand->m_newpp;
  return true;
}

SetLowThreshold::SetLowThreshold(WoolzDynThresholdedObj *object, const unsigned char lowTh, QUndoCommand * parent) :
        QUndoCommand(parent), m_object(object), m_lowTh(lowTh) { setText(QString("Threshold change to %1").arg(lowTh));}

void SetLowThreshold::undo() {
   QApplication::setOverrideCursor(Qt::WaitCursor);
   m_object->setLowThreshold(m_oldLowTh);
   m_object->update();
   setText(QString("Threshold change to %1").arg(m_lowTh));
   QApplication::restoreOverrideCursor();
}

void SetLowThreshold::redo() {
   QApplication::setOverrideCursor(Qt::WaitCursor);
   m_oldLowTh=m_object->lowTh();
   m_object->setLowThreshold(m_lowTh);
   m_object->update();
   setText(QString("Threshold change to %1").arg(m_oldLowTh));
   QApplication::restoreOverrideCursor();
}

bool SetLowThreshold::mergeWith ( const QUndoCommand * command ) {
  if (command->id() != id()) // make sure other is also an SetLowThreshold command
    return false;
  const SetLowThreshold *setLowThresholdCommand = static_cast<const SetLowThreshold*>(command);
  if (setLowThresholdCommand ->m_object != m_object) // make sure other is also referes the same object
    return false;
  m_lowTh = setLowThresholdCommand->m_lowTh;
  return true;
}

SetHighThreshold::SetHighThreshold(WoolzDynThresholdedObj *object, const unsigned char highTh, QUndoCommand * parent) :
        QUndoCommand(parent), m_object(object), m_highTh(highTh) { }

void SetHighThreshold::undo() {
   QApplication::setOverrideCursor(Qt::WaitCursor);
   m_object->setHighThreshold(m_oldHighTh);
   m_object->update();
   setText(QString("Threshold change to %1").arg(m_highTh));
   QApplication::restoreOverrideCursor();
}

void SetHighThreshold::redo() {
   QApplication::setOverrideCursor(Qt::WaitCursor);
   m_oldHighTh=m_object->highTh();
   m_object->setHighThreshold(m_highTh);
   m_object->update();
   setText(QString("Threshold change to %1").arg(m_oldHighTh));
   QApplication::restoreOverrideCursor();
}

bool SetHighThreshold::mergeWith ( const QUndoCommand * command ) {
  if (command->id() != id()) // make sure other is also an SetHighThreshold command
    return false;
  const SetHighThreshold *setHighThresholdCommand = static_cast<const SetHighThreshold*>(command);
  if (setHighThresholdCommand ->m_object != m_object) // make sure other is also referes the same object
    return false;
  m_highTh = setHighThresholdCommand->m_highTh;
  return true;
}

WoolzDynThresholdedObjSetChannel::WoolzDynThresholdedObjSetChannel(WoolzDynThresholdedObj *object, const enum WoolzDynThresholdedObj::channelTypes channel, QUndoCommand * parent) :
        QUndoCommand(parent), m_object(object), m_channel(channel) { setText(QString("Change segmentation channel")); }

void WoolzDynThresholdedObjSetChannel::undo() {
   QApplication::setOverrideCursor(Qt::WaitCursor);
   m_object->setChannel(m_oldChannel);
   m_object->update();
   QApplication::restoreOverrideCursor();
}

void WoolzDynThresholdedObjSetChannel::redo() {
   QApplication::setOverrideCursor(Qt::WaitCursor);
   m_oldChannel=m_object->channel();
   m_object->setChannel(m_channel);
   m_object->update();
   QApplication::restoreOverrideCursor();
}

bool WoolzDynThresholdedObjSetChannel::mergeWith ( const QUndoCommand * command ) {
  if (command->id() != id()) // make sure other is also an WoolzDynThresholdedObjSetChannel command
    return false;
  const WoolzDynThresholdedObjSetChannel *setChannelCommand = static_cast<const WoolzDynThresholdedObjSetChannel*>(command);
  if (setChannelCommand ->m_object != m_object) // make sure other is also referes the same object
    return false;
  m_channel = setChannelCommand->m_channel;
  return true;
}

WoolzDynObjectSetAutoUpdate::WoolzDynObjectSetAutoUpdate(WoolzDynObject *object, const bool enabled, QUndoCommand * parent) :
        QUndoCommand(parent), m_object(object), m_enabled(enabled) { setText("Auto update change");}

void WoolzDynObjectSetAutoUpdate::undo() {
   QApplication::setOverrideCursor(Qt::WaitCursor);
   m_object->setAutoUpdate(m_oldEnabled);
   if (m_oldEnabled)
       m_object->update();
   QApplication::restoreOverrideCursor();
}

void WoolzDynObjectSetAutoUpdate::redo() {
   QApplication::setOverrideCursor(Qt::WaitCursor);
   m_oldEnabled=m_object->autoUpdate();
   m_object->setAutoUpdate(m_enabled);
   if (m_enabled)
       m_object->update();
   QApplication::restoreOverrideCursor();
}

bool WoolzDynObjectSetAutoUpdate::mergeWith ( const QUndoCommand * command ) {
  if (command->id() != id()) // make sure other is also an WoolzDynObjectSetAutoUpdate command
    return false;
  const WoolzDynObjectSetAutoUpdate *visiblityCommand = static_cast<const WoolzDynObjectSetAutoUpdate*>(command);
  if (visiblityCommand->m_object != m_object) // make sure other is also referes the same object
    return false;
  m_enabled  = visiblityCommand->m_enabled;
  return true;
}

WoolzDynObjectSetSourceObj::WoolzDynObjectSetSourceObj(WoolzDynObject *object, WoolzObject *sourceObj, QUndoCommand * parent) :
        QUndoCommand(parent), m_object(object) {
    m_sourceObjID = sourceObj->ID();
    setText("Select source");
   }

void WoolzDynObjectSetSourceObj::undo() {
   QApplication::setOverrideCursor(Qt::WaitCursor);
   m_object->setSourceObj(m_oldSourceObjID);
   m_object->update();
   QApplication::restoreOverrideCursor();
}

void WoolzDynObjectSetSourceObj::redo() {
   QApplication::setOverrideCursor(Qt::WaitCursor);
   m_oldSourceObjID=m_object->sourceObjID();
   m_object->setSourceObj(m_sourceObjID);
   m_object->update();
   QApplication::restoreOverrideCursor();
}

bool WoolzDynObjectSetSourceObj::mergeWith ( const QUndoCommand * command ) {
  if (command->id() != id()) // make sure other is also an WoolzDynObjectSetSourceObj command
    return false;
  const WoolzDynObjectSetSourceObj *setValue = static_cast<const WoolzDynObjectSetSourceObj*>(command);
  if (setValue->m_object != m_object) // make sure other is also referes the same object
    return false;
  m_sourceObjID  = setValue->m_sourceObjID;
  return true;
}

/// ISO

WoolzDynISOObjSetValue::WoolzDynISOObjSetValue(WoolzDynContourISO *object, char value, QUndoCommand * parent ) :
        QUndoCommand(parent), m_object(object), m_value(value) {
    setText("Set ISO value");
   }

void WoolzDynISOObjSetValue::undo() {
   QApplication::setOverrideCursor(Qt::WaitCursor);
   m_object->setValue(m_oldValue);
   m_object->update();
   QApplication::restoreOverrideCursor();
}

void WoolzDynISOObjSetValue::redo() {
   QApplication::setOverrideCursor(Qt::WaitCursor);
   m_oldValue=m_object->value();
   m_object->setValue(m_value);
   m_object->update();
   QApplication::restoreOverrideCursor();
}

bool WoolzDynISOObjSetValue::mergeWith ( const QUndoCommand * command ) {
  if (command->id() != id()) // make sure other is also an WoolzDynISOObjSetValue command
    return false;
  const WoolzDynISOObjSetValue *setCommand = static_cast<const WoolzDynISOObjSetValue*>(command);
  if (setCommand->m_object != m_object) // make sure other is also referes the same object
    return false;
  m_value  = setCommand->m_value;
  return true;
}

WoolzDynISOObjSetBoundary::WoolzDynISOObjSetBoundary(WoolzDynContourISO *object, bool enabed, QUndoCommand * parent ) :
        QUndoCommand(parent),  m_object(object), m_enabed(enabed) {
    setText("Set ISO Boundary");
   }

void WoolzDynISOObjSetBoundary::undo() {
   QApplication::setOverrideCursor(Qt::WaitCursor);
   m_object->setBoundary(m_oldEnabed);
   m_object->update();
   QApplication::restoreOverrideCursor();
}

void WoolzDynISOObjSetBoundary::redo() {
   QApplication::setOverrideCursor(Qt::WaitCursor);
   m_oldEnabed=m_object->boundary();
   m_object->setBoundary(m_enabed);
   m_object->update();
   QApplication::restoreOverrideCursor();
}

bool WoolzDynISOObjSetBoundary::mergeWith ( const QUndoCommand * command ) {
  if (command->id() != id()) // make sure other is also an WoolzDynISOObjSetValue command
    return false;
  const WoolzDynISOObjSetBoundary *setCommand = static_cast<const WoolzDynISOObjSetBoundary*>(command);
  if (setCommand->m_object != m_object) // make sure other is also referes the same object
    return false;
  m_enabed  = setCommand->m_enabed;
  return true;
}

WoolzDynISOObjSetSubsample::WoolzDynISOObjSetSubsample(WoolzDynContourISO *object, char subsample, QUndoCommand * parent ):
        QUndoCommand(parent), m_object(object), m_subsample(subsample) {
    setText("Set ISO subsample");
   }

void WoolzDynISOObjSetSubsample::undo() {
   QApplication::setOverrideCursor(Qt::WaitCursor);
   m_object->setSubsample(m_oldSubsample);
   m_object->update();
   QApplication::restoreOverrideCursor();
}

void WoolzDynISOObjSetSubsample::redo() {
   QApplication::setOverrideCursor(Qt::WaitCursor);
   m_oldSubsample=m_object->subsample();
   m_object->setSubsample(m_subsample);
   m_object->update();
   QApplication::restoreOverrideCursor();
}

bool WoolzDynISOObjSetSubsample::mergeWith ( const QUndoCommand * command ) {
  if (command->id() != id()) // make sure other is also an WoolzDynISOObjSetValue command
    return false;
  const WoolzDynISOObjSetSubsample *setCommand = static_cast<const WoolzDynISOObjSetSubsample*>(command);
  if (setCommand->m_object != m_object) // make sure other is also referes the same object
    return false;
  m_subsample  = setCommand->m_subsample;
  return true;
}


WoolzDynISOObjSetFilter::WoolzDynISOObjSetFilter(WoolzDynContourISO *object, bool enabed, QUndoCommand * parent ) :
        QUndoCommand(parent),  m_object(object), m_enabed(enabed) {
    setText("Set ISO Boundary");
   }

void WoolzDynISOObjSetFilter::undo() {
   QApplication::setOverrideCursor(Qt::WaitCursor);
   m_object->setFilter(m_oldEnabed);
   m_object->update();
   QApplication::restoreOverrideCursor();
}

void WoolzDynISOObjSetFilter::redo() {
   QApplication::setOverrideCursor(Qt::WaitCursor);
   m_oldEnabed=m_object->filter();
   m_object->setFilter(m_enabed);
   m_object->update();
   QApplication::restoreOverrideCursor();
}

bool WoolzDynISOObjSetFilter::mergeWith ( const QUndoCommand * command ) {
  if (command->id() != id()) // make sure other is also an WoolzDynISOObjSetValue command
    return false;
  const WoolzDynISOObjSetFilter *setCommand = static_cast<const WoolzDynISOObjSetFilter*>(command);
  if (setCommand->m_object != m_object) // make sure other is also referes the same object
    return false;
  m_enabed  = setCommand->m_enabed;
  return true;
}


WoolzDynISOObjSetShellSelection::WoolzDynISOObjSetShellSelection(WoolzDynContourISO *object, int selection, QUndoCommand * parent ) :
        QUndoCommand(parent),  m_object(object), m_selection(selection) {
    setText("Set ISO Boundary");
}

void WoolzDynISOObjSetShellSelection::undo() {
   QApplication::setOverrideCursor(Qt::WaitCursor);
   m_object->setSelection(m_oldSelection);
   m_object->recomputeShells();
   QApplication::restoreOverrideCursor();
}

void WoolzDynISOObjSetShellSelection::redo() {
   QApplication::setOverrideCursor(Qt::WaitCursor);
   m_oldSelection=m_object->selection();
   m_object->setSelection(m_selection);
   m_object->recomputeShells();
   QApplication::restoreOverrideCursor();
}

bool WoolzDynISOObjSetShellSelection::mergeWith ( const QUndoCommand * command ) {
  if (command->id() != id()) // make sure other is also an WoolzDynISOObjSetValue command
    return false;
  const WoolzDynISOObjSetShellSelection *setCommand = static_cast<const WoolzDynISOObjSetShellSelection*>(command);
  if (setCommand->m_object != m_object) // make sure other is also referes the same object
    return false;
  m_selection  = setCommand->m_selection;
  return true;
}

///////////////////////////////////////////////////////////////////////////////////  Transfer function

TransferFunctionMapUpdate::TransferFunctionMapUpdate(ObjectListModel *objectListModel, int objID, SoMFFloat &colorMap, QUndoCommand * parent ) :
    QUndoCommand(parent),  m_objectListModel(objectListModel), m_objID(objID) {
         setText("Change Transfer Function");
         m_colorMap.copyFrom(colorMap);
}

void TransferFunctionMapUpdate::undo() {
  WoolzObject *obj=   m_objectListModel->getObject(m_objID) ;
  if (!obj)
    return;
  TransferFunction *transferFunction = obj->transferFunction();
  if (!transferFunction)
    return;
  transferFunction->setColorMap(m_oldColorMap);
}

void TransferFunctionMapUpdate::redo() {
  WoolzObject *obj=   m_objectListModel->getObject(m_objID) ;
  if (!obj)
    return;
  TransferFunction *transferFunction = obj->transferFunction();
  if (!transferFunction)
    return;
  m_oldColorMap.copyFrom(transferFunction->getColorMap());
  transferFunction->setColorMap(m_colorMap);
}

bool TransferFunctionMapUpdate::mergeWith ( const QUndoCommand * command ) {
  if (command->id() != id()) // make sure other is also an TransferFunctionMapUpdate command
    return false;
  const TransferFunctionMapUpdate *setCommand = static_cast<const TransferFunctionMapUpdate*>(command);
  if (setCommand->m_objID != m_objID) // make sure other is also referes the same object
    return false;
   m_colorMap.copyFrom(setCommand->m_colorMap);
  return true;
}

TransferFunctionSetLowCutOff::TransferFunctionSetLowCutOff(ObjectListModel *objectListModel, int objID, unsigned char cutOff, QUndoCommand * parent):
    QUndoCommand(parent),  m_objectListModel(objectListModel), m_objID(objID), m_cutOff(cutOff) {
         setText("Transfer Function set low cut off");
}

void TransferFunctionSetLowCutOff::undo() {
  WoolzObject *obj=   m_objectListModel->getObject(m_objID) ;
  if (!obj)
    return;
  TransferFunction *transferFunction = obj->transferFunction();
  if (!transferFunction)
    return;
  transferFunction->setLowCutOff(m_oldCutOff);
}

void TransferFunctionSetLowCutOff::redo() {
  WoolzObject *obj=   m_objectListModel->getObject(m_objID) ;
  if (!obj)
    return;

  TransferFunction *transferFunction = obj->transferFunction();
  if (!transferFunction)
    return;

  m_oldCutOff = transferFunction->lowCutOff();
  transferFunction->setLowCutOff(m_cutOff);
}

bool TransferFunctionSetLowCutOff::mergeWith ( const QUndoCommand * command ) {
  if (command->id() != id()) // make sure other is also an TransferFunctionSetLowCutOff command
    return false;
  const TransferFunctionSetLowCutOff *setCommand = static_cast<const TransferFunctionSetLowCutOff*>(command);
  if (setCommand->m_objID != m_objID) // make sure other is also referes the same object
    return false;
   m_cutOff = setCommand->m_cutOff;
  return true;
}

TransferFunctionSetHighCutOff::TransferFunctionSetHighCutOff(ObjectListModel *objectListModel, int objID, unsigned char cutOff, QUndoCommand * parent):
    QUndoCommand(parent),  m_objectListModel(objectListModel), m_objID(objID), m_cutOff(cutOff) {
         setText("Transfer Function set high cut off");
}

void TransferFunctionSetHighCutOff::undo() {
  WoolzObject *obj=   m_objectListModel->getObject(m_objID) ;
  if (!obj)
    return;
  TransferFunction *transferFunction = obj->transferFunction();
  if (!transferFunction)
    return;
  transferFunction->setHighCutOff(m_oldCutOff);
}

void TransferFunctionSetHighCutOff::redo() {
  WoolzObject *obj=   m_objectListModel->getObject(m_objID) ;
  if (!obj)
    return;

  TransferFunction *transferFunction = obj->transferFunction();
  if (!transferFunction)
    return;

  m_oldCutOff = transferFunction->highCutOff();
  transferFunction->setHighCutOff(m_cutOff);
}

bool TransferFunctionSetHighCutOff::mergeWith ( const QUndoCommand * command ) {
  if (command->id() != id()) // make sure other is also an TransferFunctionSetHighCutOff command
    return false;
  const TransferFunctionSetHighCutOff *setCommand = static_cast<const TransferFunctionSetHighCutOff*>(command);
  if (setCommand->m_objID != m_objID) // make sure other is also referes the same object
    return false;
   m_cutOff = setCommand->m_cutOff;
  return true;
}


TransferFunctionLoad::TransferFunctionLoad(ObjectListModel *objectListModel, int objID, TransferFunction *transfFunc, QUndoCommand * parent):
    QUndoCommand(parent),  m_objectListModel(objectListModel), m_objID(objID), m_transfFunc(transfFunc), m_oldTransfFunc(NULL) {
         setText("Set transfer function");
}

TransferFunctionLoad::~TransferFunctionLoad() {
   //remove unused transfer function
  if (m_oldTransfFunc)
         delete m_oldTransfFunc;
  m_oldTransfFunc = NULL;
}

void TransferFunctionLoad::undo() {
  WoolzObject *obj = m_objectListModel->getObject(m_objID) ;
  if (!obj)
    return;
  obj->transferFunction()->copyTF(m_oldTransfFunc);
  delete m_oldTransfFunc;
  m_oldTransfFunc = NULL;
}

void TransferFunctionLoad::redo() {
  WoolzObject *obj = m_objectListModel->getObject(m_objID) ;
  if (!obj)
    return;
  m_oldTransfFunc = new TransferFunction;
  m_oldTransfFunc->copyTF(obj->transferFunction());
  obj->transferFunction()->copyTF(m_transfFunc);
}


WarpingSetDelta::WarpingSetDelta(LandmarkModel *landmarkModel, double delta, QUndoCommand * parent):
    QUndoCommand(parent),  m_landmarkModel(landmarkModel), m_delta(delta) {
         setText("Set transform delta");
}

void WarpingSetDelta::undo() {
   QApplication::setOverrideCursor(Qt::WaitCursor);
   m_landmarkModel->setDelta(m_oldDelta);
   QApplication::restoreOverrideCursor();
}

void WarpingSetDelta::redo() {
   QApplication::setOverrideCursor(Qt::WaitCursor);
   m_oldDelta=m_landmarkModel->delta();
   m_landmarkModel->setDelta(m_delta);
   QApplication::restoreOverrideCursor();
}

bool WarpingSetDelta::mergeWith ( const QUndoCommand * command ) {
  if (command->id() != id()) // make sure other is also an WarpingSetDelta command
    return false;
  const WarpingSetDelta *setCommand = static_cast<const WarpingSetDelta*>(command);
  if (setCommand->m_landmarkModel!= m_landmarkModel) // make sure other is also referes the same object
    return false;
  m_delta = setCommand->m_delta;
  return true;
}

WarpingtSetBasisFnType::WarpingtSetBasisFnType(LandmarkModel *landmarkModel, LandmarkModel::BasisFnType basisFnType, QUndoCommand * parent):
    QUndoCommand(parent),  m_landmarkModel(landmarkModel), m_basisFnType(basisFnType) {
         setText(QString("Set basis transform to") + ((basisFnType == LandmarkModel::basis_IMQ) ? "Inverse multiquadric" : "Multiquadric"));
}

void WarpingtSetBasisFnType::undo() {
   QApplication::setOverrideCursor(Qt::WaitCursor);
   m_landmarkModel->setBasisFnType(m_oldBasisFnType);
   QApplication::restoreOverrideCursor();
}

void WarpingtSetBasisFnType::redo() {
   QApplication::setOverrideCursor(Qt::WaitCursor);
   m_oldBasisFnType=m_landmarkModel->basisFnType();
   m_landmarkModel->setBasisFnType(m_basisFnType);
   QApplication::restoreOverrideCursor();
}

bool WarpingtSetBasisFnType::mergeWith ( const QUndoCommand * command ) {
  if (command->id() != id()) // make sure other is also an WarpingtSetBasisFnType command
    return false;
  const WarpingtSetBasisFnType *setCommand = static_cast<const WarpingtSetBasisFnType*>(command);
  if (setCommand->m_landmarkModel != m_landmarkModel) // make sure other is also referes the same object
    return false;
  m_basisFnType = setCommand->m_basisFnType;
  return true;
}
