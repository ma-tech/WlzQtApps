#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _LandmarkController_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         LandmarkController.cpp
* \author       Zsolt Husz
* \date         October 2008
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
* \brief        Model of landmarks.
* \ingroup      Control
* \todo         Half point landmark could be refactored to separate class,
* 		with signals connecting to controller
*
*/

//Qt includes
#include <QAction>
#include <QMessageBox>
#include <QTextStream>
#include <QUndoStack>
#include <QFile>

//project includes
#include "LandmarkController.h"
#include "LandmarkView.h"
#include "WarperConfig.h"
#include "PreferencesDialog.h"

#include "Commands.h"

LandmarkController::
LandmarkController(
  QUndoStack *undoStack,
  LandmarkModel *model,
  QObject * parent):
QObject(parent),
m_undoStack(undoStack),
m_model(model)
{
  isValidnewLandmarkPair[LandmarkModel::sourceV] =
  isValidnewLandmarkPair[LandmarkModel::targetV] = false;
  moveEnabled = false;
  isHighlighted = false;
  highlightSelection = NULL;
  m_landmarkView[LandmarkModel::sourceV] =
  m_landmarkView[LandmarkModel::targetV] = NULL;
}

LandmarkController::
~LandmarkController()
{
  if(m_landmarkView[LandmarkModel::sourceV])
  {
    delete m_landmarkView[LandmarkModel::sourceV];
  }
  if(m_landmarkView[LandmarkModel::targetV])
  {
    delete m_landmarkView[LandmarkModel::targetV];
  }
}

void LandmarkController::
move(
  PointPair *pp,
  const SbVec3f newPosition,
  LandmarkModel::IndexType indexType)
{
  int index=m_model->indexOf(pp);
  if(index >= 0)
  {
    m_undoStack->push(new MoveLandmark(m_model, index, newPosition,
    				       indexType));
  }
  else
  {
    m_undoStack->push(new AddHalfLandmark(this, newPosition, indexType));
  }
}

void LandmarkController::
addHalfLandmark(
  const WlzDVertex3 point,
  const LandmarkModel::IndexType indexType)
{
  isValidnewLandmarkPair[indexType] = true;
  newLandmarkPair[indexType] = point;
  if(indexType == LandmarkModel::sourceV)
  {
    emit addedSourceLandmark(point);
  }
  else
  {
    emit addedTargetLandmark(point);
  }
}

void LandmarkController::
addSecondHalfLandmark(
  const WlzDVertex3 point,
  const LandmarkModel::IndexType indexType)
{
  newLandmarkPair[indexType] = point;
  m_model->addLandmarkPair(newLandmarkPair);
  isValidnewLandmarkPair[LandmarkModel::sourceV] =
  isValidnewLandmarkPair[LandmarkModel::targetV] = false ;
  highlight(m_model->landmarkNumber()-1, true); // highlight last landmark
}

bool LandmarkController::
getHalfLandmark(
  WlzDVertex3 & point,
  const LandmarkModel::IndexType indexType)
{
  point = newLandmarkPair[indexType];
  return(isValidnewLandmarkPair[indexType]);
}

void LandmarkController::
addSourceLandmark(
  const WlzDVertex3 point)
{
  if(isValidnewLandmarkPair[LandmarkModel::targetV])
  {
    //both pairs valid, add new feature point
    m_undoStack->push(new AddSecondHalfLandmark(this, point,
    					        LandmarkModel::sourceV));
  }
  else
  {
    m_undoStack->push(new AddHalfLandmark(this, point,
    				          LandmarkModel::sourceV));
  }
}

void LandmarkController::
addTargetLandmark(
  const WlzDVertex3 point)
{
  if(isValidnewLandmarkPair[LandmarkModel::sourceV])
  {
    //both pairs valid, add new feature point
    m_undoStack->push(new AddSecondHalfLandmark(this, point,
    					        LandmarkModel::targetV));
  }
  else
  {
    m_undoStack->push(new AddHalfLandmark(this, point,
    					  LandmarkModel::targetV));
  }
}

void LandmarkController::
addLandmarkPair(
  const WlzDVertex3 pointS,
  const WlzDVertex3 pointT)
{
  newLandmarkPair[LandmarkModel::targetV] = pointT;
  newLandmarkPair[LandmarkModel::sourceV] = pointS;

  if(!isValidnewLandmarkPair[LandmarkModel::targetV])
  {
    isValidnewLandmarkPair[LandmarkModel::targetV] = true;
  }
  if(!isValidnewLandmarkPair[LandmarkModel::sourceV])
  {
    isValidnewLandmarkPair[LandmarkModel::sourceV] = true;
  }
  m_model->addLandmarkPair(newLandmarkPair);
  isValidnewLandmarkPair[LandmarkModel::sourceV] =
  isValidnewLandmarkPair[LandmarkModel::targetV] = false ;
}

void LandmarkController::
removeLandmark(
  int index)
{
  if(isHighlighted)
  {
    if(m_model->getPointPair(index)==highlightSelection)
    {
      isHighlighted = false;
    }
  }
  m_undoStack->push(new RemoveLandmark(m_model, index));
}

void LandmarkController::
removeLandmark(
  PointPair *pp)
{
  if(isHighlighted)
  {
    if(pp==highlightSelection)
    {
      isHighlighted = false;
    }
  }
  m_undoStack->push(new RemoveLandmark(m_model, m_model->indexOf(pp)));
}

void LandmarkController::
deleteHighlightedLandmark()
{
  if(isHighlighted)
  {
    removeLandmark(highlightSelection);
  }
}

QUndoCommand *LandmarkController::
removeAllLandmarksCommand(
  QUndoCommand *parent)
{
  Q_ASSERT(parent);
  int size = m_model->landmarkNumber();
  while(size > 0)
  {
    size--;
    new RemoveLandmark(m_model, size, parent);
  }
  return(parent);
}

void LandmarkController::
removeAllLandmarks()
{
  if(m_model->landmarkNumber()<1)
  {
    return;
  }

  QUndoCommand *removeAll = new QUndoCommand;
  removeAll->setText("Remove all landmarks");

  EnableDisableUpdateCommand *first =
    new EnableDisableUpdateCommand(removeAll);
  removeAll = removeAllLandmarksCommand(removeAll);
  new EnableDisableUpdateCommand(false, first, removeAll);
  m_undoStack->push(removeAll);
}

void LandmarkController::
removeLandmark(
  const QModelIndex & index)
{
  if(index.isValid())
  {
    removeLandmark(index.row());
  }
}

LandmarkView* LandmarkController::
getLandmarkView(
  LandmarkModel::IndexType indexType)
{
  if(!m_landmarkView[indexType])
  {
    m_landmarkView[indexType] = new LandmarkView(this, indexType);
  }
  return(m_landmarkView[indexType]);
}

void LandmarkController::
cancelIncompleteLandmarks()
{
  isValidnewLandmarkPair[LandmarkModel::sourceV] =
  isValidnewLandmarkPair[LandmarkModel::targetV] = false;
  emit removedSingleLandmark();
}

void LandmarkController::
setMove(bool checked)
{
  moveEnabled = checked;
}

void LandmarkController::
highlight(
  PointPair *pp,
  const bool on)
{
  int index=m_model->indexOf(pp);
  highlight(index, on);
}

void LandmarkController::
highlight(
  int index,
  const bool on)
{
  int highlightSelectionIndex=m_model->indexOf(highlightSelection);
  Q_ASSERT(index>=0 && index<m_model->landmarkNumber());

  if(index<0 || index>=m_model->landmarkNumber())
  {
    return;
  }
  if(on)
  {
    if(isHighlighted && highlightSelectionIndex != index)
    {
      emit setHighlight(highlightSelectionIndex, false);
    }
    highlightSelection = m_model->getPointPair(index);
    isHighlighted = true;
    emit setHighlight(index, true);
  }
  else if(highlightSelectionIndex == index)
  {
    emit setHighlight(highlightSelectionIndex, false);
    isHighlighted = false;
  }
}

void LandmarkController::
highlight(
  const QModelIndex & index,
  const bool on)
{
  if(index.isValid())
  {
    highlight(index.row(), on);
  }
}

void LandmarkController::
loadLandmarks(
  QString filename)
{
  QUndoCommand *loadCommand = new QUndoCommand;

  EnableDisableUpdateCommand *first =
      new EnableDisableUpdateCommand(loadCommand);
  loadCommand = removeAllLandmarksCommand(loadCommand);

  new LoadLandmarks(m_model, filename, loadCommand);
  new EnableDisableUpdateCommand(false, first, loadCommand);

  loadCommand->setText("Load landmarks");
  m_undoStack->push(loadCommand);
}

void LandmarkController::
configurationChanged()
{
  if(m_landmarkView[0])
  {
      m_landmarkView[0]->updateLandmarks();
  }
  if(m_landmarkView[1])
  {
      m_landmarkView[1]->updateLandmarks();
  }
  if(m_model)
  {
    PreferencesDialog *pref = qobject_cast<PreferencesDialog*>(sender());
    m_model->setSnapToFitDist(pref->snapToFitDist());
  }
}

void LandmarkController::
addCommand(
  QUndoCommand *command)
{
  m_undoStack->push(command);
}
