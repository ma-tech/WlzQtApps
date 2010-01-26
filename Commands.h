#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _Commands_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         Commands.h
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

#ifndef COMMANDS_H
#define COMMANDS_H

//Qt includes
#include <QUndoCommand>
#include <QColor>

//project includes
#include "WoolzDynThresholdedObj.h"
#include "WoolzDynWarpedObject.h"

#include "LandmarkModel.h"

//coin includes
#include <Inventor/fields/SoMFFloat.h>


class WoolzDynContourISO;
class LandmarkController;
class ObjectListModel;
class WoolzObject;

class TransferFunction;
class SoMFFloat;


/////////////////////////////////////////////////////////////////////////////////// Object creation/deletion
/*!
 * \brief 	 Undoable command to create object
 * \ingroup      UI
 */
class CreateWoolzObject: public QUndoCommand
{
 public:
     CreateWoolzObject(ObjectListModel *objectListModel, WoolzObject *object, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
     virtual ~CreateWoolzObject();
 private:
     ObjectListModel *m_objectListModel;          /*!< object manager*/
     WoolzObject *m_object;                       /*!< new WoolzObject */
     bool m_allive;                 /*!< if the object is allive, otherwise if ready to be deleted*/
};

/*!
 * \brief 	 Undoable command to delete object
 * \ingroup      UI
 */
class DeleteWoolzObject: public QUndoCommand
{
 public:
     DeleteWoolzObject(ObjectListModel *objectListModel, WoolzObject *object, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
     virtual ~DeleteWoolzObject();
 private:
     ObjectListModel *m_objectListModel;          /*!< object manager*/
     WoolzObject *m_object;                       /*!< WoolzObject to remove*/
     bool m_allive;                 /*!< if the object is allive, otherwise if ready to be deleted*/
};

/*!
 * \brief 	 Undoable command to create dynamic mesh object
 * \ingroup      UI
 */
class CreateMeshObject: public QUndoCommand
{
 public:
     CreateMeshObject(ObjectListModel *objectListModel, LandmarkModel *landmarkModel, WoolzObject *object, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
     virtual ~CreateMeshObject();
 private:
     ObjectListModel *m_objectListModel;          /*!< object manager */
     LandmarkModel *m_landmarkModel;              /*!< landmark model */
     WoolzObject *m_object;                       /*!< new WoolzObject mesh object */
     WoolzObject *m_oldObject;                    /*!< previous WoolzObject mesg object */
     bool m_allive;                 /*!< if the object is allive, otherwise if ready to be deleted */
};
/////////////////////////////////////////////////////////////////////////////////// General object properties
/*!
 * \brief 	 Undoable command to rename objects
 * \ingroup      UI
 */
class RenameObject: public QUndoCommand
{
 public:
     RenameObject(WoolzObject *object, QString name, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
     int id () const;
     bool mergeWith ( const QUndoCommand * command );
 private:
     WoolzObject *m_object;                       /*!< WoolzObject to be changed*/
     QString m_name;                              /*!< new value */
     QString m_oldName;                           /*!< old value */
};

/*!
 * \brief 	 Undoable command to change object colour
 * \ingroup      UI
 */
class ColourObject: public QUndoCommand
{
 public:
     ColourObject(WoolzObject *object, const QColor colour, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
     int id () const ;
     bool mergeWith ( const QUndoCommand * command );
 private:
     WoolzObject *m_object;                       /*!< WoolzObject to be changed*/
     QColor m_colour;                             /*!< new value */
     QColor m_oldColour;                          /*!< old value */
};


/*!
 * \brief 	 Undoable command to change object global visibility
 * \ingroup      UI
 */
class WoolzObjectChangeVisibility: public QUndoCommand
{
 public:
     WoolzObjectChangeVisibility(WoolzObject *object, const bool visible, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
     int id () const ;
     bool mergeWith ( const QUndoCommand * command );
 private:
     WoolzObject *m_object;                       /*!< WoolzObject to be changed */
     bool m_visible;                              /*!< new value */
     bool m_oldVisible;                           /*!< old value */
};

/////////////////////////////////////////////////////////////////////////////////// Landmarks
/*!
 * \brief 	 Undoable command to remove landmark
 * \ingroup      UI
 */
class RemoveLandmark : public QUndoCommand
{
 public:
     RemoveLandmark (LandmarkModel *model, int index, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
 private:
     LandmarkModel *m_model;                      /*!< landmark model */
     PointPair m_pp;                              /*!< the removed point pair values*/
     int m_index;                                 /*!< the removed point index*/
};

/*!
 * \brief 	 Undoable command to add a landmark
 * \ingroup      UI
 */
class AddLandmark : public QUndoCommand
{
 public:
     AddLandmark (LandmarkModel *model, PointPair &pp, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
 private:
     LandmarkModel *m_model;                      /*!< landmark model */
     PointPair m_pp;                              /*!< new landmark pointpair*/
};

/*!
 * \brief 	 Undoable command to add half landmark in for the source or the target
 * \ingroup      UI
 */
class AddHalfLandmark: public QUndoCommand
{
 public:
     AddHalfLandmark (LandmarkController *controller, const WlzDVertex3 newPosition, LandmarkModel::IndexType indexType, QUndoCommand * parent = 0);
     AddHalfLandmark (LandmarkController *controller, const SbVec3f newPosition, LandmarkModel::IndexType indexType, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
 private:
     LandmarkController *m_controller;                     /*!< landmark controller */
     WlzDVertex3  m_position;                              /*!< new value */
     WlzDVertex3  m_positionOld;                           /*!< old value */
     LandmarkModel::IndexType m_indexType;                 /*!< the type of the half landmark*/
     bool     m_previouslySet;                             /*!< if true, the landmark has to be moved, not added*/
};

/*!
 * \brief 	 Undoable command to pair half landmark in for the source or the target. This will result a new landmark pair being created and added to the model.
 * \ingroup      UI
 */
class AddSecondHalfLandmark: public QUndoCommand
{
 public:
     AddSecondHalfLandmark (LandmarkController *controller, const WlzDVertex3 newPosition, LandmarkModel::IndexType indexType, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
 private:
     LandmarkController *m_controller;                      /*!< landmark controller */
     WlzDVertex3  m_position;                               /*!< new value */
     WlzDVertex3  m_positionOld;                            /*!< old value */
     LandmarkModel::IndexType m_indexType;                  /*!< the type of the half landmark*/
};

/*!
 * \brief 	 Undoable command to move a landmark
 * \ingroup      UI
 */
class MoveLandmark : public QUndoCommand
{
 public:
     MoveLandmark(LandmarkModel *model, int index, const SbVec3f newPosition, LandmarkModel::IndexType indexType, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
     bool mergeWith ( const QUndoCommand * command );
     int id () const;
   private:
     LandmarkModel *m_model;                            /*!< landmark model */
     PointPair m_pp;                                    /*!< old position*/
     int m_index;                                       /*!< index of the landmark*/
     LandmarkModel::IndexType m_indexType;              /*!< the type of the half landmark to be moved*/
     SbVec3f  m_newpp;                                  /*!< new position*/
};

/*!
 * \brief 	 Undoable command to load a landmark
 * \ingroup      UI
 */
class LoadLandmarks : public QUndoCommand
{
 public:
     LoadLandmarks(LandmarkModel *model, QString filename, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
  private:
     LandmarkModel *m_model;                           /*!< landmark model */
     QString m_filename;                               /*!< filename to load from*/
};

/*!
 * \brief 	 Undoable command to enable or disable global update for a group of undo commands.
 * \par          Must be used in pairs at the begining repectivelt at the end of group of commands
 * \ingroup      UI
 */
class EnableDisableUpdateCommand : public QUndoCommand
{
 public:
     EnableDisableUpdateCommand (bool isFirst, EnableDisableUpdateCommand * pair, QUndoCommand * parent = 0);
     EnableDisableUpdateCommand (QUndoCommand * parent = 0);
     void setPair(bool isFirst, EnableDisableUpdateCommand * pair);  //must not be virtual (called from constructor)
     virtual void undo();
     virtual void redo();
     virtual bool state() {return m_state;};
 private:
     bool m_isFirst;                                               /*!< true if this is the first command in the grpup and false if is the last  */
     EnableDisableUpdateCommand * m_pair;                          /*!< the pair command*/
     bool m_state;                                                 /*!< the auto update value before the command groups started */
};


/////////////////////////////////////////////////////////////////////////////////// Thresholding
/*!
 * \brief 	 Undoable command to set WoolzDynThresholdedObj low threshold
 * \ingroup      UI
 */
class SetLowThreshold: public QUndoCommand
{
 public:
     SetLowThreshold(WoolzDynThresholdedObj *object, unsigned char lowTh, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
     int id () const;
     bool mergeWith ( const QUndoCommand * command );
 private:
     WoolzDynThresholdedObj *m_object;                    /*!< WoolzObject to be changed*/
     unsigned char m_lowTh;                               /*!< new value */
     unsigned char m_oldLowTh;                            /*!< old value */
};

/*!
 * \brief 	 Undoable command to set WoolzDynThresholdedObj high threshold
 * \ingroup      UI
 */
class SetHighThreshold: public QUndoCommand
{
 public:
     SetHighThreshold(WoolzDynThresholdedObj *object, const unsigned char highTh, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
     int id () const ;
     bool mergeWith ( const QUndoCommand * command );
 private:
     WoolzDynThresholdedObj *m_object;                     /*!< WoolzObject to be changed*/
     unsigned char m_highTh;                               /*!< new value */
     unsigned char m_oldHighTh;                            /*!< old value */
};

/*!
 * \brief 	 Undoable command to set WoolzDynThresholdedObj thresholding channel
 * \ingroup      UI
 */
class WoolzDynThresholdedObjSetChannel: public QUndoCommand
{
 public:
     WoolzDynThresholdedObjSetChannel(WoolzDynThresholdedObj *object, const enum WoolzDynThresholdedObj::channelTypes channel, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
     int id () const;
     bool mergeWith ( const QUndoCommand * command );
 private:
     WoolzDynThresholdedObj *m_object;                         /*!< WoolzObject to be changed*/
     enum WoolzDynThresholdedObj::channelTypes m_channel;      /*!< new value */
     enum WoolzDynThresholdedObj::channelTypes m_oldChannel;   /*!< old value */
};


///////////////////////////////////////////////////////////////////////////////////  Dynamic Objects
/*!
 * \brief 	 Undoable command to set WoolzDynObjectSetAutoUpdate auto update
 * \ingroup      UI
 */
class WoolzDynObjectSetAutoUpdate: public QUndoCommand
{
 public:
     WoolzDynObjectSetAutoUpdate(WoolzDynObject *object, const bool enabled, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
     int id () const ;
     bool mergeWith ( const QUndoCommand * command );
 private:
     WoolzDynObject *m_object;                     /*!< WoolzObject to be changed*/
     bool m_enabled;                               /*!< new value */
     bool m_oldEnabled;                            /*!< old value */
};

/*!
 * \brief 	 Undoable command to set WoolzDynObjectSetAutoUpdate source object
 * \ingroup      UI
 */
class WoolzDynObjectSetSourceObj: public QUndoCommand
{
 public:
     WoolzDynObjectSetSourceObj(WoolzDynObject *object, WoolzObject *sourceObj, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
     int id () const ;
     bool mergeWith ( const QUndoCommand * command );
 private:
     WoolzDynObject *m_object;                       /*!< WoolzObject to be changed*/
     int m_sourceObjID;                              /*!< new value */
     int m_oldSourceObjID;                           /*!< old value */
};

///////////////////////////////////////////////////////////////////////////////////  ISO object
/*!
 * \brief 	 Undoable command to set WoolzDynISOObjSetValue ISO value
 * \ingroup      UI
 */
class WoolzDynISOObjSetValue: public QUndoCommand
{
 public:
     WoolzDynISOObjSetValue(WoolzDynContourISO *object, char value, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
     int id () const;
     bool mergeWith ( const QUndoCommand * command );
 private:
     WoolzDynContourISO *m_object;                /*!< WoolzObject to be changed*/
     char m_value;                                /*!< new value */
     char m_oldValue;                             /*!< old value */
};

/*!
 * \brief 	 Undoable command to set WoolzDynISOObjSetValue boundary flag
 * \ingroup      UI
 */
class WoolzDynISOObjSetBoundary: public QUndoCommand
{
 public:
     WoolzDynISOObjSetBoundary(WoolzDynContourISO *object, bool enabled, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
     int id () const;
     bool mergeWith ( const QUndoCommand * command );
 private:
     WoolzDynContourISO *m_object;                /*!< WoolzObject to be changed*/
     bool m_enabed;                               /*!< new value */
     bool m_oldEnabed;                            /*!< old value */
};

/*!
 * \brief 	 Undoable command to set WoolzDynISOObjSetValue subsampling rate
 * \ingroup      UI
 */
class WoolzDynISOObjSetSubsample: public QUndoCommand
{
 public:
     WoolzDynISOObjSetSubsample(WoolzDynContourISO *object, char subsample, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
     int id () const;
     bool mergeWith ( const QUndoCommand * command );
 private:
     WoolzDynContourISO *m_object;                /*!< WoolzObject to be changed*/
     char m_subsample;                            /*!< new value */
     char m_oldSubsample;                         /*!< old value */
};

/*!
 * \brief 	 Undoable command to set WoolzDynISOObjSetValue filtering on or off
 * \ingroup      UI
 */
class WoolzDynISOObjSetFilter: public QUndoCommand
{
 public:
     WoolzDynISOObjSetFilter(WoolzDynContourISO *object, bool enabled, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
     int id () const;
     bool mergeWith ( const QUndoCommand * command );
 private:
     WoolzDynContourISO *m_object;                /*!< WoolzObject to be changed*/
     bool m_enabed;                               /*!< new value */
     bool m_oldEnabed;                            /*!< old value */
};

/*!
 * \brief 	 Undoable command to set WoolzDynISOObjSetValue selected shells
 * \ingroup      UI
 */
class WoolzDynISOObjSetShellSelection: public QUndoCommand
{
 public:
     WoolzDynISOObjSetShellSelection(WoolzDynContourISO *object, int selection, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
     int id () const;
     bool mergeWith ( const QUndoCommand * command );
 private:
     WoolzDynContourISO *m_object;                /*!< WoolzObject to be changed*/
     int m_selection;                             /*!< new value */
     int m_oldSelection;                          /*!< old value */
};


///////////////////////////////////////////////////////////////////////////////////  Transfer function

/*!
 * \brief 	 Undoable command for transfer function map replacement.
 * \ingroup      UI
 */
class TransferFunctionMapUpdate: public QUndoCommand
{
 public:
     TransferFunctionMapUpdate(ObjectListModel *objectListModel, int objID, SoMFFloat &colorMap,
                                               QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
     int id () const;
     bool mergeWith ( const QUndoCommand * command );
 private:
     ObjectListModel *m_objectListModel;       /*!< object manager */
     int m_objID;                              /*!< object identifier */
     SoMFFloat m_colorMap;                     /*!< new value */
     SoMFFloat m_oldColorMap;                  /*!< old value */
};

/*!
 * \brief 	 Undoable command to set transfer function low cut off treshold.
 * \ingroup      UI
 */
class TransferFunctionSetLowCutOff: public QUndoCommand
{
 public:
     TransferFunctionSetLowCutOff(ObjectListModel *objectListModel, int objID,
                            unsigned char cutOff, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
     int id () const;
     bool mergeWith ( const QUndoCommand * command );
 private:
     ObjectListModel *m_objectListModel;       /*!< object manager */
     int m_objID;                              /*!< object identifier */
     unsigned char m_cutOff;                   /*!< new value */
     unsigned char m_oldCutOff;                /*!< old value */
};

/*!
 * \brief 	 Undoable command to set transfer function high cut off treshold.
 * \ingroup      UI
 */
class TransferFunctionSetHighCutOff: public QUndoCommand
{
 public:
     TransferFunctionSetHighCutOff(ObjectListModel *objectListModel, int objID,
                            unsigned char cutOff, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
     int id () const;
     bool mergeWith ( const QUndoCommand * command );
 private:
     ObjectListModel *m_objectListModel;       /*!< object manager */
     int m_objID;                              /*!< object identifier */
     unsigned char m_cutOff;                   /*!< new value */
     unsigned char m_oldCutOff;                /*!< old value */
};

/*!
 * \brief 	 Undoable command to set transfer function high cut off treshold.
 * \ingroup      UI
 */
class TransferFunctionLoad: public QUndoCommand
{
 public:
     TransferFunctionLoad(ObjectListModel *objectListModel, int objID,
                            TransferFunction *transfFunc, QUndoCommand * parent = 0);
     virtual ~TransferFunctionLoad();

     virtual void undo();
     virtual void redo();
 private:
     ObjectListModel *m_objectListModel;       /*!< object manager */
     int m_objID;                              /*!< object identifier */
     TransferFunction *m_transfFunc;           /*!< new transfer function*/
     TransferFunction *m_oldTransfFunc;        /*!< old transfer function*/
};


///////////////////////////////////////////////////////////////////////////////////  Warped object

/*!
 * \brief 	 Undoable command to set WoolzDynWarpedObject delta value
 * \ingroup      UI
 */
class WarpingSetDelta: public QUndoCommand
{
 public:
     WarpingSetDelta(LandmarkModel *landmarkModel, double delta, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
     int id () const;
     bool mergeWith ( const QUndoCommand * command );
 private:
     LandmarkModel *m_landmarkModel;                /*!< LandmarkModel managing delta*/
     double m_delta;                                /*!< new value */
     double m_oldDelta;                             /*!< old value */
};

/*!
 * \brief 	 Undoable command to set WoolzDynWarpedObject delta value
 * \ingroup      UI
 */
class WarpingtSetBasisFnType: public QUndoCommand
{
 public:
     WarpingtSetBasisFnType(LandmarkModel *landmarkModel, LandmarkModel::BasisFnType basisFnType, QUndoCommand * parent = 0);
     virtual void undo();
     virtual void redo();
     int id () const;
     bool mergeWith ( const QUndoCommand * command );
 private:
     LandmarkModel *m_landmarkModel;                      /*!< LandmarkModel managing delta*/
     LandmarkModel::BasisFnType m_basisFnType;            /*!< new value */
     LandmarkModel::BasisFnType m_oldBasisFnType;         /*!< old value */
};


#endif // COMMANDS_H
