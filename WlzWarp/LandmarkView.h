#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _LandmarkView_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         LandmarkView.h
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
* \brief        View for landmarks.
* \ingroup      Views
*
*/
#ifndef LANDMARKVIEW_H
#define LANDMARKVIEW_H

#include <View.h>
#include "LandmarkController.h"

class SoSeparator;
class SoSensor;
class LandmarkManip;

/*!
* \brief	View providing visualisation of landmark points
* \ingroup      Views
*/
class LandmarkView : public View
{
protected:
  Q_OBJECT
public:

 /*!
  * \ingroup      Views
  * \brief        Constructor
  * \param        parent parent viewer
  * \param        ladmarkModel landmark model to be visualised
  * \param        type type of the points from the model to be visulised
  * \return       void
  * \par      Source:
  *                LandmarkView.cpp
  */
  LandmarkView ( LandmarkController *ladmarkModel,
     LandmarkModel::IndexType type);

 /*!
  * \ingroup      Views
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *                LandmarkView.cpp
  */
  virtual ~LandmarkView ( );

 /*!
  * \ingroup      Views
  * \brief        Callback when landmark manipulator moves
  * \param        landmark moved landmark manipulator
  * \return       void
  * \par      Source:
  *                LandmarkView.cpp
  */
  virtual void movedManipulator(LandmarkManip *landmark);

public slots:

 /*!
  * \ingroup      Views
  * \brief        Slot to procces not paired landmark point additon
  * \param        point new point coordinates
  * \return       void
  * \par      Source:
  *                LandmarkView.cpp
  */
  void addLandmark(const WlzDVertex3 point);

 /*!
  * \ingroup      Views
  * \brief        Slot to procces paired landmark point additon
  * \param        index index into landmark model
  * \return       void
  * \par      Source:
  *                LandmarkView.cpp
  */
  void addedLandmarkPair(const int index);

 /*!
  * \ingroup      Views
  * \brief        Set higlighing on of a landmark
  * \param        index landmark index
  * \param        on  true if set to on, false if set to off
  * \return       void
  * \par      Source:
  *                LandmarkView.cpp
  */
  void setHighlight(int index, const bool on);

 /*!
  * \ingroup      Views
  * \brief        Set the validity of an landmark
  * \param        index landmark index
  * \param        on  true if set to valid, false if set to invalid
  * \return       void
  * \par      Source:
  *                LandmarkView.cpp
  */
  void setLandmarkValid(int index, const bool on);

 /*!
  * \ingroup      Views
  * \brief        Removes a landmark
  * \param        index landmark index
  * \return       void
  * \par      Source:
  *                LandmarkView.cpp
  */
  void removedLandmark(const int index);

 /*!
  * \ingroup      Views
  * \brief        Removes the not paired landmark
  * \return       void
  * \par      Source:
  *                LandmarkView.cpp
  */
  void removedSingleLandmark();  //removes temporary feature points

 /*!
  * \ingroup      Views
  * \brief        Processes landmark move
  * \param        index landmark index
  * \param        point new coordinate
  * \return       void
  * \par      Source:
  *                LandmarkView.cpp
  */
  virtual void movedLandmark(const int index, const WlzDVertex3 point);

 /*!
  * \ingroup      Views
  * \brief        Forwards highlighting requet of a landmark to the model
  * \param        landmark to highlight
  * \param        on new highlight state
  * \return       void
  * \par      Source:
  *                LandmarkView.cpp
  */
  virtual void highlighLandmark(LandmarkManip *landmark, bool on = true);

 /*!
  * \ingroup      Visualisation
  * \brief        Returns the number of available visualisations that is 1 for Contour2DView.
  *
  *           Implements View::getVisualisationTypesNo().
  * \return       Return the number of available visualisations (1)
  * \par      Source:
  *                LandmarkView.cpp
  */
  virtual int getVisualisationTypesNo () {return 1;}

 /*!
  * \ingroup      Visualisation
  * \brief        Returns if move is enabled calling the model.
  *
  * \return       Return if move is enabled
  * \par      Source:
  *                LandmarkView.cpp
  */
  virtual bool isMoveEnabed() {return m_landmarkController->isMoveEnabled();}

 /*!
  * \ingroup      Visualisation
  * \brief        Returns the list of available visualisations.
  *
  *           Implements View::getVisualisationTypes().
  * \return       List of visualistion type names.
  * \par      Source:
  *                LandmarkView.cpp
  */
  virtual QStringList getVisualisationTypes ();

 /*!
  * \ingroup      Views
  * \brief        Current visualisation type.
  *
  *   Currently only one visualisation is implemented, therefore it returns 0.
  *
  *           Implements View::visualisationType().
  * \return       void
  * \par      Source:
  *                LandmarkView.cpp
  */
  virtual int visualisationType () {return 0;}

 /*!
  * \ingroup      Views
  * \brief        Update landmarks
  * \return       void
  * \par      Source:
  *                LandmarkView.cpp
  */
  virtual void updateLandmarks();

protected:
 /*!
  * \ingroup      Visualisation
  * \brief        Builds the scene graph of the view.
  *
  *           Reimplements View::generateSceneGraph().
  * \param        bForce, if true force update of the scene graph elements
  * \return       void
  * \par      Source:
  *                LandmarkView.cpp
  */
  virtual void generateSceneGraph ( bool bForce = false);

protected:
  LandmarkModel::IndexType indexType;           /*!< landmark type */
  LandmarkController *m_landmarkController;     /*!< landmark model being visualised */
  SoSeparator *lastLandmarkNode;                /*!< last added node without a matching pair */
};

#endif  //LANDMARKVIEW_H
