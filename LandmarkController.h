#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _LandmarkController_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         LandmarkController.h
* \author       Zsolt Husz
* \date         October 2008
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
* \brief        Model of landmarks.
* \ingroup      Control
*
*/

#ifndef LANDMARKCONTROLLER_H
#define LANDMARKCONTROLLER_H

#include <QObject>

#include <Wlz.h>
#include <Inventor/SbLinear.h>

#include "LandmarkModel.h"

class QUndoStack;
class QModelIndex;
class QUndoCommand;
class LandmarkView;

/*!
* \brief	Model of landmarks. Stores and manages landmark corespondences.
* \ingroup      Control
*/
class LandmarkController : public QObject
{
  Q_OBJECT
public:
 /*!
  * \ingroup      Control
  * \brief        Constructor
  * \param        parent parent object
  * \return       void
  * \par      Source:
  *                LandmarkController.cpp
  */
  LandmarkController ( QUndoStack *undoStack, LandmarkModel *model, QObject * parent = 0 );

 /*!
  * \ingroup      Control
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *                LandmarkController.cpp
  */
  virtual ~LandmarkController ( );

  /*!
  * \ingroup      Control
  * \brief        Return the landmark model
  * \return       Landmark model
  * \par      Source:
  *                LandmarkController.cpp
  */
  LandmarkModel * getModel () {return m_model;}

  /*!
  * \ingroup      Control
  * \brief        Remove a landmark pair
  * \param        index landmark index to remove
  * \return       void
  * \par      Source:
  *                LandmarkController.cpp
  */
  void removeLandmark(int index);

 /*!
  * \ingroup      Control
  * \brief        Remove a landmark pair
  * \param        pp landmark to remove
  * \return       void
  * \par      Source:
  *                LandmarkController.cpp
  */
  void removeLandmark(PointPair *pp);

 /*!
  * \ingroup      Control
  * \brief        Remove a landmark pair
  * \param        index model inded of the landmark to remove
  * \return       void
  * \par      Source:
  *                LandmarkController.cpp
  */
  void removeLandmark(const QModelIndex & index);

 /*!
  * \ingroup      Control
  * \brief        Move source of target component of a landmark pair
  * \param        pp point pair to be moved
  * \param        newPosition new position
  * \param        indexType part type to be moved
  * \return       void
  * \par      Source:
  *                LandmarkController.cpp
  */
  void move(PointPair *pp, const SbVec3f newPosition, LandmarkModel::IndexType indexType);

  /*!
  * \ingroup      Visualisation
  * \brief        Returns landmark view
  * \param        indexType view type requested (sourceV or targetV)
  * \return       Returns landmar view views
  * \par      Source:
  *                LandmarkController.cpp
  */
  LandmarkView* getLandmarkView(LandmarkModel::IndexType indexType);

 /*!
  * \ingroup      Control
  * \brief        Adds a source landmark
  * \param        point source landmark
  * \return       void
  * \par      Source:
  *                LandmarkController.cpp
  */
  void addSourceLandmark(const WlzDVertex3 point);

 /*!
  * \ingroup      Control
  * \brief        Adds a target landmark
  * \param        point target landmark
  * \return       void
  * \par      Source:
  *                LandmarkController.cpp
  */
  void addTargetLandmark(const WlzDVertex3 point);

 /*!
  * \ingroup      Control
  * \brief        Adds a source and target landmark pair
  * \param        pointS source landmark
  * \param        pointT target landmark
  * \return       void
  * \par      Source:
  *                LandmarkController.cpp
  */
  void addLandmarkPair(const WlzDVertex3 pointS, const WlzDVertex3 pointT);

 /*!
  * \ingroup      Visualisation
  * \brief        Returns if move is enabled.
  * \return       Returns if move is enabled
  * \par      Source:
  *                LandmarkController.cpp
  */
  virtual bool isMoveEnabled() {return moveEnabled;}

 /*!
  * \ingroup      Control
  * \brief        Sets highlighting for landmark, delegates request to the model
  * \param        index PointPair to set highlighting
  * \param        on new highlighint. If true, any other landmark which have been on is switched off.
  * \return       void
  * \par      Source:
  *                LandmarkController.cpp
  */
  void highlight(int index, const bool on);

 /*!
  * \ingroup      Control
  * \brief        Sets highlighting for landmark
  * \param        index of the PointPair to set highlighting
  * \param        on new highlighint. If true, any other landmark which have been on is switched off.
  * \return       void
  * \par      Source:
  *                LandmarkController.cpp
  */
  void highlight(const QModelIndex & index, const bool on);

 /*!
  * \ingroup      Control
  * \brief        Sets highlighting for landmark
  *
  *               Calls highlight(int, bool).
  * \param        pp PointPair to set highlighting
  * \param        on new highlighint. If true, any other landmark which have been on is switched off.
  * \return       void
  * \par      Source:
  *                LandmarkController.cpp
  */
  void highlight(PointPair *pp, const bool on);

 /*!
  * \ingroup      Control
  * \brief        Adds source or target landmark point
  * \param        point new landmark point
  * \param        indexType if source or target
  * \return       bool if previously set
  * \par      Source:
  *                LandmarkController.cpp
  */
  void addHalfLandmark(const WlzDVertex3 point, const LandmarkModel::IndexType indexType);

 /*!
  * \ingroup      Control
  * \brief        Adds source or target landmark point, the yet unset hald
  * \param        point new landmark point
  * \param        indexType if source or target
  * \return       bool if previously set
  * \par      Source:
  *                LandmarkController.cpp
  */
  void addSecondHalfLandmark(const WlzDVertex3 point, const LandmarkModel::IndexType indexType);

 /*!
  * \ingroup      Control
  * \brief        Return source or target landmark point hald
  * \param        indexType if source or target
  * \param        point variable to store result
  * \return       bool true if requested half os valid
  * \par      Source:
  *                LandmarkController.cpp
  */
  bool getHalfLandmark(WlzDVertex3 & point, const LandmarkModel::IndexType indexType);

  /*!
  * \ingroup      Control
  * \brief        Adds command to the command (undo) stack.
  * \param        command command to add
  * \return       void
  * \par      Source:
  *                LandmarkController.cpp
  */
  void addCommand(QUndoCommand *command);

public slots:
  /*!
  * \ingroup      Control
  * \brief        Processes configuration changes
  * \return       void
  * \par      Source:
  *                LandmarkModel.cpp
  */
  void configurationChanged();

 /*!
  * \ingroup      Control
  * \brief        Removes not paired landmark.
  * \return       void
  * \par      Source:
  *                LandmarkController.cpp
  */
  virtual void cancelIncompleteLandmarks();

 /*!
  * \ingroup      Control
  * \brief        Removes all landmarks.
  * \return       void
  * \par      Source:
  *                LandmarkController.cpp
  */
  virtual void removeAllLandmarks();

 /*!
  * \ingroup      Control
  * \brief        Set move enabled flag
  * \param        checked true if move is enabled, false for move not allowed
  * \return       void
  * \par      Source:
  *                LandmarkController.cpp
  */
  virtual void setMove(bool checked);

 /*!
  * \ingroup      Control
  * \brief        Removes highlighted landmark.
  * \return       void
  * \par      Source:
  *                LandmarkController.cpp
  */
  virtual void deleteHighlightedLandmark();

 /*!
  * \ingroup      Control
  * \brief        Loads landmark from a file
  * \param        filename file to read
  * \return       void
  * \par      Source:
  *                LandmarkController.cpp
  */
  virtual void loadLandmarks(QString filename);

signals:
  /*!
  * \ingroup      Control
  * \brief        Signals to views the change in highlighting.
  * \return       void
  * \par      Source:
  *                LandmarkController.cpp
  */
  void setHighlight(const int index, const bool on);

 /*!
  * \ingroup      Control
  * \brief        Signals removal of the half landmark pair.
  * \return       void
  * \par      Source:
  *                LandmarkModel.cpp
  */
  void removedSingleLandmark();

 /*!
  * \ingroup      Control
  * \brief        Signals to target views the addition of a landmark.
  * \param        point new landmark point
  * \return       void
  * \par      Source:
  *                LandmarkController.cpp
  */
  void addedTargetLandmark(const WlzDVertex3 point);

 /*!
  * \ingroup      Control
  * \brief        Signals to source views the addition of a landmark.
  * \param        point new landmark point
  * \return       void
  * \par      Source:
  *                LandmarkController.cpp
  */
  void addedSourceLandmark(const WlzDVertex3);

private:
 /*!
  * \ingroup      Control
  * \brief        Generates command to remove all landmarks
  * \param        parent widget
  * \return       remove all command
  * \par      Source:
  *                LandmarkController.cpp
  */
  QUndoCommand *removeAllLandmarksCommand( QUndoCommand * parent);

protected:
  QUndoStack *m_undoStack;                 /*!< undo stack of the application */
  LandmarkModel *m_model;                  /*!< landmark models storing the landks */
  LandmarkView *m_landmarkView[2];         /*!< landmark views */
  bool moveEnabled;                        /*!< if move is enabled */
  bool isValidnewLandmarkPair[2];          /*!< sourceV and/or targetV components of newLandmarkPair is valid */
  PointPair newLandmarkPair;               /*!< new part LandmarkPair, not added yet to listPointPair */
  bool isHighlighted;                      /*!< if highligh is on */
  PointPair *highlightSelection;           /*!< if highlighed landmark */
};

#endif // LANDMARKCONTROLLER_H
